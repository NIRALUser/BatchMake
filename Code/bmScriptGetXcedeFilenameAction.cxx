/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptGetXcedeFilenameAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#include "bmScriptGetXcedeFilenameAction.h"
#include <fstream>
#include "XcedeLib/XcedeCatalog.h"

namespace bm {

ScriptGetXcedeFilenameAction::ScriptGetXcedeFilenameAction()
: ScriptAction()
{
}

ScriptGetXcedeFilenameAction::~ScriptGetXcedeFilenameAction()
{
}


bool ScriptGetXcedeFilenameAction::TestParam(ScriptError* error,int linenumber)
{
  if (m_Parameters.size() < 2)
    {
    error->SetError(MString("No enough parameter for GetXcedeFilename (requires 2 parameters)"),linenumber);
    return false;
    }

  if (m_Parameters.size() > 2)
    {
    error->SetError(MString("Too much parameters for GetXcedeFilename (requires 2 parameters)"),linenumber);
    return false;
    }

  m_Manager->SetTestVariable(m_Parameters[0]);

  for (unsigned int i=1;i<m_Parameters.size();i++)
    {
    m_Manager->TestConvert(m_Parameters[i],linenumber);
    }

  return true;
}


MString ScriptGetXcedeFilenameAction::Help()
{
  return "GetXcedeFilename(<variable> <dataSet>)";
}

void ScriptGetXcedeFilenameAction::Execute()
{
  MString m_value;
  std::string dataSet = m_Manager->Convert(m_Parameters[1]).removeChar('\'').toChar();

  XcedeCatalog xcedeCatalog;

  m_value += "'";
  m_value += xcedeCatalog.GetXcedeFilename(dataSet);
  m_value += "'";

  m_Manager->SetVariable(m_Parameters[0], m_value);
}

} // end namespace bm
