/*=========================================================================

  Program:   BatchMake
  Module:    ApplicationWrapperParam.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "ApplicationWrapperParam.h"
#include <iostream>

ApplicationWrapperParam::ApplicationWrapperParam()
{
  m_name = "NA";
  m_parent = 0;
  m_ValueDefined = false;
  m_ExternalData = false;
}

ApplicationWrapperParam::~ApplicationWrapperParam()
{
}

void ApplicationWrapperParam::SetType(ApplicationWrapperParam::Type type)
{
  m_type = type;
}

void ApplicationWrapperParam::SetType(int type)
{
  m_type = (Type)type;
}

ApplicationWrapperParam::Type ApplicationWrapperParam::GetType()
{
  return m_type;
}

void ApplicationWrapperParam::SetParent(int parent)
{
  m_parent = parent;
}

int ApplicationWrapperParam::GetParent() const
{
  return m_parent;
}


void ApplicationWrapperParam::SetName(MString name)
{
  m_name = name;
}

MString ApplicationWrapperParam::GetName() const
{
  return m_name;
}

void ApplicationWrapperParam::SetValue(MString value)
{
  m_value = value;
}

MString ApplicationWrapperParam::GetValue() const
{
  return m_value;
}

void ApplicationWrapperParam::SetEnum(std::vector<MString> enumlist)
{ 
  m_enum.clear();
  for (unsigned int i=0;i<enumlist.size();i++)
    m_enum.push_back(enumlist[i]);
}
  
std::vector<MString> ApplicationWrapperParam::GetEnum()
{
  return m_enum;
}


void ApplicationWrapperParam::SetOptional(bool flag)
{
  m_optional = flag;
}

bool ApplicationWrapperParam::GetOptional()
{
  return m_optional;
}


