/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptListFileInDirAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptListFileInDirAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

#include <itksys/SystemTools.hxx>
#include <itksys/Directory.hxx>
#include <itksys/RegularExpression.hxx>

#include <stdio.h>

namespace bm {

ScriptListFileInDirAction::ScriptListFileInDirAction()
: ScriptAction()
{
}

ScriptListFileInDirAction::~ScriptListFileInDirAction()
{
}

bool ScriptListFileInDirAction::TestParam(ScriptError* error,int linenumber)
{
   if (m_Parameters.size() <2)
   {
     error->SetError(MString("No enough parameter for ListFileInDir function !"),linenumber);
     return false;
   }

   m_Manager->SetTestVariable(m_Parameters[0]);

  for (unsigned int i=1;i<m_Parameters.size();i++)
      m_Manager->TestConvert(m_Parameters[i],linenumber);

  return true;
}

 
MString ScriptListFileInDirAction::Help()
{
  return "ListFileInDir(<filelist> 'directory' *.*|*.img))";
}


void ScriptListFileInDirAction::Execute()
{
  BMString m_initdir = m_Manager->Convert(m_Parameters[1]);
  if( m_initdir.isEmpty() )
    {
    m_ProgressManager->AddError( "ListFileInDir: Directory is empty" );
    return;
    }
  if( m_initdir.isVariable() )
    {
    m_initdir = m_initdir.fromVariable();
    }

  BMString m_filter = "*";
  if (m_Parameters.size() == 3)
    {
    m_filter = m_Manager->Convert(m_Parameters[2]);
    if (m_filter.isVariable())
      {
      m_filter = m_filter.fromVariable();
      }
    }

  BMString m_value;

  std::string dir = m_initdir.GetValue();

  if( (dir[dir.length()-1] != '/') && (dir[dir.length()-1] != '\\') )
    {
    dir += '/';
    }

  // Check if the given filename is a directory
  if(!itksys::SystemTools::FileIsDirectory(dir.c_str()))
    {
    m_ProgressManager->AddAction("Action: ListFileInDir()");
    std::string error = dir;
    error += " is not a valid directory";
    m_ProgressManager->AddError(error);
    return;
    }

  itksys::Directory directory;

  if(!directory.Load(dir.c_str()))
    {
    m_ProgressManager->AddAction("Action: ListFileInDir()");
    std::string error = dir;
    error += " is not a valid directory";
    m_ProgressManager->AddError(error);
    return;
    }

  std::string regexFromWildcard = m_filter.ConvertWildcardToRegEx().GetValue();
  itksys::RegularExpression regex(regexFromWildcard.c_str());

  for(unsigned int i=0;i<directory.GetNumberOfFiles();i++)
    {
    std::string dname = directory.GetFile(i);
    std::string filename = itksys::SystemTools::GetFilenameName(dname);

    if(regex.find(dname.c_str())
      && filename != "."
      && filename != "..")
      {
      if (m_value != "")
        {
        m_value += " ";
        }
      m_value += BMString(dname).toVariable();
      }
    }

  m_Manager->SetVariable(m_Parameters[0],m_value);
}

} // end namespace bm
