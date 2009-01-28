/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptMathAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptMathAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

namespace bm {

ScriptMathAction::ScriptMathAction()
: ScriptAction()
{
}

ScriptMathAction::~ScriptMathAction()
{
}

bool ScriptMathAction::TestParam(ScriptError* error,int linenumber)
{
  if (m_Parameters.size() < 4)
    { 
    error->SetError(MString("No enough parameter for Inc"),linenumber);
    return false;
    }

  m_Manager->SetTestVariable(m_Parameters[0]);

  for (unsigned int i=1;i<m_Parameters.size();i++)
   {
   m_Manager->TestConvert(m_Parameters[i],linenumber);
   }

  return true;
}

MString ScriptMathAction::Help()
{
  return "Math(<result> <var1> <operator> <var2>)";
}


void ScriptMathAction::Execute()
{
  MString value1 = m_Manager->Convert(m_Parameters[1]);

  // if we have the variable we want the value
  if(m_Parameters[1][0] != '$')
    {
    MString temp = "${";
    temp += m_Parameters[1];
    temp += "}";
    value1 = m_Manager->Convert(temp);
    }
    
  // Second argument is the operator
  MString op = m_Parameters[2];

  MString value2 = m_Manager->Convert(m_Parameters[3]);

  // if we have the variable we want the value
  if(m_Parameters[3][0] != '$')
    {
    MString temp = "${";
    temp += m_Parameters[3];
    temp += "}";
    value2 = m_Manager->Convert(temp);
    }
  
  // get the value without the ''  
  value1 = (MString(value1).rbegin("'"))+1;
  value2 = (MString(value2).rbegin("'"))+1;
  
  if(value1.length() == 0)
    {
    value1 = m_Parameters[1];
    }
  
  if(value2.length() == 0)
    {
    value2 = m_Parameters[3];
    }
    
  float val1 = value1.toFloat();
  float val2 = value2.toFloat();
    
  float result = 0;
  
  if(op == "+")
    {
    result = val1+val2;
    }
  else if(op == "-")
    {
    result = val1-val2;
    }
  else if(op == "*")
    {
    result = val1*val2;
    }
  else if(op == "/")
    {
    result = val1/val2;
    }    
  
  MString newVal(result);
  MString value ="'";
  value += newVal;
  value +="'";

  MString param = m_Parameters[0];
  // if we have the $ we want to set the var not the value
  if(m_Parameters[0][0] == '$')
    {
    param= m_Parameters[0].rbegin("}")+2;
    }

  m_Manager->SetVariable(param,value);
}

} // end namespace bm
