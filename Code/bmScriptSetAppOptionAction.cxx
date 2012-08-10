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
  unsigned int i;
/*
  BMString m_value; = m_Manager->Convert(m_Parameters[1]);
  for (i=2;i<m_Parameters.size();i++)
    {
    if (m_value != "")
      {
      m_value+= " ";
      }
    m_value += m_Manager->Convert(m_Parameters[i]);
    }
  */
  // First we search the name of the variable
  std::vector<BMString> appOptions = m_Parameters[0].tokenize(".");
  BMString application = appOptions.size() >= 1 ? appOptions[0] : "";
  BMString option1 = appOptions.size() >= 2 ? appOptions[1] : "";
  BMString option2 = appOptions.size() >= 3 ? appOptions[2] : "";

  BMString applicationName =
    m_Manager->GetVariable(application)[0].fromVariable();
  ApplicationWrapper * app =
    m_Manager->GetApplicationWrapper( applicationName );
  if( !app )
    {
    m_ProgressManager->AddError(
      BMString("SetAppOption: Cannot find application: ") +
      applicationName );
    return;
    }

  // Copy the values
  BMString value;
  for( i = 1; i < m_Parameters.size(); ++i )
    {/*
    std::size_t currentpos = 0;
    std::string param = m_Parameters[i].GetConstValue();
    std::size_t posvar = param.find("${");

    while( posvar != std::string::npos )
      {// if the second parameters has been defined as a variable
      value += param.substr( currentpos, posvar - currentpos );

      std::size_t curly = param.find("}",posvar);
      if( curly != std::string::npos )
        {
        currentpos = curly + 1;
        std::string var = param.substr( posvar, curly - posvar + 1);
        value += m_Manager->GetVariable(var)[0].toChar();
        }
       posvar = param.find( "${", posvar + 1 );
      }
    if(param.size()-currentpos>0)
      {
      value += param.substr(currentpos,param.size()-currentpos);
      }*/
    if( i != 1 )
      {
      value += " ";
      }
    value += m_Manager->Convert( m_Parameters[i] );
    }

  bool paramSet = app->SetParameterValue( option1.toChar(),
                                          option2.toChar(),
                                          value );
  if(!paramSet)
    {
    m_ProgressManager->AddError(
      BMString("SetAppOption: Cannot find parameter: ")
      + option1 + "." + option2 );
    return;
    }

  value = BMString( app->GetApplicationPath() ).toVariable();
  value += " ";
  value += BMString(
    app->GetCurrentCommandLineArguments( false ) ).toVariable();

  //std::cout << applicationName.toChar() << " " << value.toChar() << std::endl;
  m_Manager->SetVariable( application, value );

}

} // end namespace bm
