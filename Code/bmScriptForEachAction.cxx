/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptForEachAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#include "bmScriptForEachAction.h"
#include <fstream>

namespace bm {

ScriptForEachAction::ScriptForEachAction()
: ScriptAction()
{
}

ScriptForEachAction::~ScriptForEachAction()
{
}


void ScriptForEachAction::AddAction(ScriptAction* action)
{
  m_Action.push_back(action);
}

bool ScriptForEachAction::TestParam(ScriptError* error,int linenumber)
{
  if (m_parameters.size() < 2)
    {
    error->SetError(
           MString("Not enough parameters - foreach requires 2 parameters"),
           linenumber);
    return false;
    }

  for (unsigned int k=0;k<m_parameters.size();k++)
    {
    m_manager->TestConvert(m_parameters[k],linenumber);
    }

  m_manager->SetTestVariable(m_parameters[0]);

  for (unsigned int i=0;i<m_Action.size();i++)
    {
    if (!m_Action[i]->TestParam(error))
      {
      return false;
      }
    }

  return true;
}


MString ScriptForEachAction::Help()
{
  return "ForEach(<variable> <$variable>|'value1 value2 ...') ... EndForEach(<variable>)";
}

void ScriptForEachAction::CreateLoop()
{ 
  m_ForLoop.clear();
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
          {
          m_paramlist = m_paramlist + 1;
          }
        }
      else
        {
        m_value = m_paramlist.begin(" ");
        m_paramlist = m_paramlist.end(" ");
        }
        
      if (m_value.length() != 0)
        {
        m_ForLoop.push_back(MString("'") + m_value + "'");
        }
      }
    }
}

void ScriptForEachAction::Execute()
{

#ifdef BM_GRID
  // We put a grid barrier
  if(m_GridModule)
    {
    m_GridModule->SetGridBarrier();
    }
#endif

  this->CreateLoop();
  for (unsigned int loop=0;loop<m_ForLoop.size();loop++)
    {
#ifdef BM_GRID
    if(m_GridModule)
      {
      m_GridModule->SetDistributed(true);
      }
#endif
    m_manager->SetVariable(m_parameters[0],m_ForLoop[loop]); 
    for (unsigned int i=0;i<m_Action.size();i++)
      {
      if (!m_progressmanager->IsStop())
        {
        m_Action[i]->Execute();
        }
      }

    this->CreateLoop();
    }
#ifdef BM_GRID
  if(m_GridModule)
    {
    m_GridModule->SetDistributed(false);
    // Remove Grid barrier
    m_GridModule->RemoveGridBarrier();
    }
#endif
}

void ScriptForEachAction::Delete()
{
  for (unsigned int i=0;i<m_Action.size();i++)
    {
    m_Action[i]->Delete();
    delete m_Action[i];
    }
}

} // end namespace bm
