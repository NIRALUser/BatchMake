/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptRequirementsAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptRequirementsAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

namespace bm {

ScriptRequirementsAction::ScriptRequirementsAction()
: ScriptAction()
{
}

ScriptRequirementsAction::~ScriptRequirementsAction()
{
}

bool ScriptRequirementsAction::TestParam( ScriptError* error, int linenumber )
{
  for (unsigned int i=1;i<m_Parameters.size();i++)
    {
    m_Manager->TestConvert(m_Parameters[i],linenumber);
    }
  return true;
}

MString ScriptRequirementsAction::Help()
{
  return "Requirements([<requirement1> [ || <requirement2> ... ] } )";
}

void ScriptRequirementsAction::Execute()
{
  BMString m_value;
  BMString param;
  // We check if the parameter 1 is a defined variable
  // This is used by the output application
  std::vector<BMString> vars = m_Manager->GetVariable(m_Parameters[1]);
  if(vars.size()>0 && m_Parameters[1][0] != '$' && m_Parameters[1][1] != '{')
    {
    param = "${";
    param += m_Parameters[1];
    param += "}\0";
    }
  else
    {
    param = m_Parameters[1];
    }
  BMString requirements;
  ParametersType::const_iterator it;
  ParametersType::const_iterator end = m_Parameters.end();
  for( it = m_Parameters.begin(); it != end; ++it)
    {
    requirements += m_Manager->Convert( *it );
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
