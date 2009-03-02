#include "Project.h"
#include <cstring>

Project::Project()
{
  m_NbPatients = 0;
  m_BatchMakeOutput = false;
}

void Project::AddPatientToProject()
{
  Subject* sub = new Subject;
  m_Patients.push_back(sub);
  m_NbPatients++;
}


std::vector<Subject*> Project::Query()
{
  std::vector<Subject*> listOfPatients;
  std::string tagInput =  m_TagNameQuery;
  std::string valueInput1 =  m_FirstOptionQuery;
  std::string valueInput2 =  m_SecondOptionQuery;
  std::string tagSubject;
  double min = atof(valueInput1.c_str());
  double max = atof(valueInput2.c_str());
  std::transform(tagInput.begin(), tagInput.end(), tagInput.begin(), my_tolower());
  int tagnumber = -1;

  for(int i=0 ; i<m_NbPatients ; i++)
    {
    if(this->GetPatient(i)->GetNbTagsInfo() != 0)
      {
      for(int j=0 ; j<this->GetPatient(i)->GetNbTagsInfo() ; j++)
        {
        tagSubject = this->GetPatient(i)->GetTagName(j);
        std::transform(tagSubject.begin(), tagSubject.end(), tagSubject.begin(), my_tolower());
        if(strcmp(tagInput.c_str(), tagSubject.c_str()) == 0)
          {
          tagnumber = j;
          }
        }
      }

    double value = atof(this->GetPatient(i)->GetTagNameValue(tagnumber).c_str());

    if(value != 0) // if value is a number (like a birthdate, weight etc..)
      {
      if(value >= min && value <= max)
        {
        listOfPatients.push_back(this->GetPatient(i));
        }
      }
    }
  return listOfPatients;
}



std::vector<Subject*> Project::Value()
{
  std::vector<Subject*> listOfSubjects;
  std::string tagInput =  m_TagNameValue;
  std::string valueInput =  m_FirstOptionValue;
  double nbInput = atof(valueInput.c_str());
  double nbSubject;
  std::string tagSubject, valueSubject;
  std::transform(tagInput.begin(), tagInput.end(), tagInput.begin(), my_tolower());
  std::transform(valueInput.begin(), valueInput.end(), valueInput.begin(), my_tolower());

  int tagnumber = -2;

  for(int i=0 ; i<m_NbPatients ; i++)
    {
    if(this->GetPatient(i)->GetNbTagsInfo() != 0)
      {
      for(int j=0 ; j<this->GetPatient(i)->GetNbTagsInfo() ; j++)
        {
        tagSubject = this->GetPatient(i)->GetTagName(j);
        std::transform(tagSubject.begin(), tagSubject.end(), tagSubject.begin(), my_tolower());
        if(strcmp(tagInput.c_str(), tagSubject.c_str()) == 0)
          {
          tagnumber = j;
          }
        }
      }

    valueSubject = this->GetPatient(i)->GetSubjectID();
    std::transform(valueSubject.begin(), valueSubject.end(), valueSubject.begin(), my_tolower());
    if(strcmp(tagInput.c_str(),"id") == 0)
      {
      tagnumber = -1;
      if(strncmp(valueInput.c_str(), valueSubject.c_str(), strlen(valueInput.c_str())) ==0)
        {
        listOfSubjects.push_back(this->GetPatient(i));
        }
      }
    else if(this->GetPatient(i)->GetNbTagsInfo()!=0 && tagnumber != -2)
      {
      tagSubject = this->GetPatient(i)->GetTagNameValue(tagnumber);
      std::transform(tagSubject.begin(), tagSubject.end(), tagSubject.begin(), my_tolower());
      if(nbInput !=0)
        {
        nbSubject = atof(tagSubject.c_str());
        if(nbSubject == nbInput)
          {
          listOfSubjects.push_back(this->GetPatient(i));
          }
        }
      else
        {
        if(strncmp(valueInput.c_str(), tagSubject.c_str(), strlen(valueInput.c_str())) == 0)
          {
          listOfSubjects.push_back(this->GetPatient(i));
          }
        }

      }

    if(tagnumber == -2)
      {
      std::cout<<"Tagname doesn't exist, try with another one"<<std::endl;
      exit(-1);
      }
    }

  return listOfSubjects;
}


void Project::PrintPatient(int i)
{
  std::cout<<std::endl;
  std::cout<<this->GetPatient(i)->GetSubjectID()<<std::endl; // print ID

  for(int j=0 ; j<this->GetPatient(i)->GetNbTagsInfo() ; j++) // print each tag and its value
    {
    std::cout<<"    "<<this->GetPatient(i)->GetTagName(j);
    std::cout<<" : "<<this->GetPatient(i)->GetTagNameValue(j)<<std::endl;
    }
  std::cout<<std::endl;
  for(int j=0 ; j<this->GetPatient(i)->GetNbResource() ; j++)
    {
    std::cout<<"    "<<"Resource "<<this->GetPatient(i)->GetResource(j)->GetResourceID()<<" :"<<
                                    this->GetPatient(i)->GetResource(j)->GetResourceInfo().format<<" "<<
                                    this->GetPatient(i)->GetResource(j)->GetResourceInfo().subjectGroupID<<" "<<
                                    this->GetPatient(i)->GetResource(j)->GetResourceInfo().subjectID<<" "<<
                                    this->GetPatient(i)->GetResource(j)->GetResourceInfo().projectID<<std::endl;
    for(int k=0 ; k<this->GetPatient(i)->GetResource(j)->GetNbTagsResource() ; k++) // print each tag and its value
      {
      std::cout<<"        "<<this->GetPatient(i)->GetResource(j)->GetTagName(k);
      std::cout<<" : "<<this->GetPatient(i)->GetResource(j)->GetTagNameValue(k)<<std::endl;
      }
    }
}


Project Project::CreateNewProject(std::vector<Subject*> listOfPatients)
{
  Project newProj;

  newProj.SetProjectName(this->GetProjectName());
  newProj.SetGroupName(this->GetGroupName());

  std::cout<<std::endl;
  for(int i=0 ; i<(int)listOfPatients.size() ; i++)
    {
    newProj.AddPatientToProject();
    newProj.SetPatient(i, listOfPatients[i]);
    }

  return newProj;
}

void Project::PrintAll()
{
  for(int i=0 ; i<this->GetNbPatients() ; i++)
    {
    if(m_BatchMakeOutput) //if BatchMake option is actived, print only the ID
      {
      std::cout<<this->GetPatient(i)->GetSubjectID()<<std::endl;
      }
    else
      {
      this->PrintPatient(i);
      }
    }

  if(this->GetNbPatients() == 0)
    {
    std::cout<<"\nNo matches found"<<std::endl;
    }
  else
    {
    std::cout<<"\n"<<this->GetNbPatients()<<" match(es) found"<<std::endl;
    }
}


std::vector<Subject*> Project::Mix(std::vector<Subject*> list1, std::vector<Subject*> list2)
{
  std::vector<Subject*> listOfPatients;
  for(int i=0 ; i<(int)list1.size() ; i++)
    {
    for(int j=0; j<(int)list2.size() ; j++)
      {
      if(strcmp(list1[i]->GetSubjectID().c_str(),list2[j]->GetSubjectID().c_str()) == 0)
        {
        listOfPatients.push_back(list2[j]);
        }
      }
    }
  return listOfPatients;
}

