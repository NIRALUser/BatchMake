#ifndef __Subject_h_
#define __Subject_h_

#include "Resource.h"

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

struct TagNameSubject{
    std::string name;
    std::string value;
  };

class Subject
{
public:

  Subject();
  ~Subject(){;}

  void AddResource();
  int GetNbTagsInfo(){return m_TagName.size();}

  void SetResource(int i, Resource *res){m_Resources[i] = res;}
  Resource* GetResource(int i){return m_Resources[i];}


  void SetNbResource(int i){m_NbResources = i;}
  void SetNbTagsInfo(int i){m_NbTagsInfo = i;}
  int GetNbResource(){return m_NbResources;}


  void SetSubjectID(std::string id){m_SubjectID = id;}
  void SetTagName(std::string tagname);
  void SetTagValue(std::string value, int i){m_TagName[i].value = value;}
  std::string GetSubjectID(){return m_SubjectID;}
  std::string GetTagName(int i){return m_TagName[i].name;}
  std::string GetTagNameValue(int i){return m_TagName[i].value;}


private:

  int m_NbResources;
  int m_NbTagsInfo;

  std::vector<Resource*> m_Resources;
  std::string m_SubjectID;
  std::vector<TagNameSubject> m_TagName;
};

#endif
