/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptDataDirectoryAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptDataDirectoryAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

namespace bm {

/** */
ScriptDataDirectoryAction::ScriptDataDirectoryAction()
: ScriptAction()
{
}

/** */
ScriptDataDirectoryAction::~ScriptDataDirectoryAction()
{
}

/** */
bool ScriptDataDirectoryAction::TestParam(ScriptError* error,int linenumber)
{
  if( m_Parameters.size() < 1 )
    {
    error->SetError(MString("No enough parameter for DataDirectory"),linenumber);
    return false;
    }

  for( unsigned int i = 0; i < m_Parameters.size(); ++i )
    {
    m_Manager->TestConvert( m_Parameters[i], linenumber );
    }
  return true;
}

/** */
MString ScriptDataDirectoryAction::Help()
{
  return "DataDirectory(myhostnamewiththedata.com)";
}

/** */
void ScriptDataDirectoryAction::Execute()
{
  if(m_GridModule)
    {
    m_GridModule->SetDataDirectory(
      m_Parameters[0].fromVariable().toChar() );
    }
  return;
}

} // end namespace bm
