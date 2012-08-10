/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptExecutableDirectoryAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptExecutableDirectoryAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

namespace bm {

/** */
ScriptExecutableDirectoryAction::ScriptExecutableDirectoryAction()
: ScriptAction()
{
}

/** */
ScriptExecutableDirectoryAction::~ScriptExecutableDirectoryAction()
{
}

/** */
bool ScriptExecutableDirectoryAction::TestParam(ScriptError* error,int linenumber)
{
  if (m_Parameters.size() <1)
    {
    error->SetError(MString("No enough parameter for ExecutableDirectory"),linenumber);
    return false;
    }

  for( unsigned int i = 0; i < m_Parameters.size(); ++i )
    {
    m_Manager->TestConvert(m_Parameters[i],linenumber);
    }
  return true;
}

/** */
MString ScriptExecutableDirectoryAction::Help()
{
  return "ExecutableDirectory(ExecutableDirectory)";
}

/** */
void ScriptExecutableDirectoryAction::Execute()
{
  if(m_GridModule)
    {
    m_GridModule->SetExecutableDirectory(
      m_Manager->Convert(m_Parameters[0]).fromVariable().toChar());
    }
}

} // end namespace bm
