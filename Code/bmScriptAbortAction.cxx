/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptAbortAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptAbortAction.h"

namespace bm {

ScriptAbortAction::ScriptAbortAction()
: ScriptAction()
{
}

ScriptAbortAction::~ScriptAbortAction()
{
}

bool ScriptAbortAction::TestParam(ScriptError* error,int linenumber)
{
  for (unsigned int i=0;i<m_Parameters.size();i++)
    {
    m_Manager->TestConvert(m_Parameters[i],linenumber);
    }

  if (m_Parameters.size() > 0)
    {
    error->SetError(MString("Too many parameters for Abort"),linenumber);
    return false;
    }

  return true;
}

MString ScriptAbortAction::Help()
{
  return "Abort()";
}


bool ScriptAbortAction::Execute()
{
  return false;
}

} // end namespace bm
