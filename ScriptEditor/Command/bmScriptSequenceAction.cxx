/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptSequenceAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptSequenceAction.h"

namespace bm {

ScriptSequenceAction::ScriptSequenceAction()
: ScriptAction()
{
}

ScriptSequenceAction::~ScriptSequenceAction()
{
}

bool ScriptSequenceAction::TestParam(ScriptError* error,int linenumber)
{
   if (m_parameters.size() < 4)
   {
     error->SetError(MString("No enough parameter for Sequence"),linenumber);
     return false;
   }

   if (m_parameters.size() > 4)
   {
     error->SetError(MString("Too much parameters for Sequence"),linenumber);
     return false;
   }


   m_manager->SetTestVariable(m_parameters[0]);

  for (unsigned int j=1;j<m_parameters.size();j++)
    if (!m_parameters[j].isFloat())
    {
      error->SetError(MString("Parameter %1 should be an int for Sequence ").arg(j) + m_parameters[j],linenumber);
      return false;
    }

  for (unsigned int i=1;i<m_parameters.size();i++)
      m_manager->TestConvert(m_parameters[i],linenumber);


   return true;
}

MString ScriptSequenceAction::Help()
{
  return "Sequence(<variable> <From> <To> <Step>) - Ex: Sequence(seq 1 10 0.2)";
}


void ScriptSequenceAction::Execute()
{
  MString m_value;
  for (int i=m_parameters[1].toInt();i<m_parameters[2].toInt();i+=m_parameters[3].toInt())
  {
    m_value+= MString("%1 ").arg(i);
  }
  m_value+= m_parameters[2];
  m_manager->SetVariable(m_parameters[0],m_value);
}

} // end namespace bm
