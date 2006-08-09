/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptActionManager.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __ScriptActionManager_h_
#define __ScriptActionManager_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#define BM_NEWACTION(iname)\
 if (option == MString(#iname).toLower())           return new Script##iname##Action();


#define BM_NEWKEYWORD(iname)\
 m_list.push_back(MString(#iname));


#include "bmScriptAction.h"
#include "bmScriptError.h"
#include "bmProgressManager.h"
#include "ApplicationWrapper.h"
#include <vector>
#include <iostream>
#include "MString.h"
#include "bmGrid.h"
#include "TCPSocket.h"

namespace bm {

class ScriptAction;

class ScriptActionManager
{
public:

  typedef std::vector<ApplicationWrapper*> ApplicationWrapperListType;
  typedef std::pair<std::string,ApplicationWrapper*> ApplicationNameType;
  typedef std::vector<ApplicationNameType> ApplicationsListType;


  ScriptActionManager();
  ~ScriptActionManager();
  
  struct variablestruct
    {
     MString name;
     MString value;
    };
    
  struct variablestructsocket
    {
    MString   name;
    TCPSocket socket;
    };

  struct DashboardMethodParameter
    {
    std::string variable;
    std::string method;
    std::string name;
    bool        output;
    };

  struct DashboardMethod
    {
    std::string variable;
    std::string expVariable;
    std::string name;
    std::vector<DashboardMethodParameter> parameters;
    };

  struct DashboardExperiment
    {
    std::string variable;
    std::string project;
    std::string name;
    std::vector<DashboardMethod> methods;
    };

  struct Dashboard
    {
    std::string url;
    std::string user;
    std::string password;
    std::vector<DashboardExperiment> experiments;
    };
   
  ScriptAction* CreateAction(MString option);
  void SetApplicationPath(MString applicationpath);
  void SetScriptPath(MString scriptpath);
  void AddAction(ScriptAction* action);
  void AddAction(MString option,std::vector<MString> param);
  void Execute(); 
  MString Convert(MString param);
  MString ConvertExtra(MString param);
  void SetVariable(MString name,MString value);
  void SetSocketVariable(MString name);
  std::vector<MString> GetVariable(MString name);
  
  void DisplayVariableList();
  void SetLineNumber(int linenumber);
  int GetLineNumber() {return m_linenumber;}
  void SetError(ScriptError* error);
  ScriptError* GetError();
  ProgressManager* GetProgressManager();
  bool TestParam();
  void Reset();
  bool TestConvert(MString param,int linenumber);
  void SetTestVariable(MString name);
  bool IsTestVariable(MString name);
  std::vector<MString> GetParamsFromVariable(MString name);
  void SetProgressManager(ProgressManager* progressmanager);
  void SetParser(void* parser) {m_Parser = parser;}
  void* GetParser() {return m_Parser;}

  /** Set/Get the list of wrapped applications */
  void SetApplicationWrapperList(ApplicationWrapperListType* applicationlist);
  ApplicationWrapperListType* GetApplicationWrapperList() {return m_ApplicationWrapperList;}

  /** Set/Get the list of application from the editor associating an app and a keyword */
  void SetApplicationsList(ApplicationsListType* list) {m_ApplicationsList = list;}
  ApplicationsListType* GetApplicationsList() {return m_ApplicationsList;}

  std::vector<MString> GetKeywordList();

  void SetGridModule(Grid* grid) {m_GridModule = grid;}
  
  /** Set/Get variables relative to the dashboard */
  void SetDashboardURL(const char* url) {m_Dashboard.url = url;}
  const char* GetDashboardURL() {return m_Dashboard.url.c_str();}
  
  void SetDashboardUser(const char* user) {m_Dashboard.user = user;}
  const char* GetDashboardUser() {return m_Dashboard.user.c_str();}

  bool AddDashboardExperiment(const char* var, const char* projectName, const char* experimentName);
  bool AddDashboardMethod(const char* var, const char* expvar, const char* methodName);
  bool AddDashboardMethodParameter(const char* var, const char* methVar, const char* name,bool output=false);

  /** Get the dashboard as a const method, onlt to retrieve data from it */
  const Dashboard * GetDashboard() const {return &m_Dashboard;}

  TCPSocket* GetVariableSocket(MString name);
  bool RemoveSocket(MString name);

protected:
  std::vector<ScriptAction*> m_actionlist;
  ScriptAction* m_parentaction;
  std::vector<variablestruct*> m_variablelist;
  std::vector<MString> m_variabletestlist;
  std::vector<variablestructsocket*> m_variablesocketlist;
  int m_linenumber;
  ScriptError* m_error;
  ProgressManager* m_progressmanager;
  MString m_applicationpath;
  MString m_scriptpath;
  ApplicationWrapperListType* m_ApplicationWrapperList;
  ApplicationsListType* m_ApplicationsList;
  void* m_Parser;

  // Tells the output of the Run command to generate condor scripts
  Grid* m_GridModule;

  // The Script Action manager keeps global variables regarding the dashboard
  Dashboard m_Dashboard;
};

} // end namespace bm

#endif
