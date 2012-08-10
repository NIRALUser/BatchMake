/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptEchoAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptEchoAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

namespace bm {

ScriptEchoAction::ScriptEchoAction()
: ScriptAction()
{
}

ScriptEchoAction::~ScriptEchoAction()
{
}

bool ScriptEchoAction::TestParam(ScriptError* ,int linenumber)
{
  for( unsigned int i = 0; i < m_Parameters.size() ; ++i )
    {
    m_Manager->TestConvert( m_Parameters[i], linenumber );
    }
  return true;
}

MString ScriptEchoAction::Help()
{
  return "Echo(<value1> [value2] ...)";
}


void ScriptEchoAction::Execute()
{
  BMString m_value;
  for (unsigned int i=0;i<m_Parameters.size();i++)
    {
    m_value += m_Manager->Convert( m_Parameters[i] ).removeAllChars('\'');
    }
  m_Manager->GetProgressManager()->DisplayOutput( m_value + "\n" );
}

} // end namespace bm
