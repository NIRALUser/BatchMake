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

#include <iostream>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Check_Button.H>
#include <bmSplashScreenControls.h>
#include <FL/Fl_Timer.H>


#ifndef WIN32
 #include <dirent.h>
 #include <stdio.h>
#endif

namespace bm {

void ScriptEditorGUIControls::SetApplicationPath(MString applicationpath)
{
  m_applicationpath = applicationpath;
  LoadWrappedApplication();
}


ScriptEditorGUIControls::ScriptEditorGUIControls():ScriptEditorGUI()
{
  m_filename = 0;
  m_parser = 0;
  m_errorgui = 0;
  m_errorbuffer = new Fl_Text_Buffer();
  make_window();
}

ScriptEditorGUIControls::~ScriptEditorGUIControls()
{
}


void ScriptEditorGUIControls::LoadWrappedApplication()
{ 
#ifdef WIN32
 WIN32_FIND_DATA File;
 HANDLE hSearch;
 int re;
 hSearch=FindFirstFile((m_applicationpath+ "/Applications/*.*").toChar(),&File);
 if (hSearch != INVALID_HANDLE_VALUE)
 {
  re=true;
  while(re)
  {
    re = FindNextFile(hSearch,&File);
    if (re)
    {
      if (MString(File.cFileName) != "..")
      {
        ApplicationWrapper* m_newapplication = new ApplicationWrapper();
        m_newapplication->Load(m_applicationpath+ "/Applications/" + MString(File.cFileName));
        m_applicationlist.push_back(m_newapplication);
      }
    }
  }
  FindClose(hSearch);
 }

#else
  DIR *d;
  struct dirent * dir;
  d = opendir(m_applicationpath.toChar());
  if (d)
  {
    while((dir = readdir(d)) != NULL)
    {
      std::cout << dir->d_name << std::endl;
    }
  }
#endif
  

}

void ScriptEditorGUIControls::Timer(void* ui)
{
  int newvalue = ((SplashScreenControls*)ui)->g_progress->value()+1;
  ((SplashScreenControls*)ui)->g_progress->value(newvalue);
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
  if(!g_Scripteditorgui->shown())
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
  }
}



void ScriptEditorGUIControls::OnExecute()
{
  if (!m_parser)
  {
    m_errorbuffer->text("");
    ScriptParser* m_parser = new ScriptParser();
    ScriptErrorGUI* m_errorgui = new ScriptErrorGUI();
    m_parser->SetApplicationPath(m_applicationpath);
    g_output->buffer(m_errorbuffer);
    m_errorgui->SetTextDisplay(g_output);
    
    m_parser->SetError(m_errorgui);
    int m_offset = 0;
    for (unsigned int i=0;i<g_editor->buffer()->count_lines(0,g_editor->buffer()->length())+1;i++)
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

    delete m_parser;
    delete m_errorgui;
  }
}


void ScriptEditorGUIControls::OnOpenScript()
{
  m_filename = fl_file_chooser("Load script", "BatchMake Script(*.bms)", NULL);
  if(m_filename)
  {
    g_editor->Load(m_filename);
  }
}


void ScriptEditorGUIControls::OnSaveScript()
{
  if (!m_filename)
    m_filename = fl_file_chooser("Save script", "BatchMake Script(*.bms)", NULL);
  
  if(m_filename)
  {
    g_editor->Save(m_filename);
  }
}


void ScriptEditorGUIControls::OnCompile()
{
  if (!m_parser)
  {
    m_errorbuffer->text("");
    ScriptParser* m_parser = new ScriptParser();
    m_parser->SetApplicationPath(m_applicationpath);
    ScriptErrorGUI* m_errorgui = new ScriptErrorGUI();
    g_output->buffer(m_errorbuffer);
    m_errorgui->SetTextDisplay(g_output);
    
    m_parser->SetError(m_errorgui);
    int m_offset = 0;
    for (unsigned int i=0;i<g_editor->buffer()->count_lines(0,g_editor->buffer()->length())+1;i++)
    {
       const char* text = g_editor->buffer()->line_text(m_offset);
       m_parser->AddCodeLine(MString(text));
       m_offset = g_editor->buffer()->line_end(m_offset)+1;
    }
    m_errorgui->SetStatus(MString("Compiling ..."));
    m_parser->Parse();
    m_errorgui->DisplaySummary();
    delete m_parser;
    delete m_errorgui;
  }
}


void ScriptEditorGUIControls::OnApplicationWrapper()
{
  ApplicationListGUIControls* ui = new ApplicationListGUIControls();
  ui->SetApplicationPath(m_applicationpath);
  ui->SetApplicationList(&m_applicationlist);
  ui->Show();
}

} // end namespace bm
