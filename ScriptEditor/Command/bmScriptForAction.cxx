/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptForAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#include "bmScriptParser.h"
#include <fstream>

namespace bm {

ScriptForAction::ScriptForAction()
: ScriptAction()
{
}

ScriptForAction::~ScriptForAction()
{
}


void ScriptForAction::AddAction(ScriptAction* action)
{
  m_action.push_back(action);
}

bool ScriptForAction::TestParam(ScriptError* error,int linenumber)
{
  if (m_parameters.size() < 2)
  {
    error->SetError(MString("Not enough parameters - foreach function accepts only 2 parameters"),linenumber);
    return false;
  }

  for (unsigned int k=0;k<m_parameters.size();k++)
      m_manager->TestConvert(m_parameters[k],linenumber);

  m_manager->SetTestVariable(m_parameters[0]);
  


  for (unsigned int i=0;i<m_action.size();i++)
  {
    if (!m_action[i]->TestParam(error))
      return false;
  }

  return true;
}

void ScriptForAction::Execute()
{
  std::vector<MString> m_loopvalue;
  MString m_paramlist = m_manager->Convert(m_parameters[1]);
  ScriptAction::ParametersType m_params;
  MString m_value = "";


  for (unsigned int i=0; i<m_parameters.size();i++)
  {
     while ((m_paramlist != "") && (m_paramlist != m_value))
     {
        m_paramlist = m_paramlist.removeChar(' ',true);

        if (m_paramlist.startWith('\''))
        {
          m_paramlist = m_paramlist.removeChar('\'',true);
          m_value = m_paramlist.begin("\'");
          m_paramlist = m_paramlist.end("\'");

          if (m_paramlist.length() != 0)
            m_paramlist = m_paramlist + 1;
        }
        else
        {
          m_value = m_paramlist.begin(" ");
          m_paramlist = m_paramlist.end(" ");
        }
        
        if (m_value.length() != 0)
        {
            m_loopvalue.push_back(MString("'") + m_value + "'");
        }
     }
  }

  for (unsigned int loop=0;loop<m_loopvalue.size();loop++)
  {
    m_manager->SetVariable(m_parameters[0],m_loopvalue[loop]);
    for (unsigned int i=0;i<m_action.size();i++)
    {
      if (!m_progressmanager->IsStop())
      m_action[i]->Execute();
    }
  }
}


void ScriptForAction::Delete()
{
  for (unsigned int i=0;i<m_action.size();i++)
  {
      m_action[i]->Delete();
      delete m_action[i];
  }
}

} // end namespace bm
