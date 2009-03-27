/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptGridExecutableDirectoryAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptGridExecutableDirectoryAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

namespace bm {

/** */
ScriptGridExecutableDirectoryAction::ScriptGridExecutableDirectoryAction()
: ScriptAction()
{
}

/** */
ScriptGridExecutableDirectoryAction::~ScriptGridExecutableDirectoryAction()
{
}

/** */
bool ScriptGridExecutableDirectoryAction::TestParam(ScriptError* error,int linenumber)
{
  if (m_Parameters.size() <1)
    {
    error->SetError(MString("No enough parameter for GridExecutableDirectory"),linenumber);
    return false;
    }

  for( unsigned int i = 0; i < m_Parameters.size(); ++i )
    {
    m_Manager->TestConvert(m_Parameters[i],linenumber);
    }
  return true;
}

/** */
MString ScriptGridExecutableDirectoryAction::Help()
{
  return "GridExecutableDirectory(GridExecutableDirectory)";
}

/** */
void ScriptGridExecutableDirectoryAction::Execute()
{
  if(m_GridModule)
    {
    m_GridModule->SetGridExecutableDirectory( 
      m_Manager->Convert(m_Parameters[0]).fromVariable().toChar());
    }
}

} // end namespace bm
