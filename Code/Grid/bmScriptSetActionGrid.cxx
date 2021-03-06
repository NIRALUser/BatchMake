/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptSetActionGrid.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#include "bmScriptSetAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

namespace bm {

/** Generate the condor script */
void ScriptSetAction::GenerateGrid(std::string name,std::string value)
{
#ifdef BM_BATCHBOARD
  // We create the bmGridStore application and send it to condor
  ApplicationWrapper app;
  std::string appName = "bmGridStore";
  bool appFound = false;
  ScriptActionManager::ApplicationWrapperListType::iterator itApp = m_Manager->GetApplicationWrapperList()->begin();
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

  if(m_GridModule->HasCurrentScopeFile())
    {
    app.SetParameterValue("inputfilename","",m_GridModule->GetCurrentScopeFile());
    app.SetParameterValue("inputfilename","inputfilename",m_GridModule->GetCurrentScopeFile()); // This is a bug in MetaIO
    }
  app.SetParameterValue("filename","",m_GridModule->GetCurrentScopeFile());
  app.SetParameterValue("name","",name);
  app.SetParameterValue("value","",value);

  m_GridModule->AddApplication(&app);
#endif
}

} // end namespace bm
