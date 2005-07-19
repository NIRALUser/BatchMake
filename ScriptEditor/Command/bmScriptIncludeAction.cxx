/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptIncludeAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptIncludeAction.h"
#include "bmScriptParser.h"

namespace bm {

ScriptIncludeAction::ScriptIncludeAction()
: ScriptAction()
{
}

ScriptIncludeAction::~ScriptIncludeAction()
{
}

bool ScriptIncludeAction::TestParam(ScriptError* error,int linenumber)
{
   if (m_parameters.size() <1)
   {
     error->SetError(MString("No enough parameter for Include"),linenumber);
     return false;
   }

   m_manager->SetTestVariable(m_parameters[0]);

  for (unsigned int i=1;i<m_parameters.size();i++)
      m_manager->TestConvert(m_parameters[i],linenumber);

  MString filename = m_manager->Convert(m_parameters[0]);
  filename = filename.removeChar('\'');
  std::cout << "Compiling : " << filename.toChar() << std::endl;
  static_cast<ScriptParser*>(m_manager->GetParser())->Compile(filename);
  std::cout << "Compiling done" << std::endl;

  return true;
}

MString ScriptIncludeAction::Help()
{
  return "Include(<myScriptFileName>)";
}

void ScriptIncludeAction::Execute()
{
  MString filename = m_manager->Convert(m_parameters[0]);
  static_cast<ScriptParser*>(m_manager->GetParser())->Execute(filename);

  //std::cout << filename.toChar() << std::endl;
  //static_cast<ScriptParser*>(m_manager->GetParser())->AddCodeLine(filename);
  //std::cout << static_cast<ScriptParser*>(m_manager->GetParser())->GetCode()[0].toChar() << std::endl;
}

} // end namespace bm
