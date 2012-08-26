/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptFileExistsAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptFileExistsAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"
#include <itksys/SystemTools.hxx>

namespace bm {

ScriptFileExistsAction::ScriptFileExistsAction()
: ScriptAction()
{
}

ScriptFileExistsAction::~ScriptFileExistsAction()
{
}

bool ScriptFileExistsAction::TestParam(ScriptError* error,int linenumber)
{
   if (m_Parameters.size() < 2)
   {
     error->SetError(MString("No enough parameter for FileExists"),linenumber);
     return false;
   }

   m_Manager->SetTestVariable(m_Parameters[0]);

  for (unsigned int i=1;i<m_Parameters.size();i++)
    {
    m_Manager->TestConvert(m_Parameters[i],linenumber);
    }
   return true;
}

MString ScriptFileExistsAction::Help()
{
  return "FileExists(<myvar> <filename>)";
}


void ScriptFileExistsAction::Execute()
{
  BMString filename = m_Manager->Convert(m_Parameters[1]);
  filename = filename.removeAllChars('\'');

  MString value = "0";

  if( itksys::SystemTools::FileExists(filename.toChar()) )
    {
    value = "1";
    }

  BMString param = m_Parameters[0];
  // if we have the $ we want to set the var not the value
  if(m_Parameters[0][0] == '$')
    {
    param= m_Parameters[0].rbeginCopy("}")+2;
    }

  m_Manager->SetVariable(param,value);
}

} // end namespace bm
