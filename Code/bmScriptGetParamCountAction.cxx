/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptGetParamCountAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptGetParamCountAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

namespace bm {

ScriptGetParamCountAction::ScriptGetParamCountAction()
: ScriptAction()
{
}

ScriptGetParamCountAction::~ScriptGetParamCountAction()
{
}

bool ScriptGetParamCountAction::TestParam(ScriptError* error,int linenumber)
{
  if( m_Parameters.size() < 2 )
    {
    error->SetError( "No enough parameter for GetParamCount", linenumber );
    return false;
    }
  
  if( m_Parameters.size() > 2 )
    {
    error->SetError( "Too many parameter for GetParamCount", linenumber );
    return false;
    }
  
  m_Manager->SetTestVariable(m_Parameters[0]);

  for (unsigned int i=1;i<m_Parameters.size();i++)
    {
    m_Manager->TestConvert(m_Parameters[i],linenumber);
    }
    
  return true;
}

MString ScriptGetParamCountAction::Help()
{
  return "GetParamCount(<paramcount> <$variable>)";
}

void ScriptGetParamCountAction::Execute()
{
  std::vector<BMString> m_list = 
    m_Manager->ConvertToArray( m_Parameters[1] );
  
  BMString m_param = m_Parameters[0];
  BMString m_value = BMString( static_cast<int>(m_list.size()) ).toVariable();

  m_Manager->SetVariable( m_param, m_value );
}

} // end namespace bm
