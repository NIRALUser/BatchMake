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

namespace bm {

ScriptParser::ScriptParser()
{
  m_scriptactionmanager = new ScriptActionManager();
  m_error = 0;
}

ScriptParser::~ScriptParser()
{
}

void ScriptParser::SetApplicationPath(MString applicationpath)
{
 m_applicationpath = applicationpath;
 m_scriptactionmanager->SetApplicationPath(m_applicationpath);
}


void ScriptParser::SetProgressManager(ProgressManager* progressmanager)
{
  m_scriptactionmanager->SetProgressManager(progressmanager);
}

void ScriptParser::SetError(ScriptError* error)
{
  m_scriptactionmanager->SetError(error);
  m_error = error;
}

bool  ScriptParser::Compile(MString filename)
{
  std::ifstream m_file;
  m_file.open(filename.toChar(),std::ifstream::binary);
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
    AddCodeLine(m_currentline);
  }
  m_file.close();

  Parse();
  m_scriptactionmanager->GetError()->DisplaySummary();
  
  return true;
}

bool  ScriptParser::Execute(MString filename)
{ 
  std::ifstream m_file;
  m_file.open(filename.toChar(),std::ifstream::binary);
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
    AddCodeLine(m_currentline);
  }
  m_file.close();

  
  if (Parse())
  {
    m_scriptactionmanager->Execute();
    return true;
  }
  else
    return false;
}

void ScriptParser::Load(MString filename)
{
  m_linenumber = 0;
  std::ifstream m_file;
  m_file.open(filename.toChar(),std::ifstream::binary);
  char* data = (char*)malloc(1000);
  strcpy(data,"");
  MString m_currentline;
  MString m_line;
  while(!m_file.eof())
  {
    m_file.getline(data,1000);
    m_linenumber++;

    m_currentline = data;
    if (m_currentline.startWith('#'))
    {
      //std::cout << "Comment: " <<  m_currentline.toChar() << std::endl;
    }
    else
    {
      if (m_currentline.find("(") != -1) 
        m_line = m_currentline;
      else
        m_line += m_currentline;
      
       if (m_currentline.find(")") != -1)
         Parse(m_line);
    }
  }
  m_file.close();

  m_scriptactionmanager->Execute();
//  m_scriptactionmanager->DisplayVariableList();
}


bool ScriptParser::Parse(MString line)
{
  /*if ((line.find("(") == -1) || (line.find("(") == -1)) && (m_error))
     m_error->SetError(MString("Undefined parameter"),m_linenumber);*/

  MString m_option = line.begin("(").removeChar(' ').removeChar('\t').toLower();
  if (m_option == "")
    return true;

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

  for (unsigned int i=0;i<m_param.length();i++)
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











/*
  while ((m_param != "") && (m_param != m_value))
  {
    m_param = m_param.removeChar(' ',true);
    if (m_param.startWith('\''))
    {
      m_param = m_param.removeChar('\'',true);
      m_value = MString("'") + m_param.begin("'");
      if (m_value.length() != 0)
      {
        m_value += MString("'");
        m_params.push_back(m_value);
      }

      m_param = m_param.end("'")+1;
      //m_param = m_param.end(" ");
    }
    else
    {
      m_value = m_param.begin(" ");

      if (m_value.length() != 0)
        m_params.push_back(m_value);
      m_param = m_param.end(" ");
    }
  }
*/
  return m_params;
}

void ScriptParser::AddCodeLine(MString line)
{
  m_code.push_back(line);
}


bool ScriptParser::Parse()
{
  m_scriptactionmanager->Clear();
  m_linenumber = 0;
  MString m_currentline;
  MString m_line;

  for (unsigned int i=0;i<m_code.size();i++)
  {
    m_linenumber++;
    m_currentline = m_code[i];

    if (m_currentline.startWith('#'))
    {
      //std::cout << "Comment: " <<  m_currentline.toChar() << std::endl;
    }
    else
    {
      /*if (m_currentline.find("(") != -1) 
        m_line = m_currentline;
      else*/
        m_line += " ";
        m_line += m_currentline;

       if ((m_currentline.find(")") != -1) || (i==m_code.size()-1))
       {
         if (Parse(m_line) == false)
           return false;
         m_line = "";
       }
    }
  }
//   m_scriptactionmanager->DisplayVariableList();
  if (m_scriptactionmanager->GetError()->GetError() != 0)
    return false;

  //std::cout << "End Parse" << std::endl;
  //return m_scriptactionmanager->TestParam();
  return true;
}


bool ScriptParser::Execute()
{
  m_scriptactionmanager->Execute();
  return true;
}

} // end namespace bm
