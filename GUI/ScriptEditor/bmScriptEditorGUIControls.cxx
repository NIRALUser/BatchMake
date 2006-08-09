/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptEditorGUIControls.cxx
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
#include "bmScriptParser.h"
#include "bmScriptErrorGUI.h"
#include "bmProgressManagerGUI.h"
#include "bmProgressGUIControls.h"
#include "bmApplicationListGUIControls.h"
#include "XMLIniIO.h"

#include <iostream>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Check_Button.H>
#include <bmSplashScreenControls.h>
#include <FL/Fl_Timer.H>

#include "bmLaunch.h"


#ifndef WIN32
 #include <dirent.h>
 #include <stdio.h>
#endif

namespace bm {

ScriptEditorGUIControls::ScriptEditorGUIControls():ScriptEditorGUI()
{
  m_filename = "";
  m_parser = new ScriptParser();

  MakeWindow();

  // Center the window on the screen
  g_Scripteditorgui->position(Fl::w()/2-g_Scripteditorgui->w()/2,
                              Fl::h()/2-g_Scripteditorgui->h()/2);

  m_errorgui = new ScriptErrorGUI();
  m_errorbuffer = new Fl_Text_Buffer();
  g_output->buffer(m_errorbuffer);

  // Give a pointer of the parser to the editor (to get the list of apps, etc...)
  g_editor->SetParser(m_parser);
  g_editor->SetScriptEditorGUI(this);

  m_CondorWatcher = new CondorWatcher;

}

ScriptEditorGUIControls::~ScriptEditorGUIControls()
{
  delete m_parser;
  delete m_CondorWatcher;
}

void ScriptEditorGUIControls::SetApplicationPath(MString applicationpath)
{
  m_applicationpath = applicationpath;
  m_parser->SetApplicationPath(m_applicationpath);
}


void ScriptEditorGUIControls::Timer(void* ui)
{
  int newvalue = (int)((SplashScreenControls*)ui)->g_progress->value()+1;
  ((SplashScreenControls*)ui)->g_progress->value((float)newvalue);
  if (((SplashScreenControls*)ui)->g_progress->value() == ((SplashScreenControls*)ui)->g_progress->maximum()+1)
  {
    ((SplashScreenControls*)ui)->g_Splashscreen->hide();
  }
  else
  {
     Fl::add_timeout(0.1,Timer,ui);
  }
}

void ScriptEditorGUIControls::Show()
{
  // disable the splash screen for the moment
  /*if(!g_Scripteditorgui->shown())
  {
    SplashScreenControls* ui = new SplashScreenControls();
    g_Scripteditorgui->show();
    int x = g_Scripteditorgui->x() + (g_Scripteditorgui->h() - ui->g_Splashscreen->h())/2;
    int y = g_Scripteditorgui->y() + (g_Scripteditorgui->w() - ui->g_Splashscreen->w())/2;

    ui->g_Splashscreen->position(x,y);
    g_Scripteditorgui->show();
    ui->Show();
    ui->g_progress->maximum(10);
    Fl::add_timeout(0.1,Timer,ui);
  }*/


  //Load .ini
  XMLIniIO* m_inifile = new XMLIniIO();
  m_inifile->SetFileName(m_applicationpath + "/BatchMake.ini");
  if (m_inifile->Read() != -1)
  {
    MString m_inivalue = m_inifile->Find("Test .ini value");
    std::cout << "Ini value: " << m_inivalue.toChar() << std::endl;
  }
  else
  {
    m_inifile->Update("Test .ini value","Seems to work!");
    m_inifile->Write();
  }

  g_Scripteditorgui->show();
  m_title = MString(g_Scripteditorgui->label()) + " [script]";
  g_Scripteditorgui->label(m_title.toChar());
  g_editor->SetParentWindow(g_Scripteditorgui);
}


void ScriptEditorGUIControls::OnOpenScript()
{
  const char* filename = 0;
  filename = fl_file_chooser("Load script", "BatchMake Script(*.bms)", NULL);
 
  if(filename)
    {
    m_filename = filename;
    g_editor->Load(m_filename.toChar());
    m_parser->SetScriptPath(m_filename);
    m_title = MString(g_Scripteditorgui->label()).begin("[") + " [" + (m_filename.rend("/")+1) + "]" ;
    g_Scripteditorgui->label(m_title.toChar());
    }
}

void ScriptEditorGUIControls::hide()
{
  if (MString(g_Scripteditorgui->label()).find("*") != -1)
  OnSaveScript();     
  g_Scripteditorgui->hide();
}

void ScriptEditorGUIControls::OnSaveScript()
{
  int resultempty = 1;
  if(g_editor->IsBufferEmpty())
    {
    resultempty = fl_choice("Your script is empty! Are you sure you want to write it?!","Cancel","Ok",0L);
    }
  if (resultempty == 0)
    {
    return;
    }


  const char* filename = 0;
  int result = 2;
  if (m_filename == "")
    {
    result = 2;
    }
  else
    {
    result = fl_choice("Script already exists !","Cancel","Overwrite","Save As ...");
    }

  if (result == 0)
    {
    return;
    }

  if (result == 2)
    {
    filename = fl_file_chooser("Save script", "BatchMake Script(*.bms)", NULL);
    }

  if(filename || (m_filename != ""))
    {
    if (filename != 0)
      {
      m_filename = filename;
      if (MString(m_filename).rend(".") != ".bms")
        {
        m_filename = MString(m_filename) +  ".bms";
        }
      }

    g_editor->Save(m_filename.toChar());
    m_parser->SetScriptPath(m_filename);

    m_title = MString(g_Scripteditorgui->label()).begin("[") + " [" + (m_filename.rend("/")+1) + "]" ;
    g_Scripteditorgui->label(m_title.toChar());
    }
}


void ScriptEditorGUIControls::OnCompile()
{
  m_errorbuffer->text("");
  m_parser->Reset();
  m_errorgui->Reset();
  m_errorgui->SetTextDisplay(g_output);
  m_parser->SetError(m_errorgui);
  int m_offset = 0;
  for (int i=0;i<g_editor->buffer()->count_lines(0,g_editor->buffer()->length())+1;i++)
    {
    const char* text = g_editor->buffer()->line_text(m_offset);
    m_parser->AddCodeLine(MString(text));
    m_offset = g_editor->buffer()->line_end(m_offset)+1;
    }
  m_errorgui->SetStatus(MString("Compiling ..."));
  m_parser->Parse();
  m_errorgui->DisplaySummary();
}

void ScriptEditorGUIControls::OnExecute()
{
  m_parser->Reset();
  m_errorgui->Reset();
  m_errorbuffer->text("");
  m_errorgui->SetTextDisplay(g_output);
  m_parser->SetError(m_errorgui);
  int m_offset = 0;
  for (int i=0;i<g_editor->buffer()->count_lines(0,g_editor->buffer()->length())+1;i++)
    {
    const char* text = g_editor->buffer()->line_text(m_offset);
    m_parser->AddCodeLine(MString(text));
    m_offset = g_editor->buffer()->line_end(m_offset)+1;
    }

  m_errorgui->SetStatus(MString("Compiling ..."));
    
  ProgressManagerGUI* m_progressmanagergui = new ProgressManagerGUI();
  ProgressGUIControls* m_progressgui = new ProgressGUIControls();
  m_progressgui->g_progress->box( FL_DOWN_BOX );
  // m_progressgui->g_progress->auto_branches( true );  
  m_progressgui->g_progress->show_leaves( true );
  m_progressgui->g_progress->show_branches( true );
  m_progressgui->g_progress->get_root()->always_open( true );
  m_progressgui->g_progress->insertion_mode(FLU_INSERT_BACK);

  m_progressgui->g_progress->label( "BatchMake processing ..." );
  m_progressmanagergui->SetProgressGUI(m_progressgui);
  m_parser->SetProgressManager(m_progressmanagergui);

  if (m_parser->Parse())
    {
    m_errorgui->DisplaySummary();
    m_errorgui->SetStatus(MString("Running ..."));
    m_progressgui->Show();
    m_parser->Execute();
    }
  else
    {
    m_errorgui->DisplaySummary();
    }
}

/** Launch the application wrapper */
void ScriptEditorGUIControls::OnApplicationWrapper()
{
  ApplicationListGUIControls* ui = new ApplicationListGUIControls();
  ui->SetApplicationPath(m_applicationpath);
  ui->SetApplicationList(m_parser->GetApplicationList());
  ui->Show();
}

/** Generate a KWGrid script */
void ScriptEditorGUIControls::OnGenerateGAD()
{
  const char* filename = 0;
  filename = fl_file_chooser("Save KWGrid script", "Grid Script(*.gad)", NULL);

  if(!filename)
    {
    return;
    }

  // Check first if the script is valid
  m_parser->Reset();
  m_errorgui->Reset();
  m_errorbuffer->text("");
  m_errorgui->SetTextDisplay(g_output);
  m_parser->SetError(m_errorgui);
  int m_offset = 0;
  for (int i=0;i<g_editor->buffer()->count_lines(0,g_editor->buffer()->length())+1;i++)
    {
    const char* text = g_editor->buffer()->line_text(m_offset);
    m_parser->AddCodeLine(MString(text));
    m_offset = g_editor->buffer()->line_end(m_offset)+1;
    }

  m_errorgui->SetStatus(MString("Generating kwgrid script ..."));
    
  Grid grid;
  grid.SetFileName(filename);
  m_parser->SetGridModule(&grid);
  if (m_parser->Parse())
    {
    m_errorgui->DisplaySummary();
    m_parser->Execute();
    }
  else
    {
    m_errorgui->DisplaySummary();
    }

  grid.WriteGAD();

  m_parser->SetGridModule(NULL);
}

/** Generate a Shell script */
void ScriptEditorGUIControls::OnGenerateShell()
{
  const char* filename = 0;
  filename = fl_file_chooser("Save Shell script", "Shell script (*.sh)", NULL);

  if(!filename)
    {
    return;
    }

  // Check first if the script is valid
  m_parser->Reset();
  m_errorgui->Reset();
  m_errorbuffer->text("");
  m_errorgui->SetTextDisplay(g_output);
  m_parser->SetError(m_errorgui);
  int m_offset = 0;
  for (int i=0;i<g_editor->buffer()->count_lines(0,g_editor->buffer()->length())+1;i++)
    {
    const char* text = g_editor->buffer()->line_text(m_offset);
    m_parser->AddCodeLine(MString(text));
    m_offset = g_editor->buffer()->line_end(m_offset)+1;
    }

  m_errorgui->SetStatus(MString("Generating shell script ..."));
    
  Grid grid;
  grid.SetFileName(filename);
  m_parser->SetGridModule(&grid);
  if (m_parser->Parse())
    {
    m_errorgui->DisplaySummary();
    m_parser->Execute();
    }
  else
    {
    m_errorgui->DisplaySummary();
    }

  grid.WriteShell();

  m_parser->SetGridModule(NULL);
}

/** Generate a Condor script */
void ScriptEditorGUIControls::OnGenerateCondor()
{
  const char* filename = 0;
  filename = fl_file_chooser("Save Condor script", "Condor Script(*.bmc)", NULL);

  if(!filename)
    {
    return;
    }

  // Check first if the script is valid
  m_parser->Reset();
  m_errorgui->Reset();
  m_errorbuffer->text("");
  m_errorgui->SetTextDisplay(g_output);
  m_parser->SetError(m_errorgui);
  int m_offset = 0;
  for (int i=0;i<g_editor->buffer()->count_lines(0,g_editor->buffer()->length())+1;i++)
    {
    const char* text = g_editor->buffer()->line_text(m_offset);
    m_parser->AddCodeLine(MString(text));
    m_offset = g_editor->buffer()->line_end(m_offset)+1;
    }

  m_errorgui->SetStatus(MString("Generating condor script ..."));
    
  Grid grid;
  grid.SetFileName(filename);
  m_parser->SetGridModule(&grid);
  if (m_parser->Parse())
    {
    m_errorgui->DisplaySummary();
    m_parser->Execute();
    }
  else
    {
    m_errorgui->DisplaySummary();
    }

  grid.WriteCondor();
  
  if(fl_ask("Run condor watcher?"))
    {
    m_CondorWatcher->Window->show();
    m_CondorWatcher->Watch();
    }

  if(fl_ask("Run current script?"))
    {
    Launch starter;
    std::string run = "condor_submit ";
    run += filename;
    std::cout << "Running Condor script " << run.c_str() << std::endl;
    starter.Execute(run.c_str());
    MString output = starter.GetOutput();
    output = output.replaceChar(13,' ');
    output = output.replaceChar(10,' ');
    m_errorgui->SetStatus(output);
    MString error = starter.GetError();
    error = error.replaceChar(13,' ');
    error = error.replaceChar(10,' ');
    m_errorgui->SetStatus(error);
    }

  m_parser->SetGridModule(NULL);
}

/** Generate a batch scripts */
void ScriptEditorGUIControls::OnGenerateScripts()
{
  // Check first if the script is valid
  this->OnCompile();






}

} // end namespace bm
