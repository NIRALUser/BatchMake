/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptOutputDirectoryAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptOutputDirectoryAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

namespace bm {

/** */
ScriptOutputDirectoryAction::ScriptOutputDirectoryAction()
: ScriptAction()
{
}

/** */
ScriptOutputDirectoryAction::~ScriptOutputDirectoryAction()
{
}

/** */
bool ScriptOutputDirectoryAction::TestParam(ScriptError* error,int linenumber)
{
  if (m_Parameters.size() <1)
    {
    error->SetError(MString("No enough parameter for OutputDirectory"),linenumber);
    return false;
    }

  //m_Manager->SetTestVariable(m_Parameters[0]);

  for( unsigned int i = 0; i < m_Parameters.size(); ++i )
    {
    m_Manager->TestConvert(m_Parameters[i],linenumber);
    }
  return true;
}

/** */
MString ScriptOutputDirectoryAction::Help()
{
  return "OutputDirectory(outputdirectory)";
}

/** */
void ScriptOutputDirectoryAction::Execute()
{
  if(!m_GridModule)
    {
    return;
    }
  m_GridModule->SetOutputDirectory( m_Parameters[0].fromVariable().toChar() );
}

} // end namespace bm
