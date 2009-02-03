/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptError.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptError.h"

namespace bm {


ScriptError::ScriptError()
{
  nb_error = 0;
  nb_warning = 0;
}

ScriptError::~ScriptError()
{
}

void ScriptError::Reset()
{
  nb_error = 0;
  nb_warning = 0;
}

int ScriptError::GetError()
{
  return nb_error;
}

int ScriptError::GetWarning()
{
  return nb_warning;
}


void ScriptError::SetError( const BMString& error, int linenumber )
{
  std::cout << " Error (" << linenumber 
            << "): '"     << error.toChar() << std::endl;
  nb_error++;
}

void ScriptError::SetWarning( const BMString& warning, int linenumber )
{
    std::cout << " Warning (" << linenumber 
              << "): '"       << warning.toChar() << std::endl;
    nb_warning++;
}

void ScriptError::SetStatus( const BMString& status )
{
      std::cout << status.toChar() << std::endl;
}

void ScriptError::DisplaySummary()
{
      std::cout << nb_error   << " error(s), " 
                << nb_warning <<" warning(s)" << std::endl;
}

} // end namespace bm
