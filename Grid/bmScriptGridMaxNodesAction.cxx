/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptGridMaxNodesAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptGridMaxNodesAction.h"

namespace bm {

/** */
ScriptGridMaxNodesAction::ScriptGridMaxNodesAction()
: ScriptAction()
{
}

/** */
ScriptGridMaxNodesAction::~ScriptGridMaxNodesAction()
{
}

/** */
bool ScriptGridMaxNodesAction::TestParam(ScriptError* error,int linenumber)
{
  if (m_parameters.size() <1)
    {
    error->SetError(MString("No enough parameter for GridMaxNodes"),linenumber);
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
MString ScriptGridMaxNodesAction::Help()
{
return "GridMaxNodes(# of nodes)";
}

/** */
void ScriptGridMaxNodesAction::Execute()
{
  if(m_GridModule)
    {
    m_GridModule->SetMaxNodes(m_parameters[0].toInt());
    }
}

} // end namespace bm
