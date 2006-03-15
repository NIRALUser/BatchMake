/*=========================================================================

  Program:   BatchMake
  Module:    ApplicationWrapper.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __ApplicationWrapper_h_
#define __ApplicationWrapper_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <vector>
#include <iostream>
#include "ApplicationWrapperParam.h"
#include "MString.h"
#include "XMLReader.h"


class ApplicationWrapper
{
public:
  ApplicationWrapper();
  ~ApplicationWrapper();

  void SetApplicationPath(MString applicationpath);
  MString GetApplicationPath();
  void SetName(MString name);
  MString GetName();
  void SetVersion(MString version);
  MString GetVersion();
  void AddParam(ApplicationWrapperParam param);
  ApplicationWrapperParam* GetParam(MString name);
  void Save(MString filename);
  void Load(MString filename);
  MString GetExampleLine();
  void ReadModule(XMLReader& m_reader);
  void ReadParam(XMLReader& m_reader);
  std::vector<ApplicationWrapperParam> & GetParams();
  void DisplayParam(MString& m_line,int offset);
  void DeleteParam(MString name);
  void UpParam(MString name);
  void DownParam(MString name);
  void AutomaticCommandLineParsing(const char * _path);

  /** Return the current command line arguments */
  std::string GetCurrentCommandLineArguments(bool relativePath=true);

  /** Set the parameter value */
  void SetParameterValue(std::string first, std::string second, std::string value);

  /** Set/Get if the application uses sequential arguments */
  void SetSequentialParsing(bool val)
    {
    m_Sequential = val; 
    m_SequentialParams.clear();
    }
  bool GetSequentialParsing() {return m_Sequential;}

private:
  MString m_name;
  MString m_applicationpath;
  MString m_version;
  bool    m_Sequential; // Set if we should use the sequential application
  std::vector<ApplicationWrapperParam> m_params;
  std::vector<ApplicationWrapperParam> m_SequentialParams;
  
};

#endif
