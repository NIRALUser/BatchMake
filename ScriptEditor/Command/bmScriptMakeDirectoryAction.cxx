/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptMakeDirectoryAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptMakeDirectoryAction.h"
#include "itksys/SystemTools.hxx"

namespace bm {

ScriptMakeDirectoryAction::ScriptMakeDirectoryAction()
: ScriptAction()
{
}

ScriptMakeDirectoryAction::~ScriptMakeDirectoryAction()
{
}

bool ScriptMakeDirectoryAction::TestParam(ScriptError* error,int linenumber)
{
  m_manager->TestConvert(m_parameters[0],linenumber);
  if (m_parameters.size() != 1)
    {
    error->SetError(MString("No enough parameter for MakeDirectory"),linenumber);
    return false;
    }   

  return true;
}

MString ScriptMakeDirectoryAction::Help()
{
  return "MakeDirectory(<dirname>)";
}


void ScriptMakeDirectoryAction::Execute()
{
  itksys::SystemTools systemTools;
  MString dirname = this->m_manager->Convert(this->m_parameters[0]);

  // If we have a variable MakeDirectory(${dir}), get the dirname
  if(m_parameters[0][0] == '$')
    {
    dirname = m_manager->Convert(this->m_parameters[0]);
    }

  dirname = dirname.removeChar('\'');
  if (!systemTools.MakeDirectory(dirname.toChar()))
    {
    // failed to create dir.. notify manager
    if (this->m_manager)
      {
      this->m_manager->GetError()->SetError(MString("Failed to create directory ") + dirname); 
      }
    }
}

} // end namespace bm
