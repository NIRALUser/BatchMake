/*=========================================================================

  Program:   BatchMake
  Module:    ApplicationWrapperParam.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __ApplicationWrapperParam_h_
#define __ApplicationWrapperParam_h_

#include <vector>
#include <iostream>
#include "MString.h"

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

class ApplicationWrapperParam
{
public:
  ApplicationWrapperParam();
  ~ApplicationWrapperParam();

  enum Type { Filepath,Flag,Int, Float, String, Enum };
  
  void SetType(Type type);
  void SetType(int type);
  Type GetType();
  void SetParent(int parent);
  int GetParent();

  void SetName(MString name);
  MString GetName();
  void SetValue(MString value);
  MString GetValue();
  void SetEnum(std::vector<MString>);
  std::vector<MString> GetEnum();
  void SetOptional(bool flag);
  bool GetOptional();


private:
  Type m_type;
  MString m_name;
  MString m_value;
  bool m_optional;
  int m_parent;
  std::vector<MString> m_enum;

};

#endif
