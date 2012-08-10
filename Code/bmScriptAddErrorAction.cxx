/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptAddErrorAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptAddErrorAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"
#include "BMString.h"

namespace bm {

ScriptAddErrorAction::ScriptAddErrorAction()
: ScriptAction()
{
}

ScriptAddErrorAction::~ScriptAddErrorAction()
{
}

bool ScriptAddErrorAction::TestParam(ScriptError* ,int )
{
  return true;
}

MString ScriptAddErrorAction::Help()
{
  return "AddError( [<error description>] ...)";
}


void ScriptAddErrorAction::Execute()
{
  BMString m_error;
  for (unsigned int i=0;i<m_Parameters.size();i++)
    {
    m_error += BMString(m_Manager->Convert(m_Parameters[i])).removeAllChars('\'');
    }
  m_ProgressManager->AddError(m_error.toMString());
}

} // end namespace bm
