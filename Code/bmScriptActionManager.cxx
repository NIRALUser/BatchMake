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
#include "bmScriptForNFoldAction.h"
#include "bmScriptSequenceAction.h"
#include "bmScriptEchoAction.h"
#include "bmScriptRandomizeAction.h"
#include "bmScriptIfAction.h"
#include "bmScriptRunAction.h"
#include "bmScriptSetAction.h"
#include "bmScriptSetAppAction.h"
#include "bmScriptSetAppOptionAction.h"
#include "bmScriptSetAppFlagAction.h"
#include "bmScriptGetParamAction.h"
#include "bmScriptGetParamCountAction.h"
#include "bmScriptListFileInDirAction.h"
#include "bmScriptListDirInDirAction.h"
#include "bmScriptIncAction.h"
#include "bmScriptIntAction.h"
#include "bmScriptWriteFileAction.h"
#include "bmScriptAppendFileAction.h"
#include "bmScriptExtractSliceAction.h"
#include "bmScriptExtractStringAction.h"
#include "bmScriptIncludeAction.h"
#include "bmScriptSinAction.h"
#include "bmScriptCopyFileAction.h"
#include "bmScriptDeleteFileAction.h"
#include "bmScriptRegExAction.h"
#include "bmScriptMakeDirectoryAction.h"
#include "bmScriptGetTimeAction.h"
#include "bmScriptGetCurrentDateTimeAction.h"
#include "bmScriptGlobAction.h"
#include "bmScriptGetFilenameAction.h"
#include "bmScriptConvertImageAction.h"
#include "bmScriptFileExistsAction.h"
#include "bmScriptGetListSizeAction.h"
#include "bmScriptExitAction.h"
#include "bmScriptExitOnErrorAction.h"
#include "bmScriptAddErrorAction.h"
#include "bmScriptGetErrorCountAction.h"
#include "bmScriptClearErrorsAction.h"
#include "bmScriptMathAction.h"
#include "BMString.h"

#ifdef BM_GRID
  #include "bmScriptGridSingleNodeAction.h"
  #include "bmScriptGridDataHostAction.h"
  #include "bmScriptGridOutputHostAction.h"
  #include "bmScriptGridTempDirectoryAction.h"
  #include "bmScriptInputDirectoryAction.h"
  #include "bmScriptOutputDirectoryAction.h"
  #include "bmScriptExecutableDirectoryAction.h"
  #include "bmScriptWorkingDirectoryAction.h"
  #include "bmScriptGridMaxNodesAction.h"
  #include "bmScriptGridSetGroupingAction.h"
  #include "bmScriptGridTransferFileAction.h"
  #include "bmScriptGridRequirementsAction.h"
  #include "bmScriptGridOwnerAction.h"
#endif

#ifdef BM_DASHBOARD
  #include "bmScriptDbSendValueAction.h"
  #include "bmScriptDbSendFileAction.h"
  #include "bmScriptDbClearAction.h"
  #include "bmScriptDashboardHostAction.h"
  #include "bmScriptDashboardUserAction.h"
  #include "bmScriptDashboardKeyAction.h"
  #include "bmScriptCreateExperimentAction.h"
  #include "bmScriptCreateMethodAction.h"
  #include "bmScriptCreateGraphAction.h"
  #include "bmScriptCreateDashboardAction.h"
  #include "bmScriptAddDashboardLabelAction.h"

  #include "bmScriptDashboardSendAction.h"
  #include "bmScriptDashboardNotifyAction.h"
  #include "bmScriptOpenTCPSocketAction.h"
  #include "bmScriptSendTCPAction.h"
  #include "bmScriptAddMethodIdealOutputAction.h"
  #include "bmScriptCloseTCPSocketAction.h"
  #include "bmScriptAddMethodInputAction.h"
  #include "bmScriptAddMethodOutputAction.h"
  #include "bmScriptSetIdealOutputAction.h"
#endif

#ifdef BM_XCEDE
  #include "bmScriptGetXnatDataSetsAction.h"
  #include "bmScriptDownloadXnatDataSetAction.h"
  #include "bmScriptGetXnatFilenameAction.h"
  #include "bmScriptGetXnatURIAction.h" 
  #include "bmScriptGetXcedeDataSetsAction.h"
  #include "bmScriptDownloadXcedeDataSetAction.h"
  #include "bmScriptGetXcedeFilenameAction.h"
#endif

#include "Timer.h"
#include <algorithm>
#include <sstream>
#include <iterator>

