/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptDownloadXcedeDataSetAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#include "bmScriptDownloadXcedeDataSetAction.h"
#include <fstream>
#include "bmScriptActionManager.h"
#include "bmScriptError.h"
#include "XcedeLib/XcedeCatalog.h"

namespace bm {

ScriptDownloadXcedeDataSetAction::ScriptDownloadXcedeDataSetAction()
: ScriptAction()
{
}

ScriptDownloadXcedeDataSetAction::~ScriptDownloadXcedeDataSetAction()
{
}


bool ScriptDownloadXcedeDataSetAction::TestParam(ScriptError* error,int linenumber)
{
   if (m_Parameters.size() < 2)
   {
     error->SetError("No enough parameter for DownloadXcedeDataSet (requires 2 parameters min)",linenumber);
     return false;
   }

   if (m_Parameters.size() > 4)
   {
     error->SetError("Too much parameters for DownloadXcedeDataSet (requires 4 parameters max)",linenumber);
     return false;
   }

  m_Manager->SetTestVariable(m_Parameters[0]);

  for (unsigned int i=1;i<m_Parameters.size();i++)
    {
    m_Manager->TestConvert(m_Parameters[i],linenumber);
    }

  return true;
}


MString ScriptDownloadXcedeDataSetAction::Help()
{
  return "DownloadXcedeDataSet(<dataSet> <directory> [login] [password])";
}

void ScriptDownloadXcedeDataSetAction::Execute()
{
  std::string dataSet = m_Manager->Convert(m_Parameters[0]).fromVariable().GetValue();
  std::string directory = m_Manager->Convert(m_Parameters[1]).fromVariable().GetValue();
  XcedeCatalog xcedeCatalog;

  if(m_Parameters.size() > 2)
    {
    std::string login = m_Manager->Convert(m_Parameters[2]).fromVariable().GetValue();
    std::string password = m_Manager->Convert(m_Parameters[3]).fromVariable().GetValue();
    xcedeCatalog.DownloadXcedeDatasets(dataSet, directory, login, password);
    }
  else
    {
    xcedeCatalog.DownloadXcedeDatasets(dataSet, directory);
    }
}

} // end namespace bm
