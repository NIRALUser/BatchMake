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
#ifdef WIN32
  #include <windows.h>
#endif

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

  MString m_filter = "";
  if (m_parameters.size() == 3)
  {
    m_filter = m_manager->Convert(m_parameters[2]);
    if (m_filter.startWith('\''))
      m_filter = m_filter.rbegin("'") + 1;
  }

  MString m_value;


   #ifdef WIN32
     WIN32_FIND_DATA File;
     HANDLE hSearch;
     int re;
     hSearch=FindFirstFile((m_initdir + "/*.*").toChar(),&File);
     if (hSearch != INVALID_HANDLE_VALUE)
     {
      re=true;
      while(re)
      {
        re = FindNextFile(hSearch,&File);
        if (re)
        {
          if (MString(File.cFileName) != "..")
          {      
            if (m_filter != "")
            {
              if(MString(File.cFileName).rend(".") == m_filter)
              {
                if (m_value != "")
                  m_value += " ";
                m_value += MString("'") + MString(File.cFileName) + MString("'");
              } 
            }
            else
            {
              if (m_value != "")
                 m_value += " ";

               m_value += MString("'") + MString(File.cFileName) + MString("'");
            }
          }
        }
      }
      FindClose(hSearch);
     }
    #else
      DIR *d;
      struct dirent * dir;
      d = opendir((m_initdir).toChar());
      if (d)
      {
        while((dir = readdir(d)) != NULL)
        {

          if (m_filter != "")
          {
            if(MString(dir->d_name).rend(".") == m_filter))
            {
              if (m_value != "")
                m_value += " ";
              m_value += MString("'") + MString(dir->d_name) + MString("'");
            } 
          }
          else
          {
            if (m_value != "")
               m_value += " ";

             m_value += MString("'") + MString(dir->d_name) + MString("'");
          }

        }
      }
    #endif

  m_manager->SetVariable(m_parameters[0],m_value);
}

} // end namespace bm
