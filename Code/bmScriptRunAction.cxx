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

/** Parse the XML output and set some variables */
void ScriptRunAction::ParseXMLOutput(const char* output)
{
  typedef itk::XMLBufferReader XMLParserType;
  XMLParserType::Pointer xmlReader = XMLParserType::New();
  std::string Buffer = output;

  // Trim the buffer
  long int begin = Buffer.find("<?xml");
  std::string endstring = "</MetaOutputFile>";
  long int end = Buffer.find(endstring);

  if(begin == -1 || end == -1)
    {
    std::cout << "ScriptRunAction::ParseXMLOutput cannot find XML description" 
              << std::endl;
    return;
    }

  Buffer = Buffer.substr(begin,end-begin+endstring.size());

  try
    {
    xmlReader->Parse(Buffer.c_str(),Buffer.size());
    }
  catch(...)
    {
    //std::cout << "Cannot parse output" << std::endl;
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
#ifdef BM_GRID
  if(m_GridModule)
    {
    this->GenerateGrid(m_parameters[1].toChar());
    return;
    }
#endif

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
  m_progressmanager->SetStatus(MString("Finish: Execution time %1ms").arg(m_timer.getMilliseconds()));
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
