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
#include <stdio.h>
#include "FL/filename.H"

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
   if (m_parameters.size() <2)
   {
     error->SetError(MString("No enough parameter for ListFileInDir function !"),linenumber);
     return false;
   }

   m_manager->SetTestVariable(m_parameters[0]);

  for (unsigned int i=1;i<m_parameters.size();i++)
      m_manager->TestConvert(m_parameters[i],linenumber);

  return true;
}

 


void ScriptListFileInDirAction::Execute()
{
  MString m_initdir = m_manager->Convert(m_parameters[1]);
  if (m_initdir.startWith('\''))
    m_initdir = m_initdir.rbegin("'") + 1;

  MString m_filter = "*";
  if (m_parameters.size() == 3)
  {
    m_filter = m_manager->Convert(m_parameters[2]);
    if (m_filter.startWith('\''))
      m_filter = m_filter.rbegin("'") + 1;
  }

  MString m_value;

  std::string dir = m_initdir.toChar();

  if( (dir[dir.length()-1] != '/') && (dir[dir.length()-1] != '\\') )
    {
    dir += '/';
    }

  dirent** dirList;
  
  unsigned int size = fl_filename_list(dir.c_str(),&dirList);
  
  for(unsigned int i=0;i<size;i++)
    {
    if(fl_filename_match((*dirList)->d_name,m_filter.toChar())
      && !fl_filename_match((*dirList)->d_name,"./")
      && !fl_filename_match((*dirList)->d_name,"../")
      && !fl_filename_match((*dirList)->d_name,"*/")
      )
      {
      if (m_value != "")
        {
        m_value += " ";
        }
      m_value += MString("'") + MString((*dirList)->d_name) + MString("'");
      }
    dirList++;
    }

  m_manager->SetVariable(m_parameters[0],m_value);
}

} // end namespace bm