#define BM_NEWACTION(option, iname)\
if (option == BMString(#iname).toLower())  return new Script##iname##Action();

#define BM_NEWKEYWORD(keywordList, iname)\
keywordList.push_back(BMString(#iname));

namespace bm {

ScriptActionManager::ScriptActionManager()
{
  m_ParentAction = 0;
  m_Error = new ScriptError();
  m_ProgressManager = new ProgressManager();
  m_ApplicationWrapperList = 0;
  m_ApplicationsList = 0;
  m_ScriptPath = "";
  m_Parser = NULL;
  m_ScriptFullPath = "";

#ifdef BM_GRID
  m_GridModule = NULL;
#endif
}

ScriptActionManager::~ScriptActionManager()
{
/*
  std::vector<variablestruct*>::iterator it = m_VariableList.begin();
  while(it != m_VariableList.end())
   {
   variablestruct* var = *it;
   it = m_VariableList.erase(it);
   delete var;
   }

  std::vector<ScriptAction*>::iterator itA = m_InternalActionList.begin();
  while(itA != m_InternalActionList.end())
   {
   ScriptAction* var = *itA;
   itA = m_InternalActionList.erase(itA);
   delete var;
   }*/

  //delete m_Error;
  //delete m_ProgressManager;
}

void ScriptActionManager::SetApplicationPath( const BMString& applicationpath )
{
  m_ApplicationPath = applicationpath;
}

void ScriptActionManager::SetScriptPath( const BMString& scriptpath )
{
  m_ScriptPath = scriptpath;
}

void ScriptActionManager::SetScriptFullPath(const BMString& scriptpath)
{
  m_ScriptFullPath = scriptpath;
}

void ScriptActionManager::AddAction(ScriptAction* action)
{
  m_ActionList.push_back(action);
}

void ScriptActionManager::SetProgressManager(ProgressManager* progressmanager)
{
  if (m_ProgressManager)
    {
    delete m_ProgressManager;
    }
  m_ProgressManager = progressmanager;
};

ProgressManager* ScriptActionManager::GetProgressManager()
{
  return m_ProgressManager;
}

void ScriptActionManager::SetError(ScriptError* error)
{
  if (m_Error != error)
    {
    delete m_Error;
    }
  m_Error = error;
}

ScriptError* ScriptActionManager::GetError()
{
  return m_Error;
}


void ScriptActionManager::SetLineNumber(int linenumber)
{
  m_LineNumber = linenumber;
}

std::vector<BMString> ScriptActionManager::GenerateKeywordList()const
{
  std::vector<BMString> _list;
  BM_NEWKEYWORD(_list, ForEach);
  BM_NEWKEYWORD(_list, ForNFold);
  BM_NEWKEYWORD(_list, Sequence);
  BM_NEWKEYWORD(_list, Echo);
  BM_NEWKEYWORD(_list, If);
  BM_NEWKEYWORD(_list, Run);
  BM_NEWKEYWORD(_list, Set);
  BM_NEWKEYWORD(_list, SetApp);
  BM_NEWKEYWORD(_list, SetAppOption);
  BM_NEWKEYWORD(_list, SetAppFlag);
  BM_NEWKEYWORD(_list, ListDirInDir);
  BM_NEWKEYWORD(_list, ListFileInDir);
  BM_NEWKEYWORD(_list, GetParam);
  BM_NEWKEYWORD(_list, GetParamCount);
  BM_NEWKEYWORD(_list, Randomize);
  BM_NEWKEYWORD(_list, Inc);
  BM_NEWKEYWORD(_list, Int);
  BM_NEWKEYWORD(_list, AppendFile);
  BM_NEWKEYWORD(_list, WriteFile);
  BM_NEWKEYWORD(_list, DbSendValue);
  BM_NEWKEYWORD(_list, DbSendFile);
  BM_NEWKEYWORD(_list, DbClear);
  BM_NEWKEYWORD(_list, ExtractSlice);
  BM_NEWKEYWORD(_list, ExtractString);
  BM_NEWKEYWORD(_list, Include);
  BM_NEWKEYWORD(_list, DashboardHost);
  BM_NEWKEYWORD(_list, DashboardUser);
  BM_NEWKEYWORD(_list, DashboardKey);
  BM_NEWKEYWORD(_list, DashboardSend);
  BM_NEWKEYWORD(_list, DashboardNotify);
  BM_NEWKEYWORD(_list, CreateExperiment);
  BM_NEWKEYWORD(_list, CreateMethod);
  BM_NEWKEYWORD(_list, CreateDashboard);
  BM_NEWKEYWORD(_list, AddDashboardLabel);
  BM_NEWKEYWORD(_list, CreateGraph);
  BM_NEWKEYWORD(_list, Sin);
  BM_NEWKEYWORD(_list, CopyFile);
  BM_NEWKEYWORD(_list, DeleteFile);
  BM_NEWKEYWORD(_list, OpenTCPSocket);
  BM_NEWKEYWORD(_list, SendTCP);
  BM_NEWKEYWORD(_list, CloseTCPSocket);
  BM_NEWKEYWORD(_list, AddMethodInput);
  BM_NEWKEYWORD(_list, AddMethodOutput);
  BM_NEWKEYWORD(_list, AddMethodIdealOutput);
  BM_NEWKEYWORD(_list, SetIdealOutput);
  BM_NEWKEYWORD(_list, RegEx);
  BM_NEWKEYWORD(_list, MakeDirectory);
  BM_NEWKEYWORD(_list, GetTime);
  BM_NEWKEYWORD(_list, GetCurrentDateTime);
  BM_NEWKEYWORD(_list, GetListSize);
  BM_NEWKEYWORD(_list, Glob);
  BM_NEWKEYWORD(_list, GetFilename);
  BM_NEWKEYWORD(_list, ConvertImage);
  BM_NEWKEYWORD(_list, FileExists);
  BM_NEWKEYWORD(_list, Exit);
  BM_NEWKEYWORD(_list, ExitOnError);
  BM_NEWKEYWORD(_list, AddError);
  BM_NEWKEYWORD(_list, GetErrorCount);
  BM_NEWKEYWORD(_list, ClearErrors);
  BM_NEWKEYWORD(_list, Math);
  
#ifdef BM_GRID
  BM_NEWKEYWORD(_list, GridDataHost);
  BM_NEWKEYWORD(_list, GridOutputHost);
  BM_NEWKEYWORD(_list, GridSingleNode);
  BM_NEWKEYWORD(_list, InputDirectory);
  BM_NEWKEYWORD(_list, OutputDirectory);
  BM_NEWKEYWORD(_list, ExecutableDirectory);
  BM_NEWKEYWORD(_list, WorkingDirectory);
  BM_NEWKEYWORD(_list, GridSetGrouping);
  BM_NEWKEYWORD(_list, GridMaxNodes);
  BM_NEWKEYWORD(_list, GridTempDirectory);
  BM_NEWKEYWORD(_list, GridTransferFile);
  BM_NEWKEYWORD(_list, GridRequirements);
  BM_NEWKEYWORD(_list, GridOwner);
#endif

#ifdef BM_XCEDE
  BM_NEWKEYWORD(_list, GetXnatDataSets);
  BM_NEWKEYWORD(_list, DownloadXnatDataSet);
  BM_NEWKEYWORD(_list, GetXnatFilename);
  BM_NEWKEYWORD(_list, GetXnatURI);
  BM_NEWKEYWORD(_list, GetXcedeDataSets);
  BM_NEWKEYWORD(_list, DownloadXcedeDataSet);
  BM_NEWKEYWORD(_list, GetXcedeFilename);
#endif

  return _list;
}


ScriptAction* ScriptActionManager::CreateAction(const BMString& option)
{
  BM_NEWACTION(option, ForEach);
  BM_NEWACTION(option, ForNFold);
  BM_NEWACTION(option, Sequence);
  BM_NEWACTION(option, Echo);
  BM_NEWACTION(option, If);
  BM_NEWACTION(option, Run);
  BM_NEWACTION(option, Set);
  BM_NEWACTION(option, SetApp);
  BM_NEWACTION(option, SetAppOption);
  BM_NEWACTION(option, SetAppFlag);
  BM_NEWACTION(option, ListDirInDir);
  BM_NEWACTION(option, ListFileInDir);
  BM_NEWACTION(option, GetParam);
  BM_NEWACTION(option, GetParamCount);
  BM_NEWACTION(option, Randomize);
  BM_NEWACTION(option, Inc);
  BM_NEWACTION(option, Int);
  BM_NEWACTION(option, AppendFile);
  BM_NEWACTION(option, WriteFile);
  BM_NEWACTION(option, ExtractSlice);
  BM_NEWACTION(option, ExtractString);
  BM_NEWACTION(option, Include);
  BM_NEWACTION(option, Sin);
  BM_NEWACTION(option, CopyFile);
  BM_NEWACTION(option, DeleteFile);
  BM_NEWACTION(option, RegEx);
  BM_NEWACTION(option, MakeDirectory);
  BM_NEWACTION(option, GetTime);
  BM_NEWACTION(option, GetCurrentDateTime);
  BM_NEWACTION(option, Glob);
  BM_NEWACTION(option, GetListSize);
  BM_NEWACTION(option, GetFilename);
  BM_NEWACTION(option, ConvertImage);
  BM_NEWACTION(option, FileExists);
  BM_NEWACTION(option, Exit);
  BM_NEWACTION(option, ExitOnError);
  BM_NEWACTION(option, AddError);
  BM_NEWACTION(option, GetErrorCount);
  BM_NEWACTION(option, ClearErrors);
  BM_NEWACTION(option, Math);
  
#ifdef BM_GRID
  BM_NEWACTION(option, GridDataHost);
  BM_NEWACTION(option, GridOutputHost);
  BM_NEWACTION(option, GridSingleNode);
  BM_NEWACTION(option, InputDirectory);
  BM_NEWACTION(option, OutputDirectory);
  BM_NEWACTION(option, ExecutableDirectory);
  BM_NEWACTION(option, WorkingDirectory);
  BM_NEWACTION(option, GridSetGrouping);
  BM_NEWACTION(option, GridMaxNodes);
  BM_NEWACTION(option, GridTempDirectory);
  BM_NEWACTION(option, GridTransferFile);
  BM_NEWACTION(option, GridRequirements);
  BM_NEWACTION(option, GridOwner);
#endif

#ifdef BM_DASHBOARD
  BM_NEWACTION(option, DbSendValue);
  BM_NEWACTION(option, DbSendFile);
  BM_NEWACTION(option, DbClear);
  BM_NEWACTION(option, DashboardHost);
  BM_NEWACTION(option, DashboardUser);
  BM_NEWACTION(option, DashboardKey);
  BM_NEWACTION(option, DashboardSend);
  BM_NEWACTION(option, DashboardNotify);
  BM_NEWACTION(option, CreateExperiment);
  BM_NEWACTION(option, CreateMethod);
  BM_NEWACTION(option, CreateGraph);
  BM_NEWACTION(option, CreateDashboard);
  BM_NEWACTION(option, OpenTCPSocket);
  BM_NEWACTION(option, SendTCP);
  BM_NEWACTION(option, CloseTCPSocket);
  BM_NEWACTION(option, AddMethodInput);
  BM_NEWACTION(option, AddMethodOutput);
  BM_NEWACTION(option, AddMethodIdealOutput);
  BM_NEWACTION(option, SetIdealOutput);
  BM_NEWACTION(option, AddDashboardLabel);
#endif

#ifdef BM_XCEDE
  BM_NEWACTION(option, GetXnatDataSets);
  BM_NEWACTION(option, DownloadXnatDataSet);
  BM_NEWACTION(option, GetXnatFilename);
  BM_NEWACTION(option, GetXnatURI);  
  BM_NEWACTION(option, GetXcedeDataSets);
  BM_NEWACTION(option, DownloadXcedeDataSet);
  BM_NEWACTION(option, GetXcedeFilename);
#endif

  return 0;
}

void ScriptActionManager
::SetApplicationWrapperList( ApplicationWrapperListType* applicationlist )
{
  m_ApplicationWrapperList = applicationlist;
}

ApplicationWrapper* ScriptActionManager
::GetApplicationWrapper( const BMString& applicationPath )
{
  ApplicationWrapperListType::iterator it =
    std::find_if( m_ApplicationWrapperList->begin(),
               m_ApplicationWrapperList->end(),
               AppWrapFinder( applicationPath.GetConstValue() ) );
  if( it == m_ApplicationWrapperList->end() )
    {
    return NULL;
    }
  return *it;
}

void ScriptActionManager::Reset()
{
  m_VariableTestList.clear();
  m_VariableList.clear();
  this->SetVariable( "applicationpath", m_ApplicationPath.toVariable() );
  
  if (m_ScriptPath.length() == 0)
    {
    this->SetVariable( "scriptpath", m_ApplicationPath.toVariable() );
    }
  else
    {
    this->SetVariable( "scriptpath", m_ScriptPath.toVariable() );
    }
   
  this->SetTestVariable("applicationpath");
  this->SetTestVariable("scriptpath");

  if (m_ApplicationWrapperList)
    {
    for( unsigned int k=0; k < m_ApplicationWrapperList->size(); k++ )
      { 
      this->SetVariable( (*m_ApplicationWrapperList)[k]->GetName(),
                         BMString((*m_ApplicationWrapperList)[k]
                          ->GetApplicationPath()).toVariable() );
      this->SetTestVariable((*m_ApplicationWrapperList)[k]->GetName()); 
      }
    }

  m_ParentAction = 0;
  for( unsigned int i=0; i < m_ActionList.size(); i++ )
    {
    m_ActionList[i]->Delete();
    }
  
  m_ActionList.clear();

#ifdef BM_DASHBOARD
  m_Dashboard.url = "";
  m_Dashboard.user = "";
  m_Dashboard.password = "";
  m_Dashboard.experiments.clear();
#endif
}

void ScriptActionManager
::AddAction( const BMString& option, const std::vector<BMString>& param )
{
  if( (option == "endforeach") || 
      (option == "endif") ||
      (option == "endfornfold") )
    {
    if( m_ParentAction != 0 )
      {
      m_ParentAction = m_ParentAction->GetParent();
      }
    return;
    }
  
  if( option == "else" )
    {
    dynamic_cast<ScriptIfAction*>(m_ParentAction)->SetMode(1);
    return;
    }

  ScriptAction* _action = this->CreateAction(option);

  if( !_action )
   {
   m_Error->SetError( BMString("Undefined parameter [") + option + "]" ,
                      m_LineNumber);
   return;
   }
     
  m_InternalActionList.push_back(_action);
  _action->SetName(option);
  _action->SetParameters(param);
  _action->SetParent(m_ParentAction);
  _action->SetManager(this);
  _action->SetProgressManager(m_ProgressManager);

#ifdef BM_GRID
  _action->SetGridModule(m_GridModule);
#endif
  if( !_action->TestParam(m_Error,m_LineNumber) )
    {
    if ( _action->Help() != "" )
      {
      m_Error->SetStatus( BMString("\tCommand: ") + _action->Help() );
      }
    }
   
  if( option == "include" )
    {
    return;
    }
   
  if( m_ParentAction == 0 )
    {
    this->AddAction( _action );
    }
  else
    {
    m_ParentAction->AddAction( _action );
    }

  if( (option == "foreach")  || 
      (option == "if") ||
      (option == "fornfold") )
    {
    m_ParentAction = _action;
    }
}

void ScriptActionManager::Execute()
{
  Timer m_timer;
  m_timer.start();
  m_ProgressManager->Start();
  for( unsigned int i=0 ; i < m_ActionList.size() ; i++)
    {
    m_ActionList[i]->Execute();
    if (m_ProgressManager->IsStop())
      {
      break;
      } 
    }
  m_ProgressManager->SetFinished(
    BMString("Total Execution time: %1ms")
      .arg(m_timer.getMilliseconds()).toMString() );
}

void ScriptActionManager::SetTestVariable( const BMString& name )
{
  if( std::find( m_VariableTestList.begin(), m_VariableTestList.end(), name )
        == m_VariableTestList.end() )
    {
#ifdef VERBOSE
  std::cout<< "Add " << name.GetConstValue() << " as a test variable" << std::endl;
#endif
    m_VariableTestList.push_back( name );
    }
}


bool ScriptActionManager::IsTestVariable( const BMString& name )const
{
  bool res = true;
  std::vector<BMString>::const_iterator it = 
    std::find( m_VariableTestList.begin(), m_VariableTestList.end(), name);
  if( it == m_VariableTestList.end() )
    {
    // name has not been found in VariableTestList
    res = false;
    }
#ifdef VERBOSE
  std::cout<< "Is " << name.GetConstValue() << " a test variable? " << res << std::endl;
#endif
  return res;
}


void ScriptActionManager
::SetVariable( const BMString& name, const BMString& value )
{
  /*
  bool _detected = false;
  std::vector<variablestruct*>::iterator it;
  std::vector<variablestruct*>::iterator end = m_VariableList.end();
  for( it = m_VariableList.begin(); it != end; ++it)
    {
     if( (*it)->name == name)
       {
       _detected = true;
       (*it)->value = value;
       // @todo shall we break here ?
       }
    }

  if( _detected == false )
    {
    variablestruct* m_newvar = new variablestruct;
    m_newvar->name = name;
    m_newvar->value = value;
    m_VariableList.push_back(m_newvar);
    }
  */
  m_VariableList[name] = value;
}


/** Return the value of a given variable. The variable (name) can
 *  be ${i} $i or i 
std::vector<MString> ScriptActionManager::GetVariable(MString name)const
{
  // We strip any ${} or $ as a variable name
  MString varname = name.removeChar('$');
  varname = varname.removeChar('{');
  varname = varname.removeChar('}');

  std::vector<MString> _list;
  for (unsigned int i=0;i<m_VariableList.size();i++)
    {
    if (m_VariableList[i]->name == varname)
      {
      MString _param = m_VariableList[i]->value;
      while (_param != "")
        {
        _param = _param.removeChar(' ',true);
        _param = _param.removeChar('\'',true);
        MString _value = _param.begin("\'");
        if (_value != "")
          {
          _list.push_back(_value);
          }
        _param = _param.end("\'");
        if (_param.length() != 0)
          {
          _param = _param+1;
          }
        }
      return _list;
      }
    }

  return _list;
}*/

std::vector<BMString> ScriptActionManager
::GetVariable( const BMString& name )const
{
  // We strip any ${} or $ as a variable name
  BMString varname( name );
  varname.removeAllChars('$');
  varname.removeAllChars('{');
  varname.removeAllChars('}');

  std::vector<BMString> _list;
  std::map<BMString,BMString>::const_iterator it = 
    m_VariableList.find( varname );
  if( it != m_VariableList.end() )
    {
    _list = it->second.extractVariables();
    }
#ifdef VERBOSE
  std::cout<< " GetVariable(" << name.GetConstValue() << "): ";
  if( it != m_VariableList.end() )
    {
    std::cout << it->second.GetConstValue() << " -> " << std::endl;
    }
  std::vector<BMString>::iterator it2 = _list.begin();
  for( ; it2 != _list.end(); ++it2 )
    {
    std::cout << "[" << (*it2).GetConstValue() << "]";
    }
  std::cout<<std::endl;
#endif
  return _list;
}

void ScriptActionManager::SetSocketVariable(const BMString& name)
{
  /*
  bool _detected = false;
  unsigned int size = m_VariableSocketList.size();
  for( unsigned int i=0 ; i < size; ++i )
    {
    if (m_VariableSocketList[i]->name == name)
      {
      _detected = true;
      return;
      }
    }

  if (_detected == false)
    {
    variablestructsocket* m_newvar = new variablestructsocket;
    m_newvar->name = name;
    m_VariableSocketList.push_back(m_newvar);
    }
  */
  m_VariableSocketList[name];
}

TCPSocket* ScriptActionManager::GetVariableSocket(const BMString& name)
{
  /*
  unsigned int size = m_VariableSocketList.size();
  for( unsigned int i = 0; i < size; ++i )
  {
     if (m_VariableSocketList[i]->name == name)
     {
        return &(m_VariableSocketList[i]->socket);
     }
  }
  return NULL;
  */
  TCPSocket* res = NULL;
  std::map<BMString,TCPSocket>::iterator it = m_VariableSocketList.find(name);
  if( it != m_VariableSocketList.end() )
    {
    res = &(it->second);
    }
  return res;
}

/*
std::vector<BMString> ScriptActionManager
::GetParamsFromVariable(const BMString& var)const
{
  return var.tokenize(" ");
}
*/

BMString ScriptActionManager
::GetVariableFromParams(const std::vector<BMString> & params)const
{
  /*
  BMString var;
  
  unsigned int size = params.size();
  for( unsigned int i = 0; i < size; ++i)
    {
    var += params[i];
    if( i < size - 1 )
      {
      var += ' ';
      }
    }
  */
  //separate all the params with a space
  std::stringstream concatenatedParams;
  std::ostream_iterator<std::string> out_it( concatenatedParams, " " );
  std::copy( params.begin(), params.end(), out_it );
  BMString var( concatenatedParams.str() );
  // unfortunately a space has been added after the last param, remove it.
  return var.rbegin( " " );
}


void ScriptActionManager::DisplayVariableList()const
{
  std::cout << "Variable List" << std::endl;
  /*
  unsigned int size = m_VariableList.size();
  for( unsigned int i = 0; i < size; ++i)
    {
    std::cout << m_VariableList[i]->name.toChar() 
              << "\t" << m_VariableList[i]->value.toChar() << std::endl;
    }
  */
  std::map<BMString,BMString>::const_iterator it;
  std::map<BMString,BMString>::const_iterator end = m_VariableList.end();
  for( it = m_VariableList.begin(); it != end; ++it)
    {
    std::cout << it->first.toChar() << "\t" 
              << it->second.toChar() << std::endl;
    }
}
/*
MString ScriptActionManager::Convert(MString param)
{
  MString _value="'";
  bool _vardetected = false;
  MString _var;

  for (int i=0;i<param.length();i++)
    {
    if (param[i] == '$')
      {
      _vardetected=true;
      _var = "";
      }
    else if (_vardetected)
      {
      if ((_var.length() == 0) && (param[i] == '{'))
        {
        _var = "";
        }
      else if ((param[i] == ' ') || (param[i] == '}') || 
               (param[i] == '\'')  || (i==param.length()-1))
        {
        if ((i==param.length()-1) && (param[i] != '}') && (param[i] != '\''))
          {
          _var += param[i];
          }

        _vardetected = false;

        //MString _variable;
        std::vector<MString> _variable =  GetVariable(_var);
        for (unsigned int k=0;k<_variable.size();k++)
          {
          if (k!=0)
            {
            _value += "'";
            }

          if (_variable[k] != "null")
            {
            _value += _variable[k];
            }
          if (k != _variable.size()-1)
            {
            _value += "' ";
            }
          }

        if (param[i] == ' ')
          {
          _value += " ";
          }
        }
      else
        {
        _var += param[i];
        }
      }
    else if (param[i] != '\'')
      {
      if (param[i] == ' ')
        {
        _value += " ";
        }
      else
        {
        _value += param[i];
        }
      }
    }

  _value += "'";

  return _value;

}
*/


BMString ScriptActionManager::Convert(const BMString& parameter)const
{
  BMString _value="'";
  BMString _var;
  const std::string& param = parameter.GetConstValue();
  std::string::size_type end = param.length();
  std::string::size_type endQuote, endBrace;
  for ( std::string::size_type i = 0; i < end; ++i)
    {
    switch( param[i] )
      {
      case '\'': // opening quote
        ++i;
        // find the closing quote
        endQuote = param.find( '\'', i);
        if( endQuote != std::string::npos )
          {
          //i+1 to remove the quote
          _value += param.substr( i, endQuote - i);
          }
        else
          {//we didn't find the closing quote, must be an error
          //TODO add a warning instead of an error
          m_ProgressManager->AddError( BMString( "ScriptActionManager:"
            "Can't find closing quote in parameter: ") + param  );
          // keep going
          _value += param.substr( i, std::string::npos );
          }
        i = endQuote;
        break;
      case '$': // in a ${...} ?
        if( (i + 1) < end && param[i+1] == '{' &&
            ( endBrace = param.find( '}', i + 2 ) ) != std::string::npos )
          {//found variable
          //extract variable name
          std::vector<BMString> _variable = this->GetVariable( 
            param.substr( i + 2, endBrace - i - 2 ) );
          std::vector<BMString>::const_iterator it2 = _variable.begin();
          std::vector<BMString>::const_iterator end2 = _variable.end();
          for (; it2 != end2; ++it2)
            {
            if ( it2 != _variable.begin() )
              {
              _value += "'";
              }
            if ( *it2 != "null" )
              {
              _value += (*it2).fromVariable();
              }
            if ( (it2+1) != end2 )
              {
              _value += "' ";
              }
            }
          i = endBrace; 
          }
        else
          {// it is not something like: ${...}, 
          // process param[i] as a normal text
          _value += param[i];
          }
        break;
      default:
        _value += param[i];
        break;
      }
    }

  _value += "'";
#ifdef VERBOSE
  std::cout<< "Convert param: "<< parameter.GetConstValue()
           << " -> " << _value.GetConstValue() 
           << std::endl;
#endif
  return _value;
}


/*
BMString ScriptActionManager::Convert(const BMString& param)const
{
//  return this->ExpandParameter(param);
  BMString _expression;
  BMString _value="'";
  bool _vardetected = false;
  BMString _var;
  
  std::string::const_iterator it;
  std::string::const_iterator end = param.GetConstValue().end();
  std::string::const_iterator endVariable;
  for( it = param.GetConstValue().begin() ; it != end ; ++it )
    {
    if ( *it == '$' )
      {
      _vardetected = true;
      _var = "";
      }
    else if (_vardetected)
      {
      if ((_var.length() == 0) && (*it == '{'))
        {
        _var = "";
        }
      else if( *it == '}' )
        {
        if (( (it+1)== end ) && ( *it != '}') && ( *it != '\''))
          {
          _var += *it;
          }

        _vardetected = false;

        //MString _variable;
        std::vector<BMString> _variable = this->GetVariable( _var );
        std::vector<BMString>::const_iterator it2 = _variable.begin();
        std::vector<BMString>::const_iterator end2 = _variable.end();
        for (; it2 != end2; ++it2)
          {
          if ( it2 != _variable.begin() )
            {
            _value += "'";
            }

          if ( *it2 != "null" )
            {
            _value += (*it2).fromVariable();
            }
          if ( (it2+1) != end2 )
            {
            _value += "' ";
            }
          }

        if ( *it == ' ' )
          {
          _value += " ";
          }
        }
      else
        {
        _var += *it;
        }
      }
    else 
      {
      _value += *it;
      }
    if ( *it != '\'')
      {
      if ( *it == ' ')
        {
        _value += " ";
        }
      else
        {
        _value += *it;
        }
      }
    }
  
  _value += "'";

#ifdef VERBOSE
  std::cout<< "Convert param: "<< param.GetConstValue()
           << " -> " << _value.GetConstValue() 
           << std::endl;
#endif
  return _value;

  }*/

BMString ScriptActionManager::ConvertExtra(const BMString& param)const
{
  BMString _value;// = "'";
  bool _vardetected = false;
  BMString _var;
  
  for (int i=0;i<param.length();i++)
    {
    if (param[i] == '$')
      {
      _vardetected=true;
      _var = "";
      }
    else if (_vardetected)
      {
      if ((_var.length() == 0) && (param[i] == '{'))
        {
        _var = "";
        }
      else if ((param[i] == ' ') || (param[i] == '}') || 
               (param[i] == '\'')  || (i==param.length()-1))
        {
        if ((i==param.length()-1) && (param[i] != '}') && (param[i] != '\''))
          {
          _var+=param[i];
          }
        
        _vardetected=false;
        //MString _variable;
        std::vector<BMString> _variable =  GetVariable(_var);
        for (unsigned int k=0;k<_variable.size();k++)
          {
          if (k!=0)
            {
//            _value += "'";
            }
          
          if (_variable[k] != "null")
            {
            _value += _variable[k];
            }
          if (k != _variable.size()-1)
            {
            _value += " ";//"' ";
            }
          }
        
        if ( param[i] == ' ' )
          _value += "' '";
        }
      else
        _var+=param[i];
      }
    else if (param[i] != '\'')
      {
      if (param[i] == ' ')
        {
        _value += "' '";
        }
      else
        {
        _value += param[i];
        }
      }
    }
  
  //_value += "'";
  return _value;
}


std::vector<BMString> ScriptActionManager
::ConvertToArray(const BMString& parameter)const
{
  std::vector<BMString> _res;
  BMString _value="'";
  BMString _var;
  const std::string& param = parameter.GetConstValue();
  std::string::size_type end = param.length();
  std::string::size_type endQuote, endBrace;
  for ( std::string::size_type i = 0; i < end; ++i)
    {
    switch( param[i] )
      {
      case '\'': // opening quote
        ++i;
        // find the closing quote
        endQuote = param.find( '\'', i);
        if( endQuote != std::string::npos )
          {
          //i+1 to remove the quote
          _value += param.substr( i, endQuote - i);
          }
        else
          {//we didn't find the closing quote, must be an error
          //TODO add a warning instead of an error
          m_ProgressManager->AddError( BMString( "ScriptActionManager:"
            "Can't find closing quote in parameter: ") + param  );
          // keep going
          _value += param.substr( i, std::string::npos );
          }
        i = endQuote;
        break;
      case ' ':
        //check if it is a single space
        if( (i + 1) < end && param[i+1] != ' ' )
          {
          // flush the variable
          _value += "'";
          _res.push_back( _value );
          _value = "'";
          }
        else
          {
          // this space is part of many spaces. skip it
          }
        break;
      case '$': // in a ${...} ?
        if( (i + 1) < end && param[i+1] == '{' &&
            ( endBrace = param.find( '}', i + 2 ) ) != std::string::npos )
          {//found variable
          //extract variable name
          std::vector<BMString> _variable = this->GetVariable( 
            param.substr( i + 2, endBrace - i - 2 ) );
          std::vector<BMString>::const_iterator it2 = _variable.begin();
          std::vector<BMString>::const_iterator end2 = _variable.end();
          for (; it2 != end2; ++it2)
            {
            if ( *it2 != "null" )
              {
              _value += (*it2).fromVariable();
              }
            if ( (it2+1) != end2 )
              {// flush the variable
              _value += "'";
              _res.push_back( _value );
              _value = "'";
              }
            }
          i = endBrace; 
          }
        else
          {//it is not something like: ${...}, process param[i] as a normal text
          _value += param[i];
          }
        break;
      default:
        _value += param[i];
        break;
      }
    }

  _value += "'";
  if( _value != "''" )
    {
    _res.push_back( _value );
    }
#ifdef VERBOSE
  std::cout<< "Convert param: "<< parameter.GetConstValue() << " -> ";
  for( unsigned int i = 0; i < _res.size(); ++i )
    {
    std::cout<< '[' <<  _res[i].GetConstValue() << ']' ;
    }
  std::cout << std::endl;
#endif
  return _res;
}


/** If param is like: "${my_variable}" we replace its content by the 
 *  value of my_variable between quotes: "'content'".
 *  In other cases: "my param string", copy the content of param between
 *  quotes in the result: "'my param string'"
 *  if param: " toto${foo}${bar}tonton tutu", res: "'totofoo_expandedbar_expanded' 'tutu'"
 *  
 *
BMString ScriptActionManager
::ExpandParameter( const BMString& param, bool quote )const
{
  std::vector<BMString> variableContent = 
    this->ExpandParameterToArray( param, quote );
  // separate all the items by a space
  std::stringstream concatenatedContent;
  std::ostream_iterator<std::string> out_it( concatenatedContent, " " );
  std::copy( variableContent.begin(), variableContent.end(), out_it );
  BMString res( concatenatedContent.str() );
  // a space is automatically added after the last variableContentItem, remove it.
  return res.rbegin( " " );
}
*/
/** same as "return this->Expand( param ).extractVariables();"
 *
std::vector<BMString> ScriptActionManager
::ExpandParameterToArray( const BMString& param, bool quote )const
{
  std::vector<BMString> res;
  BMString variable = param.extractExpandableVariable();
  if( !variable.isEmpty() )
    {
    std::vector<BMString> variableContent = this->GetVariable(variable);
    std::vector<BMString>::const_iterator it = variableContent.begin();
    std::vector<BMString>::const_iterator end = variableContent.end();
    for( it = variableContent.begin(); it != end; ++it )
      {
      BMString content = (*it != "null")? *it : "";
      res.push_back( quote? content.toVariable() : content.fromVariable() );
      }
    }
  else
    {
    res.push_back( quote ? param.toVariable() : param.fromVariable() );
    }
  return res;
}
*/

bool ScriptActionManager::TestExpand( const BMString& param, int linenumber )const
{
#ifdef VERBOSE
  std::cout << "Test Expand: " << param.GetConstValue() << std::endl;
#endif
  BMString variable = param.extractExpandableVariable();
  if( !variable.isEmpty() && !this->IsTestVariable(variable) )
    {
    // param == ${undefined_variable}
    m_Error->SetError( BMString("In parameter: ") + param 
                     + ", undefined variable [" + variable + "]"
                       , m_LineNumber );
    return false;
    }
  return true;
}

/*
bool ScriptActionManager::TestConvert(const BMString& param, int linenumber)
{
  bool _vardetected = false;
  BMString _var;
  for (int i=0; i<param.length(); i++)
    {
    if (param[i] == '$')
      {
      _vardetected = true;
      _var = "";
      }
    else if (_vardetected)
      {
      if ((_var.length() == 0) && (param[i] == '{'))
        {
        _var = "";
        }
      else if ((param[i] == ' ') || (param[i] == '}') || 
               (param[i] == '\'')  || (i == param.length()-1))
        {
        if ((i == param.length()-1) && (param[i] != '}') && (param[i] != '\''))
          {
          _var += param[i];
          }

        _vardetected = false;

        if (IsTestVariable(_var) == false)
          {
          m_Error->SetError( BMString("In parameter: ") 
                           + param 
                           + ", undefined variable [" + _var + "]"
                             , m_LineNumber );
          return false;
          }
        }
      else
        {
        _var += param[i];
        }
      }
    }
  return true;
}
*/

bool ScriptActionManager
::TestConvert(const BMString& parameter, int linenumber)const
{
#ifdef VERBOSE
  std::cout << "Test param: "<< parameter.GetConstValue() << std::endl;
#endif

  BMString _variable;
  const std::string& param = parameter.GetConstValue();
  std::string::size_type end = param.length(), endBrace;
  for ( std::string::size_type i = 0; i < end; ++i)
    {
    switch( param[i] )
      {
      case '\'': // opening quote
        // don't test what is inside quotes
        // find the closing quote
        i = param.find( '\'', i+1);
        break;
      case '$': // in a ${...} ?
        if( (i + 1) < end && param[i+1] == '{' &&
            ( endBrace = param.find( '}', i + 2 ) ) != std::string::npos )
          {// found variable
          // extract variable name
          _variable = param.substr( i + 2, endBrace - i - 2 );
          if( !this->IsTestVariable( _variable)  )
            {
            m_Error->SetError( BMString("In parameter: ") 
                             + param 
                             + ", undefined variable [" + _variable + "]"
                               , m_LineNumber );
            return false;
            }
          i = endBrace; 
          }
        break;
      default:
        break;
      }
    }
  return true;
}



bool ScriptActionManager::TestParam()
{
  bool flag = true;
  std::vector<ScriptAction*>::iterator it;
  std::vector<ScriptAction*>::iterator end = m_ActionList.end();
  for ( it = m_ActionList.begin(); it != end; ++it )
    {
    if ( !(*it)->TestParam(m_Error) )
      {
      flag = false;
      }
    }
  return flag;
}

bool ScriptActionManager::RemoveSocket(const BMString& name)
{
 /*
  bool found = false;
  std::vector<variablestructsocket*>::iterator it;
  std::vector<variablestructsocket*>::iterator end = m_VariableSocketList.end();
  for(it = m_VariableSocketList.begin(); it != end; ++it )
    {
    if( (*it)->name == name)
      {
      m_VariableSocketList.erase(it);
      found = true;
      break;
      }
    }
  return found;
  */
  unsigned int eraseCount = m_VariableSocketList.erase(name);
  return eraseCount == 1;
}

} // end namespace bm
