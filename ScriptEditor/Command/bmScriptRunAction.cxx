/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptRunAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptRunAction.h"
#include "bmLaunch.h"
#include "Timer.h"
#include "XMLBufferReader.h"

namespace bm {

ScriptRunAction::ScriptRunAction()
: ScriptAction()
{
}

ScriptRunAction::~ScriptRunAction()
{
}

bool ScriptRunAction::TestParam(ScriptError* error,int linenumber)
{
   if (m_parameters.size() <2)
   {
     error->SetError(MString("No enough parameter for Run"),linenumber);
     return false;
   }

   if (m_parameters.size() >2)
   {
     error->SetWarning(MString("Too much parameter for Run"),linenumber);
     return false;
   }
  
  m_manager->SetTestVariable(m_parameters[0]);


  return (m_manager->TestConvert(m_parameters[1],linenumber));
}

MString ScriptRunAction::Help()
{
  return "Run(<outputvariable> 'program.exe param1 param2 ...')";
}

/** Generate the condor script */
void ScriptRunAction::GenerateCondor(const char* appname)
{
  // we look for the current application
  // first we remove any ${}
  MString appvar(appname);
  appvar = appvar.removeChar('$');
  appvar = appvar.removeChar('{');
  appvar = appvar.removeChar('}');

  // second we try to find the current variable in the list of applications
  ApplicationWrapper * application = NULL;
  MString appName = m_manager->GetVariable(appvar)[0];
  bool appFound = false;
  ScriptActionManager::ApplicationWrapperListType::iterator it = m_manager->GetApplicationWrapperList()->begin();
  while (it != m_manager->GetApplicationWrapperList()->end())
    {
    if(!strcmp((*it)->GetApplicationPath().toChar(),appName.toChar()))
      {
      application = *it;
      appFound = true;
      break;
      }
    it++;
  }

  if(!application)
    {
    std::cout << "ScriptRunAction::GenerateCondor : Cannot find the application corresponding to " << appvar.toChar() << std::endl;
    return;
    }

  // We force the application to write the output out as XML
  std::string xmloutputname = application->GetName().toChar();
  xmloutputname += ".";
  char* num = new char[10];
  sprintf(num,"%d",m_GridModule->GetNumberOfApplications());
  xmloutputname += num;
  xmloutputname += ".out.xml";
  application->SetParameterValue("GenerateXMLFile","","1");
  application->SetParameterValue("GenerateXMLFile.GenerateXMLFile","",xmloutputname);
  delete [] num;

  m_GridModule->AddApplication(application);

  // We create the bmGridStore application and send it to condor
  ApplicationWrapper app;
  MString gsName = "bmGridStore";
  bool gsFound = false;
  ScriptActionManager::ApplicationWrapperListType::iterator itApp = m_manager->GetApplicationWrapperList()->begin();
  while (itApp != m_manager->GetApplicationWrapperList()->end())
    {
    if(!strcmp((*itApp)->GetName().toChar(),gsName.toChar()))
      {
      app = *(*itApp);
      gsFound = true;
      break;
      }
    itApp++;
    }

  if(!gsFound)
    {
    std::cout << "ScriptDashboardSendAction::GenerateCondor : Cannot find bmGridStore " 
              << appName.toChar() << std::endl;
    return;
    }

  app.SetParameterValue("filename","",m_GridModule->GetCurrentScopeFile());
  app.SetParameterValue("name","","\" \"");
  app.SetParameterValue("value","","\" \"");
  //std::string parse = "-p ";
  //parse += xmloutputname;
  app.SetParameterValue("parse","","1");
  app.SetParameterValue("parse.parse","",xmloutputname.c_str());
  //std::string appvarname = "-a ";
  MString appn = m_parameters[1];
  appn = appn.removeChar('$');
  appn = appn.removeChar('{');
  appn = appn.removeChar('}');
  //appvarname += appn.toChar();
  app.SetParameterValue("appname","","1");
  app.SetParameterValue("appname.appname","",appn.toChar());

  m_GridModule->AddApplication(&app);
  
}

/** Parse the XML output and set some variables */
void ScriptRunAction::ParseXMLOutput(const char* output)
{
  typedef itk::XMLBufferReader XMLParserType;
  XMLParserType::Pointer xmlReader = XMLParserType::New();
  std::string Buffer = output;
  try
    {
    xmlReader->Parse(Buffer.c_str(),Buffer.size());
    }
  catch(...)
    {
    std::cout << "Cannot parse output" << std::endl;
    return;
    }
  XMLParserType::TagVectorType tags = xmlReader->GetTags();

  XMLParserType::TagVectorType::const_iterator it = tags.begin();
  while(it != tags.end())
    {
    if(((*it).name == "Input") || ((*it).name == "Output"))
      {
      std::string name = "";
      std::string value = "";
      std::vector<XMLParserType::PairType> values = (*it).values;
      std::vector<XMLParserType::PairType>::const_iterator itValues = values.begin();
      while(itValues != values.end())
        {
        if((*itValues).first == "name")
          {
          name = (*itValues).second;
          }
        else if((*itValues).first == "value")
          {
          value = "'"+(*itValues).second+"'";
          }
        itValues++;
        }
      // We only add the output values
      if((*it).name == "Output")
        {
        MString app = m_parameters[1];
        app = app.removeChar('$');
        app = app.removeChar('{');
        app = app.removeChar('}');
        std::string fullname = app.toChar();
        fullname += "."+name;
        m_manager->SetVariable(fullname,value);
        }
      }
    it++;
    }
}

/** Execute the action */
void ScriptRunAction::Execute()
{
  if(m_GridModule)
    {
    this->GenerateCondor(m_parameters[1].toChar());
    return;
    }

  Launch m_launch;
  Timer m_timer;
  m_timer.start();
  m_launch.SetProgressManager(m_progressmanager);

  m_progressmanager->SetStatus(MString("Start ") + m_manager->Convert(m_parameters[1]).removeChar('\''));
  MString m_actioname = m_manager->ConvertExtra(m_parameters[1]);

  m_actioname = m_actioname.removeChar(' ',true);
  m_actioname = m_actioname.removeChar('\'',true);
  m_actioname = m_actioname.begin("'").rend("\\");
  if (m_actioname.length() != 0)
    m_actioname = m_actioname + 1;

  m_progressmanager->AddAction(m_actioname);
 
  m_launch.Execute(m_manager->Convert(m_parameters[1]).removeChar('\''));
  MString m_output = m_launch.GetOutput();
  MString m_error = m_launch.GetError();
  
  // Parse the output of the application and set the variables
  this->ParseXMLOutput(m_output.toChar());

  m_manager->SetVariable(m_parameters[0],MString("'") + m_output + "'");
  m_progressmanager->SetStatus(MString("Finish: Execution time %1ms").arg(m_timer.getMilliseconds()) + m_manager->Convert(m_parameters[1]));
  m_progressmanager->FinishAction(MString("Execution time: %1ms").arg(m_timer.getMilliseconds()));
  
  int m_offset = 0;
  int m_offset2 = 0;
  while (m_offset != -1)
    {
    m_offset = m_output.find("\n");
    m_progressmanager->AddOutput(m_output.begin("\n"));
    if (m_offset != -1)
      {
      m_output = m_output.end("\n")+1;
      }
    }

  m_offset = 0;
  while (m_offset != -1)
    {
    m_offset = m_error.find("\n");
    m_progressmanager->AddError(m_error.begin("\n"));
    if (m_offset != -1)
      {
      m_error = m_error.end("\n")+1;
      }
    }

  m_timer.stop();
}

} // end namespace bm
