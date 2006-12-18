/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptParser.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptParser.h"
#include <fstream>

#ifdef WIN32
  #include <windows.h>
#else
  #include <dirent.h>
#endif

namespace bm {

ScriptParser::ScriptParser()
{
  m_scriptactionmanager = new ScriptActionManager();
  m_scriptactionmanager->SetParser(this);
  m_error = 0;
  m_applicationlist = 0;
}

ScriptParser::~ScriptParser()
{
  delete m_scriptactionmanager;
  delete m_applicationlist;
  delete m_error;
}

void ScriptParser::SetApplicationPath(MString applicationpath)
{
 m_applicationpath = applicationpath;
 m_scriptactionmanager->SetApplicationPath(m_applicationpath);
 LoadWrappedApplication(m_applicationpath);
}

void ScriptParser::SetScriptPath(MString scriptpath)
{
   m_scriptactionmanager->SetScriptPath(scriptpath.rbegin("/"));
}



void  ScriptParser::LoadWrappedApplication(MString applicationpath) 
{
  if(m_applicationlist)
    {
    delete m_applicationlist;
    }
  m_applicationlist = new std::vector<ApplicationWrapper*>;

  #ifdef WIN32
   WIN32_FIND_DATA File;
   HANDLE hSearch;
   int re;
   hSearch=FindFirstFile((applicationpath+ "/Applications/*.*").toChar(),&File);
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
          ApplicationWrapper* m_newapplication = new ApplicationWrapper();
          m_newapplication->Load(applicationpath + "/Applications/" + MString(File.cFileName));
          m_applicationlist->push_back(m_newapplication);
        }
      }
    }
    FindClose(hSearch);
   }
  #else
    DIR *d;
    struct dirent * dir;
    d = opendir((applicationpath + "/Applications/").toChar());
    if (d)
    {
      while((dir = readdir(d)) != NULL)
      {
       if (MString(dir->d_name).rend(".") == ".bmm")
       {
          ApplicationWrapper* m_newapplication = new ApplicationWrapper();  
          m_newapplication->Load(applicationpath + "/Applications/" + MString(dir->d_name));
          m_applicationlist->push_back(m_newapplication);
       }
      }
    }
  #endif

 m_scriptactionmanager->SetApplicationWrapperList(m_applicationlist);

}

std::vector<ApplicationWrapper*>* ScriptParser::GetApplicationList()
{
  return m_applicationlist;
}

void ScriptParser::SetProgressManager(ProgressManager* progressmanager)
{
  m_scriptactionmanager->SetProgressManager(progressmanager);
}

void ScriptParser::SetError(ScriptError* error)
{
  m_error = error;
  m_scriptactionmanager->SetError(error);
}

void  ScriptParser::RemoveCodeLine(unsigned int line)
{
  std::vector<MString>::iterator it = m_code.begin();
  for(unsigned int i=0;i<line;i++)
    {
    it++;
    }   
  m_code.erase(it);
}


bool  ScriptParser::Compile(MString filename,unsigned long pos,bool isInclude)
{
  std::ifstream m_file;
  m_file.open(filename.toChar(),std::ifstream::binary);
  if(!m_file.good())
    {
    std::cout << "Cannot open file: " << filename.toChar() << std::endl;
    return false;
    }
  unsigned long position = pos;
  char* data = (char*)malloc(1000);
  strcpy(data,"");
  MString m_currentline;
  while(!m_file.eof())
    {
    m_file.getline(data,1000);
    if (data[strlen(data)-1] == '\r')
      {
      data[strlen(data)-1] = '\0';
      }
    m_currentline = data;
    if(isInclude && position == 0)
      {
      std::vector<MString>::iterator it = m_code.begin();
      m_code.insert(it,m_currentline);
      }
    else
      {
      this->AddCodeLine(m_currentline,position);
      }
    position++;
    }

  delete data;
  m_file.close();

  if(!isInclude)
    {
    this->Parse();
    m_scriptactionmanager->GetError()->DisplaySummary();
    }

  return true;
}

bool  ScriptParser::Execute(MString filename,unsigned long pos)
{ 
  std::ifstream m_file;
  m_file.open(filename.toChar(),std::ifstream::binary);
  if(!m_file.good())
    {
    std::cout << "Cannot open file: " << filename.toChar() << std::endl;
    return false;
    }
  char* data = new char[1000];
  strcpy(data,"");
  MString m_currentline;
  unsigned long position = pos;
  while(!m_file.eof())
    {
    m_file.getline(data,1000);
   if (strlen(data)>0 && data[strlen(data)-1] == '\r')
      {
      data[strlen(data)-1] = '\0';
      }
   m_currentline = data;
   this->AddCodeLine(m_currentline,position);

   position++;
   }
  m_file.close();

  delete [] data;
 
  // TO REMOVE
  return true;
 
  if (this->Parse())
    {
    m_scriptactionmanager->Execute();
    return true;
    }
  else
    {
    return false;
    }
}

