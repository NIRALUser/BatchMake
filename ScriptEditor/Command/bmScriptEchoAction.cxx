/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptEchoAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptEchoAction.h"

namespace bm {

ScriptEchoAction::ScriptEchoAction()
: ScriptAction()
{
}

ScriptEchoAction::~ScriptEchoAction()
{
}

bool ScriptEchoAction::TestParam(ScriptError* error,int linenumber)
{
  for (unsigned int i=0;i<m_parameters.size();i++)
      m_manager->TestConvert(m_parameters[i],linenumber);
    
  return true;
}


void ScriptEchoAction::Execute()
{
  MString m_value;
  for (unsigned int i=0;i<m_parameters.size();i++)
  {
    m_value += m_manager->Convert(m_parameters[i]).removeChar('\'');
  }
  m_manager->GetError()->SetStatus(m_value);
}

} // end namespace bm
