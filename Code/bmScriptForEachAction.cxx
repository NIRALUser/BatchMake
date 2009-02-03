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
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

#include <fstream>
#include "BMString.h"

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
  if (m_Parameters.size() < 2)
    {
    error->SetError(
           MString("Not enough parameters - foreach requires 2 parameters"),
           linenumber);
    return false;
    }

  for (unsigned int k=0;k<m_Parameters.size();k++)
    {
    m_Manager->TestConvert(m_Parameters[k],linenumber);
    }

  m_Manager->SetTestVariable(m_Parameters[0]);

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
/*
void ScriptForEachAction::CreateLoop()
{ 
  m_ForLoop.clear();
  MString m_paramlist = m_Manager->Convert(m_Parameters[1]);
  ScriptAction::ParametersType m_params;
  MString m_value = "";

  for (unsigned int i=0; i<m_Parameters.size();i++)
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
}*/

void ScriptForEachAction::CreateLoop()
{ 
  m_ForLoop.clear();
  BMString m_paramlist( m_Manager->Convert(m_Parameters[1]) );
  //BMString m_paramlist2( m_Manager->Convert(m_Parameters[1]) );
  //BMString m_paramlist3( m_Manager->Convert(m_Parameters[1]) );
  //BMString m_paramlist4( m_Manager->Convert(m_Parameters[1]) );
  //BMString m_paramlist5( m_Manager->Convert(m_Parameters[1]) );
  //ScriptAction::ParametersType m_params;
  BMString m_value = "";

  //for (unsigned int i=0; i<m_Parameters.size();i++)
    {
    while ( (!m_paramlist.isEmpty()) && (m_paramlist != m_value))
      {
      m_paramlist.removeFirstChar(' ');

      if ( m_paramlist.startWith('\'') )
        {
        m_paramlist.removeFirstChar('\'');
        m_value = m_paramlist.beginCopy("\'");
        m_paramlist.end("\'");

        if (m_paramlist.length() != 0)
          {
          ++m_paramlist;
          }
        }
      else
        {
        m_value = m_paramlist.beginCopy(" ");
        m_paramlist.end(" ");
        }
        
      if (!m_value.isEmpty())
        {
        m_ForLoop.push_back( m_value.toVariable() );
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
    m_Manager->SetVariable(m_Parameters[0],m_ForLoop[loop]); 
    for (unsigned int i=0;i<m_Action.size();i++)
      {
      m_Action[i]->Execute();
      if (m_ProgressManager->IsStop())
        {
        break;
        }
      }
    if ( m_ProgressManager->IsStop() )
      {
      break;
      }
    this->CreateLoop();
    if ( m_ProgressManager->IsStop() )
      {
      break;
      }
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
