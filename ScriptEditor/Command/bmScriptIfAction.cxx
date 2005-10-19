/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptIfAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptIfAction.h"
#include <fstream>

namespace bm {

ScriptIfAction::ScriptIfAction()
: ScriptAction()
{
  m_mode = 0;
}

ScriptIfAction::~ScriptIfAction()
{
}

void ScriptIfAction::SetMode(int mode)
{
  m_mode = mode;
}

bool ScriptIfAction::TestParam(ScriptError* error,int linenumber)
{
   if (m_parameters.size() <3)
   {
     error->SetError(MString("if() takes 3 arguments"),linenumber);
     return false;
   }

   m_manager->SetTestVariable(m_parameters[0]);

  for (unsigned int i=1;i<m_parameters.size();i++)
      m_manager->TestConvert(m_parameters[i],linenumber);


   return true;
}


void ScriptIfAction::AddAction(ScriptAction* action)
{
  if (m_mode == 0)
    m_thenaction.push_back(action);
  else
    m_elseaction.push_back(action);
}

MString ScriptIfAction::Help()
{
  return "If(<$variable1> ==|<=|>=|!= <$variable2>) ... EndIf(<variable>)";
}


void ScriptIfAction::Execute()
{  
  MString left = m_parameters[0];
  if(left[0] == '$')
    {
    left = m_manager->GetVariable(m_parameters[0])[0];
    }
  
  MString right = m_parameters[2];
  if(right[0] == '$')
    {
    right = m_manager->GetVariable(m_parameters[2])[0];
    }

  // if we only have on parameter
  // something like if(${myvar}) we just test if the var exist
  if(m_parameters.size()==1)
    {
    if (left != "")
      {
      for (unsigned int i=0;i<m_thenaction.size();i++)
         m_thenaction[i]->Execute();
      }
    else
      {
      for (unsigned int i=0;i<m_elseaction.size();i++)
        m_elseaction[i]->Execute();
      }
    }

  else if (m_parameters[1].toLower() == "==")
  {
    if (left == right)
    {
      for (unsigned int i=0;i<m_thenaction.size();i++)
        m_thenaction[i]->Execute();
    }
    else
    {
      for (unsigned int i=0;i<m_elseaction.size();i++)
        m_elseaction[i]->Execute();
    }
  }

  else if (m_parameters[1].toLower() == "!=")
  {
    if (left != right)
    {
      for (unsigned int i=0;i<m_thenaction.size();i++)
        m_thenaction[i]->Execute();
    }
    else
    {
      for (unsigned int i=0;i<m_elseaction.size();i++)
        m_elseaction[i]->Execute();
    }
  }

 else if (m_parameters[1].toLower() == ">")
 {
    if (left.toFloat() > right.toFloat())
    {
      for (unsigned int i=0;i<m_thenaction.size();i++)
        m_thenaction[i]->Execute();
    }
    else
    {
      for (unsigned int i=0;i<m_elseaction.size();i++)
        m_elseaction[i]->Execute();
    }
 }
  
 else if (m_parameters[1].toLower() == "<")
 {
    if (left.toFloat() < right.toFloat())
    {
      for (unsigned int i=0;i<m_thenaction.size();i++)
        m_thenaction[i]->Execute();
    }
    else
    {
      for (unsigned int i=0;i<m_elseaction.size();i++)
        m_elseaction[i]->Execute();
    }
 }

 else if (m_parameters[1].toLower() == ">=")
 {
    if (left.toFloat() >= right.toFloat())
    {
      for (unsigned int i=0;i<m_thenaction.size();i++)
        m_thenaction[i]->Execute();
    }
    else
    {
      for (unsigned int i=0;i<m_elseaction.size();i++)
        m_elseaction[i]->Execute();
    }
 }


 else if (m_parameters[1].toLower() == "<=")
 {
    if (left.toFloat() <= right.toFloat())
    {
      for (unsigned int i=0;i<m_thenaction.size();i++)
        m_thenaction[i]->Execute();
    }
    else
    {
      for (unsigned int i=0;i<m_elseaction.size();i++)
        m_elseaction[i]->Execute();
    }
 }
}

void ScriptIfAction::Delete()
{
  for (unsigned int i=0;i<m_thenaction.size();i++)
  {
      m_thenaction[i]->Delete();
      delete m_thenaction[i];
  }

  for (unsigned int j=0;j<m_elseaction.size();j++)
  {
      m_elseaction[j]->Delete();
      delete m_elseaction[j];
  }
}

} // end namespace bm
