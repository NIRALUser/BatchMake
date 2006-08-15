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
#include "bmScriptForEachAction.h"
#include "bmScriptSequenceAction.h"
#include "bmScriptEchoAction.h"
#include "bmScriptRandomizeAction.h"
#include "bmScriptIfAction.h"
#include "bmScriptRunAction.h"
#include "bmScriptSetAction.h"
#include "bmScriptSetAppAction.h"
#include "bmScriptSetAppOptionAction.h"
#include "bmScriptGetParamAction.h"
#include "bmScriptListFileInDirAction.h"
#include "bmScriptListDirInDirAction.h"
#include "bmScriptIncAction.h"
#include "bmScriptIntAction.h"
#include "bmScriptWriteFileAction.h"
#include "bmScriptAppendFileAction.h"
#include "bmScriptDbSendValueAction.h"
#include "bmScriptDbSendFileAction.h"
#include "bmScriptDbClearAction.h"
#include "bmScriptExtractSliceAction.h"
#include "bmScriptExtractStringAction.h"
#include "bmScriptIncludeAction.h"
#include "bmScriptDashboardHostAction.h"
#include "bmScriptDashboardUserAction.h"
#include "bmScriptCreateExperimentAction.h"
#include "bmScriptCreateMethodAction.h"
#include "bmScriptDashboardSendAction.h"
#include "bmScriptDashboardNotifyAction.h"
#include "bmScriptSinAction.h"
#include "bmScriptDeleteFileAction.h"
#include "bmScriptOpenTCPSocketAction.h"
#include "bmScriptSendTCPAction.h"
#include "bmScriptCloseTCPSocketAction.h"
#include "bmScriptAddMethodInputAction.h"
#include "bmScriptAddMethodOutputAction.h"
#include "bmScriptGridDataHostAction.h"
#include "bmScriptGridOutputHostAction.h"
#include "bmScriptDataDirectoryAction.h"
#include "bmScriptOutputDirectoryAction.h"
#include "bmScriptGridSingleNodeAction.h"
#include "bmScriptAddMethodIdealOutputAction.h"
#include "bmScriptSetIdealOutputAction.h"
#include "bmScriptRegExAction.h"

#include "Timer.h"


