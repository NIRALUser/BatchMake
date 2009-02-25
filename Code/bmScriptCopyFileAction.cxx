/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptCopyFileAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptCopyFileAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

#include <itksys/SystemTools.hxx>

namespace bm {

ScriptCopyFileAction::ScriptCopyFileAction()
: ScriptAction()
{
}

ScriptCopyFileAction::~ScriptCopyFileAction()
{
}

bool ScriptCopyFileAction::TestParam(ScriptError* error,int linenumber)
{
  for (unsigned int i=0;i<m_Parameters.size();i++)
    {
    m_Manager->TestConvert(m_Parameters[i],linenumber);
    }
    
  if (m_Parameters.size() < 2)
    {
    error->SetError(MString("No enough parameter for CopyFile"),linenumber);
    return false;
    }   

  return true;
}

MString ScriptCopyFileAction::Help()
{
  return "CopyFile(<source filename> <destination1"
         " filename> [destination2 filename] ...)";
}


void ScriptCopyFileAction::Execute()
{
  MString source = (m_Manager->Convert(m_Parameters[0]).rbegin("'")+1).latin1();

  for( unsigned int i = 1; i < m_Parameters.size(); ++i)
    {
    MString destination = (m_Manager->Convert(m_Parameters[i]).rbegin("'")+1)
                              .latin1();
    if( itksys::SystemTools::FileIsDirectory( source.toChar() ) )
      {
      if( !itksys::SystemTools::CopyADirectory( source.toChar(), 
                                                destination.toChar() ) )
        {
        m_ProgressManager->AddError( 
          MString("CopyFile: Cannot write directory \"") + source 
                                   + "\" in directory \"" + destination + "\"" );
        }
      }
    else
      {
      if( !itksys::SystemTools::CopyFileAlways( source.toChar(), 
                                                destination.toChar() ) )
        {
        m_ProgressManager->AddError( 
          MString("CopyFile: Cannot write file \"") + source 
                                   + "\" in file \"" + destination + "\"" );
        }
      }
    if( m_ProgressManager->IsStop() )
      {
      break;
      }
    }
  return;
}

} // end namespace bm
