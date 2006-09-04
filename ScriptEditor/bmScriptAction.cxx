/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptAction.h"

namespace bm {

ScriptAction::ScriptAction()
{
  m_parent = 0;
#ifdef BM_GRID
  m_GridModule = NULL;
#endif
}

ScriptAction::~ScriptAction()
{
}

void ScriptAction::SetName(MString name)
{
  m_name = name;
}

MString ScriptAction::GetName()
{
  return m_name;
}


void ScriptAction::SetParameters(ParametersType parameters)
{
  m_parameters.clear();
  for (unsigned int i=0;i<parameters.size();i++)
    m_parameters.push_back(parameters[i]);
}


void ScriptAction::Execute()
{
}


void ScriptAction::SetParent(ScriptAction* parent)
{ 
  m_parent = parent;
} 

ScriptAction* ScriptAction::GetParent()
{
  return m_parent;
}

void  ScriptAction::AddAction(ScriptAction* action)
{

}

void ScriptAction::SetManager(ScriptActionManager* manager)
{
  m_manager = manager;
}

void ScriptAction::SetProgressManager(ProgressManager* progressmanager)
{
  m_progressmanager = progressmanager;
}


bool ScriptAction::TestParam(ScriptError* error,int linenumber)
{
  return true;
}

void ScriptAction::Delete()
{
}

MString ScriptAction::Help()
{
  return "";
}

} // end namespace bm
