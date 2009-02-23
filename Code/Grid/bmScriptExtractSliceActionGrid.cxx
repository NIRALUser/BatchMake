/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptExtractSliceActionGrid.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptExtractSliceAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"
#include "ApplicationWrapper.h"
#include "SliceExtractor.h"

namespace bm {

void ScriptExtractSliceAction::GenerateGrid()
{
  // We create the bmSliceExtractor application and send it to grid
  ApplicationWrapper app;
  BMString appName = "bmSliceExtractor";
  bool appFound = false;
  ScriptActionManager::ApplicationWrapperListType::iterator itApp = m_Manager->GetApplicationWrapperList()->begin();
  while (itApp != m_Manager->GetApplicationWrapperList()->end())
    {
    if( appName == (*itApp)->GetName() )
      {
      app = *(*itApp);
      appFound = true;
      break;
      }
    itApp++;
  }

  if(!appFound)
    {
    std::cout << "ScriptExtractSliceAction::GenerateGrid : Cannot find bmSliceExtractor " 
              << appName.toChar() << std::endl;
    return;
    }

  BMString m_input = m_Manager->Convert(m_Parameters[0]).removeAllChars('\'');
  BMString m_Output = m_Manager->Convert(m_Parameters[1]).removeAllChars('\'');
  
  BMString m_orientation = "-1";
  if(m_Parameters.size()>2)
    {
    m_orientation = m_Manager->Convert(m_Parameters[2]).removeAllChars('\'');
    }

  BMString m_slice = "-1";
  if(m_Parameters.size()>3)
    {
    m_slice = m_Manager->Convert(m_Parameters[2]).removeAllChars('\'');
    }
  // m_Manager->Convert(m_Parameters[3]).removeChar('\'').latin1();

  std::string withslash = "\"";
  withslash += m_input.toChar();
  withslash += "\"";
  app.SetParameterValue("volume","",withslash);
  withslash = "\"";
  withslash += m_Output.toChar();
  withslash += "\"";
  app.SetParameterValue("slice","",withslash);
  app.SetParameterValue("orientation","",m_orientation.toChar());
  app.SetParameterValue("sliceNumber","",m_slice.toChar());

  if(m_Parameters.size()>4 && !strcmp(m_Parameters[4].toChar(),"FROM_MIDDLE"))
    {
    app.SetParameterValue("middle","","1");
    }

  m_GridModule->AddApplication(&app);
}

} // end namespace bm
