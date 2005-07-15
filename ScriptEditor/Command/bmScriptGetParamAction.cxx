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
   if (m_parameters.size() <3)
   {
     error->SetError(MString("No enough parameter for GetParam"),linenumber);
     return false;
   }

   m_manager->SetTestVariable(m_parameters[0]);

  for (unsigned int i=1;i<m_parameters.size();i++)
    {
    m_manager->TestConvert(m_parameters[i],linenumber);
    }
    
  /*for (unsigned int j=2;j<m_parameters.size();j++)
    if (!m_parameters[j].isInt())
    {
      error->SetError(MString("Parameter %1 should be an int for GetParam ").arg(j) + m_parameters[j],linenumber);
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
  MString m_value;
  std::vector<MString> m_list = m_manager->GetParamsFromVariable(m_manager->Convert(m_parameters[1]));

  for (unsigned int i=2;i<m_parameters.size();i++)
  {
  // if we have the variable we want the value
  MString m_param = m_parameters[i];
  if(m_parameters[i][0] == '$')
    {
    m_param = m_manager->Convert(m_parameters[i]);
    }

   m_param = m_param.removeChar('\'');

    if (m_param.toInt() >= (int)m_list.size())
    {
      m_manager->GetError()->SetStatus(MString("GetParam: Exeed value for param %1").arg(i));
      return;
    }

      if (m_value != "")
        m_value+=" ";

      m_value+= m_list[m_param.toInt()]; 
  }

  m_manager->SetVariable(m_parameters[0],m_value);
}

} // end namespace bm
