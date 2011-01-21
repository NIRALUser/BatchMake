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
#include "bmScriptError.h"
#include "bmScriptActionManager.h"
#include "ApplicationWrapper.h"
#include <sstream>

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

  if(m_Manager->GetVariable(appvar).size()==0)
    {
    std::cout << "ScriptRunAction::GenerateGrid():" 
              << "Applications should be wrapped to be used on the grid"
              << std::endl;
    return;
    }

  // second we try to find the current variable in the list of applications
  ApplicationWrapper * application = NULL;
  BMString appName = m_Manager->GetVariable(appvar)[0].fromVariable();
  bool appFound = false;
  ScriptActionManager::ApplicationWrapperListType::iterator it = m_Manager->GetApplicationWrapperList()->begin();
  while (it != m_Manager->GetApplicationWrapperList()->end())
    {
    if( appName == (*it)->GetApplicationPath() )
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

  if(application->ParameterExists("GenerateXMLFile"))
    {
    m_GridModule->AddApplication(application);
    }
  else
    {
    // If the application support it. we force the application to write the output out as XML
    std::stringstream ss; 
    ss << application->GetName() << "." 
       << m_GridModule->GetNumberOfApplications() << ".out.xml";
    std::string xmloutputname = ss.str();
    application->SetParameterValue("GenerateXMLFile","","1");
    application->SetParameterValue("GenerateXMLFile","GenerateXMLFile",xmloutputname);

    m_GridModule->AddApplication(application);

    // We create the bmGridStore application and send it to condor
    ApplicationWrapper app;
    BMString gsName = "bmGridStore";
    bool gsFound = false;
    ScriptActionManager::ApplicationWrapperListType::iterator itApp = m_Manager->GetApplicationWrapperList()->begin();
    while (itApp != m_Manager->GetApplicationWrapperList()->end())
      {
      if( gsName == (*itApp)->GetName() )
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
                << appName.GetConstValue() << std::endl;
      return;
      }

    app.SetParameterValue("filename","",m_GridModule->GetCurrentScopeFile());
    app.SetParameterValue("name","","\" \"");
    app.SetParameterValue("value","","\" \"");
    //std::string parse = "-p ";
    //parse += xmloutputname;
    app.SetParameterValue("parse","","1");
    app.SetParameterValue("parse","parse",xmloutputname.c_str());
    //std::string appvarname = "-a ";
    BMString appn = m_Parameters[1];
    appn.removeAllChars('$');
    appn.removeAllChars('{');
    appn.removeAllChars('}');
    //appvarname += appn.toChar();
    app.SetParameterValue("appname","","1");
    app.SetParameterValue("appname","appname",appn.GetValue());
    m_GridModule->AddApplication(&app);
    }
  
}

} // end namespace bm