namespace bm {

ScriptActionManager::ScriptActionManager()
{
  m_parentaction = 0;
  m_error = new ScriptError();
  m_progressmanager = new ProgressManager();
  m_ApplicationWrapperList = 0;
  m_ApplicationsList = 0;
  m_scriptpath = "";
  m_GridModule = NULL;
  m_Parser = NULL;
}

ScriptActionManager::~ScriptActionManager()
{
}

void ScriptActionManager::SetApplicationPath(MString applicationpath)
{
  m_applicationpath = applicationpath;
}

void ScriptActionManager::SetScriptPath(MString scriptpath)
{
  m_scriptpath = scriptpath;
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

ProgressManager* ScriptActionManager::GetProgressManager()
{
  return m_progressmanager;
}

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


std::vector<MString> ScriptActionManager::GetKeywordList()
{
  std::vector<MString> m_list;
  BM_NEWKEYWORD(ForEach);
  BM_NEWKEYWORD(Sequence);
  BM_NEWKEYWORD(Echo);
  BM_NEWKEYWORD(If);
  BM_NEWKEYWORD(Run);
  BM_NEWKEYWORD(Set);
  BM_NEWKEYWORD(SetApp);
  BM_NEWKEYWORD(SetAppOption);
  BM_NEWKEYWORD(ListDirInDir);
  BM_NEWKEYWORD(ListFileInDir);
  BM_NEWKEYWORD(GetParam);
  BM_NEWKEYWORD(Randomize);
  BM_NEWKEYWORD(Inc);
  BM_NEWKEYWORD(Int);
  BM_NEWKEYWORD(AppendFile);
  BM_NEWKEYWORD(WriteFile);
  BM_NEWKEYWORD(DbSendValue);
  BM_NEWKEYWORD(DbSendFile);
  BM_NEWKEYWORD(DbClear);
  BM_NEWKEYWORD(ExtractSlice);
  BM_NEWKEYWORD(ExtractString);
  BM_NEWKEYWORD(Include);
  BM_NEWKEYWORD(DashboardHost);
  BM_NEWKEYWORD(DashboardUser);
  BM_NEWKEYWORD(DashboardSend);
  BM_NEWKEYWORD(DashboardNotify);
  BM_NEWKEYWORD(CreateExperiment);
  BM_NEWKEYWORD(CreateMethod);
  BM_NEWKEYWORD(Sin);
  BM_NEWKEYWORD(DeleteFile);
  BM_NEWKEYWORD(OpenTCPSocket);
  BM_NEWKEYWORD(SendTCP);
  BM_NEWKEYWORD(CloseTCPSocket);
  BM_NEWKEYWORD(AddMethodInput);
  BM_NEWKEYWORD(AddMethodOutput);
  BM_NEWKEYWORD(GridDataHost);
  BM_NEWKEYWORD(GridOutputHost);
  BM_NEWKEYWORD(DataDirectory);
  BM_NEWKEYWORD(OutputDirectory);
  BM_NEWKEYWORD(GridSingleNode);
  BM_NEWKEYWORD(AddMethodIdealOutput);
  BM_NEWKEYWORD(SetIdealOutput);
  BM_NEWKEYWORD(RegEx);
  return m_list;
}


ScriptAction* ScriptActionManager::CreateAction(MString option)
{
  BM_NEWACTION(ForEach);
  BM_NEWACTION(Sequence);
  BM_NEWACTION(Echo);
  BM_NEWACTION(If);
  BM_NEWACTION(Run);
  BM_NEWACTION(Set);
  BM_NEWACTION(SetApp);
  BM_NEWACTION(SetAppOption);
  BM_NEWACTION(ListDirInDir);
  BM_NEWACTION(ListFileInDir);
  BM_NEWACTION(GetParam);
  BM_NEWACTION(Randomize);
  BM_NEWACTION(Inc);
  BM_NEWACTION(Int);
  BM_NEWACTION(AppendFile);
  BM_NEWACTION(WriteFile);
  BM_NEWACTION(DbSendValue);
  BM_NEWACTION(DbSendFile);
  BM_NEWACTION(DbClear);
  BM_NEWACTION(ExtractSlice);
  BM_NEWACTION(ExtractString);
  BM_NEWACTION(Include);
  BM_NEWACTION(DashboardHost);
  BM_NEWACTION(DashboardUser);
  BM_NEWACTION(DashboardSend);
  BM_NEWACTION(DashboardNotify);
  BM_NEWACTION(CreateExperiment);
  BM_NEWACTION(CreateMethod);
  BM_NEWACTION(Sin);
  BM_NEWACTION(DeleteFile);
  BM_NEWACTION(OpenTCPSocket);
  BM_NEWACTION(SendTCP);
  BM_NEWACTION(CloseTCPSocket);
  BM_NEWACTION(AddMethodInput);
  BM_NEWACTION(AddMethodOutput);
  BM_NEWACTION(GridDataHost);
  BM_NEWACTION(GridOutputHost);
  BM_NEWACTION(DataDirectory);
  BM_NEWACTION(OutputDirectory);
  BM_NEWACTION(GridSingleNode);
  BM_NEWACTION(AddMethodIdealOutput);
  BM_NEWACTION(SetIdealOutput);
  BM_NEWACTION(RegEx);
  return 0;
}

void ScriptActionManager::SetApplicationWrapperList(std::vector<ApplicationWrapper*>* applicationlist)
{
  m_ApplicationWrapperList = applicationlist;
}

void ScriptActionManager::Reset()
{
   m_variabletestlist.clear();
   m_variablelist.clear();
   SetVariable(MString("applicationpath"),MString("'") + m_applicationpath + "'");
  
   if (m_scriptpath.length() == 0)
      SetVariable(MString("scriptpath"),MString("'") + m_applicationpath + "'");
   else
      SetVariable(MString("scriptpath"),MString("'") + m_scriptpath + "'");
   
   SetTestVariable(MString("applicationpath"));
   SetTestVariable(MString("scriptpath"));

   if (m_ApplicationWrapperList)
   {
     for (unsigned int k=0;k<m_ApplicationWrapperList->size();k++)
     { 
       SetVariable((*m_ApplicationWrapperList)[k]->GetName(),MString("'") + (*m_ApplicationWrapperList)[k]->GetApplicationPath() + "'");
       SetTestVariable((*m_ApplicationWrapperList)[k]->GetName()); 
     }
   }

   m_parentaction = 0;
   for (unsigned int i=0;i<m_actionlist.size();i++)
     {
     m_actionlist[i]->Delete();
     }
  
   m_actionlist.clear();

   m_Dashboard.url = "";
   m_Dashboard.user = "";
   m_Dashboard.password = "";
   m_Dashboard.experiments.clear();
}

void ScriptActionManager::AddAction(MString option,std::vector<MString> param)
{

  if ((option == "endforeach") || (option == "endif"))
    {
    if (m_parentaction != 0)
      {
      m_parentaction = m_parentaction->GetParent();
      }
    }
  else if (option == "else")
    {
    ((ScriptIfAction*)m_parentaction)->SetMode(1);
    }
  else
    {
    ScriptAction* m_action = CreateAction(option);

    if (m_action == 0)
      {
      m_error->SetError(MString("Undefined parameter [") + option + "]" ,m_linenumber);
      }
    else if(option == "include")
      {
      m_action->SetName(option);
      m_action->SetParameters(param);
      m_action->SetParent(m_parentaction);
      m_action->SetManager(this);
      m_action->SetProgressManager(m_progressmanager);
      m_action->SetGridModule(m_GridModule);
      if (!m_action->TestParam(m_error,m_linenumber))
        {
        if (m_action->Help() != "")
          {
          m_error->SetStatus(MString("\tCommand: ") + m_action->Help());
          }
        }
      }
    else
      {
      m_action->SetName(option);
      m_action->SetParameters(param);
      m_action->SetParent(m_parentaction);
      m_action->SetManager(this);
      m_action->SetProgressManager(m_progressmanager);
      m_action->SetGridModule(m_GridModule);

      if (!m_action->TestParam(m_error,m_linenumber))
        {
        if (m_action->Help() != "")
          {
          m_error->SetStatus(MString("\tCommand: ") + m_action->Help());
          }
        }
        
        if (m_parentaction == 0)
          {
          AddAction(m_action);
          }
        else
          {
          m_parentaction->AddAction(m_action);
          }
        if ((option == "foreach")  || (option == "if"))
          {
          m_parentaction = m_action;
          }
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


/** Return the value of a given variable. The variable (name) can
 *  be ${i} $i or i */
std::vector<MString> ScriptActionManager::GetVariable(MString name)
{
  // We strip any ${} or $ as a variable name
  MString varname = name.removeChar('$');
  varname = varname.removeChar('{');
  varname = varname.removeChar('}');

  std::vector<MString> m_list;
  for (unsigned int i=0;i<m_variablelist.size();i++)
    {
    if (m_variablelist[i]->name == varname)
      {
      MString m_param = m_variablelist[i]->value;
      while (m_param != "")
        {
        m_param = m_param.removeChar(' ',true);
        m_param = m_param.removeChar('\'',true);
        MString m_value = m_param.begin("\'");
        if (m_value != "")
          {
          m_list.push_back(m_value);
          }
        m_param = m_param.end("\'");
        if (m_param.length() != 0)
          {
          m_param = m_param+1;
          }
        }
      return m_list;
      }
    }

  return m_list;
}

void ScriptActionManager::SetSocketVariable(MString name)
{
  bool m_detected = false;
  for (unsigned int i=0;i<m_variablesocketlist.size();i++)
  {
     if (m_variablesocketlist[i]->name == name)
     {
        m_detected = true;
        return;
     }
  }

  if (m_detected == false)
  {
    variablestructsocket* m_newvar = new variablestructsocket;
    m_newvar->name = name;
    m_variablesocketlist.push_back(m_newvar);
  }
}

TCPSocket* ScriptActionManager::GetVariableSocket(MString name)
{
  for (unsigned int i=0;i<m_variablesocketlist.size();i++)
  {
     if (m_variablesocketlist[i]->name == name)
     {
        return &(m_variablesocketlist[i]->socket);
     }
  }
  return NULL;
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
    std::cout << m_variablelist[i]->name.toChar() 
              << "\t" << m_variablelist[i]->value.toChar() << std::endl;
    }
}



MString ScriptActionManager::Convert(MString param)
{
  MString m_value="'";
  bool m_vardetected = false;
  MString m_var;
  bool m_isquote = false;

  for (int i=0;i<param.length();i++)
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

  for (int i=0;i<param.length();i++)
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
  for (int i=0;i<param.length();i++)
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

bool ScriptActionManager::RemoveSocket(MString name)
{
  bool found = false;
  std::vector<variablestructsocket*>::iterator it;
  it = m_variablesocketlist.begin();
  
  while( it != m_variablesocketlist.end() )
  {
     if( (*it)->name == name)
     {
        m_variablesocketlist.erase(it);
        found = true;
        return found;
     }
     ++it;
  }
  return found;
}

/** Add an experiement to the dashboard */
bool ScriptActionManager::AddDashboardExperiment(const char* var, const char* projectName, const char* experimentName)
{
  // Check that the experiment does not exist
  std::vector<DashboardExperiment>::const_iterator it = m_Dashboard.experiments.begin();
  while(it != m_Dashboard.experiments.end())
    {
    if((!strcmp((*it).project.c_str(),projectName)) 
      && (!strcmp((*it).name.c_str(),experimentName)))
      {
      return false;
      }
    it++;
    }

  DashboardExperiment exp;
  exp.variable = var;
  exp.project = projectName;
  exp.name = experimentName;
  m_Dashboard.experiments.push_back(exp);
  return true;
}

/** Add a method to an experiment */
bool ScriptActionManager::AddDashboardMethod(const char* var, const char* expvar, const char* methodName)
{
  // Check that the experiment exist
  std::vector<DashboardExperiment>::iterator it = m_Dashboard.experiments.begin();
  while(it != m_Dashboard.experiments.end())
    {
    if((!strcmp((*it).variable.c_str(),expvar)))
      {
      // Check that the method doesn't exit
      std::vector<DashboardMethod>::const_iterator itMeth = (*it).methods.begin();
      while(itMeth != (*it).methods.end())
        {
        if((!strcmp((*itMeth).expVariable.c_str(),expvar))
          && (!strcmp((*itMeth).name.c_str(),methodName))
          )
          {
          return false;
          }
        itMeth++;
        }
      DashboardMethod meth;
      meth.variable = var;
      meth.expVariable = expvar;
      meth.name = methodName;  
      (*it).methods.push_back(meth);
      return true;
      }
    it++;
    }
  return false;
}

/** Add a parameter to a specific method */
bool ScriptActionManager::AddDashboardMethodParameter(const char* var, 
                                                      const char* methVar, 
                                                      const char* name,
                                                      bool output,
                                                      bool ideal,
                                                      const char* type)
{
  // Check that the experiment exist
  std::vector<DashboardExperiment>::iterator it = m_Dashboard.experiments.begin();
  while(it != m_Dashboard.experiments.end())
    {
    std::vector<DashboardMethod>::iterator itMeth = (*it).methods.begin();
    while(itMeth != (*it).methods.end())
      {
      if((!strcmp((*itMeth).variable.c_str(),methVar)))
        {
        std::vector<DashboardMethodParameter>::const_iterator itParam 
                                                    = (*itMeth).parameters.begin();
        bool found = false;
        while(itParam != (*itMeth).parameters.end())
          {
          if(!strcmp((*itParam).variable.c_str(),var) && (*itParam).ideal == ideal)
            {
            found = true;
            break;
            }
          itParam++;
          }
        if(!found)
          {
          DashboardMethodParameter param;
          param.variable = var;
          param.method = methVar;
          param.name = name;
          param.output = output;
          param.ideal = ideal;
          if(type)
            {
            param.type = type;
            }
          (*itMeth).parameters.push_back(param);
          }
        }
      itMeth++;
      }
    it++;
    }
  return false;
}

} // end namespace bm
