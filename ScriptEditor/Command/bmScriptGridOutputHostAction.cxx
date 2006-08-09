/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptGridOutputHostAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptGridOutputHostAction.h"

namespace bm {

/** */
ScriptGridOutputHostAction::ScriptGridOutputHostAction()
: ScriptAction()
{
}

/** */
ScriptGridOutputHostAction::~ScriptGridOutputHostAction()
{
}

/** */
bool ScriptGridOutputHostAction::TestParam(ScriptError* error,int linenumber)
{
  if (m_parameters.size() <1)
    {
    error->SetError(MString("No enough parameter for GridOutputHost"),linenumber);
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
MString ScriptGridOutputHostAction::Help()
{
  return "GridOutputHost(myhostnameoutput.com)";
}

/** */
void ScriptGridOutputHostAction::Execute()
{
  m_GridModule->SetOutputHost(m_parameters[0].toChar());
}

} // end namespace bm
