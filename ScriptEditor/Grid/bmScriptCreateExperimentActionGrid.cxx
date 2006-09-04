/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptCreateExperimentActionGrid.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptCreateExperimentAction.h"
#include "HttpRequest.h"

namespace bm {

/** */
void ScriptCreateExperimentAction::GenerateGrid()
{
  // We create the bmGridSend application and send it to condor
  ApplicationWrapper app;
  MString appName = "bmGridSend";
  bool appFound = false;
  ScriptActionManager::ApplicationWrapperListType::iterator it = m_manager->GetApplicationWrapperList()->begin();
  while (it != m_manager->GetApplicationWrapperList()->end())
    {
    if(!strcmp((*it)->GetName().toChar(),appName.toChar()))
      {
      app = *(*it);
      appFound = true;
      break;
      }
    it++;
  }

  if(!appFound)
    {
    std::cout << "ScriptDashboardSendAction::GenerateCondor : Cannot find bmGridSend " 
              << appName.toChar() << std::endl;
    return;
    }

  std::string withslash = "\"";
  withslash += m_manager->GetDashboardUser();
  withslash += "\"";
  app.SetParameterValue("hostname","",m_manager->GetDashboardURL());
  app.SetParameterValue("user","",withslash);
  withslash = "\"";
  withslash += m_parameters[1].toChar();
  withslash += "\"";
  app.SetParameterValue("project","",withslash);
  app.SetParameterValue("createExperiment","","1");
  withslash = "\"";
  withslash += m_parameters[2].toChar();
  withslash += "\"";
  app.SetParameterValue("createExperiment.name","",withslash);
  if(m_parameters.size()>3)
    {
    withslash = "\"";
    withslash += m_parameters[3].toChar();
    withslash += "\"";
    app.SetParameterValue("createExperiment.description","",withslash);
    }
  m_GridModule->AddApplication(&app);
}

} // end namespace bm
