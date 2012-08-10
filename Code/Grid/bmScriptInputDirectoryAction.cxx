/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptInputDirectoryAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptInputDirectoryAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

namespace bm {

/** */
ScriptInputDirectoryAction::ScriptInputDirectoryAction()
: ScriptAction()
{
}

/** */
ScriptInputDirectoryAction::~ScriptInputDirectoryAction()
{
}

/** */
bool ScriptInputDirectoryAction::TestParam(ScriptError* error,int linenumber)
{
  if( m_Parameters.size() < 1 )
    {
    error->SetError(MString("No enough parameter for InputDirectory"),linenumber);
    return false;
    }

  for( unsigned int i = 0; i < m_Parameters.size(); ++i )
    {
    m_Manager->TestConvert( m_Parameters[i], linenumber );
    }
  return true;
}

/** */
MString ScriptInputDirectoryAction::Help()
{
  return "InputDirectory( <directory> )";
}

/** */
void ScriptInputDirectoryAction::Execute()
{
  if(m_GridModule)
    {
    m_GridModule->SetInputDirectory(
      m_Manager->Convert(m_Parameters[0]).fromVariable().toChar() );
    }
  return;
}

} // end namespace bm
