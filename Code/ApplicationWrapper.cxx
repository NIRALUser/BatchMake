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

#include <itksys/Directory.hxx>
#include <itksys/SystemTools.hxx>
#include <itksys/Process.h>

#include "metaCommand.h"

#ifndef WIN32
  #include <unistd.h>
#endif

#ifdef WIN32
  #include <Windows.h>
  #include <fcntl.h>
  #include <errno.h>
  #include <io.h>
  #include <stdio.h>
  #include <process.h>
#else
  #include <unistd.h>
  #include <sys/wait.h>
  #include <fcntl.h>
  #include <errno.h>
#endif

static int parent = 0;

ApplicationWrapper::ApplicationWrapper()
{
  m_name = "";
  m_Sequential = false;
  m_SequentialParams.clear();
  m_DependsOn = -1; // previous one, i.e sequential
  m_SingleNode = 0;
  m_AfterEndBarrier = false;
  m_ExecutionBlockNumber = 0;
  m_InputDataToTransfer.clear();
}

ApplicationWrapper::~ApplicationWrapper()
{
}

void ApplicationWrapper::SetApplicationPath(MString applicationpath)
{
  m_applicationpath = applicationpath;
}


/** Return the current command line arguments */
std::string ApplicationWrapper::GetCurrentCommandLineArguments(bool relativePath)
{
  std::cout<<"GetCurrentCommandLineArguments"<<std::endl;
  std::string line = "";

  std::vector<ApplicationWrapperParam>::iterator it = m_params.begin();
  std::vector<ApplicationWrapperParam>::iterator end = m_params.end();
  
  if(m_Sequential)
    {
    it  = m_SequentialParams.begin();
    end = m_SequentialParams.end();
    }

  int currentParam = 0;

  while(it != end)
    {
    if((*it).GetType() == ApplicationWrapperParam::Flag)
      {
      currentParam++;
      }
    bool valueDefined = (*it).IsValueDefined();
    if(!valueDefined) // if the value is not defined we check if we have children defined
      {
      std::vector<ApplicationWrapperParam>::iterator itc = it;
      while(itc != end)
        {
        if(currentParam>0 && (*itc).GetParent() == currentParam
          && (*itc).IsValueDefined()
          )
          {
          valueDefined = true;
          }
        itc++;
        }
      }

    if(valueDefined)
      {
      if(line.size()>0)
        {
        line += " ";
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

        line += sappname;
        
        }
      else
        {
        line += (*it).GetValue().toChar();
        }     
      }
    it++;
    }

  return line;
}

/** Return true if the parameter exists */
bool ApplicationWrapper::ParameterExists(std::string first)
{
  std::cout<<"ParameterExists"<<std::endl;
  std::vector<ApplicationWrapperParam>::iterator it = m_params.begin();

  unsigned int parent = 0;
  while(it != m_params.end())
    {
    if(!strcmp((*it).GetName().toChar(),first.c_str()))
      {
      return true;
      }
    it++;
    }
  return false;
}

/** Clear all the parameter values */
void ApplicationWrapper::ClearParameterValues()
{
  std::cout<<"ClearParameterValues"<<std::endl;
  std::vector<ApplicationWrapperParam>::iterator it = m_params.begin();
  while(it != m_params.end())
    {
    (*it).SetValueDefined(false);
    /*if((*it).GetType() != ApplicationWrapperParam::Flag)
      {
      (*it).SetValue("");
      }
    else
      {
      (*it).SetValue("0");
      }*/
    it++;
    }
}

