/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptMakeDirectoryAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptMakeDirectoryAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"
#include "itksys/SystemTools.hxx"

namespace bm {

ScriptMakeDirectoryAction::ScriptMakeDirectoryAction()
: ScriptAction()
{
}

ScriptMakeDirectoryAction::~ScriptMakeDirectoryAction()
{
}

bool ScriptMakeDirectoryAction::TestParam(ScriptError* error,int linenumber)
{
  m_Manager->TestConvert(m_Parameters[0],linenumber);
  if (m_Parameters.size() != 1)
    {
    error->SetError(MString("No enough parameter for MakeDirectory"),linenumber);
    return false;
    }   

  return true;
}

MString ScriptMakeDirectoryAction::Help()
{
  return "MakeDirectory(<dirname>)";
}


void ScriptMakeDirectoryAction::Execute()
{
  BMString dirname = this->m_Manager->Convert(this->m_Parameters[0]);

  // If we have a variable MakeDirectory(${dir}), get the dirname
  if(m_Parameters[0][0] == '$')
    {
    dirname = m_Manager->Convert(this->m_Parameters[0]);
    }

  dirname.removeAllChars('\'');
  if( !itksys::SystemTools::MakeDirectory(dirname.toChar()) )
    {
    // failed to create dir..
    m_ProgressManager->AddError( 
      BMString("Failed to create directory ") + dirname );
    return;
    }
}

} // end namespace bm
