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

#include "bmScriptAction.h"
#include "bmScriptError.h"
#include "bmProgressManager.h"
#include "ApplicationWrapper.h"
#include <vector>
#include <map>
#include <iostream>
#include "MString.h"
#include "BMString.h"
#include "TCPSocket.h"

#ifdef BM_GRID
  #include "bmGrid.h"
#endif // BM_GRID

namespace bm {

class ScriptAction;

class ScriptActionManager
{
public:

  typedef std::vector<ApplicationWrapper*> ApplicationWrapperListType;
  typedef std::pair<std::string,ApplicationWrapper*> ApplicationNameType;
  typedef std::vector<ApplicationNameType> ApplicationsListType;

  ScriptActionManager();
  virtual ~ScriptActionManager();
  
  /*
  struct variablestruct
    {
    BMString name;
    BMString value;
    };
  */
  /*
  struct variablestructsocket
    {
    BMString   name;
    TCPSocket socket;
    };
  */
  struct DashboardMethodParameter
    {
    std::string variable;
    std::string method;
    std::string name;
    bool output;
    bool ideal;
    std::string type;
    };

  struct DashboardMethod
    {
    std::string variable;
    std::string expVariable;
    std::string name;
    std::vector<DashboardMethodParameter> parameters;
    };

  struct BatchBoard
    {
    std::string variable;
    std::string title;
    bool isPublic;
    std::string SQLid;
    };

  struct DashboardExperiment
    {
    std::string variable;
    std::string project;
    std::string name;
    std::vector<DashboardMethod> methods;
    std::vector<BatchBoard> batchboards;
    };

  struct Dashboard
    {
    std::string url;
    std::string user;
    std::string key;
    std::string password;
    std::vector<DashboardExperiment> experiments;
    };
   
  ScriptAction* CreateAction(const BMString& option);

  void        SetApplicationPath(const BMString& applicationpath);
  void        SetScriptPath(const BMString& scriptpath);
  void        SetScriptFullPath(const BMString& scriptpath);
  const BMString& GetScriptFullPath()const { return m_ScriptFullPath; }

  void AddAction(ScriptAction* action);
  void AddAction( const BMString& option, const std::vector<BMString>& param );

  void Execute(); 

  //MString Convert(MString param);
  BMString Convert(const BMString& param)const;
  BMString ConvertExtra(const BMString& param)const;
  std::vector<BMString> ConvertToArray( const BMString& param )const;
  // Expand does the same than convert
  //BMString ExpandParameter( const BMString& param, bool quote = true ) const;
  //BMString ExpandExpression( const BMString& expression, 
  //                          bool quote = true ) const;
  /** Give an action parameter as input and it returns its content
   *  expanded if needed.
   *    param value of the script action parameter, can be: "foo", "'foo'" or "${foo}"
   *    quote if true, every returned string are set between quotes, if false
   *          the surrounding quotes are removed .
   */
  //std::vector<BMString> ExpandParameterToArray( const BMString& param, 
  //                                              bool quote = true )const;

  void                  SetVariable(const BMString& name, const BMString& value);
  void                  SetSocketVariable(const BMString& name);
  std::vector<BMString> GetVariable(const BMString& name)const;
  
  void DisplayVariableList()const;
  void SetLineNumber(int linenumber);
  int  GetLineNumber() 
         { return m_LineNumber; }

  void         SetError(ScriptError* error);
  ScriptError* GetError();

  void             SetProgressManager(ProgressManager* progressmanager);
  ProgressManager* GetProgressManager();

  bool TestParam();
  void Reset();
  bool TestConvert(const BMString& param, int linenumber)const;
  /** if param looks like ${var}, verify that var is a defined variable
   */
  bool TestExpand( const BMString& param, int linenumber )const;

  void SetTestVariable(const BMString& name);
  bool IsTestVariable(const BMString& name)const;

  /** Obsolete, use ExpandToArray instead
  std::vector<BMString> GetParamsFromVariable( const BMString& name )const;
  */
  BMString              GetVariableFromParams( const std::vector<BMString> &
                                               params )const;

