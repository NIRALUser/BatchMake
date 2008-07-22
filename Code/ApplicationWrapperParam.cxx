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
#include <cstring>

ApplicationWrapperParam::ApplicationWrapperParam()
{
}

ApplicationWrapperParam::~ApplicationWrapperParam()
{
}

ApplicationWrapperParamSub* ApplicationWrapperParam::GetParamSub(MString name)
{
  for (unsigned int i=0 ; i<m_Params.size() ; i++)
    {
    if (m_Params[i].GetName() == name)
	  {
      return &m_Params[i];
      }
    }

  return 0;
}


void ApplicationWrapperParam::RemoveParamSub(MString name)
{
  std::vector<ApplicationWrapperParamSub>::iterator m_it = m_Params.begin();
  for(unsigned int i=0 ; i<m_Params.size() ; i++)
    {
    if (m_Params[i].GetName() == name)
	  {
	  m_Params.erase(m_it);
      return;
	  }
	m_it++;
	}
}


void ApplicationWrapperParam::ClearParamSubValues()
{
  std::vector<ApplicationWrapperParamSub>::iterator it = m_Params.begin();
  while(it != m_Params.end())
    {
    (*it).SetValueDefined(false);
	it++;
    }
}


bool ApplicationWrapperParam::ParamSubExists(std::string first)
{
  std::vector<ApplicationWrapperParamSub>::iterator it = m_Params.begin();
  while(it != m_Params.end())
    {
    if(!strcmp((*it).GetName().toChar(),first.c_str()))
      {
      return true;
      }
    it++;
    }
  return false;
}


bool ApplicationWrapperParam::CheckSubValueDefined(bool relativePath, std::string* line)
{
  std::vector<ApplicationWrapperParamSub>::iterator it = m_Params.begin();
  while(it != m_Params.end())
    {
	if((*it).IsValueDefined())
	  {
	  if(line->size()>0)
        {
		*line += " ";
        }

	  // remove the absolute path if the relativePath is on
      if(relativePath && (*it).GetExternalData()>0)
        {
        MString appname = (*it).GetValue().rend("/");
        appname = appname.rend("\\");
        appname = appname.removeChar('\\');
        appname = appname.removeChar('/');

        appname = appname.removeChar('\"');
        std::string sappname = "\"";
        sappname += appname.toChar();

        while(sappname[sappname.size()-1]==' ')
          {
          sappname = sappname.substr(0,sappname.size()-1);
          }

        sappname += "\"";

        *line += sappname;
        }
      else
        {
        *line += (*it).GetValue().toChar();
        } 
	  }
	else
	  {
	  return false;
	  }
	it++;
    }
  return false;
}


void ApplicationWrapperParam::SetParamSubValue(std::string first, std::string second, std::string value, bool boolean)
{
  if(boolean)
	{
    std::vector<ApplicationWrapperParamSub>::iterator itChild = m_Params.begin();
    while(itChild != m_Params.end())
	  {
      if(!strcmp((*itChild).GetName().toChar(),second.c_str()))
        {
        (*itChild).SetValueDefined(true);
        (*itChild).SetValue(value.c_str());
        }
	  itChild++;
	  }
	}

  else
    {
	std::vector<ApplicationWrapperParamSub>::iterator it = m_Params.end();
	do
      {
      it--;
      if(!strcmp((*it).GetName().toChar(),second.c_str()))
        {
        (*it).SetValueDefined(true);
        (*it).SetValue(value.c_str());
	    break;
        }
      }
    while(it != m_Params.begin());
	}
}


std::vector<std::string> ApplicationWrapperParam::ShowApplicationOptionsSubParams(std::string parent) const
{
  std::vector<std::string> parameters;
  for(unsigned int i=0 ; i<m_Params.size() ; i++)
    {
    std::string text = parent;
	text += ".";
	text +=  m_Params[i].GetName().toChar();
	parameters.push_back(text);
    }

  return parameters;
}
