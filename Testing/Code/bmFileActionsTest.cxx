/*=========================================================================

  Program:   BatchMake
  Module:    bmFileActionsTest.cxx
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

int FileActionsTest (int argc, char *argv[])
{
  if (argc < 2)
    {
    std::cout << "Usage: FileActionTest. Need input filename" << std::endl;
    return EXIT_FAILURE;
    }

  std::string filename(argv[1]);
  
  bm::ScriptParser m_Parser;
  //m_Parser.LoadWrappedApplication(m_ApplicationPath.toChar());
  //m_Parser.SetBatchMakeBinaryPath(m_ApplicationPath.toChar());
  if( m_Parser.Execute( filename.c_str()) == false )
    {
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
