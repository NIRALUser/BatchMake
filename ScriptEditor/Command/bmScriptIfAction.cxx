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
    error->SetError(MString("if() takes at least 3 arguments"),linenumber);
    return false;
    }

  m_manager->SetTestVariable(m_parameters[0]);

  for (unsigned int i=1;i<m_parameters.size();i++)
    {
    m_manager->TestConvert(m_parameters[i],linenumber);
    }
   return true;
}


void ScriptIfAction::AddAction(ScriptAction* action)
{
  if (m_mode == 0)
    {
    m_thenaction.push_back(action);
    }
  else
    {
    m_elseaction.push_back(action);
    }
}

MString ScriptIfAction::Help()
{
  return "If(<$variable1> ==|<=|>=|!= <$variable2> || && <condition>) ... EndIf(<variable>)";
}

/** Return the status of the current condition*/
bool ScriptIfAction::TestCurrentCondition(unsigned int startingParameter,unsigned int conditionSize)
{
  MString left = m_parameters[startingParameter];
  if(left[0] == '$')
    {
    if ((m_manager->GetVariable(m_parameters[startingParameter])).size())
      {
      left = m_manager->GetVariable(m_parameters[startingParameter])[0];
      }
    else
      {
      left = "";
      }
    }
  
  MString right = "";

  if(conditionSize>2)
    {
    right = m_parameters[startingParameter+2];
    if(right[0] == '$')
      {
      right = m_manager->GetVariable(m_parameters[startingParameter+2])[0];
      }
    }

  // if we only have on parameter
  // something like if(${myvar}) we just test if the var exist
  if(conditionSize==1)
    {
    if (left != "")
      {
      return true;
      }
    }
  else if (m_parameters[startingParameter+1].toLower() == "==")
    {
    if (left == right)
      {
      return true;
      }
    }
  else if (m_parameters[startingParameter+1].toLower() == "!=")
    {
    if (left != right)
      {
      return true;
      }
    }
  else if (m_parameters[startingParameter+1].toLower() == ">")
    {
    if (left.toFloat() > right.toFloat())
      {
      return true;
      }
    } 
  else if (m_parameters[startingParameter+1].toLower() == "<")
    {
    if (left.toFloat() < right.toFloat())
      {
      return true;
      }
    }
  else if (m_parameters[startingParameter+1].toLower() == ">=")
    {
    if (left.toFloat() >= right.toFloat())
      {
      return true;
      }
    }
  else if (m_parameters[startingParameter+1].toLower() == "<=")
    {
    if (left.toFloat() <= right.toFloat())
      {
      return true;
      }
    }

  return false;

}

void ScriptIfAction::Execute()
{
  bool thenAction = false;

  unsigned int currentPosition = 0;
  // We loop through the conditions
  while(currentPosition < m_parameters.size())
    {
    std::string nextCondition = "";
    unsigned int endPosition = m_parameters.size();
    // Find the end of the condition
    for(unsigned int i=currentPosition;i<m_parameters.size();i++)
      {
      if(m_parameters[i] == "&&" || m_parameters[i] == "||")
        {
        nextCondition = m_parameters[i].toChar();
        endPosition = i;
        break;
        }
      }
   
    bool currentCondition = this->TestCurrentCondition(currentPosition,endPosition-currentPosition);

    // If we have already the condition we stop
    if((nextCondition.find("&&") != -1) && !currentCondition)
      {
      thenAction = false;
      break;
      }
    else if((nextCondition.find("||") != -1) && currentCondition)
      {
      thenAction = true;
      break;
      }
    else
      {
      thenAction = currentCondition;
      }

    currentPosition = endPosition+1;
    }

  if(thenAction)
    {
    for (unsigned int i=0;i<m_thenaction.size();i++)
      {
      m_thenaction[i]->Execute();
      }
    }
  else
    {
    for (unsigned int i=0;i<m_elseaction.size();i++)
      {
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
