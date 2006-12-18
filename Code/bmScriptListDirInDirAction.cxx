/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptListDirInDirAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptListDirInDirAction.h"
#include "FL/filename.H"
#include <itksys/Directory.hxx>
#include <itksys/SystemTools.hxx>

namespace bm {

ScriptListDirInDirAction::ScriptListDirInDirAction()
: ScriptAction()
{
}

ScriptListDirInDirAction::~ScriptListDirInDirAction()
{
}

bool ScriptListDirInDirAction::TestParam(ScriptError* error,int linenumber)
{
   if (m_parameters.size() <2)
   {
     error->SetError(MString("No enough parameter for ListDirInDir function !"),linenumber);
     return false;
   }

   m_manager->SetTestVariable(m_parameters[0]);

  for (unsigned int i=1;i<m_parameters.size();i++)
      m_manager->TestConvert(m_parameters[i],linenumber);

  return true;
}

MString ScriptListDirInDirAction::Help()
{
  return "ListDirInDir(<dirlist> 'directory' *.* [NOOVERWRITE])";
}


void ScriptListDirInDirAction::Execute()
{
  MString m_initdir = m_manager->Convert(m_parameters[1]);
  if (m_initdir.startWith('\''))
    m_initdir = m_initdir.rbegin("'") + 1;

  MString m_filter = "*";
  if (m_parameters.size() == 3 && strcmp(m_parameters[2].toChar(),"NOOVERWRITE"))
    {
    m_filter = m_manager->Convert(m_parameters[2]);
    if (m_filter.startWith('\''))
      m_filter = m_filter.rbegin("'") + 1;

    if(m_filter[m_filter.length()-1] != '/')
      {
      m_filter += '/';
      }
    }

  // By default the value is the current value of the first variable
  MString value = "";
 
  // We do not overwrite the variable if specified
  if( (m_parameters.size() == 4 && !strcmp(m_parameters[3].toChar(),"NOOVERWRITE"))
    || (m_parameters.size() == 3 && !strcmp(m_parameters[2].toChar(),"NOOVERWRITE"))
    )
    {
    std::vector<MString> values = m_manager->GetVariable(m_parameters[0]);
    for(unsigned int i=0;i<values.size();i++)
      {
      if(i>0)
        {
        value += " ";
        }
      value += "'";
      value += values[i].toChar();
      value += "'";
      }
    }
 
  std::string dir = m_initdir.toChar();

  if( (dir[dir.length()-1] != '/') && (dir[dir.length()-1] != '\\') )
    {
    dir += '/';
    }
 
  itksys::Directory directory;

  if(!directory.Load(dir.c_str()))
    {
    m_progressmanager->AddAction("Action: ListDirInDir()");
    std::string error = dir;
    error += " is not a valid directory";
    m_progressmanager->AddError(error.c_str());
    return;
    }

  for(unsigned int i=0;i<directory.GetNumberOfFiles();i++)
    {
    std::string dname = directory.GetFile(i);
    if(fl_filename_match(dname.c_str(),m_filter.toChar())
      && fl_filename_match(dname.c_str(),"*/")
      && !fl_filename_match(dname.c_str(),"./")
      && !fl_filename_match(dname.c_str(),"../") 
      )
      {
      // Check that the value doesn't exists already
      bool exists = false;
      std::vector<MString> values = m_manager->GetVariable(m_parameters[0]);
      for(unsigned int i=0;i<values.size();i++)
        {
        if(!strcmp(values[i].toChar(),dname.c_str()))
          {
          exists = true;
          break;
          }
        }
      if(!exists)
        {
        if (value != "")
          {
          value += " ";
          }
        value += MString("'") + MString(dname) + MString("'");
        }
      }
    }
   m_manager->SetVariable(m_parameters[0],value);
}

} // end namespace bm
