/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptRunAction.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __ScriptRunAction_h_
#define __ScriptRunAction_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <vector>
#include <iostream>
#include "MString.h"
#include "bmScriptAction.h"

namespace bm {

class ScriptRunAction : public ScriptAction
{
public:
  typedef std::pair<std::string,ApplicationWrapper*> ApplicationNameType;

  /** Constructor */
  ScriptRunAction();
  /** Descructor */
  ~ScriptRunAction();
  /** Test if the script is valid */
  bool TestParam(ScriptError* error,int linenumber);
  /** Execute the current action*/
  void Execute();
  /** Return the help for the current action */
  MString Help();
  /** Generate condor script. The function delegate to the bmCondor */
  void GenerateCondor(const char* appname);
  /** Parse the XML output */
  void ParseXMLOutput(const char* output);

};

} // end namespace bm

#endif