/** Set the parameter value */
void ApplicationWrapper::SetParameterValue(std::string first, std::string second, std::string value)
{
  std::cout<<"SetParameterValue"<<std::endl;
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
      if((*it).GetType() != ApplicationWrapperParam::Flag)
        {
        (*it).SetValueDefined(true);
        (*it).SetValue(value.c_str());
        }
      else
        {
        (*it).SetValueDefined(true);

        if(!strncmp(value.c_str(),"0",1)
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

      
      // We manage the sequential params here
      if(m_Sequential)
        {
        if((m_SequentialParams.size() == 0)
          || (m_SequentialParams[m_SequentialParams.size()-1].GetParent()!=parent)
          )
          {
          m_SequentialParams.push_back(*it);
          // We add all the things
          std::vector<ApplicationWrapperParam>::iterator itChild = m_params.begin();
          while(itChild != m_params.end())
            {
            if((*itChild).GetParent() == parent)
              {
              if(!strcmp((*itChild).GetName().toChar(),second.c_str()))
                {
                (*itChild).SetValueDefined(true);
                (*itChild).SetValue(value.c_str());
                }
              (*itChild).SetParent(parent);
              m_SequentialParams.push_back(*itChild);
                //std::cout << (*itChild).GetParent() << std::endl;
              //  std::cout << "Addding " << (*itChild).GetName().toChar() << std::endl;
              }
            itChild++;
            }
          }
        else // we look in the list where is the parameter
          {
          std::vector<ApplicationWrapperParam>::iterator it = m_SequentialParams.end();

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
            while(it != m_SequentialParams.begin());
          }
        }
      else
        {
        // Look for the child
        if(second.size() > 0)
          {        
          std::vector<ApplicationWrapperParam>::iterator itChild = m_params.begin();
          while(itChild != m_params.end())
            {
            std::string childname = first;
            childname += ".";
            childname += second;
             if(!strcmp((*itChild).GetName().toChar(),childname.c_str()))
              {
              (*itChild).SetValueDefined(true);
              (*itChild).SetValue(value.c_str());
              }
            itChild++;
            }
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
  std::cout<<"DeleteParam"<<std::endl;
  int m_offset=0;
  for (unsigned int l=0;l<m_params.size();l++)
  {
    if (m_params[l].GetName() == name)
      m_offset = l;
  };

  int m_currentflag = 0;
  for (int k=0;k<m_offset+1;k++)
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
  std::cout<<"UpParam"<<std::endl;
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
  std::cout<<"DownParam"<<std::endl;
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
  std::cout<<"DisplayParam"<<std::endl;
  if (m_params[offset].GetOptional())
      m_line += " [";
    else
      m_line += " <";

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

      case 4: if(m_params[offset].GetName().length()>0)
                {m_line += m_params[offset].GetName();}
              else
                {m_line += "string";}
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
      for (int k=0;k<offset+1;k++)
      {
        if (m_params[k].GetType() == 1)
            m_currentflag++;
      }
    
	  std::cout<<m_currentflag<<" "<<std::endl;
      if (m_params[offset].GetType() == 1)
      {
        if (m_currentflag != 0)
        {
          for (unsigned int i=0;i<m_params.size();i++)
          {
            if (m_params[i].GetParent() == m_currentflag)
            {
				std::cout<<m_line.toChar()<<std::endl;
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
  std::cout<<"GetExampleLine"<<std::endl;
  MString m_line = "";
  std::string applicationPath = m_applicationpath.toChar();
  if (m_applicationpath.length() != 0)
    {
    if(applicationPath.find("/") != -1)
      {
      m_line = m_applicationpath.rend("/")+1;
      }
    else
      {
      m_line = m_applicationpath;  
      }  
    }
  
  for (unsigned int i=0;i<m_params.size();i++)
    {
    if (m_params[i].GetParent() == 0)
      {
      DisplayParam(m_line,i);
      }
    }

  return m_line;
}


void ApplicationWrapper::Save(MString filename)
{
  std::cout<<"Save"<<std::endl;
  std::vector<std::string> parentParameters, parameters;
  std::string parentParameter, parameter;

  XMLWriter m_writer;
  m_writer.Open(filename.toChar());
  m_writer.Start("?xml version=\"1.0\" encoding=\"utf-8\"?");
  m_writer.Start("executable");
      m_writer.Write(MString("Name"),m_name);
      m_writer.Write(MString("Version"),m_version);
      m_writer.Write(MString("Path"),m_applicationpath);

      m_writer.Start("Parameters");
      for (unsigned int i=0 ; i<m_params.size() ; i++)
        {
        if(m_params[i].GetParent() == 0)
          {
          parentParameter = m_params[i].GetName().toChar();
          parentParameters.push_back(m_params[i].GetName().toChar());
          m_writer.Start("Parameter");
          m_writer.Write(MString("Type"),m_params[i].GetType());
          m_writer.Write(MString("Name"),m_params[i].GetName());
          m_writer.Write(MString("Value"),m_params[i].GetValue());
          m_writer.Write(MString("External"),(int)m_params[i].GetExternalData());
          m_writer.Write(MString("Optional"),(int)m_params[i].GetOptional());
          for(unsigned int j=0 ; j<m_params.size() ; j++)
            {
            if(m_params[j].GetParent()!= 0)
              {
              parameter =  m_params[j].GetName().toChar();
              std::string::size_type loc =parameter.find(parentParameter, 0); 
              if(loc != string::npos)
                {
                parameters.push_back(m_params[j].GetName().toChar());
                m_writer.Start("SubParameter");
                m_writer.Write(MString("Type"),m_params[j].GetType());
                m_writer.Write(MString("Name"),m_params[j].GetName());
                m_writer.Write(MString("Value"),m_params[j].GetValue());
                m_writer.Write(MString("External"),(int)m_params[j].GetExternalData());
                m_writer.Write(MString("Optional"),(int)m_params[j].GetOptional());
                m_writer.End("SubParameter");
                }
              }
            }
            m_writer.End("Parameter");
          }
        }
      m_writer.End("Parameters");
  m_writer.End("executable");
  m_writer.Close();
}


void ApplicationWrapper::Load(MString filename)
{
  std::cout<<"Load"<<std::endl;
  XMLReader m_reader;
  m_reader.Open(filename.toChar());
  MString m_balise = m_reader.GetBalise();

  if (m_balise == "BatchMakeApplicationWrapper")
    {
    while (m_balise != "/BatchMakeApplicationWrapper")
      {
      if (m_balise == "Module")
        {
        ReadModule(m_reader, false);
        }
      m_balise = m_reader.GetBalise();
      }
    }
  else
    {
    ReadModule(m_reader, true);
    }
}

void ApplicationWrapper::ReadModule(XMLReader& m_reader, bool newVersion)
{
  MString m_balise = m_reader.GetBalise();

  if(newVersion)
    {
    while (m_balise != "/executable")
      {
      if (m_balise == "Name")      m_name = m_reader.GetValue();
      if (m_balise == "Version")   m_version = m_reader.GetValue();
      if (m_balise == "Path")      m_applicationpath = m_reader.GetValue();
      if (m_balise == "Parameter")
        {
        parent++;
        ReadParam(m_reader, true);
        }
      m_balise = m_reader.GetBalise(); 
      }
    }
  else
    {
    while (m_balise != "/Module")
      {
      if (m_balise == "Name")      m_name = m_reader.GetValue();
      if (m_balise == "Version")   m_version = m_reader.GetValue();
      if (m_balise == "Path")      m_applicationpath = m_reader.GetValue();

      if (m_balise == "Param")
        {
        ReadParam(m_reader, false);
        }
      m_balise = m_reader.GetBalise(); 
      }
    }
}

void ApplicationWrapper::ReadParam(XMLReader& m_reader, bool newVersion)
{
  MString m_balise = m_reader.GetBalise();
  ApplicationWrapperParam m_param;
  std::vector<MString> m_list;

  if(newVersion)
    {
    while (m_balise != "/Parameter")
      {
      m_param.SetParent(0);
      if (m_balise == "Name")     m_param.SetName(m_reader.GetValue());
      if (m_balise == "Type")     m_param.SetType(m_reader.GetValue().toInt());
      if (m_balise == "Value")    m_param.SetValue(m_reader.GetValue());

      if (m_balise == "External") m_param.SetExternalData(m_reader.GetValue().toInt());
      if (m_balise == "Optional") m_param.SetOptional(m_reader.GetValue().toBool());   
      if (m_balise == "Enum")     m_list.push_back(m_reader.GetValue());
      if (m_balise == "SubParameter")
        {
        MString m_balise1 = m_reader.GetBalise();
        ApplicationWrapperParam m_param;
        //std::vector<MString> m_list;
        while (m_balise1 != "/SubParameter")
          {
          m_param.SetParent(parent);
          if (m_balise1 == "Name")     m_param.SetName(m_reader.GetValue());
          if (m_balise1 == "Type")     m_param.SetType(m_reader.GetValue().toInt());
          if (m_balise1 == "Value")    m_param.SetValue(m_reader.GetValue());
          if (m_balise1 == "External") m_param.SetExternalData(m_reader.GetValue().toInt());
          if (m_balise1 == "Optional") m_param.SetOptional(m_reader.GetValue().toBool());   
          if (m_balise1 == "Enum")     m_list.push_back(m_reader.GetValue());
          m_balise1 = m_reader.GetBalise();
          }
        m_param.SetEnum(m_list);
        AddParam(m_param);
        }
      m_balise = m_reader.GetBalise();
      }
    m_param.SetEnum(m_list);
    AddParam(m_param);
    }
  else
    {
    while (m_balise != "/Param")
      {
      if (m_balise == "Name")     m_param.SetName(m_reader.GetValue());
      if (m_balise == "Type")     m_param.SetType(m_reader.GetValue().toInt());
      if (m_balise == "Value")    m_param.SetValue(m_reader.GetValue());
      if (m_balise == "Parent")   m_param.SetParent(m_reader.GetValue().toBool());
      if (m_balise == "External") m_param.SetExternalData(m_reader.GetValue().toInt());
      if (m_balise == "Optional") m_param.SetOptional(m_reader.GetValue().toBool());   
      if (m_balise == "Enum")     m_list.push_back(m_reader.GetValue());
      m_balise = m_reader.GetBalise();
      }
    m_param.SetEnum(m_list);
    AddParam(m_param);
    }
}


/** Automatic command line parsing. If the current pointed program 
 *  supports --xml option */
bool ApplicationWrapper::AutomaticCommandLineParsingSlicer(const char * _path)
{
  std::cout<<"AutomaticCommandLineParsingSlicer"<<std::endl;
  // Run the application
  std::string path = _path;
  std::string program = path;
  std::string vxmlarg = "--xml";
  std::cout << "Running = " << program.c_str() << " " << vxmlarg.c_str() << std::endl;
  std::string output = "";
  unsigned int i=0;

  // Extract the arguments from the command line
  // Warning: Only works for one space between arguments
  std::vector<const char*> args;
  args.push_back(path.c_str());
  args.push_back(vxmlarg.c_str());
  args.push_back(0);

  // Run the application
  itksysProcess* gp = itksysProcess_New();
  itksysProcess_SetCommand(gp, &*args.begin());
  itksysProcess_SetOption(gp,itksysProcess_Option_HideWindow,1);

  itksysProcess_Execute(gp);

  char* data = NULL;
  int length;
  double timeout = 255;

  while(itksysProcess_WaitForData(gp,&data,&length,&timeout)) // wait for 1s
    {
    for(int i=0;i<length;i++)
      {
      output += data[i];
      }
    }
  itksysProcess_WaitForExit(gp, 0);

  int result = 1;
  switch(itksysProcess_GetState(gp))
    {
    case itksysProcess_State_Exited:
      {
      result = itksysProcess_GetExitValue(gp);
      } break;
    case itksysProcess_State_Error:
      {
      std::cerr << "Error: Could not run " << args[0] << ":\n";
      std::cerr << itksysProcess_GetErrorString(gp) << "\n";
      } break;
    case itksysProcess_State_Exception:
      {
      std::cerr << "Error: " << args[0]
                << " terminated with an exception: "
                << itksysProcess_GetExceptionString(gp) << "\n";
      } break;
    case itksysProcess_State_Starting:
    case itksysProcess_State_Executing:
    case itksysProcess_State_Expired:
    case itksysProcess_State_Killed:
      {
      // Should not get here.
      std::cerr << "Unexpected ending state after running " << args[0]
                << std::endl;
      } break;
    }
  itksysProcess_Delete(gp);
  ModuleDescription moduleDescription;
  ModuleDescriptionParser parser;

  parser.Parse(output.c_str(),moduleDescription);

  // Converting the ModuleDescriptionParser to BatchMake
  this->AddSlicerModuleDescription(&moduleDescription);

  return true;
}

/** Convert a Module description to an internal representation */
bool ApplicationWrapper::
AddSlicerModuleDescription(ModuleDescription* module)
{
  std::cout<<"AddSlicerModuleDescription"<<std::endl;
  // BatchMake only understands parameter types:
  //     file=0, bool=1, int=2, float=3, string=4, enum=5
  //
  // Map what we can into these.
  //
  typedef std::map<std::string, int> ModuleParameterToBatchMakeTypeMap;
  ModuleParameterToBatchMakeTypeMap mp2bm;
  mp2bm["integer"] = 2;  // int
  mp2bm["float"] = 3;  // float
  mp2bm["double"] = 3;  // float
  mp2bm["boolean"] = 1; // bool
  mp2bm["string"] = 4;  // string
  mp2bm["integer-vector"] = 4;  // string
  mp2bm["float-vector"] = 4;  // string
  mp2bm["double-vector"] = 4;  // string
  mp2bm["string-vector"] = 4;  // string
  mp2bm["integer-enumeration"] = 5; // enum
  mp2bm["float-enumeration"] = 3;  // float ???
  mp2bm["double-enumeration"] = 3;  // float ???
  mp2bm["string-enumeration"] = 4;  // string ???
  mp2bm["file"] = 0;  // file
  mp2bm["directory"] = 0;  // file ???
  mp2bm["image"] = 0;  // file
  mp2bm["geometry"] = 0;  // file
  mp2bm["point"] = 4;  // string ???

  // Convert the metaCommand to ApplicationWrapper
  this->SetVersion(module->GetVersion().c_str());

  // extract the name from the filename
  std::string revname;
 
  std::string path=module->GetLocation();
  unsigned int i=0;
  for(i=0;i<path.size();i++)
    {
    if(path[path.size()-1-i] == '/'
      || path[path.size()-1-i] == '\\')
      {
      break;
      }
    revname += path[path.size()-1-i];
    }

  std::string name;
  
  int end=0;
  if(revname.find("exe.") != -1)
    {
    end=4;
    }

  for(i=0;i<revname.size()-end;i++)
    {
    name += revname[revname.size()-i-1];
    }

  this->SetName(module->GetTitle().c_str());
  this->SetApplicationPath(path);

  
  std::vector<ModuleParameterGroup>::const_iterator pgbeginit
    = module->GetParameterGroups().begin();
  std::vector<ModuleParameterGroup>::const_iterator pgendit
    = module->GetParameterGroups().end();
  std::vector<ModuleParameterGroup>::const_iterator pgit;

  ModuleParameterToBatchMakeTypeMap::iterator mp2bmIt;


  // Loop over all parameters that have a flag then loop over the
  // parameters that are index parameters
  //
  
  // Loop over executables with flags
  int pcount = 0;
  for (pgit = pgbeginit; pgit != pgendit; ++pgit)
    {
    // iterate over each parameter in this group
    std::vector<ModuleParameter>::const_iterator pbeginit
      = (*pgit).GetParameters().begin();
    std::vector<ModuleParameter>::const_iterator pendit
      = (*pgit).GetParameters().end();
    std::vector<ModuleParameter>::const_iterator pit;

    for (pit = pbeginit; pit != pendit; ++pit)
      {
      std::string prefix;
      std::string flag;
      bool hasFlag = false;
      
      if ((*pit).GetLongFlag() != "")
        {
        prefix = "--";
        flag = (*pit).GetLongFlag();
        hasFlag = true;
        }
      else if ((*pit).GetFlag() != "")
        {
        prefix = "-";
        flag = (*pit).GetFlag();
        hasFlag = true;
        }

      if (hasFlag)
        {
        pcount++;
       
        // Batchmake uses two parameters to represent non-boolean
        // options.  The first of the parameters is the "flag", the
        // second is the value.  THe Parent tag is used to ling flags
        // to values.
        ApplicationWrapperParam param;
    
        // Batchmake uses two parameters to represent non-boolean
        // options.  THe first of the parameters is the "flag", the
        // second is the value.  THe Parent tag is used to ling flags
        // to values.
        // first parameter in the pair is ALWAYS a bool. If there is
        // not a pair, then the parameter is bool anyway.
        param.SetType(1);
        
        // if we are going to be creating a paired set of parameters,
        // then name the first one the <parameter name>.flag and the
        // second one just <parameter name> for the value.
        if ((*pit).GetTag() != "boolean")
          {
          param.SetName((*pit).GetName());
          }
        else
          {
          // just use the name
          param.SetName((*pit).GetName());
          }

        // Value is the flag
        std::string value = prefix + flag;
        param.SetValue(value.c_str());

        // The flag itself never has a parent
        param.SetParent(0);

        // BatchMake external flag is:
        //   0=nothing, 1=input, 2=output
        //
        // The flag itself is always external = 0
        param.SetExternalData(0);
        
        // Any module parameter that has a flag is optional.
        param.SetOptional(1);
        
        this->AddParam(param);
                
        // Is a child BatchMake parameter needed for the parameter?
        if ((*pit).GetTag() != "boolean")
          {
          //pcount++;
          
          // find the module parameter type in the map to BatchMake types
          mp2bmIt = mp2bm.find((*pit).GetTag());
          if (mp2bmIt != mp2bm.end())
            {
            param.SetType((*mp2bmIt).second);
            }
          else
            {
            // unsupported type. map to string
            param.SetType(4);
            }

          // The name of the BatchMake paired parameter is just the
          // original parameter name
          param.SetName((*pit).GetName());         
          param.SetValue((*pit).GetDefault());

          // The parent is the previous parameter (the parameter for
          // the flag)
          param.SetParent(pcount);

          // BatchMake external flag is:
          //   0=nothing, 1=input, 2=output
          if ((*pit).GetTag() == "image" || (*pit).GetTag() == "geometry"
              || (*pit).GetTag() == "file")
            {
            if ((*pit).GetChannel() == "input")
              {
              param.SetExternalData(1);
              }
            else if ((*pit).GetChannel() == "output")
              {
              param.SetExternalData(2);
              }
            else
              {
              param.SetExternalData(0);
              }
            }
          else
            {
            param.SetExternalData(0);
            }
          
          // If the flag was specified, then the parameter for the
          // value cannot be option
          param.SetOptional(0);
          }
        this->AddParam(param);
        }
      }
    }  

  // now tack on any parameters that are based on indices
  //
  // build a list of indices to traverse in order
  std::map<int, ModuleParameter> indexmap;
  for (pgit = pgbeginit; pgit != pgendit; ++pgit)
    {
    // iterate over each parameter in this group
    std::vector<ModuleParameter>::const_iterator pbeginit
      = (*pgit).GetParameters().begin();
    std::vector<ModuleParameter>::const_iterator pendit
      = (*pgit).GetParameters().end();
    std::vector<ModuleParameter>::const_iterator pit;
  
    for (pit = pbeginit; pit != pendit; ++pit)
      {
      if ((*pit).GetIndex() != "")
        {
        indexmap[atoi((*pit).GetIndex().c_str())] = (*pit);
        }
      }
    }

  // walk the index parameters in order
  std::map<int, ModuleParameter>::const_iterator iit;
  for (iit = indexmap.begin(); iit != indexmap.end(); ++iit)
    {
    ApplicationWrapperParam param;
    
    // find the module parameter type in the map to BatchMake types
    mp2bmIt = mp2bm.find((*iit).second.GetTag());
    if (mp2bmIt != mp2bm.end())
      {
      param.SetType((*mp2bmIt).second);
      }
    else
      {
      // unsupported type. map to string
       param.SetType(4);
      }
   
    param.SetName((*iit).second.GetName());
    param.SetValue((*iit).second.GetDefault());

    // BatchMake external flag is:
    //   0=nothing, 1=input, 2=output
    if ((*iit).second.GetTag() == "image"
        || (*iit).second.GetTag() == "geometry"
        || (*iit).second.GetTag() == "file")
      {
      if ((*iit).second.GetChannel() == "input")
        {
        param.SetExternalData(1);
        }
      else if ((*iit).second.GetChannel() == "output")
        {
        param.SetExternalData(2);
        }
      else
        {
        param.SetExternalData(0);
        }
      }
    else
      {
      param.SetExternalData(0);
      }
   
    param.SetOptional(0);
    param.SetParent(0);
    this->AddParam(param);
    }

  return true;
}


/** Automatic command line parsing. If the current pointed program 
 *  supports -vxml option */
bool ApplicationWrapper::AutomaticCommandLineParsing(const char * _path)
{
  // Run the application
  std::string path = _path;
  std::string program = path;
  std::string vxmlarg = "-vxml";
  std::cout << "Running = " << program.c_str() << " " << vxmlarg.c_str() << std::endl;
  std::string m_output = "";
  unsigned int i=0;

  // Extract the arguments from the command line
  // Warning: Only works for one space between arguments
  std::vector<const char*> args;
  args.push_back(path.c_str());
  args.push_back(vxmlarg.c_str());
  args.push_back(0);

  // Run the application
  itksysProcess* gp = itksysProcess_New();
  //itksysProcess_SetPipeShared(gp, itksysProcess_Pipe_STDOUT, 1);
  //itksysProcess_SetPipeShared(gp, itksysProcess_Pipe_STDERR, 1);
  itksysProcess_SetCommand(gp, &*args.begin());
  itksysProcess_SetOption(gp,itksysProcess_Option_HideWindow,1);

  itksysProcess_Execute(gp);

  char* data = NULL;
  int length;
  double timeout = 255;

  while(itksysProcess_WaitForData(gp,&data,&length,&timeout)) // wait for 1s
    {
    for(int i=0;i<length;i++)
      {
      m_output += data[i];
      }
    }
  itksysProcess_WaitForExit(gp, 0);

  int result = 1;
  switch(itksysProcess_GetState(gp))
    {
    case itksysProcess_State_Exited:
      {
      result = itksysProcess_GetExitValue(gp);
      } break;
    case itksysProcess_State_Error:
      {
      std::cerr << "Error: Could not run " << args[0] << ":\n";
      std::cerr << itksysProcess_GetErrorString(gp) << "\n";
      } break;
    case itksysProcess_State_Exception:
      {
      std::cerr << "Error: " << args[0]
                << " terminated with an exception: "
                << itksysProcess_GetExceptionString(gp) << "\n";
      } break;
    case itksysProcess_State_Starting:
    case itksysProcess_State_Executing:
    case itksysProcess_State_Expired:
    case itksysProcess_State_Killed:
      {
      // Should not get here.
      std::cerr << "Unexpected ending state after running " << args[0]
                << std::endl;
      } break;
    }
  itksysProcess_Delete(gp);

  // Analyze the output of the program
  MetaCommand parser;
  parser.ParseXML(m_output.c_str());

  // Check that the parsing has been done correctly
  if(parser.GetOptions().size()==0)
    {
    return false;
    }

  // Convert the metaCommand to ApplicationWrapper
  this->SetVersion(parser.GetVersion().c_str());

  // extract the name from the filename
  std::string revname;
 
  for(i=0;i<path.size();i++)
    {
    if(path[path.size()-1-i] == '/'
      || path[path.size()-1-i] == '\\')
      {
      break;
      }
    revname += path[path.size()-1-i];
    }

  std::string name;
  
  int end=0;
  if(revname.find("exe.") != -1)
    {
    end=4;
    }

  for(i=0;i<revname.size()-end;i++)
    {
    name += revname[revname.size()-i-1];
    }

  this->SetName(name.c_str());

  this->SetApplicationPath(path);

  // Now adding the parameters
  const MetaCommand::OptionVector & options = parser.GetOptions();
  MetaCommand::OptionVector::const_iterator it = options.begin();

  unsigned int parentId=0;
  while(it != options.end())
    {
    bool gotParent = false;
    ApplicationWrapperParam parentParam;
    parentParam.SetName((*it).name);
    parentParam.SetOptional(!(*it).required);
    if((*it).tag != "" || (*it).longtag != "") // we have one value
      {
      parentParam.SetType(ApplicationWrapperParam::Flag);
      std::string tag = "-";
      if((*it).longtag.size()>0) // use the long tag if there is one
        {
        tag += "-";
        tag += (*it).longtag;
        }
      else
        {
        tag += (*it).tag;
        }
      parentParam.SetValue(tag);
      parentParam.SetName((*it).name);

      this->AddParam(parentParam);
      gotParent = true;
      parentId++;
      }
    else
      {
      gotParent = false;
      }
    
    std::vector<MetaCommand::Field>::const_iterator itField = (*it).fields.begin();
    while(itField != (*it).fields.end())
      {

      ApplicationWrapperParam param;
      std::string fullname = "";
      if(gotParent)      
        {
        fullname += (*it).name;
        fullname += ".";
        }
      fullname += (*itField).name;
      param.SetName(fullname);
      param.SetValue((*itField).value);

      if((*itField).externaldata == MetaCommand::DATA_IN)
        {
        param.SetExternalData(1);
        }      
      else if((*itField).externaldata == MetaCommand::DATA_OUT)
        {
        param.SetExternalData(2);
        }

      if((*itField).type == MetaCommand::FLOAT)
        {
        param.SetType(ApplicationWrapperParam::Float);
        }
      else if((*itField).type == MetaCommand::INT)
        {
        param.SetType(ApplicationWrapperParam::Int);
        }
      else if((*itField).type == MetaCommand::STRING)
        {
        param.SetType(ApplicationWrapperParam::String);
        }
      else if((*itField).type == MetaCommand::FLAG)
        {
        itField++;
        continue;
        }
      
      param.SetOptional(!(*itField).required);
      if(gotParent)
        {
        param.SetParent(parentId);
        }
      else
        {
        param.SetParent(0);
        }
      this->AddParam(param);
      itField++; 
      }
    
    it++;
    }

  return true;
}
