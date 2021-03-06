/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptSetAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptSetAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

namespace bm {

ScriptSetAction::ScriptSetAction()
: ScriptAction()
{
}

ScriptSetAction::~ScriptSetAction()
{
}

bool ScriptSetAction::TestParam(ScriptError* error,int linenumber)
{
  if( m_Parameters.size() < 2 )
    {
    error->SetError(MString("No enough parameter for Set"),linenumber);
    return false;
    }

  m_Manager->SetTestVariable(m_Parameters[0]);

  for (unsigned int i=1;i<m_Parameters.size();i++)
    {
    m_Manager->TestConvert(m_Parameters[i],linenumber);
    }
  return true;
}

MString ScriptSetAction::Help()
{
  return "Set(<variable> <name1> <name2> ...)";
}

void ScriptSetAction::Execute()
{
  BMString m_value;
  BMString param;
  // Check if the parameter 1 is a defined variable
  // This is used by the output application
  if( m_Manager->IsTestVariable( m_Parameters[1] ) &&
      m_Parameters[1][0] != '$' &&
      m_Parameters[1][1] != '{')
    {
    // m_Parameters[1] is a variable, but the user forgot to
    // write it between ${ and }. We do it for him
    param = "${";
    param += m_Parameters[1];
    param += "}\0";
    }
  else
    {
    param = m_Parameters[1];
    }

  m_value = m_Manager->Convert(param);

  for( unsigned int i = 2; i < m_Parameters.size(); ++i)
    {
    if (m_value != "")
      {
      m_value+= " ";
      }
    m_value += m_Manager->Convert(m_Parameters[i]);
    }
#ifdef VERBOSE
  std::cout<< "Set: " << m_Parameters[0].GetConstValue() << " <- " << m_value.GetConstValue() << "(" << param.GetConstValue() << ")" << std::endl;
#endif
  m_Manager->SetVariable(m_Parameters[0],m_value);

#ifdef BM_GRID
  // If we are on the grid we use the bmGridStore to store the variable
  if(m_GridModule)
    {
    this->GenerateGrid( m_Parameters[0].GetConstValue(),
                        m_value.GetConstValue() );
    return;
    }
#endif

}

} // end namespace bm
