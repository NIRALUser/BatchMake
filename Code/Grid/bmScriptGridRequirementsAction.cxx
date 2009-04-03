/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptGridRequirementsAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptGridRequirementsAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

namespace bm {

ScriptGridRequirementsAction::ScriptGridRequirementsAction()
: ScriptAction()
{
}

ScriptGridRequirementsAction::~ScriptGridRequirementsAction()
{
}

bool ScriptGridRequirementsAction::TestParam( ScriptError* error, int linenumber )
{
  for (unsigned int i=1;i<m_Parameters.size();i++)
    {
    m_Manager->TestConvert(m_Parameters[i],linenumber);
    }
  return true;
}

MString ScriptGridRequirementsAction::Help()
{
  return "GridRequirements([<requirement1> [ || <requirement2> ... ] } )";
}

void ScriptGridRequirementsAction::Execute()
{
  BMString requirements;
  ParametersType::const_iterator it;
  ParametersType::const_iterator end = m_Parameters.end();
  for( it = m_Parameters.begin(); it != end; ++it)
    {
    requirements += m_Manager->Convert( *it ).fromVariable();
    requirements += " ";
    }

#ifdef BM_GRID
  if(m_GridModule)
    {
    m_GridModule->SetRequirements( requirements );
    return;
    }
#endif

}

} // end namespace bm