  void SetParser(void* parser) 
         { m_Parser = parser; }
  void* GetParser() 
         { return m_Parser; }

  /** Set/Get the list of wrapped applications */
  void   SetApplicationWrapperList(ApplicationWrapperListType* applicationlist);
  ApplicationWrapperListType* GetApplicationWrapperList() 
         { return m_ApplicationWrapperList; }
  ApplicationWrapper* GetApplicationWrapper( const BMString& applicationPath );
  
  /** Set/Get the list of application from the editor associating an app 
   *  and a keyword */
  void SetApplicationsList(ApplicationsListType* list) 
         { m_ApplicationsList = list; }
  ApplicationsListType* GetApplicationsList() 
         { return m_ApplicationsList; }

  std::vector<BMString> GenerateKeywordList()const;
  
  
#ifdef BM_GRID
  void SetGridModule(Grid* grid) 
         { m_GridModule = grid; }
#endif // BM_GRID

#ifdef BM_DASHBOARD 
  /** Set/Get variables relative to the dashboard */
  void SetDashboardURL(const char* url) 
         { m_Dashboard.url = url; }
  const char* GetDashboardURL() 
         { return m_Dashboard.url.c_str(); }
  
  void SetDashboardUser( const std::string& user) 
         { m_Dashboard.user = user; }
  void SetDashboardKey( const std::string& key) 
         { m_Dashboard.key = key; }

  const std::string& GetDashboardUser() const
         { return m_Dashboard.user; }
  const std::string& GetDashboardKey()const
         { return m_Dashboard.key; }

  bool AddDashboardExperiment( const std::string& var,
                               const std::string& projectName,
                               const std::string& experimentName );
  bool AddDashboardMethod( const std::string& var,
                           const std::string& expvar,
                           const std::string& methodName );

  bool AddDashboardMethodParameter(const std::string& var,
                                   const std::string& methVar, 
                                   const std::string& name,
                                   bool output=false,
                                   bool ideal=false,
                                   const std::string& type = "");

  BatchBoard*  AddBatchBoard(const std::string& var,
                             const std::string& experimentVar,
                             const std::string& title,
                             const std::string& isPublic);

  /** Get the dashboard as a const method, onlt to retrieve data from it */
  const Dashboard * GetDashboard() const 
          { return &m_Dashboard; }
#endif

  TCPSocket* GetVariableSocket( const BMString& name);
  bool RemoveSocket( const BMString& name);

protected:
  
  std::vector<ScriptAction*>         m_ActionList;
  ScriptAction                     * m_ParentAction;
  //std::vector<variablestruct*>       m_VariableList;
  // key = Variable Name, content = Variable Value
  std::map<BMString,BMString>        m_VariableList;
  std::vector<BMString>              m_VariableTestList;
  //std::vector<variablestructsocket*> m_VariableSocketList;
  std::map<BMString,TCPSocket>        m_VariableSocketList;
  int                                m_LineNumber;
  ScriptError                      * m_Error;
  ProgressManager                  * m_ProgressManager;
  BMString                           m_ApplicationPath;
  BMString                           m_ScriptPath;
  BMString                           m_ScriptFullPath;
  ApplicationWrapperListType       * m_ApplicationWrapperList;
  ApplicationsListType             * m_ApplicationsList;
  void                             * m_Parser;

  // Tells the output of the Run command to generate grid scripts
#ifdef BM_GRID 
  Grid* m_GridModule;
#endif

#ifdef BM_DASHBOARD 
  // The Script Action manager keeps global variables regarding the dashboard
  Dashboard m_Dashboard;
#endif

private:
  
  std::vector<ScriptAction*> m_InternalActionList;

};

struct AppWrapFinder
{
  AppWrapFinder( const std::string& applicationPath )
    :m_AppPath( applicationPath ){}
  bool operator() ( ApplicationWrapper* obj )
  { return obj && obj->GetApplicationPath() == m_AppPath;}
  std::string m_AppPath;
};
 

} // end namespace bm

#endif
