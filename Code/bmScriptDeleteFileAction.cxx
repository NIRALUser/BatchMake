/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptDeleteFileAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptDeleteFileAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

#include <itksys/SystemTools.hxx>

namespace bm {

ScriptDeleteFileAction::ScriptDeleteFileAction()
: ScriptAction()
{
}

ScriptDeleteFileAction::~ScriptDeleteFileAction()
{
}

bool ScriptDeleteFileAction::TestParam(ScriptError* error,int linenumber)
{
  for (unsigned int i=0;i<m_Parameters.size();i++)
    {
    m_Manager->TestConvert(m_Parameters[i],linenumber);
    }

  if (m_Parameters.size() > 1)
  {
    error->SetError(MString("too many parameters for DeleteFile"),linenumber);
    return false;
  }
  if (m_Parameters.size() < 1)
  {
    error->SetError(MString("No enough parameter for DeleteFile"),linenumber);
    return false;
  }

  return true;
}

MString ScriptDeleteFileAction::Help()
{
  return "DeleteFile(<filename>)";
}


void ScriptDeleteFileAction::Execute()
{
  MString location =
    (m_Manager->Convert(m_Parameters[0]).rbegin("'")+1).latin1();
  bool res;
  if( !itksys::SystemTools::FileIsDirectory( location.toChar() ) )
    {
    res = itksys::SystemTools::RemoveFile( location.toChar() );
    }
  else
    {
    res = itksys::SystemTools::RemoveADirectory( location.toChar() );
    }
  if( !res )
    {
    m_ProgressManager->AddError(
      MString("DeleteFile: File ") + location +
      MString(" failed to be deleted.") );
    }
}

} // end namespace bm
