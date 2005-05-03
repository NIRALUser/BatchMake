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
  if (m_parameters.size() < 2)
    {
     error->SetError(MString("SetApp() takes at least two arguments"),linenumber);
     return false;
    }

  m_manager->SetTestVariable(m_parameters[0]);

  for (unsigned int i=1;i<m_parameters.size();i++)
    {
    m_manager->TestConvert(m_parameters[i],linenumber);
    }

  MString appName = m_parameters[1];
  appName = appName.removeChar('@');

  bool appFound = false;
  ScriptActionManager::ApplicationWrapperListType::const_iterator it = m_manager->GetApplicationWrapperList()->begin();
  while (it != m_manager->GetApplicationWrapperList()->end())
    {
    if(!strcmp((*it)->GetName().toChar(),appName.toChar()))
      {
      m_value = "'";
      m_value +=(*it)->GetApplicationPath();
      m_value +="'";
      appFound = true;
      
      // Set all the variable of the application to off
      std::vector<ApplicationWrapperParam> & params = (*it)->GetParams();
      std::vector<ApplicationWrapperParam>::iterator itParams = params.begin();
        
      while(itParams != params.end())
        {
        (*itParams).SetValueDefined(false);
        itParams++;
        }

      break;
      }
    it++;
  }

  if(!appFound)
    {
    error->SetError(MString("SetApp() cannot find the corresponding application"),linenumber);
    }


  return true;
}

MString ScriptSetAppAction::Help()
{
  return "SetApp(<variable> <application>)";
}


void ScriptSetAppAction::Execute()
{
  m_manager->SetVariable(m_parameters[0],m_value);
}

} // end namespace bm
