/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptSetIdealOutputActionGrid.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptSetIdealOutputAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

namespace bm {

/** Generate the condor script */
void ScriptSetIdealOutputAction
::GenerateGrid( const BMString& name, const BMString& value)
{
#ifdef BM_BATCHBOARD
  // We create the bmGridStore application and send it to condor
  ApplicationWrapper app;
  std::string appName = "bmGridStore";
  bool appFound = false;
  ScriptActionManager::ApplicationWrapperListType::iterator itApp =
    m_Manager->GetApplicationWrapperList()->begin();
  while (itApp != m_Manager->GetApplicationWrapperList()->end())
    {
    if( (*itApp)->GetName() == appName )
      {
      app = *(*itApp);
      appFound = true;
      break;
      }
    itApp++;
    }

  if(!appFound)
    {
    std::cout << "ScriptDashboardSendAction::GenerateCondor : Cannot find bmGridSend "
              << appName << std::endl;
    return;
    }

  app.SetParameterValue("filename", "", m_GridModule->GetCurrentScopeFile() );
  app.SetParameterValue("name",     "", name.GetConstValue() );
  app.SetParameterValue("value",    "", value.GetConstValue() );

  m_GridModule->AddApplication(&app);
#endif
}

} // end namespace bm
