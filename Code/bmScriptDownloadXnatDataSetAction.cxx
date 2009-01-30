/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptDownloadXnatDataSetAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#include "bmScriptDownloadXnatDataSetAction.h"
#include <fstream>
#include "bmScriptActionManager.h"
#include "bmScriptError.h"
#include "XcedeLib/XnatCatalog.h"

namespace bm {

ScriptDownloadXnatDataSetAction::ScriptDownloadXnatDataSetAction()
: ScriptAction()
{
}

ScriptDownloadXnatDataSetAction::~ScriptDownloadXnatDataSetAction()
{
}


bool ScriptDownloadXnatDataSetAction::TestParam(ScriptError* error,int linenumber)
{
   if (m_Parameters.size() < 2)
   {
     error->SetError(MString("No enough parameter for DownloadXcedeDataSet (requires 2 parameters min)"),linenumber);
     return false;
   }

   if (m_Parameters.size() > 4)
   {
     error->SetError(MString("Too much parameters for DownloadXcedeDataSet (requires 4 parameters max)"),linenumber);
     return false;
   }

  m_Manager->SetTestVariable(m_Parameters[0]);

  for (unsigned int i=1;i<m_Parameters.size();i++)
    {
    m_Manager->TestConvert(m_Parameters[i],linenumber);
    }

  return true;
}


MString ScriptDownloadXnatDataSetAction::Help()
{
return "DownloadXnatDataSet(<dataSet> <directory> [login] [password])";
}

void ScriptDownloadXnatDataSetAction::Execute()
{
  std::string dataSet = m_Manager->Convert(m_Parameters[0]).removeChar('\'').toChar();
  std::string directory = m_Manager->Convert(m_Parameters[1]).removeChar('\'').toChar();
  XnatCatalog xnatCatalog;

  if(m_Parameters.size() > 4)
    {
    std::string login = m_Manager->Convert(m_Parameters[2]).removeChar('\'').toChar();
    std::string password = m_Manager->Convert(m_Parameters[3]).removeChar('\'').toChar();
    xnatCatalog.DownloadXnatDatasets(dataSet, directory, login, password);
    }
  else
    {
    xnatCatalog.DownloadXnatDatasets(dataSet, directory);
    }
}

} // end namespace bm
