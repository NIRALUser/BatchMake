/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptGetXcedeDataSetsAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#include "bmScriptGetXcedeDataSetsAction.h"
#include <fstream>
#include "bmScriptActionManager.h"
#include "bmScriptError.h"
#include "XcedeLib/XcedeCatalog.h"

namespace bm {

ScriptGetXcedeDataSetsAction::ScriptGetXcedeDataSetsAction()
: ScriptAction()
{
}

ScriptGetXcedeDataSetsAction::~ScriptGetXcedeDataSetsAction()
{
}


bool ScriptGetXcedeDataSetsAction::TestParam(ScriptError* error,int linenumber)
{
   if (m_Parameters.size() < 2)
     {
     error->SetError("No enough parameter for GetXcedeDataSets (requires 2 parameters min)",linenumber);
     return false;
     }

   if (m_Parameters.size() > 5)
     {
     error->SetError("Too much parameters for GetXcedeDataSets (requires 5 parameters max)",linenumber);
     return false;
     }

  m_Manager->SetTestVariable(m_Parameters[0]);

  for (unsigned int i=1;i<m_Parameters.size();i++)
    {
    m_Manager->TestConvert(m_Parameters[i],linenumber);
    }

  return true;
}


MString ScriptGetXcedeDataSetsAction::Help()
{
  return "GetXnatFilename(<variable> <catalog> <catalogID> [login] [password])";
}

void ScriptGetXcedeDataSetsAction::Execute()
{
  MString m_value;
  std::string catalog = m_Manager->Convert(m_Parameters[1]).fromVariable().GetValue();
  std::string catalogId = m_Manager->Convert(m_Parameters[2]).fromVariable().GetValue();
  std::vector<std::string> dataSets;

  XcedeCatalog xcedeCatalog;
  xmlDocPtr doc = xcedeCatalog.ParseDoc(catalog);

  if(m_Parameters.size() > 4)
    {
    std::string login = m_Manager->Convert(m_Parameters[3]).fromVariable().GetValue();
    std::string password = m_Manager->Convert(m_Parameters[4]).fromVariable().GetValue();
    dataSets = xcedeCatalog.GetXcedeDataSets(doc, catalogId, login, password);
    }
  else
    {
    dataSets = xcedeCatalog.GetXcedeDataSets(doc, catalogId);
    }

  for(int i=0 ; i<(int)dataSets.size() ; i++)
    {
    m_value += "'";
    m_value += dataSets[i];
    m_value += "' ";
    }

  m_Manager->SetVariable(m_Parameters[0], m_value);
}

} // end namespace bm
