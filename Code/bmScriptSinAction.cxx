/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptSinAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptSinAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"
#include <math.h>

namespace bm {

ScriptSinAction::ScriptSinAction()
: ScriptAction()
{
}

ScriptSinAction::~ScriptSinAction()
{
}

bool ScriptSinAction::TestParam(ScriptError* error,int linenumber)
{
  m_Manager->SetTestVariable(m_Parameters[0]);

  if (m_Parameters.size()<2)
    {
    error->SetError(MString("No enough parameter for Sin"),linenumber);
    return false;
    }

  return true;
} 

MString ScriptSinAction::Help()
{
  return "Sin(<outputvariale> <input>)";
}


void ScriptSinAction::Execute()
{
  MString m_value;
  MString test = m_Manager->Convert(m_Parameters[1]);
  //test = test.removeChar('\'');
  float m_X = test.fromVariable().toFloat();
  m_X = static_cast<float>(sin(m_X));
  //char m_text[100];
  //sprintf(m_text,"'%f'",(float)sin(m_X));
  //m_value = m_text;
  m_value = MString( m_X ).toVariable();
  m_Manager->SetVariable(m_Parameters[0],m_value);
}

} // end namespace bm
