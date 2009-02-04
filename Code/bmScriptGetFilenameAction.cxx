/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptGetFilenameAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptGetFilenameAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"
#include <itksys/SystemTools.hxx>

namespace bm {

bool ScriptGetFilenameAction::TestParam(ScriptError* error,int linenumber)
{
  m_Manager->SetTestVariable(m_Parameters[0]);

  if (m_Parameters.size()<3)
    {
    error->SetError(MString("No enough parameter for GetFilename"),linenumber);
    return false;
    }

  return true;
} 

MString ScriptGetFilenameAction::Help()
{
  return "GetFilename(<outputvariale> <input> [NAME PATH PARENT_PATH EXTENSION NAME_WITHOUT_EXTENSION])";
}


void ScriptGetFilenameAction::Execute()
{
  BMString input = m_Manager->Convert(m_Parameters[1]).fromVariable();
  BMString option = m_Manager->Convert(m_Parameters[2]).toLower();

  MString value = "'";
  if( option == "'name'" )
    {
    value += itksys::SystemTools::GetFilenameName(input.toChar());
    }
  else if( option == "'path'" )
    {
    value += itksys::SystemTools::GetFilenamePath(input.toChar());
    }
  else if( option == "'name_without_extension'" )
    {
    value += itksys::SystemTools::GetFilenameWithoutExtension(input.toChar());
    }
  else if( option == "'parent_path'" )
    {
    std::string currentPath = itksys::SystemTools::GetFilenamePath(input.toChar());
    value += itksys::SystemTools::GetFilenamePath(currentPath);
    }
  else if( option == "'extension'" )
    {
    value += itksys::SystemTools::GetFilenameExtension(input.toChar());
    }
  else
    {
    //std::cout << "ScriptGetFilenameAction: " << option.toChar() << " not defined!" << std::endl;
    m_ProgressManager->AddError( 
      BMString("ScriptGetFilenameAction: ") + option + " not defined!" );
    }
  value += "'";
  m_Manager->SetVariable(m_Parameters[0],value);
}

} // end namespace bm
