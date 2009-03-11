/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptSetAppAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptSetAppAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

namespace bm {

ScriptSetAppAction::ScriptSetAppAction()
: ScriptAction()
{
}

ScriptSetAppAction::~ScriptSetAppAction()
{
}

bool ScriptSetAppAction::TestParam(ScriptError* error,int linenumber)
{  
  if (m_Parameters.size() < 2)
    {
    error->SetError(MString("SetApp() takes at least two arguments"),linenumber);
    return false;
    }

  m_Manager->SetTestVariable(m_Parameters[0]);

  for (unsigned int i=1;i<m_Parameters.size();i++)
    {
    m_Manager->TestConvert(m_Parameters[i],linenumber);
    }

  BMString appName = m_Parameters[1];
  appName.removeAllChars('@');

  bool appFound = false;

  if(!m_Manager->GetApplicationWrapperList())
    {
    error->SetError(
      "Calling SetApp() but no applications are defined",linenumber);
    return false;
    }

  ScriptActionManager::ApplicationWrapperListType::iterator it = 
    m_Manager->GetApplicationWrapperList()->begin();
  while (it != m_Manager->GetApplicationWrapperList()->end())
    {
    if( appName == (*it)->GetName() )
      {
      appFound = true;
      
      // Set all the variables of the application to off
      std::vector<ApplicationWrapperParam> & params = (*it)->GetParams();
      std::vector<ApplicationWrapperParam>::iterator itParams = params.begin();
        
      while(itParams != params.end())
        {
        (*itParams).SetValueDefined(false);
        itParams++;
        }

      (*it)->SetSequentialParsing(false);
      if(m_Parameters.size()==3)
        {
        if(m_Parameters[2].toInt())
          {
          (*it)->SetSequentialParsing(true);
          }
        }
      break;
      }
    it++;
    }

  if(!appFound)
    {
    std::string errorstr = "SetApp() cannot find the application: ";
    errorstr += appName.toChar();
    error->SetError(errorstr,linenumber);
    }

  return true;
}

MString ScriptSetAppAction::Help()
{
  return "SetApp(<variable> <application>)";
}


void ScriptSetAppAction::Execute()
{
  BMString appName = m_Parameters[1];
  appName.removeAllChars('@');
  bool appFound = false;
  ScriptActionManager::ApplicationWrapperListType::iterator it;
  ScriptActionManager::ApplicationWrapperListType::iterator end = 
    m_Manager->GetApplicationWrapperList()->end();
  for( it = m_Manager->GetApplicationWrapperList()->begin(); it != end ; ++it )
    {
    if( appName == (*it)->GetName() )
      {
      m_Value = "'";
      std::string val = (*it)->GetApplicationPath();
      m_Value += val.c_str();
      m_Value += "'";
      appFound = true;
     
      
      // Set all the variables of the application to off
      std::vector<ApplicationWrapperParam> & params = (*it)->GetParams();
      std::vector<ApplicationWrapperParam>::iterator itParams;
      std::vector<ApplicationWrapperParam>::iterator endParams = params.end();
      for( itParams = params.begin(); itParams != endParams; ++itParams)
        {
        (*itParams).SetValueDefined(false);
        }

      (*it)->SetSequentialParsing(false);
      if(m_Parameters.size()==3)
        {
        if(m_Parameters[2].toInt())
          {
          (*it)->SetSequentialParsing(true);
          }
        }
      break;
      }
    }
  m_Manager->SetVariable(m_Parameters[0],m_Value);
}

} // end namespace bm
