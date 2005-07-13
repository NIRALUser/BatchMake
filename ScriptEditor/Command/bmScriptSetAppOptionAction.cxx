/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptSetAppOptionAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptSetAppOptionAction.h"

namespace bm {

ScriptSetAppOptionAction::ScriptSetAppOptionAction()
: ScriptAction()
{
}

ScriptSetAppOptionAction::~ScriptSetAppOptionAction()
{
}

bool ScriptSetAppOptionAction::TestParam(ScriptError* error,int linenumber)
{  
  if (m_parameters.size() < 2)
    {
     error->SetError(MString("SetAppOption() takes at least two arguments"),linenumber);
     return false;
    }

   m_manager->SetTestVariable(m_parameters[0]);

  for (unsigned int i=1;i<m_parameters.size();i++)
    {
    m_manager->TestConvert(m_parameters[i],linenumber);
    }

   return true;
}

MString ScriptSetAppOptionAction::Help()
{
  return "SetAppOption(<application.option> <value>)";
}


void ScriptSetAppOptionAction::Execute()
{
  MString m_value;
  m_value=m_manager->Convert(m_parameters[1]);

  unsigned int i; 
  for (i=2;i<m_parameters.size();i++)
    {
    if (m_value != "")
      {
      m_value+= " ";
      }
    m_value+=m_manager->Convert(m_parameters[i]);
    }


  // First we search the name of the variable
  long pos = m_parameters[0].find(".",0);
  std::string first = m_parameters[0].mid(0,pos).toChar();

  long pos1 = m_parameters[0].find(".",pos+1);

  std::string second = "";
  std::string third = "";  
  if(pos1 !=-1)
    {
    second = m_parameters[0].mid(pos+1,pos1-pos-1).toChar();
    }
  else if(pos>-1)
    {
    second = m_parameters[0].mid(pos+1,m_parameters.size()-pos-1).toChar();
    }

  if(pos1 > -1)
    {
    third = m_parameters[0].mid(pos1+1,m_parameters.size()-pos1-1).toChar();
    }

  // Copy the values
  std::string value = "";
  for(i=1;i<m_parameters.size();i++)
    {
    if(m_parameters[i].find("$") != -1) // if the second parametershas been defined as a variable
      {
      value += m_manager->GetVariable(m_parameters[i])[0].toChar();
      }
    else
      {
      value += m_parameters[i].toChar();
      }
    value += " ";
    }

  ApplicationWrapper * app = NULL;
  MString appName = m_manager->GetVariable(first.c_str())[0];
  bool appFound = false;
  ScriptActionManager::ApplicationWrapperListType::iterator it = m_manager->GetApplicationWrapperList()->begin();
  while (it != m_manager->GetApplicationWrapperList()->end())
    {
    if(!strcmp((*it)->GetApplicationPath().toChar(),appName.toChar()))
      {
      (*it)->SetParameterValue(second,third,value);
      app = *it;
      appFound = true;
      break;
      }
    it++;
  }

  if(!appFound) 
    {
    std::cout << "Cannot find application : "  << appName.toChar() << std::endl;
    return;
    }

  m_value = "'";
  m_value += app->GetApplicationPath();
  m_value += "' '";
  m_value += app->GetCurrentCommandLineArguments(false);
  m_value += "'";

  m_manager->SetVariable(first.c_str(),m_value);
}

} // end namespace bm
