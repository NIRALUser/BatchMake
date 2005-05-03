/*=========================================================================

  Program:   BatchMake
  Module:    ApplicationWrapper.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "ApplicationWrapper.h"
#include <iostream>
#include "XMLWriter.h"
#include "XMLReader.h"

ApplicationWrapper::ApplicationWrapper()
{
  m_name = "";
}

ApplicationWrapper::~ApplicationWrapper()
{
}

void ApplicationWrapper::SetApplicationPath(MString applicationpath)
{
  m_applicationpath = applicationpath;
}


/** Return the current command line arguments */
std::string ApplicationWrapper::GetCurrentCommandLineArguments()
{
  std::string line = "";

  std::vector<ApplicationWrapperParam>::iterator it = m_params.begin();

  while(it != m_params.end())
    {
    if((*it).IsValueDefined())
      {
      if(line.size()>0)
        {
        line += " ";
        }
      line += (*it).GetValue().toChar();
      }
    it++;
    }

  return line;
}

/** Set the parameter value */
void ApplicationWrapper::SetParameterValue(std::string first, std::string second, std::string value)
{
  std::vector<ApplicationWrapperParam>::iterator it = m_params.begin();

  unsigned int parent = 0;
  while(it != m_params.end())
    {
    if((*it).GetParent() == 0)
      {
      parent++;
      }
    if(!strcmp((*it).GetName().toChar(),first.c_str()))
      {
      // Look for the child
      if(second.size() > 0)
        {
        std::vector<ApplicationWrapperParam>::iterator itChild = m_params.begin();
        while(itChild != m_params.end())
          {
          if(!strcmp((*itChild).GetName().toChar(),second.c_str())
            && ((*itChild).GetParent() == parent)
            )
            {
            (*itChild).SetValueDefined(true);
            (*itChild).SetValue(value.c_str());
            }
          itChild++;
          }
        }
      if((*it).GetType() != ApplicationWrapperParam::Flag)
        {
        (*it).SetValueDefined(true);
        (*it).SetValue(value.c_str());
        }
      else
        {
        (*it).SetValueDefined(true);

        if(!strcmp(value.c_str(),"0")
          || !strcmp(value.c_str(),"false")
          || !strcmp(value.c_str(),"false ")
          || !strcmp(value.c_str(),"False ")
          || !strcmp(value.c_str(),"FALSE ")          
          || !strcmp(value.c_str(),"False")
          || !strcmp(value.c_str(),"FALSE")
          )
          {
          (*it).SetValueDefined(false);
          }
        }
      }
    it++;
    }


}

MString ApplicationWrapper::GetApplicationPath()
{
  return m_applicationpath;
}

void ApplicationWrapper::SetName(MString name)
{
  m_name = name;
}

MString ApplicationWrapper::GetName()
{
  return m_name;
}

void ApplicationWrapper::SetVersion(MString version)
{
  m_version = version;
}

MString ApplicationWrapper::GetVersion()
{
  return m_version;
}

void ApplicationWrapper::AddParam(ApplicationWrapperParam param)
{
  m_params.push_back(param);
}

void ApplicationWrapper::DeleteParam(MString name)
{
  int m_offset=0;
  for (unsigned int l=0;l<m_params.size();l++)
  {
    if (m_params[l].GetName() == name)
      m_offset = l;
  };

  int m_currentflag = 0;
  for (unsigned int k=0;k<m_offset+1;k++)
  {
    if (m_params[k].GetType() == 1)
      m_currentflag++;
  }

  if (m_params[m_offset].GetType() == 1)
  {
    if (m_currentflag != 0)
    {
      for (unsigned int l=0;l<m_params.size();l++)
      {
        if (m_params[l].GetParent() == m_currentflag)
        {
           m_params[l].SetParent(0);
        }
      }
    }
  }
  
  std::vector<ApplicationWrapperParam>::iterator m_it = m_params.begin();

  for (unsigned int i=0;i<m_params.size();i++)
  {
    if (m_params[i].GetName() == name)
    {
        m_params.erase(m_it);
        return;
    }
    m_it++;
  }
}


std::vector<ApplicationWrapperParam> & ApplicationWrapper::GetParams()
{
  return m_params;
}

ApplicationWrapperParam* ApplicationWrapper::GetParam(MString name)
{
  for (unsigned int i=0;i<m_params.size();i++)
  {
    if (m_params[i].GetName() == name)
      return &m_params[i];
  }

  return 0;
}


void ApplicationWrapper::UpParam(MString name)
{
  int m_offset=0;
  for (unsigned int l=0;l<m_params.size();l++)
  {
    if (m_params[l].GetName() == name)
      m_offset = l;
  };

  if (m_offset != 0)
  {
    ApplicationWrapperParam m_paramtemp = m_params[m_offset-1];
    m_params[m_offset-1] = m_params[m_offset];
    m_params[m_offset] = m_paramtemp;
  }
}


void ApplicationWrapper::DownParam(MString name)
{
  int m_offset=0;
  for (unsigned int l=0;l<m_params.size();l++)
  {
    if (m_params[l].GetName() == name)
      m_offset = l;
  };

  if (m_offset != m_params.size()-2)
  {
    ApplicationWrapperParam m_paramtemp = m_params[m_offset+1];
    m_params[m_offset+1] = m_params[m_offset];
    m_params[m_offset] = m_paramtemp;
  }
}

