/*=========================================================================

  Program:   BatchMake
  Module:    bmErrorActionsTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "bmScriptParser.h"

int ErrorActionsTest (int argc, char *argv[])
{
  if (argc < 1)
    {
    std::cout << "Usage: ErrorActionTest <input_file> [how many errors to look for, optional] [application_path, optional]" << std::endl;
    return EXIT_FAILURE;
    }

  bm::ScriptParser m_Parser;

  std::string filename(argv[1]);
  std::string applicationPath;
  if( argc > 3 ) 
    {
    applicationPath = argv[3];
    m_Parser.LoadWrappedApplication( applicationPath.c_str() );
    }
    

   
  if( argc > 2 )
    {
    // in this case we need the script to return false and to have the right
    // number of errors
    int expectedNumErrors = atoi(argv[2]);
    if( m_Parser.Execute( filename.c_str()) == false )
      {
      int actualNumErrors = m_Parser.GetScriptActionManager()->GetProgressManager()->GetErrorCount();
      if(expectedNumErrors == actualNumErrors)
        {
        return EXIT_SUCCESS;
        }
      }
      return EXIT_FAILURE;
    }
 
  if( m_Parser.Execute( filename.c_str()) == false )
    {
    // here we are just looking for the script to return false
    return EXIT_SUCCESS;
    }
  // default is failue 
  return EXIT_FAILURE;
}