void ScriptParser::Load(MString filename)
{
  SetScriptPath(filename);
  m_linenumber = 0;
  std::ifstream m_file;
  m_file.open(filename.toChar(),std::ifstream::binary);
  char* data = (char*)malloc(1000);
  strcpy(data,"");
  MString m_currentline;
  MString m_line;
  bool inComment = false;
  while(!m_file.eof())
  {
    m_file.getline(data,1000);
    m_linenumber++;

    m_currentline = data;
    if (m_currentline.startWith('#'))
      {
      //Comment
      }
    else if (m_currentline.find("/*") != -1)
      {
      inComment = true;
      }
    else if (m_currentline.find("*/") != -1)
      {
      inComment = false;
      }
    else
    {
      if(!inComment)
        {
        if (m_currentline.find("(") != -1) 
          m_line = m_currentline;
        else
          m_line += m_currentline;
      
         if (m_currentline.find(")") != -1)
           Parse(m_line);
        }
    }
  }
  m_file.close();

  m_scriptactionmanager->Execute();
}


bool ScriptParser::Parse(MString line)
{
  MString m_option = line.begin("(").removeChar(' ').removeChar('\t').toLower();
  if (m_option == "")
    {
    return true;
    }

  return AddOption(m_option,line.end("("));
}


bool ScriptParser::AddOption(MString option, MString param)
{
  if (CheckOption(param))
  {
    m_scriptactionmanager->SetLineNumber(m_linenumber);
    m_scriptactionmanager->AddAction(option,GetParams(param));
    return true;
  }
  return false;
}

bool ScriptParser::CheckOption(MString param)
{
  if (param.count('(') != param.count(')'))
  {
     if (m_error)
        m_error->SetError(MString("Missing '(' or ')'"),m_linenumber);
     return false;
  }
 
  return true;
}

ScriptAction::ParametersType ScriptParser::GetParams(MString param)
{
  ScriptAction::ParametersType m_params;
  MString m_param = param.removeChar('(');
  m_param = m_param.begin(")");
  m_param = m_param.removeChar(')');
  MString m_value = "";
  bool m_isquote = false;

  for (int i=0;i<m_param.length();i++)
  {
      if (m_param[i] == '\'')
      {
        m_isquote=!m_isquote;
      }
    
      if ((m_isquote) && (m_param[i] != '\''))
        m_value += m_param[i];
      
      if ((!m_isquote) && (m_param[i] != '\'') && (m_param[i] != ' '))
        m_value += m_param[i];

      if (((m_param[i] == ' ') && (m_value != "") && (!m_isquote)) || (i==m_param.length()-1))
      {
       // m_value = MString("'") + m_value + "'";
        m_params.push_back(m_value);
        m_value = "";
      }
   }
  return m_params;
}


void ScriptParser::Reset()
{
  m_scriptactionmanager->Reset();
  m_code.clear();
}


void ScriptParser::AddCodeLine(MString line,unsigned long pos)
{
  if(pos==0)
    {
    m_code.push_back(line);
    return;
    }
  std::vector<MString>::iterator it = m_code.begin();
  for(unsigned long i=0;i<pos;i++)
    {
    it++;
    }
  m_code.insert(it,line);
}


bool ScriptParser::Parse()
{
  m_scriptactionmanager->Reset();
  m_linenumber = 0;
  MString m_currentline;
  MString m_line;
  bool inComment = false;

  for (unsigned int i=0;i<m_code.size();i++)
  {
    m_linenumber++;
    m_currentline = m_code[i];
      
    MString optionName = m_currentline.begin("(").removeChar(' ').removeChar('\t').toLower();

    if (m_currentline.startWith('#'))
      {
      //Comments
      }
    else if (m_currentline.find("/*") != -1 )
      {
      inComment = true;
      }
    else if (m_currentline.find("*/")!= -1 )
      {
      inComment = false;
      }
    else if (optionName == "include")
      {
      this->Parse(m_currentline);
      this->Parse();
      return true;
      }
    else
    {
    if(!inComment)
      {
      m_line += " ";
      m_line += m_currentline;

      if ((m_currentline.find(")") != -1) || (i==m_code.size()-1))
        {
        if (this->Parse(m_line) == false)
          {
          return false;
          }
        m_line = "";
        }
      }
    }
  }

  if (m_scriptactionmanager->GetError()->GetError() != 0)
    return false;

  return true;
}


bool ScriptParser::Execute()
{
  m_scriptactionmanager->Execute();
  return true;
}

} // end namespace bm
