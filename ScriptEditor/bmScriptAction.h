/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptAction.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __ScriptAction_h_
#define __ScriptAction_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <vector>
#include <iostream>
#include "MString.h"
#include "bmScriptActionManager.h"
#include "bmProgressManager.h"
#include "bmScriptError.h"

namespace bm {

class ScriptActionManager;

class ScriptAction
{
public:
  ScriptAction();
  ~ScriptAction();
  
  typedef std::vector<MString> ParametersType;
  typedef char* (*Help_Func)(void);

  virtual void SetName(MString name);
  virtual MString GetName();
  virtual void SetParameters(ParametersType parameters);
  virtual void Execute();
  virtual void SetParent(ScriptAction* parent);
  virtual ScriptAction* GetParent();
  virtual void AddAction(ScriptAction* action);
  virtual void SetManager(ScriptActionManager* manager);
  virtual void SetProgressManager(ProgressManager* progressmanager);
  virtual bool TestParam(ScriptError* error,int linenumber=-1);
  virtual void Delete();
  virtual MString Help();

protected:
  ParametersType m_parameters;
  MString m_name;
  ScriptAction* m_parent;
  ScriptActionManager* m_manager;
  ProgressManager* m_progressmanager;

};

} // end namespace bm

#endif