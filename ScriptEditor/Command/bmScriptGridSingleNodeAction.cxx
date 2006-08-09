/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptGridSingleNodeAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptGridSingleNodeAction.h"

namespace bm {

/** */
ScriptGridSingleNodeAction::ScriptGridSingleNodeAction()
: ScriptAction()
{
}

/** */
ScriptGridSingleNodeAction::~ScriptGridSingleNodeAction()
{
}

/** */
bool ScriptGridSingleNodeAction::TestParam(ScriptError* error,int linenumber)
{
  if (m_parameters.size() <1)
    {
    error->SetError(MString("No enough parameter for GridSingleNode"),linenumber);
    return false;
    }

  m_manager->SetTestVariable(m_parameters[0]);

  for (unsigned int i=1;i<m_parameters.size();i++)
    {
    m_manager->TestConvert(m_parameters[i],linenumber);
    }
  return true;
}

/** */
MString ScriptGridSingleNodeAction::Help()
{
  return "GridSingleNode(on/off)";
}

/** */
void ScriptGridSingleNodeAction::Execute()
{
  if(!strcmp(m_parameters[0].toChar(),"on"))
    {
    m_GridModule->SetSingleNode(true);
    }
  else
    {
    m_GridModule->SetSingleNode(false);
    }
}

} // end namespace bm
