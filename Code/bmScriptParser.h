/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptParser.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __ScriptParser_h_
#define __ScriptParser_h_
#include "bmScriptAction.h"
#include "bmScriptError.h"
#include "bmProgressManager.h"

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <vector>
#include <iostream>
#include "MString.h"
#include "bmScriptActionManager.h"

#ifdef BM_GRID
  #include "bmGrid.h"
#endif

namespace bm {

class ScriptParser
{
public:
  ScriptParser();
  ~ScriptParser();
  
  void Load(MString filename);
  bool Parse(MString Line);
  bool CheckOption(MString param);
  bool AddOption(MString option, MString param);
  ScriptAction::ParametersType GetParams(MString param);
  void AddCodeLine(MString line,unsigned long pos=0);
  bool Parse();
  void SetError(ScriptError* error);
  bool Execute();
  void SetApplicationPath(MString applicationpath);
  void SetScriptPath(MString scriptpath);
  void SetProgressManager(ProgressManager* progressmanager);
  bool Execute(MString filename,unsigned long pos=0);
  bool Compile(MString filename,unsigned long pos=0,bool isInclude = false);
  void LoadWrappedApplication(MString applicationpath);
  std::vector<ApplicationWrapper*>* GetApplicationList();
  void Reset();

#ifdef BM_GRID
  void SetGridModule(Grid* grid) {m_scriptactionmanager->SetGridModule(grid);}
#endif

  /** Return the ScriptActionManager */
  ScriptActionManager* GetScriptActionManager() {return m_scriptactionmanager;}

  std::vector<MString> & GetCode() {return m_code;}
  void RemoveCodeLine(unsigned int line);

  unsigned long GetLineNumber() {return m_linenumber;}

protected:
  int m_linenumber;
  ScriptActionManager* m_scriptactionmanager;
  std::vector<MString> m_code;
  ScriptError* m_error;
  MString m_applicationpath;
  std::vector<ApplicationWrapper*>* m_applicationlist;
};

} // end namespace bm

#endif