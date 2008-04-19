/*=========================================================================

  Program:   BatchMake
  Module:    ApplicationWrapperParamSub.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __ApplicationWrapperParamSub_h_
#define __ApplicationWrapperParamSub_h_

#include <vector>
#include <iostream>
#include "MString.h"

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

class ApplicationWrapperParamSub
{
public:
  ApplicationWrapperParamSub();
  ~ApplicationWrapperParamSub();

  enum Type { Filepath,Flag,Int, Float, String, Enum };
  
  void SetType(Type type);
  void SetType(int type);
  Type GetType() const;
  const char* GetTypeAsChar() const;
  void SetName(MString name);
  MString GetName() const;
  void SetValue(MString value);
  MString GetValue() const;
  void SetEnum(std::vector<MString>);
  std::vector<MString> GetEnum() const;
  void SetOptional(bool flag);
  bool GetOptional() const;

  bool IsValueDefined() const {return m_ValueDefined;}
  void SetValueDefined(bool val) {m_ValueDefined = val;}

  /** Set/Get if the parameters is an input data file */
  void SetExternalData(unsigned int external) {m_ExternalData = external;}
  unsigned int GetExternalData() const {return m_ExternalData;}


private:
  Type m_type;
  MString m_name;
  MString m_value;
  bool m_optional;
  std::vector<MString> m_enum;
  bool m_ValueDefined;
  unsigned int m_ExternalData;
};

#endif
