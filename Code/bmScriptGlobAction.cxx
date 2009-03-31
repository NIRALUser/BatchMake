/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptGlobAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptGlobAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"
#include <itksys/Glob.hxx>
#include <itksys/SystemTools.hxx>

#include <stdio.h>

namespace bm {

bool ScriptGlobAction::TestParam(ScriptError* error,int linenumber)
{
  if (m_Parameters.size() < 2)
    {
    error->SetError(MString("No enough parameter for Glob function !"),linenumber);
    return false;
    }

  m_Manager->SetTestVariable(m_Parameters[0]);

  for (unsigned int i=1;i<m_Parameters.size();i++)
    {
    m_Manager->TestConvert(m_Parameters[i],linenumber);
    }
  return true;
}

 
MString ScriptGlobAction::Help()
{
  return "Glob(<var> 'globexpression' '[RECURSIVE]')";
}

void ScriptGlobAction::Execute()
{
  // Read the glob expression
  BMString globexpression = m_Manager->Convert(m_Parameters[1]);
  if (globexpression.startWith('\''))
    {
    globexpression.rbegin("'") + 1;
    }

  itksys::Glob glob;

  // Are we recursive
  if (m_Parameters.size() == 3)
    {
    BMString recurse = m_Manager->Convert(m_Parameters[2]);
    if( recurse.find("RECURSIVE") != std::string::npos || 
        recurse.find("recursive") != std::string::npos )
      {
      glob.RecurseOn();
      }
    }

  if(globexpression.endWith('\\') || globexpression.endWith('/'))
    {
    globexpression += "*.*";
    }

  BMString value="";
 
  std::string convertedGlobExpression = globexpression.GetValue();
  itksys::SystemTools::ConvertToUnixSlashes(convertedGlobExpression);

  if(glob.FindFiles(convertedGlobExpression))
    {
    std::vector<std::string> filenames = glob.GetFiles();

    for(unsigned int i=0;i<filenames.size();i++)
      {
      if (value != "")
        {
        value += " ";
        }
      value += BMString("'") + BMString(filenames[i]) + BMString("'");
      }
    }

  m_Manager->SetVariable(m_Parameters[0],value);
}

} // end namespace bm
