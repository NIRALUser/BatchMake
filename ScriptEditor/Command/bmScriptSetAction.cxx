/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptSetAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptSetAction.h"

namespace bm {

ScriptSetAction::ScriptSetAction()
: ScriptAction()
{
}

ScriptSetAction::~ScriptSetAction()
{
}

bool ScriptSetAction::TestParam(ScriptError* error,int linenumber)
{
  if (m_parameters.size() <2)
    {
    error->SetError(MString("No enough parameter for Set"),linenumber);
    return false;
    }

  m_manager->SetTestVariable(m_parameters[0]);

  for (unsigned int i=1;i<m_parameters.size();i++)
    {
    m_manager->TestConvert(m_parameters[i],linenumber);
    }
  return true;
}

MString ScriptSetAction::Help()
{
  return "Set(<variable> <name1> <name2> ...)";
}


void ScriptSetAction::Execute()
{
  MString m_value;
  MString param;
  // We check if the parameter 1 is a defined variable
  // This is used by the output application
  std::vector<MString> vars = m_manager->GetVariable(m_parameters[1]);
  if(vars.size()>0 && m_parameters[1][0] != '$' && m_parameters[1][1] != '{')
    {
    param = "${";
    param += m_parameters[1].toChar();
    param += "}\0";
    }
  else
    {
    param = m_parameters[1];
    }

  m_value = m_manager->Convert(param);

  for (unsigned int i=2;i<m_parameters.size();i++)
    {
    if (m_value != "")
      {
      m_value+= " ";
      }
    m_value+=m_manager->Convert(m_parameters[i]);
    }

  m_manager->SetVariable(m_parameters[0],m_value);
}

} // end namespace bm
