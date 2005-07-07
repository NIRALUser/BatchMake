/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptExtractStringAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptExtractStringAction.h"

namespace bm {

ScriptExtractStringAction::ScriptExtractStringAction()
: ScriptAction()
{
}

ScriptExtractStringAction::~ScriptExtractStringAction()
{
}

bool ScriptExtractStringAction::TestParam(ScriptError* error,int linenumber)
{
   if (m_parameters.size() <2)
   {
     error->SetError(MString("No enough parameter for ExtractString"),linenumber);
     return false;
   }

   m_manager->SetTestVariable(m_parameters[0]);

  for (unsigned int i=1;i<m_parameters.size();i++)
      m_manager->TestConvert(m_parameters[i],linenumber);


   return true;
}

MString ScriptExtractStringAction::Help()
{
  return "Set(<output> <input> <lenght>)";
}


void ScriptExtractStringAction::Execute()
{
  MString m_value="";
  m_value = "'";
  unsigned int size = m_parameters[2].toInt();

  if(!strcmp(m_parameters[3].toChar(),"FROMEND"))
    {
    unsigned int lenght = strlen(m_manager->Convert(m_parameters[1]).toChar());
    for(unsigned int i=1;i<lenght-size-1;i++)
      {
      char v = m_manager->Convert(m_parameters[1])[i];
      m_value+=v;
      }
    }
  else
    {
    for(unsigned int i=1;i<size+1;i++)
      {
      char v = m_manager->Convert(m_parameters[1])[i];
      m_value+=v;
      }
    }
  m_value+="'";
  m_manager->SetVariable(m_parameters[0],m_value);
}

} // end namespace bm
