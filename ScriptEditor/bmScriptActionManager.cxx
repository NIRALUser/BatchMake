/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptActionManager.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptActionManager.h"
#include "bmScriptForAction.h"
#include "bmScriptSequenceAction.h"
#include "bmScriptEchoAction.h"
#include "bmScriptRandomizeAction.h"
#include "bmScriptIfAction.h"
#include "bmScriptRunAction.h"
#include "bmScriptSetAction.h"
#include "bmScriptGetParamAction.h"
#include "bmScriptListFileInDirAction.h"

#include "Timer.h"

namespace bm {

ScriptActionManager::ScriptActionManager()
{
  m_parentaction = 0;
  m_error = new ScriptError();
  m_progressmanager = new ProgressManager();
  m_applicationlist = 0;
}

ScriptActionManager::~ScriptActionManager()
{
}


void ScriptActionManager::SetApplicationPath(MString applicationpath)
{
  m_applicationpath = applicationpath;
}


void ScriptActionManager::AddAction(ScriptAction* action)
{
  m_actionlist.push_back(action);
}

void ScriptActionManager::SetProgressManager(ProgressManager* progressmanager)
{
  if (m_progressmanager)
    delete m_progressmanager;

  m_progressmanager = progressmanager;
};

void ScriptActionManager::SetError(ScriptError* error)
{
  if (m_error != error)
    delete m_error;

  m_error = error;
}

ScriptError* ScriptActionManager::GetError()
{
  return m_error;
}


void ScriptActionManager::SetLineNumber(int linenumber)
{
  m_linenumber = linenumber;
}

ScriptAction* ScriptActionManager::CreateAction(MString option)
{
   if (option == "foreach")        return new ScriptForAction();
   if (option == "sequence")       return new ScriptSequenceAction();
   if (option == "echo")           return new ScriptEchoAction();
   if (option == "randomize")      return new ScriptRandomizeAction();
   if (option == "if")             return new ScriptIfAction();
   if (option == "run")            return new ScriptRunAction();
   if (option == "set")            return new ScriptSetAction();
   if (option == "validationdashboard")  return new ScriptAction(); 
   if (option == "getparam")  return new ScriptGetParamAction(); 
   if (option == "listfileindir")  return new ScriptListFileInDirAction(); 
   return 0;
}

void ScriptActionManager::SetApplicationWrapperList(std::vector<ApplicationWrapper*>* applicationlist)
{
  m_applicationlist = applicationlist;
}

void ScriptActionManager::Reset()
{
   m_variabletestlist.clear();
   m_variablelist.clear();
   SetVariable(MString("applicationpath"),MString("'") + m_applicationpath + "'");
   SetTestVariable(MString("applicationpath"));

   if (m_applicationlist)
   {
     for (unsigned int k=0;k<m_applicationlist->size();k++)
     { 
       SetVariable((*m_applicationlist)[k]->GetName(),MString("'") + (*m_applicationlist)[k]->GetApplicationPath() + "'");
       SetTestVariable((*m_applicationlist)[k]->GetName()); 
     }
   }

   m_parentaction = 0;
   for (unsigned int i=0;i<m_actionlist.size();i++)
   {
     m_actionlist[i]->Delete();
   }
  
   m_actionlist.clear();

}

void ScriptActionManager::AddAction(MString option,std::vector<MString> param)
{
  if ((option == "endforeach") || (option == "endif"))
  {
      if (m_parentaction != 0)
        m_parentaction = m_parentaction->GetParent();
  }
  else if (option == "else")
  {
    ((ScriptIfAction*)m_parentaction)->SetMode(1);
  }
  else
  {
      ScriptAction* m_action = CreateAction(option);

      if (m_action == 0)
        m_error->SetError(MString("Undefined parameter [") + option + "]" ,m_linenumber);
      else
      {

        m_action->SetName(option);
        m_action->SetParameters(param);
        m_action->SetParent(m_parentaction);
        m_action->SetManager(this);
        m_action->SetProgressManager(m_progressmanager);
        if (!m_action->TestParam(m_error,m_linenumber))
          if (m_action->Help() != "")
            m_error->SetStatus(MString("\tCommand: ") + m_action->Help());

        if (m_parentaction == 0)
          AddAction(m_action);
        else
          m_parentaction->AddAction(m_action);

        if ((option == "foreach")  || (option == "if"))
          m_parentaction = m_action;
      }

  }
}

void ScriptActionManager::Execute()
{
  Timer m_timer;
  m_timer.start();
  for (unsigned int i=0;i<m_actionlist.size();i++)
  {
    m_actionlist[i]->Execute();
  }
  m_progressmanager->SetFinished(MString("Total Execution time: %1ms").arg(m_timer.getMilliseconds()));
}

void ScriptActionManager::SetTestVariable(MString name)
{
  bool m_detected = false;
  for (unsigned int i=0;i<m_variabletestlist.size();i++)
  {
     if (m_variabletestlist[i] == name)
        m_detected = true;
  }

  if (m_detected == false)
    m_variabletestlist.push_back(name);

}


bool ScriptActionManager::IsTestVariable(MString name)
{
  for (unsigned int i=0;i<m_variabletestlist.size();i++)
  {
     if (m_variabletestlist[i] == name)
        return true;
  }

  return false;
}


void ScriptActionManager::SetVariable(MString name,MString value)
{
  bool m_detected = false;
  for (unsigned int i=0;i<m_variablelist.size();i++)
  {
     if (m_variablelist[i]->name == name)
     {
        m_detected = true;
        m_variablelist[i]->value = value;
     }
  }

  if (m_detected == false)
  {
    variablestruct* m_newvar = new variablestruct;
    m_newvar->name = name;
    m_newvar->value = value;
    m_variablelist.push_back(m_newvar);
  }
}

std::vector<MString> ScriptActionManager::GetVariable(MString name)
{

  std::vector<MString> m_list;
 for (unsigned int i=0;i<m_variablelist.size();i++)
 {
   if (m_variablelist[i]->name == name)
   {
     MString m_param = m_variablelist[i]->value;
     while (m_param != "")
     {
        m_param = m_param.removeChar(' ',true);
        m_param = m_param.removeChar('\'',true);
        MString m_value = m_param.begin("\'");
        if (m_value != "")
          m_list.push_back(m_value);

         m_param = m_param.end("\'");
         if (m_param.length() != 0)
           m_param = m_param+1;
     }
     return m_list;
   }
 }

  return m_list;
}

std::vector<MString> ScriptActionManager::GetParamsFromVariable(MString name)
{

  std::vector<MString> m_params;
  MString m_param = name;
  /*for (unsigned int i=0;i<m_variablelist.size();i++)
  {
   if (m_variablelist[i]->name == name)
     m_param = m_variablelist[i]->value;
  }

  if (m_param == "")
    return m_params;*/

  MString m_value = "";

  while ((m_param != "") && (m_param != m_value))
  {
    m_param = m_param.removeChar(' ',true);
    m_value = m_param.begin(" ");
    if (m_value.length() != 0)
        m_params.push_back(m_value);
      m_param = m_param.end(" ");
  }

  return m_params;
}


void ScriptActionManager::DisplayVariableList()
{
  std::cout << "Variable List" << std::endl;
  for (unsigned int i=0;i<m_variablelist.size();i++)
  {
    std::cout << m_variablelist[i]->name.toChar() <<"\t"<< m_variablelist[i]->value.toChar() << std::endl;
  }
}



MString ScriptActionManager::Convert(MString param)
{
  MString m_value="'";
  bool m_vardetected = false;
  MString m_var;
  bool m_isquote = false;

  for (unsigned int i=0;i<param.length();i++)
  {
    if (param[i] == '$')
    {
      m_vardetected=true;
      m_var = "";
    }
    else if (m_vardetected)
    {
       if ((m_var.length() == 0) && (param[i] == '{'))
       {
          m_var = "";
       }
       else if ((param[i] == ' ') || (param[i] == '}') || (param[i] == '\'')  || (i==param.length()-1))
       {
         if ((i==param.length()-1) && (param[i] != '}') && (param[i] != '\''))
          m_var+=param[i];

         m_vardetected=false;
         //MString m_variable;
         std::vector<MString> m_variable =  GetVariable(m_var);
         for (unsigned int k=0;k<m_variable.size();k++)
         {
           if (k!=0)
             m_value += "'";

           if (m_variable[k] != "null")
            m_value += m_variable[k];
           if (k != m_variable.size()-1)
              m_value += "' ";
         }

         if (param[i] == ' ')
          m_value += " ";
       }
       else
         m_var+=param[i];
    }
    else if (param[i] != '\'')
      if (param[i] == ' ')
        m_value += " ";
      else
        m_value += param[i];
  }

  m_value += "'";
  return m_value;
}


MString ScriptActionManager::ConvertExtra(MString param)
{
  MString m_value="'";
  bool m_vardetected = false;
  MString m_var;
  bool m_isquote = false;

  for (unsigned int i=0;i<param.length();i++)
  {
    if (param[i] == '$')
    {
      m_vardetected=true;
      m_var = "";
    }
    else if (m_vardetected)
    {
       if ((m_var.length() == 0) && (param[i] == '{'))
       {
          m_var = "";
       }
       else if ((param[i] == ' ') || (param[i] == '}') || (param[i] == '\'')  || (i==param.length()-1))
       {
         if ((i==param.length()-1) && (param[i] != '}') && (param[i] != '\''))
          m_var+=param[i];

         m_vardetected=false;
         //MString m_variable;
         std::vector<MString> m_variable =  GetVariable(m_var);
         for (unsigned int k=0;k<m_variable.size();k++)
         {
           if (k!=0)
             m_value += "'";

           if (m_variable[k] != "null")
            m_value += m_variable[k];
           if (k != m_variable.size()-1)
              m_value += "' ";
         }

         if (param[i] == ' ')
          m_value += "' '";
       }
       else
         m_var+=param[i];
    }
    else if (param[i] != '\'')
      if (param[i] == ' ')
        m_value += "' '";
      else
        m_value += param[i];
  }

  m_value += "'";
  return m_value;
}


bool ScriptActionManager::TestConvert(MString param,int linenumber)
{
  bool m_vardetected = false;
  MString m_var;
  for (unsigned int i=0;i<param.length();i++)
  {
    if (param[i] == '$')
    {
      m_vardetected=true;
      m_var = "";
    }
    else if (m_vardetected)
    {
       if ((m_var.length() == 0) && (param[i] == '{'))
       {
          m_var = "";
       }
       else if ((param[i] == ' ') || (param[i] == '}') || (param[i] == '\'')  || (i==param.length()-1))
       {
         if ((i==param.length()-1) && (param[i] != '}') && (param[i] != '\'')   )
          m_var+=param[i];


         m_vardetected=false;

         if (IsTestVariable(m_var) == false)
         {
           m_error->SetError(MString("Undefined variable [") + m_var + "]" ,m_linenumber);
           return false;
         }
       }
       else
         m_var+=param[i];
    }
  }

  return true;
}

bool ScriptActionManager::TestParam()
{
  bool flag = true;
  for (unsigned int i=0;i<m_actionlist.size();i++)
  {
    if (!m_actionlist[i]->TestParam(m_error))
      flag = false;

  }

  return flag;
}

} // end namespace bm

