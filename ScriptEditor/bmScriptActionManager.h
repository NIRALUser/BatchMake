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
#include <vector>
#include <iostream>
#include "MString.h"

namespace bm {

class ScriptAction;

class ScriptActionManager
{
public:
  ScriptActionManager();
  ~ScriptActionManager();
  
  struct variablestruct
  {
     MString name;
     MString value;
  };

  ScriptAction* CreateAction(MString option);
  void SetApplicationPath(MString applicationpath);
  void AddAction(ScriptAction* action);
  void AddAction(MString option,std::vector<MString> param);
  void Execute();

  MString Convert(MString param);
  MString ConvertExtra(MString param);
  void SetVariable(MString name,MString value);
  std::vector<MString> GetVariable(MString name);
  void DisplayVariableList();
  void SetLineNumber(int linenumber);
  void SetError(ScriptError* error);
  ScriptError* GetError();
  bool TestParam();
  void Clear();
  bool TestConvert(MString param,int linenumber);
  void SetTestVariable(MString name);
  bool IsTestVariable(MString name);
  std::vector<MString> GetParamsFromVariable(MString name);
  void SetProgressManager(ProgressManager* progressmanager);


protected:
  std::vector<ScriptAction*> m_actionlist;
  ScriptAction* m_parentaction;
  std::vector<variablestruct*> m_variablelist;
  std::vector<MString> m_variabletestlist;
  int m_linenumber;
  ScriptError* m_error;
  ProgressManager* m_progressmanager;
  MString m_applicationpath;
};

} // end namespace bm

#endif
