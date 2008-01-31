#ifndef __Resource_h_
#define __Resource_h_

#include "itksys/SystemTools.hxx"

#include <conio.h> 
#include <winsock2.h> 
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

struct TagNameResource{
    std::string name;
    std::string value;
  };

struct ResourceInfo{
    std::string format;
    std::string subjectGroupID;
    std::string subjectID;
    std::string projectID;
  };

struct ProcessStep{
  std::string program;
  std::string programArguments;
  std::string timeStamp;
  std::string user;
  std::string hostname;
  std::string platformVersion;
  std::string platformName;
} ;

class Resource
{
public:

  Resource(){;}
  ~Resource(){;}

  int GetNbTagsResource(){return m_TagName.size();}
  void SetTagName(std::string tagname);
  void SetTagValue(std::string value, int i){m_TagName[i].value = value;}
  std::string GetTagName(int i){return m_TagName[i].name;}
  std::string GetTagNameValue(int i){return m_TagName[i].value;}

  void SetResourceID(std::string id){m_ResourceID = id;}
  void SetSchemaLocation(std::string path){m_SchemaLocation = path;}
  void SetInfo(std::string format, std::string subjectGroupID,
               std::string subjectID, std::string projectID);
  void SetProcessStep(std::string program, std::string programArguments);
  std::string GetResourceID(){return m_ResourceID;}
  ResourceInfo GetResourceInfo(){return m_Info;}
  ProcessStep GetProcessStep(){return m_ProcessStep;}

  void GenerateProcessStepInfo();


private:

  ResourceInfo m_Info;
  std::string m_ResourceID;
  std::string m_SchemaLocation;
  std::vector<TagNameResource> m_TagName; //uri, dimension, modality, etc...
  ProcessStep m_ProcessStep;
};

#endif
