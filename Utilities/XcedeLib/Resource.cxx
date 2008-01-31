#include "Resource.h"


void Resource::SetTagName(std::string tagname)
{
  TagNameResource newTag;
  newTag.name = tagname;
  m_TagName.push_back(newTag);
}


void Resource::SetInfo(std::string format, std::string subjectGroupID,
                       std::string subjectID, std::string projectID)
{
  m_Info.format = format;
  m_Info.subjectGroupID = subjectGroupID;
  m_Info.subjectID = subjectID;
  m_Info.projectID = projectID;
}


void Resource::SetProcessStep(std::string program, std::string programArguments)
{
  m_ProcessStep.program = program.c_str();
  m_ProcessStep.programArguments = programArguments.c_str();
}


void Resource::GenerateProcessStepInfo()
{
  int nRet; 
  WSADATA wsaData; 
  WORD version = MAKEWORD(1,1); 

  m_ProcessStep.timeStamp = itksys::SystemTools::GetCurrentDateTime("%Y-%m-%dT%X").c_str();

  if(itksys::SystemTools::GetEnv("USER") != NULL)
    {
    m_ProcessStep.user = itksys::SystemTools::GetEnv("USER");
    }
  else 
    {
    m_ProcessStep.user = "(unknown)";
    }

  if (itksys::SystemTools::GetEnv("HOSTNAME") == NULL)
    {
    if (itksys::SystemTools::GetEnv("HOST") == NULL)
      {
      nRet = WSAStartup(version, &wsaData); 
      char hostname[256]; 
      nRet = gethostname(hostname, 256);
      m_ProcessStep.hostname =hostname;
      }
    else
      {
      m_ProcessStep.hostname = itksys::SystemTools::GetEnv("HOST");
      }
    }
  else
    {
    m_ProcessStep.hostname = itksys::SystemTools::GetEnv("HOSTNAME");
    }

  #if defined(sun) || defined(__sun)
  #if defined(__SunOS_5_7)
  m_ProcessStep.platformVersion = "2.7";
  #endif
  #if defined(__SunOS_5_8)
  m_ProcessStep.platformVersion = "8";
  #endif
  #endif
  #if defined(linux) || defined(__linux)
  m_ProcessStep.platformVersion = "unknown";
  #endif

  // now the platform name
  #if defined(linux) || defined(__linux)
  m_ProcessStep.platformName = "Linux";
  #endif
  #if defined(macintosh) || defined(Macintosh)
  m_ProcessStep.platformName = "MAC OS 9";
  #endif
  #ifdef __MACOSX__
  m_ProcessStep.platformName = "MAC OS X";
  #endif
  #if defined(sun) || defined(__sun)
  # if defined(__SVR4) || defined(__svr4__)
  m_ProcessStep.platformName = "Solaris";
  # else
  m_ProcessStep.platformName = "SunOS";
  # endif
  #endif
  #if defined(_WIN32) || defined(__WIN32__)
  m_ProcessStep.platformName = "Windows";
  #endif

  if (itksys::SystemTools::GetEnv("MACHTYPE") != NULL)
    {
    m_ProcessStep.platformName = itksys::SystemTools::GetEnv("MACHTYPE");
    }
}
