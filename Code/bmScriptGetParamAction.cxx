/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptGetParamAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptGetParamAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

namespace bm {

ScriptGetParamAction::ScriptGetParamAction()
: ScriptAction()
{
}

ScriptGetParamAction::~ScriptGetParamAction()
{
}

bool ScriptGetParamAction::TestParam(ScriptError* error,int linenumber)
{
   if (m_Parameters.size() <3)
   {
     error->SetError(MString("No enough parameter for GetParam"),linenumber);
     return false;
   }

   m_Manager->SetTestVariable(m_Parameters[0]);

  for (unsigned int i=1;i<m_Parameters.size();i++)
    {
    m_Manager->TestConvert(m_Parameters[i],linenumber);
    }

  /*for (unsigned int j=2;j<m_Parameters.size();j++)
    if (!m_Parameters[j].isInt())
    {
      error->SetError(MString("Parameter %1 should be an int for GetParam ").arg(j) + m_Parameters[j],linenumber);
      return false;
    }
*/

   return true;
}

MString ScriptGetParamAction::Help()
{
  return "GetParam(<new variable> <$variable> <int> [int] ...)";
}




void ScriptGetParamAction::Execute()
{
  BMString m_value;
  std::vector<BMString> m_list =
    //m_Manager->GetParamsFromVariable(m_Manager->Convert(m_Parameters[1]));
    //m_Manager->ExpandParameterToArray( m_Parameters[1] );
    m_Manager->ConvertToArray( m_Parameters[1] );

  for (unsigned int i=2;i<m_Parameters.size();i++)
    {
    // if we have the variable we want the value
    BMString m_param = m_Parameters[i];
    if(m_Parameters[i][0] == '$')
      {
      m_param = m_Manager->Convert(m_Parameters[i]);
      }

    m_param.removeAllChars('\'');

    if (m_param.toInt() >= (int)m_list.size())
      {
      m_ProgressManager->AddError(
        BMString("GetParam: Exeed value for param %1").arg(i) );
      m_Manager->GetError()->SetStatus(
        BMString("GetParam: Exeed value for param %1").arg(i));
      return;
      }

     if (m_value != "")
      {
      m_value+=" ";
      }

    m_value+= m_list[m_param.toInt()];
    }

  m_Manager->SetVariable(m_Parameters[0],m_value);
}

} // end namespace bm
