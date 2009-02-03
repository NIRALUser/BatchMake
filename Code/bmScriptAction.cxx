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
  m_Parent = 0;
#ifdef BM_GRID
  m_GridModule = NULL;
#endif
}

ScriptAction::~ScriptAction()
{
}

void ScriptAction::SetName(const BMString& name)
{
  m_Name = name;
}

const BMString& ScriptAction::GetName()const
{
  return m_Name;
}

void ScriptAction::SetParameters( const ParametersType& parameters)
{
  /*
  m_Parameters.clear();
  for (unsigned int i=0;i<parameters.size();i++)
    {
    m_Parameters.push_back(parameters[i]);
    }
  */
  const_cast<ParametersType&>(m_Parameters) = parameters;
}


void ScriptAction::Execute()
{
}


void ScriptAction::SetParent(ScriptAction* parent)
{ 
  m_Parent = parent;
} 

ScriptAction* ScriptAction::GetParent()
{
  return m_Parent;
}

void  ScriptAction::AddAction(ScriptAction* action)
{

}

void ScriptAction::SetManager(ScriptActionManager* manager)
{
  m_Manager = manager;
}

void ScriptAction::SetProgressManager(ProgressManager* progressmanager)
{
  m_ProgressManager = progressmanager;
}


bool ScriptAction::TestParam(ScriptError* error, int linenumber)
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
