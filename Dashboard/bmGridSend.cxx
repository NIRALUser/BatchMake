/*=========================================================================

  Program:   BatchMake
  Module:    bmGridSend.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#include "metaCommand.h"
#include "HttpRequest.h"
#include "MString.h"

int main(int argc, char* argv[])
{
  MetaCommand command;
  
  command.SetName("bmGridSend");
  command.SetVersion("1.0");
  command.SetAuthor("Kitware Inc");
  command.SetDescription("Send batchmake validation results to the dashboard");

  // Required field
  command.AddField("hostname","Hostname of the dashboard",MetaCommand::STRING);
  command.AddField("user","Username to connect to the dashboard",MetaCommand::STRING);
  command.AddField("project","Name of the project",MetaCommand::STRING);

  // Create an experiment
  command.SetOption("createExperiment","ce",false,"Create an Experiment");
  command.AddOptionField("createExperiment","name",MetaCommand::STRING,true);
  command.AddOptionField("createExperiment","description",MetaCommand::STRING,false);
  
  // Create a method
  command.SetOption("createMethod","cm",false,"Create an Method for an experiment");
  command.AddOptionField("createMethod","experimentName",MetaCommand::STRING,true);
  command.AddOptionField("createMethod","name",MetaCommand::STRING,true);
  command.AddOptionField("createMethod","description",MetaCommand::STRING,false);

  // Create a method input
  command.SetOption("createMethodParameter","cp",false,"Create a parameter for a method");
  command.AddOptionField("createMethodParameter","experiment",MetaCommand::STRING,true);
  command.AddOptionField("createMethodParameter","method",MetaCommand::STRING,true);
  command.AddOptionField("createMethodParameter","name",MetaCommand::STRING,true);
  command.AddOptionField("createMethodParameter","type",MetaCommand::STRING,false);

  // Send the data
  command.SetOption("send","send",false,"Send data to the dashboard");
  command.AddOptionField("send","experiment",MetaCommand::STRING,true);
  command.AddOptionField("send","method",MetaCommand::STRING,true);

  // Send the data
  command.SetOption("data","data",false,"Send data to the dashboard",MetaCommand::LIST);
  
  // Parse an input file to find the values
  command.SetOption("inputFile","i",false,
                    "Parse input file to find the missing values",MetaCommand::STRING);

  // Parsing
  if(!command.Parse(argc,argv))
    {
    return 1;
    }

  std::string hostname = command.GetValueAsString("hostname");
  std::string user = command.GetValueAsString("user");
  std::string project = command.GetValueAsString("project");

  std::string createExperimentName = command.GetValueAsString("createExperiment","name");
  std::string createExperimentDescription = command.GetValueAsString("createExperiment","description");
    
  std::string createMethodName = command.GetValueAsString("createMethod","name");
  std::string createMethodDescription = command.GetValueAsString("createMethod","description");
  std::string createMethodExperiment = command.GetValueAsString("createMethod","experimentName");

  std::string createParameterName = command.GetValueAsString("createMethodParameter","name");
  std::string createParameterExperiment = command.GetValueAsString("createMethodParameter","experiment");
  std::string createParameterMethod = command.GetValueAsString("createMethodParameter","method");
  std::string createParameterType = command.GetValueAsString("createMethodParameter","type");

  std::string sendDataExperiment = command.GetValueAsString("send","experiment");
  std::string sendDataMethod = command.GetValueAsString("send","method");

  std::string url = hostname;

  HttpRequest m_request;
  m_request.AddParam("user",user);
  m_request.AddParam("project",project);
  m_request.AddParam("hostname",m_request.GetHostName());
  m_request.AddParam("hostip",m_request.GetHostIp());

  if(command.GetOptionWasSet("createExperiment"))
    {
    m_request.AddParam("method","CreateExperiment");
    m_request.AddParam("name",createExperimentName);
    m_request.AddParam("description",createExperimentDescription);
    }
  else if(command.GetOptionWasSet("createMethod"))
    {
    m_request.AddParam("method","CreateMethod");
    m_request.AddParam("name",createMethodName);
    m_request.AddParam("experiment",createMethodExperiment);
    m_request.AddParam("description",createMethodDescription);
    }
 else if(command.GetOptionWasSet("createMethodParameter"))
    {
    m_request.AddParam("method","CreateParameter");
    m_request.AddParam("name",createParameterName);
    m_request.AddParam("experiment",createParameterExperiment);
    m_request.AddParam("methodname",createParameterMethod);
    m_request.AddParam("type",createParameterType); // 0 = input | 1 = output
    }
 else if(command.GetOptionWasSet("send"))
    {
    m_request.AddParam("method","AddData");
    m_request.AddParam("methodname",sendDataMethod);
    m_request.AddParam("experiment",sendDataExperiment);

    if(!command.GetOptionWasSet("data"))
      {
      std::cout << "Please use the -data option to set the data fields" << std::endl;
      return 0;
      }

    std::string buffer = "";

    if(command.GetOptionWasSet("inputFile"))
      {
      std::string inputFile = command.GetValueAsString("inputFile");
      // Read the file
      std::ifstream file;
      file.open(inputFile.c_str(), std::ios::binary | std::ios::in);
      if(!file.is_open())
        {
        std::cout << "Cannot open file: " << inputFile.c_str() << std::endl;
        return 0;
        }
      file.seekg(0,std::ios::end);
      unsigned long fileSize = file.tellg();
      file.seekg(0,std::ios::beg);

      char* buf = new char[fileSize+1];
      file.read(buf,fileSize);
      buf[fileSize] = 0; 
      buffer = buf;
      buffer.resize(fileSize);
      delete [] buf;
      }

    std::list<std::string> values = command.GetValueAsList("data");
    std::list<std::string>::const_iterator it = values.begin();
    while(it!=values.end())
      {
      std::string name = *it;
      it++;
      std::string value = *it;
      it++;
     

      std::string val = value+":";
      int pos = buffer.find(val);
      if(pos != -1)
        {
        unsigned int size = buffer.find("\n",pos);
        value = buffer.substr(pos+val.size(),size-pos-val.size());
        }
      m_request.AddParam(name,value);
      std::cout << name.c_str() << " : " << value.c_str() << std::endl;
      }
    }

  // Send the data to the dashboard
  url += "/dashboard.php";
  MString m_output = m_request.Send(url.c_str());
 
  if (m_output.length()>3)
    {
    std::cout << "Bad Host or connexion problem" << std::endl;
    }
  else
    {
    if (m_output.toInt() == 0)
      {
      std::cout << "Data sent" << std::endl;
      }
    else
      {
      std::cout << "Dashboard problem when sending data" << std::endl;
      switch(m_output.toInt())
        {
        case 1 :  std::cout << "Bad user name" << std::endl; break;
        case 2 :  std::cout << "Bad project name" << std::endl; break;
        case 3 :  std::cout << "User doesn't belong to project" << std::endl; break;
        case 4 :  std::cout << "Over quota: please use DbClear function first" << std::endl; break;
        case 5 :  std::cout << "Host Database error" << std::endl; break;
        case -1 : std::cout << "Connexion problem" << std::endl; break;
        }  
      }
    }

  return 0;
}
