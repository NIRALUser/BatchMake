/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptGetListSizeAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptGetListSizeAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"
#include <sstream>
#include <string>

namespace bm {

ScriptGetListSizeAction::ScriptGetListSizeAction()
: ScriptAction()
{
}

ScriptGetListSizeAction::~ScriptGetListSizeAction()
{
}

bool ScriptGetListSizeAction::TestParam(ScriptError* error,int linenumber)
{
   if (m_Parameters.size() < 2)
     {
     error->SetError(MString("No enough parameter for GetListSize"),linenumber);
     return false;
     }

  m_Manager->SetTestVariable(m_Parameters[0]);

  for (unsigned int i=1;i<m_Parameters.size();i++)
    {
    m_Manager->TestConvert(m_Parameters[i],linenumber);
    }

   return true;
}

MString ScriptGetListSizeAction::Help()
{
  return "GetListSize(<new variable> <$variable>)";
}

void ScriptGetListSizeAction::Execute()
{
  const std::vector<BMString> list = 
    //m_Manager->GetParamsFromVariable( m_Manager->Convert(m_Parameters[1]) );
    //m_Manager->ExpandParameterToArray( m_Parameters[1] );
    m_Manager->ConvertToArray( m_Parameters[1] );

  std::stringstream size;
  if( !(size << list.size()) )
    {
    m_ProgressManager->AddError("Cannot convert list size into variable value");
    return;
    }
  
  m_Manager->SetVariable( m_Parameters[0], size.str() );
}

} // end namespace bm
