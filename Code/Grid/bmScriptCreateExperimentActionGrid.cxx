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
#include "bmScriptError.h"
#include "bmScriptActionManager.h"
#include "ApplicationWrapper.h"
#include "HttpRequest.h"

namespace bm {

/** */
void ScriptCreateExperimentAction::GenerateGrid()
{
#ifdef BM_BATCHBOARD
  // We create the bmGridSend application and send it to condor
  ApplicationWrapper app;
  std::string appName = "bmGridSend";
  bool appFound = false;
  ScriptActionManager::ApplicationWrapperListType::iterator it = m_Manager->GetApplicationWrapperList()->begin();
  while (it != m_Manager->GetApplicationWrapperList()->end())
    {
    if( (*it)->GetName() == appName )
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
              << appName << std::endl;
    return;
    }

  app.ClearParameterValues();

  std::string withslash = "\"";
  withslash += m_Manager->GetDashboardUser();
  withslash += "\"";
  app.SetParameterValue("hostname","",m_Manager->GetDashboardURL());
  app.SetParameterValue("user","",withslash);

  app.SetParameterValue("key","","1");
  withslash = "\"";
  withslash += m_Manager->GetDashboardKey();
  withslash += "\"";
  app.SetParameterValue("key","key",withslash);

  withslash = "\"";
  withslash += m_Parameters[1].toChar();
  withslash += "\"";
  app.SetParameterValue("project","",withslash);
  app.SetParameterValue("createExperiment","","1");
  withslash = "\"";
  withslash += m_Parameters[2].toChar();
  withslash += "\"";
  app.SetParameterValue("createExperiment","name",withslash);
  if(m_Parameters.size()>3)
    {
    withslash = "\"";
    withslash += m_Parameters[3].toChar();
    withslash += "\"";
    app.SetParameterValue("createExperiment","description",withslash);
    }
  m_GridModule->AddApplication(&app);
#endif
}

} // end namespace bm
