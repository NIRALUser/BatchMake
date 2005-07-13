/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptIntAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptIntAction.h"

namespace bm {

ScriptIntAction::ScriptIntAction()
: ScriptAction()
{
}

ScriptIntAction::~ScriptIntAction()
{
}

bool ScriptIntAction::TestParam(ScriptError* error,int linenumber)
{
   if (m_parameters.size() <1)
   {
     error->SetError(MString("No enough parameter for Int"),linenumber);
     return false;
   }

   m_manager->SetTestVariable(m_parameters[0]);

  for (unsigned int i=1;i<m_parameters.size();i++)
      m_manager->TestConvert(m_parameters[i],linenumber);

   return true;
}

MString ScriptIntAction::Help()
{
  return "Inc(<myvar>)";
}


void ScriptIntAction::Execute()
{
  MString m_value = m_manager->Convert(m_parameters[0]);

  // if we have the variable we want the value
  if(m_parameters[0][0] != '$')
    {
    MString temp = "${";
    temp += m_parameters[0];
    temp += "}";
    m_value = m_manager->Convert(temp);
    }

  // get the value without the ''
  MString test = (MString(m_value).rbegin("'"))+1;
  int val = test.toInt();
  MString newval(val);
  m_value ="'";
  m_value += newval;
  m_value +="'";

  MString param = m_parameters[0];
  // if we have the $ we want to set the var not the value
  if(m_parameters[0][0] == '$')
    {
    param= m_parameters[0].rbegin("}")+2;
    }

  m_manager->SetVariable(param,m_value);
}

} // end namespace bm