void ApplicationWrapper::DisplayParam(MString& m_line,int offset)
{
  if (m_params[offset].GetOptional())
      m_line += " [";
    else
      m_line += " <";

    std::cout << m_params[offset].GetName().toChar() << std::endl;

    switch(m_params[offset].GetType())
    {
      case 0: m_line += m_params[offset].GetName();
              break;

      case 1: m_line += m_params[offset].GetValue();
              break;
      
      case 2: m_line += "int";
              break;

      case 3: m_line += "float";
              break;

      case 4: m_line += "string";
              break;

      case 5: for (unsigned j=0;j<m_params[offset].GetEnum().size();j++)
              {
                m_line += m_params[offset].GetEnum()[j];
                if (j != m_params[offset].GetEnum().size()-1)
                  m_line += "|";
              }  
              break;

    }

    int m_currentflag = 0;
      for (unsigned int k=0;k<offset+1;k++)
      {
        if (m_params[k].GetType() == 1)
            m_currentflag++;
      }
    
      if (m_params[offset].GetType() == 1)
      {
        if (m_currentflag != 0)
        {
          for (unsigned int i=0;i<m_params.size();i++)
          {
            if (m_params[i].GetParent() == m_currentflag)
            {
               DisplayParam(m_line,i);
            }
          }
        }
      }

   if (m_params[offset].GetOptional())
      m_line += "]";
    else
      m_line += ">";
}


MString ApplicationWrapper::GetExampleLine()
{
  MString m_line = "";
  if (m_applicationpath.length() != 0)
    m_line = m_applicationpath.rend("/")+1;
  
  for (unsigned int i=0;i<m_params.size();i++)
  {
    if (m_params[i].GetParent() == 0)
      DisplayParam(m_line,i);
  }

  return m_line;
}

void ApplicationWrapper::Save(MString filename)
{
  XMLWriter m_writer;
  m_writer.Open(filename.toChar());
  m_writer.Start("BatchMakeApplicationWrapper");
  m_writer.Write(MString("BatchMakeApplicationWrapperVersion"),MString("1.0"));
    m_writer.Start("Module");
      m_writer.Write(MString("Name"),m_name);
      m_writer.Write(MString("Version"),m_version);
      m_writer.Write(MString("Path"),m_applicationpath);
      m_writer.Start("Parameters");
      for (unsigned int i=0;i<m_params.size();i++)
      {
        m_writer.Start("Param");
          m_writer.Write(MString("Type"),m_params[i].GetType());
          m_writer.Write(MString("Name"),m_params[i].GetName());
          m_writer.Write(MString("Value"),m_params[i].GetValue());
          m_writer.Write(MString("Parent"),m_params[i].GetParent());
          m_writer.Write(MString("Optional"),(int)m_params[i].GetOptional());
          for (unsigned j=0;j<m_params[i].GetEnum().size();j++)
          {
             m_writer.Write(MString("Enum"),m_params[i].GetEnum()[j]);
          }
        m_writer.End("Param");
      }
      m_writer.End("Parameters");
    m_writer.End("Module");
  m_writer.End("BatchMakeApplicationWrapper");
  m_writer.Close();
}

void ApplicationWrapper::Load(MString filename)
{
  XMLReader m_reader;
  m_reader.Open(filename.toChar());
  MString m_balise = m_reader.GetBalise();
  while (m_balise != "/BatchMakeApplicationWrapper")
  {
    if (m_balise == "Module")
      ReadModule(m_reader);
    
    m_balise = m_reader.GetBalise();
  }
}

void ApplicationWrapper::ReadModule(XMLReader& m_reader)
{
 MString m_balise = m_reader.GetBalise();
 while (m_balise != "/Module")
 {
   if (m_balise == "Name")      m_name = m_reader.GetValue();
   if (m_balise == "Version")   m_version = m_reader.GetValue();
   if (m_balise == "Path")      m_applicationpath = m_reader.GetValue();

   if (m_balise == "Param")
      ReadParam(m_reader);
  
   m_balise = m_reader.GetBalise(); 
 }
}

void ApplicationWrapper::ReadParam(XMLReader& m_reader)
{
  MString m_balise = m_reader.GetBalise();
  ApplicationWrapperParam m_param;
  std::vector<MString> m_list;
  while (m_balise != "/Param")
  {
    if (m_balise == "Name")     m_param.SetName(m_reader.GetValue());
    if (m_balise == "Type")     m_param.SetType(m_reader.GetValue().toInt());
    if (m_balise == "Value")    m_param.SetValue(m_reader.GetValue());
    if (m_balise == "Parent")   m_param.SetParent(m_reader.GetValue().toInt());
    if (m_balise == "Optional") m_param.SetOptional(m_reader.GetValue().toBool());
    if (m_balise == "Enum")     m_list.push_back(m_reader.GetValue());

    m_balise = m_reader.GetBalise();
  }

  m_param.SetEnum(m_list);
  AddParam(m_param);
}
