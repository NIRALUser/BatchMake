/*=========================================================================

  Program:   BatchMake
  Module:    BatchMake.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "BatchMakeScriptEditorGUIControls.h"
#include "CommandLineArgumentParser.h"
#include "FL/fl.H"
#include "MString.h"
#include <iostream>

#define BatchMakeVersion "0.1 Beta"

void Help()
{
  std::cout << "BatchMake - " << BatchMakeVersion << std::endl;
  std::cout << "---------------------------------" << std::endl;
  std::cout << "BatchMake [-v] [-i] <filename>" << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "-v : Verbose mode" << std::endl;
  std::cout << "-c <input filename> : Compile script" << std::endl;
  std::cout << "-e <input filename> : Execute script" << std::endl;
}


int main(int argc, char **argv)
{
  if (argc < 2)
  {
	  // Create a UI object
    BatchMakeScriptEditorGUIControls* ui = new BatchMakeScriptEditorGUIControls();
    MString m_windowtitle("BatchMake - ");
    m_windowtitle += BatchMakeVersion;
    ui->g_batchmakescripteditorgui->label(m_windowtitle.toChar());
    ui->SetApplicationPath(MString(argv[0]).rbegin("\\"));

    // Initialize FLTK
    Fl::visual(FL_DOUBLE|FL_INDEX); 
    Fl::background(236,233,216);

    // Show BatchMake Editor Interface
    ui->Show();
  
	  // Run the FL driver
    Fl::run();
  
    delete ui;
  }
  else
  {
    // Parse command line parameters
    CommandLineArgumentParser parser;
    // input
    parser.AddOption("-v",0);  // Verbose mode
    parser.AddOption("-c",1);  // Compile script
    parser.AddOption("-e",1);  // Execute script
    CommandLineArgumentParseResult parseResult;
    if(!parser.TryParseCommandLine(argc,argv,parseResult))
    {
      Help();
      return -1;
    }

    if (argc < 3)
    {
      Help();
      return -1;
    }

    if (parseResult.IsOptionPresent("-v"))
    {
      Help();
      return 0;
    }

    if (parseResult.IsOptionPresent("-c"))
    {
      MString m_filename = parseResult.GetOptionParameter("-c");
      std::cout << "Compiling ..." << std::endl;
      BatchMakeScriptParser m_parser;
      m_parser.Compile(m_filename);
      return 0;
    }

    if (parseResult.IsOptionPresent("-e"))
    {
      MString m_filename = parseResult.GetOptionParameter("-e");
      BatchMakeScriptParser m_parser;
      m_parser.Execute(m_filename);
      return 0;
    }
  }

  return 0;
}
