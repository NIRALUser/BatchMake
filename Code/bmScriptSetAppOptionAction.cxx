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
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

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
  if (m_Parameters.size() < 2)
    {
     error->SetError(MString("SetAppOption() takes at least two arguments"),linenumber);
     return false;
    }

  m_Manager->SetTestVariable(m_Parameters[0]);

  for (unsigned int i=1;i<m_Parameters.size();i++)
    {
    m_Manager->TestConvert(m_Parameters[i],linenumber);
    }

   return true;
}

MString ScriptSetAppOptionAction::Help()
{
  return "SetAppOption(<application.option> <value>)";
}


void ScriptSetAppOptionAction::Execute()
{
  BMString m_value = m_Manager->Convert(m_Parameters[1]);

  unsigned int i; 
  for (i=2;i<m_Parameters.size();i++)
    {
    if (m_value != "")
      {
      m_value+= " ";
      }
    m_value += m_Manager->Convert(m_Parameters[i]);
    }

  // First we search the name of the variable
  const BMString firstParam = m_Parameters[0];
  long pos = firstParam.find(".",0);
  std::string first = firstParam.midCopy(0,pos).GetValue();

  long pos1 = firstParam.find(".",pos+1);

  std::string second = "";
  std::string third = "";  
  if(pos1 !=-1)
    {
    second = firstParam.midCopy(pos+1,pos1-pos-1).GetValue();
    }
  else if(pos>-1)
    {
    second = firstParam.midCopy(pos+1,m_Parameters.size()-pos-1).GetValue();
    }

  if(pos1 > -1)
    {
    third = firstParam.midCopy(pos1+1,m_Parameters.size()-pos1-1).GetValue();
    }

  // Copy the values
  std::string value = "";
  for(i=1;i<m_Parameters.size();i++)
    {
    std::string param = m_Parameters[i].GetValue();
    long int currentpos = 0;
    long int posvar = param.find("${");

    while(posvar != -1) // if the second parameters has been defined as a variable
      {
      value +=  param.substr(currentpos,posvar-currentpos);

      long int curly = param.find("}",posvar); 
      if(curly!=-1)
        {
        currentpos = curly+1;
        std::string var = param.substr(posvar,curly-posvar+1);
        value += m_Manager->GetVariable(var)[0].toChar();
        }
       posvar =param.find("$",posvar+1);
      }
    if(param.size()-currentpos>0)
      {
      value +=  param.substr(currentpos,param.size()-currentpos);
      }
    value += " ";
    }
  ApplicationWrapper * app = NULL;
  BMString appName = m_Manager->GetVariable(first.c_str())[0];
  bool appFound = false;
  ScriptActionManager::ApplicationWrapperListType::iterator it = 
    m_Manager->GetApplicationWrapperList()->begin();
  while (it != m_Manager->GetApplicationWrapperList()->end())
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
    //std::cout << "Cannot find application : "  << appName.toChar() << std::endl;
    m_ProgressManager->AddError( 
      BMString("SetAppOption: Cannot find application") + appName );
    return;
    }

  m_value = "'";
  m_value += app->GetApplicationPath();
  m_value += "' '";
  m_value += app->GetCurrentCommandLineArguments(false);
  m_value += "'";

  m_Manager->SetVariable(first,m_value);

}

} // end namespace bm
