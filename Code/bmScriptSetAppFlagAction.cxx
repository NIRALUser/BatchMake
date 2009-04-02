/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptSetAppFlagAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptSetAppFlagAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

namespace bm {

ScriptSetAppFlagAction::ScriptSetAppFlagAction()
: ScriptAction()
{
}

ScriptSetAppFlagAction::~ScriptSetAppFlagAction()
{
}

bool ScriptSetAppFlagAction::TestParam(ScriptError* error,int linenumber)
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

MString ScriptSetAppFlagAction::Help()
{
  return "SetAppFlag(<application.flag[.flag]> <value>)";
}


void ScriptSetAppFlagAction::Execute()
{
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
  unsigned int i; 
  for( i = 1; i < m_Parameters.size(); ++i )
    {
    if( i != 1 )
      {
      value += " ";
      }
    value += m_Manager->Convert( m_Parameters[i] );
    }

  bool paramSet = false;
  const ApplicationWrapperParam* option = 
    app->GetParamByFlag( option1.toChar() );
  if( option != NULL )
    {
    option2 = option2 == option1 ? option->GetName() : option2;
    option1 = option->GetName();
    paramSet = app->SetParameterValue( option1.toChar(), 
                                       option2.toChar(), 
                                       value );
    }

  if(!paramSet)
    {
    // we don't want to generate an error as it is usually not important
    // parameters that can't be found. 
    // i.e. processinformationaddress, xml, echo, ignore-rest, version, help
#ifdef VERBOSE
    std::cout << "SetAppFlag: Cannot find parameter: " << option1 
              << "." << option2 << std::endl;
#endif
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
