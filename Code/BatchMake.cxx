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

#include "bmScriptEditorGUIControls.h"
#include "CommandLineArgumentParser.h"
#include "FL/Fl.H"
#include "MString.h"
#include "ApplicationWrapper.h"
#include <iostream>

#define BatchMakeVersion "0.1 Beta"

void Help()
{
  std::cout << "BatchMake - " << BatchMakeVersion << std::endl;
  std::cout << "---------------------------------" << std::endl;
  std::cout << "BatchMake [-h] [-i] <filename>" << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "-h : Display help" << std::endl;
  std::cout << "-c <input filename> : Compile script" << std::endl;
  std::cout << "-e <input filename> : Execute script" << std::endl;
  std::cout << "-a <application path> <application name> : Add the application into the application wrapper" << std::endl;
}


int main(int argc, char **argv)
{
   #ifdef WIN32
     MString m_applicationpath = MString(argv[0]).rbegin("\\");
   #else
     MString m_applicationpath = MString(argv[0]).rbegin("/");
   #endif

  if (argc < 2)
  {
    // Create a UI object
    bm::ScriptEditorGUIControls* ui = new bm::ScriptEditorGUIControls();
    MString m_windowtitle("BatchMake - ");
    m_windowtitle += BatchMakeVersion;
    ui->g_Scripteditorgui->label(m_windowtitle.toChar());
    ui->SetApplicationPath(m_applicationpath);
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
    parser.AddOption("-h",0);  // Verbose mode
    parser.AddOption("-c",1);  // Compile script
    parser.AddOption("-e",1);  // Execute script
    parser.AddOption("-a",2);  // Add application
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

    if (parseResult.IsOptionPresent("-h"))
    {
      Help();
      return 0;
    }

    if (parseResult.IsOptionPresent("-c"))
    {
      MString m_filename = parseResult.GetOptionParameter("-c");
      std::cout << "Compiling ..." << std::endl;
      bm::ScriptParser m_parser;
      m_parser.SetApplicationPath(m_applicationpath);
      m_parser.Compile(m_filename);
      return 0;
    }

    if (parseResult.IsOptionPresent("-e"))
    {
      MString m_filename = parseResult.GetOptionParameter("-e");
      bm::ScriptParser m_parser;
      m_parser.SetApplicationPath(m_applicationpath);
      m_parser.Execute(m_filename);
      return 0;
    }
    if (parseResult.IsOptionPresent("-a"))
    {
      m_applicationpath = MString(argv[0]).rbegin("/");
      
      MString m_path = parseResult.GetOptionParameter("-a",0);
      MString m_modulename = parseResult.GetOptionParameter("-a",1);
      ApplicationWrapper m_ApplicationWrapper;

      m_ApplicationWrapper.AutomaticCommandLineParsing(m_path.toChar());
      
      m_ApplicationWrapper.SetApplicationPath(m_path);
      m_ApplicationWrapper.SetName(m_modulename);
      
      FILE* m_file = fopen((m_applicationpath + "/Applications/" + m_modulename + ".bmm").toChar(),"rb");
      if (m_file == 0)
      {
        m_ApplicationWrapper.Save(m_applicationpath + "/Applications/" + m_modulename + ".bmm");
      }
      else
      {
        fclose(m_file);
      }
    }
  }

  return 0;
}
