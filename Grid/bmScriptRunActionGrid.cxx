/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptRunActionGrid.cxx
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
/*#include "bmLaunch.h"
#include "Timer.h"
#include "XMLBufferReader.h"
*/
namespace bm {


/** Generate the condor script */
void ScriptRunAction::GenerateGrid(const char* appname)
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

} // end namespace bm