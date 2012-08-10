/*=========================================================================

  Program:   BatchMake
  Module:    ApplicationWrapperParamSub.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "ApplicationWrapperParamSub.h"
#include <iostream>

ApplicationWrapperParamSub::ApplicationWrapperParamSub()
{
  m_name = "NA";
  m_ValueDefined = false;
  m_ExternalData = 0;
  m_Noquote = false;
}

ApplicationWrapperParamSub::~ApplicationWrapperParamSub()
{
}

void ApplicationWrapperParamSub::SetType(ApplicationWrapperParamSub::Type type)
{
  m_type = type;
}

void ApplicationWrapperParamSub::SetType(int type)
{
  m_type = (Type)type;
}

ApplicationWrapperParamSub::Type ApplicationWrapperParamSub::GetType() const
{
  return m_type;
}

void ApplicationWrapperParamSub::SetName(MString name)
{
  m_name = name;
}

MString ApplicationWrapperParamSub::GetName() const
{
  return m_name;
}

void ApplicationWrapperParamSub::SetValue(MString value)
{
  m_value = value;
}

MString ApplicationWrapperParamSub::GetValue() const
{
  return m_value;
}

void ApplicationWrapperParamSub::SetEnum(std::vector<MString> enumlist)
{
  m_enum.clear();
  for (unsigned int i=0;i<enumlist.size();i++)
    m_enum.push_back(enumlist[i]);
}

std::vector<MString> ApplicationWrapperParamSub::GetEnum() const
{
  return m_enum;
}


void ApplicationWrapperParamSub::SetOptional(bool flag)
{
  m_optional = flag;
}

bool ApplicationWrapperParamSub::GetOptional() const
{
  return m_optional;
}

const char* ApplicationWrapperParamSub::GetTypeAsChar() const
{
  switch(m_type)
    {
    case Filepath: return "string"; break;
    case Flag: return "string"; break;
    case Int: return "int"; break;
    case Float: return "float"; break;
    case String: return "string"; break;
    case Enum: return "enum"; break;
    default:
      return "NA";
    }
}
