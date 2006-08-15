/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptRegExAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptRegExAction.h"
#include <stdio.h>
#include "itksys/RegularExpression.hxx"

namespace bm {

ScriptRegExAction::ScriptRegExAction()
: ScriptAction()
{
}

ScriptRegExAction::~ScriptRegExAction()
{
}

bool ScriptRegExAction::TestParam(ScriptError* error,int linenumber)
{
   if (m_parameters.size() < 3)
   {
     error->SetError(MString("No enough parameter for RegEx function !"),linenumber);
     return false;
   }

   m_manager->SetTestVariable(m_parameters[0]);

  for (unsigned int i=1;i<m_parameters.size();i++)
    {
    m_manager->TestConvert(m_parameters[i],linenumber);
    }

  return true;
}

 
MString ScriptRegExAction::Help()
{
  return "RegEx(<newvar> <var> <RegularExpression>)";
}


void ScriptRegExAction::Execute()
{
  itksys::RegularExpression regEx;

  MString expression = m_manager->Convert(m_parameters[2]);
  expression = expression.removeChar('\'');

  MString word = m_manager->Convert(m_parameters[1]);
  word = word.removeChar('\'');

  regEx.compile(expression.toChar());
  
  if(regEx.find(word.toChar()))
    {
    std::string value = "'";
    value += regEx.match(0);
    value += "'";
    m_manager->SetVariable(m_parameters[0],value.c_str());
    }
}

} // end namespace bm
