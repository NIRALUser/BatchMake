/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptGetXnatURIAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#include "bmScriptGetXnatURIAction.h"
#include <fstream>
#include "XcedeLib/XnatCatalog.h"

namespace bm {

ScriptGetXnatURIAction::ScriptGetXnatURIAction()
: ScriptAction()
{
}

ScriptGetXnatURIAction::~ScriptGetXnatURIAction()
{
}

bool ScriptGetXnatURIAction::TestParam(ScriptError* error,int linenumber)
{
   if (m_Parameters.size() < 2)
     {
     error->SetError(MString("No enough parameter for GetXnatURI (requires 2 parameters)"),linenumber);
     return false;
     }

   if (m_Parameters.size() > 2)
     {
     error->SetError(MString("Too much parameters for GetXnatURI (requires 2 parameters)"),linenumber);
     return false;
     }

  m_Manager->SetTestVariable(m_Parameters[0]);

  for (unsigned int i=1;i<m_Parameters.size();i++)
    {
    m_Manager->TestConvert(m_Parameters[i],linenumber);
    }

  return true;
}


MString ScriptGetXnatURIAction::Help()
{
  return "GetXnatURI(<variable> <url>)";
}

void ScriptGetXnatURIAction::Execute()
{
  MString m_value;
  std::string dataSet = m_Manager->Convert(m_Parameters[1]).removeChar('\'').toChar();

  XnatCatalog xnatCatalog;

  m_value += "'";
  m_value += xnatCatalog.GetXnatUrl(dataSet);
  m_value += "'";

  m_Manager->SetVariable(m_Parameters[0], m_value);
}

} // end namespace bm