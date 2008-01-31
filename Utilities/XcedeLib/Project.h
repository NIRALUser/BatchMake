#ifndef __Project_h_
#define __Project_h_

#include "Subject.h"

#include <cctype>
#include <algorithm> 
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

struct my_tolower { 
  char operator()(char c) const 
    {
    return std::tolower(static_cast<unsigned char>(c));
    } 
  };

class Project
{
public:

  Project();
  ~Project(){;}

  void AddPatientToProject();
  int GetNbPatients(){return m_NbPatients;}

  void SetPatient(int i, Subject *sub){m_Patients[i] = sub;}
  Subject* GetPatient(int i){return m_Patients[i];}

  void SetProjectName(std::string name){m_ProjectName = name;}
  void SetGroupName(std::string name){m_GroupName = name;}
  std::string GetProjectName(){return m_ProjectName;}
  std::string GetGroupName(){return m_GroupName;}

  void PrintAll();
  void PrintPatient(int);
  Project CreateNewProject(std::vector<Subject*> listOfPatients);

  std::vector<Subject*> Query();
  std::vector<Subject*> Value();
  std::vector<Subject*> Mix(std::vector<Subject*> list1, std::vector<Subject*> list2);

  void SetBatchMakeOutput(bool value){m_BatchMakeOutput = value;}
  bool GetBatchMakeOutput(){return m_BatchMakeOutput;}

  void SetTagNameQuery(std::string tagname){m_TagNameQuery = tagname;}
  void SetFirstOptionQuery(std::string first){m_FirstOptionQuery = first;}
  void SetSecondOptionQuery(std::string second){m_SecondOptionQuery = second;}
  void SetTagNameValue(std::string tagname){m_TagNameValue = tagname;}
  void SetFirstOptionValue(std::string first){m_FirstOptionValue = first;}
  void SetSecondOptionValue(std::string second){m_SecondOptionValue = second;}


private:
  
  std::string m_ProjectName;
  std::string m_GroupName;
  std::vector<Subject*> m_Patients;

  int m_NbPatients;
  bool m_BatchMakeOutput;

  std::string m_TagNameQuery;
  std::string m_TagNameValue;
  std::string m_FirstOptionQuery;
  std::string m_SecondOptionQuery;
  std::string m_FirstOptionValue;
  std::string m_SecondOptionValue;
};

#endif
