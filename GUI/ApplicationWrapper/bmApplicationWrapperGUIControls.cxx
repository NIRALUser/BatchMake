/*=========================================================================

  Program:   BatchMake
  Module:    bmApplicationWrapperGUIControls.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmApplicationWrapperGUIControls.h"
#include <iostream>
#include <FL/Fl_File_Chooser.H>
#include "ApplicationWrapperParam.h"
#include "metaCommand.h"

#include "MString.h"
/*
#ifndef WIN32
  #include <unistd.h>
#endif

#ifdef WIN32
  #include <Windows.h>
  #include <fcntl.h>
  #include <errno.h>
  #include <io.h>
  #include <stdio.h>
  #include <process.h>
#else
  #include <unistd.h>
  #include <sys/wait.h>
  #include <fcntl.h>
  #include <errno.h>
#endif
*/
namespace bm {

ApplicationWrapperGUIControls::ApplicationWrapperGUIControls():ApplicationWrapperGUI()
{
    make_window();
    g_type->add("filepath");
    g_type->add("flag");
    g_type->add("int");
    g_type->add("float");
    g_type->add("string");
    g_type->add("enum");
    g_type->value(0);

    g_parent->add("no parent");
    g_parent->value(0);

    g_enumlist->add("New...");
    g_enumlist->value(0);
    g_addenum->hide();
    g_delenum->hide();
    g_enumlist->hide();

    g_parameters->add("New...");
    g_parameters->value(0);
    g_add->label("Add");
    g_delete->hide();

    g_optional->value(0);
    g_external->value(0);
    g_moduleversion->value("1.0");
    m_applicationwrapper = new ApplicationWrapper();

    m_currentfilename="";
}

ApplicationWrapperGUIControls::~ApplicationWrapperGUIControls()
{
}

void ApplicationWrapperGUIControls::SetApplicationListGUIControls(ApplicationListGUIControls* applicationlistguicontrols)
{
  m_applicationlistguicontrols = applicationlistguicontrols;
}

void ApplicationWrapperGUIControls::SetApplicationPath(MString applicationpath)
{
  m_applicationpath = applicationpath;
}

void ApplicationWrapperGUIControls::SetApplicationWrapper(ApplicationWrapper* applicationwrapper)
{
  m_applicationwrapper = applicationwrapper;
  m_currentfilename = m_applicationwrapper->GetName().removeChar(' ');
}

void ApplicationWrapperGUIControls::Show()
{
  if(!g_Applicationwrappergui->shown())
    g_Applicationwrappergui->show();
}

void ApplicationWrapperGUIControls::OnSelectPath()
{
  char* fName = fl_file_chooser(
    "Select program", "Program (*.*)", NULL,Fl_File_Chooser::MULTI);
    
  if(fName)
    {
    g_path->value(fName);
    }

  // Try to parse the commande line arguments automatically
  // this requires the current program to support the option
  // -vxml
  this->AutomaticCommandLineParsing();
}

void ApplicationWrapperGUIControls::OnSelectType()
{
  switch(g_type->value())
  {
    //enum
    case 1: g_value->label("Flag:");
            g_addenum->hide();
            g_enumlist->hide();
            break;

    //enum
    case 5: g_value->label("Value:");
            g_addenum->show();
            g_enumlist->show();
            break;

    default: g_value->label("Default value:");
             g_addenum->hide();
             g_enumlist->hide();
  }

  g_Applicationwrappergui->redraw();
}

void ApplicationWrapperGUIControls::OnSelectEnumList()
{
  if (g_enumlist->value() != 0)
  {
    g_value->value(g_enumlist->text());
    g_addenum->label("Change");
  }
  else
  {
    g_value->value("");
    g_addenum->label("Add");
  }

  g_value->redraw();
}

void ApplicationWrapperGUIControls::OnAddEnum()
{
  if (g_enumlist->value() == 0)
  {
    g_enumlist->add(g_value->value());
  }
  else
  {
    g_enumlist->replace(g_enumlist->value(),g_value->value());
  }

  g_enumlist->value(0);
  g_enumlist->redraw();
  OnSelectEnumList();
}


void ApplicationWrapperGUIControls::OnSelectParameters()
{
  if (g_parameters->value() == 0)
  {
    g_add->label("Add");
    g_delete->hide();
    g_type->value(0);
    g_parent->value(0);
    g_value->value("");
    g_name->value("");
    g_optional->value(0);
    g_external->value(0);
    g_enumlist->clear();
    g_enumlist->add("New...");
    g_enumlist->redraw();
  }
  else
  {
    g_add->label("Change");
    g_delete->show();
    ApplicationWrapperParam* m_param = m_applicationwrapper->GetParam(MString(g_parameters->text()));
    if (m_param != 0)
    {
      g_name->value(m_param->GetName().latin1());
      if (m_param->GetType() != 5)
        g_value->value(m_param->GetValue().latin1());
      else
        g_value->value("");

      g_type->value(m_param->GetType());
      g_parent->value(m_param->GetParent());
      g_optional->value(m_param->GetOptional());
      g_external->value(m_param->GetExternalData());
      g_enumlist->clear();
      g_enumlist->add("New...");
      for (unsigned int i=0;i<m_param->GetEnum().size();i++)
        g_enumlist->add(m_param->GetEnum()[i].latin1());

      g_enumlist->value(m_param->GetValue().toInt());
      m_currentname = m_param->GetName();
    }
  }
  OnSelectType();
}

void ApplicationWrapperGUIControls::OnAddParameters()
{
  if (g_parameters->value() == 0)
  {
    g_parameters->add(g_name->value());
    if (g_type->value() == 1) //flag
      g_parent->add(g_name->value());
   
    //Add Param
    ApplicationWrapperParam m_param;
    m_param.SetName(MString(g_name->value()));
    m_param.SetType(g_type->value()); //ApplicationWrapperParam::Float
    m_param.SetValue(MString(g_value->value()));
    m_param.SetParent(g_parent->value());
    m_param.SetOptional(MString(g_optional->value()).toBool());
    m_param.SetExternalData(MString(g_external->value()).toBool());
    std::vector<MString> m_list;
    if (g_type->value() == 5)
    {

      for (unsigned int i=1;i<g_enumlist->size()-1;i++)
      {
        m_list.push_back(MString(g_enumlist->text(i)));
      }
      m_param.SetValue(MString((int)g_enumlist->value()));
    }
    m_param.SetEnum(m_list);
    m_applicationwrapper->AddParam(m_param);
  }
  else
  {
    ApplicationWrapperParam* m_param = m_applicationwrapper->GetParam(m_currentname); 
    g_parameters->replace(g_parameters->value(),g_name->value());
    
    unsigned int m_offset = 0;
    for (unsigned int i=1;i<g_parent->size()-1;i++)
      if (m_currentname == g_parent->text(i))
        m_offset = i;
    
    if (g_type->value() == 1) //flag
      if (m_offset == 0)
        g_parent->add(g_name->value());
      else
        g_parent->replace(m_offset,g_name->value());

    else
    {
      if (m_offset != 0)
        g_parent->remove(m_offset);
    }

    m_param->SetName(MString(g_name->value()));
    m_param->SetType(g_type->value());
    m_param->SetValue(MString(g_value->value()));
    m_param->SetParent(g_parent->value());
    m_param->SetOptional(MString(g_optional->value()).toBool());
    m_param->SetExternalData(MString(g_external->value()).toBool());

    std::vector<MString> m_list;
    if (g_type->value() == 5)
    {
      for (unsigned int i=1;i<g_enumlist->size()-1;i++)
      {
        m_list.push_back(MString(g_enumlist->text(i)));
      }
      m_param->SetValue(MString((int)g_enumlist->value()));
    }
    m_param->SetEnum(m_list);
  }

 
  m_applicationwrapper->SetApplicationPath(MString(g_path->value()));
  m_applicationwrapper->SetName(MString(g_modulename->value()));
  m_applicationwrapper->SetVersion(MString(g_moduleversion->value()));

  DisplayExample();

  g_parameters->value(0);
  g_parameters->redraw();
  OnSelectParameters();
}


void ApplicationWrapperGUIControls::OnDeleteParameters()
{
  m_applicationwrapper->DeleteParam(m_currentname);
  g_parameters->remove(g_parameters->value());
  unsigned int m_offset = 0;
  for (unsigned int i=1;i<g_parent->size()-1;i++)
    if (m_currentname == g_parent->text(i))
      m_offset = i;
  
  if (g_type->value() == 1)
    if (m_offset != 0)
      g_parent->remove(m_offset);

  m_applicationwrapper->SetApplicationPath(MString(g_path->value()));
  m_applicationwrapper->SetName(MString(g_modulename->value()));
  m_applicationwrapper->SetVersion(MString(g_moduleversion->value()));
  DisplayExample();
  g_parameters->value(0);
  g_parameters->redraw();
  OnSelectParameters();
}


void ApplicationWrapperGUIControls::OnSaveModule()
{
  m_applicationwrapper->SetApplicationPath(MString(g_path->value()));
  m_applicationwrapper->SetName(MString(g_modulename->value()));
  m_applicationwrapper->SetVersion(MString(g_moduleversion->value()));

  if (m_applicationwrapper->GetName() == "")
  {
    fl_alert("Please set a module name!");
    return;
  }

  if (m_applicationwrapper->GetName().removeChar(' ') == m_currentfilename)
  {
    m_applicationwrapper->Save(m_applicationpath + "/Applications/" + m_applicationwrapper->GetName().removeChar(' ') + ".bmm");
    g_Applicationwrappergui->hide();
     if (m_currentfilename == "")
        m_applicationlistguicontrols->UpdateNew(m_applicationwrapper);
      else
        m_applicationlistguicontrols->Update(m_applicationwrapper->GetName());
    m_applicationlistguicontrols->g_Applicationlistgui->show();
  }
  else
  {
    FILE* m_file = fopen((m_applicationpath + "/Applications/" + m_applicationwrapper->GetName().removeChar(' ') + ".bmm").toChar(),"rb");
    if (m_file == 0)
    {
      m_applicationwrapper->Save(m_applicationpath + "/Applications/" + m_applicationwrapper->GetName().removeChar(' ') + ".bmm");
      unlink((m_applicationpath + "/Applications/" + m_currentfilename + ".bmm").toChar());
      g_Applicationwrappergui->hide();
      if (m_currentfilename == "")
        m_applicationlistguicontrols->UpdateNew(m_applicationwrapper);
      else
        m_applicationlistguicontrols->Update(m_applicationwrapper->GetName());

      m_applicationlistguicontrols->g_Applicationlistgui->show();
    }
    else
    {
      fl_alert("Module name already exists!\nPlease modify module name.");
      fclose(m_file);
    }
  }
}

void ApplicationWrapperGUIControls::Refresh()
{
  g_path->value(m_applicationwrapper->GetApplicationPath().toChar());
  g_modulename->value(m_applicationwrapper->GetName().toChar());
  g_moduleversion->value(m_applicationwrapper->GetVersion().toChar());
  
  for (unsigned int i=0;i<m_applicationwrapper->GetParams().size();i++)
  {
    g_parameters->add(m_applicationwrapper->GetParams()[i].GetName().toChar());
    if (m_applicationwrapper->GetParams()[i].GetType() == 1) //flag
      g_parent->add(m_applicationwrapper->GetParams()[i].GetName().toChar());
  }

  DisplayExample();  
  OnSelectParameters();
}

void ApplicationWrapperGUIControls::OnLoadModule()
{
  char* fName = fl_file_chooser(
    "Load module", "Image Files (*.bmm)", NULL);
 
  if(fName)
  {
    m_applicationwrapper->Load(MString(fName));
    g_path->value(m_applicationwrapper->GetApplicationPath().toChar());
    g_modulename->value(m_applicationwrapper->GetName().toChar());
    g_moduleversion->value(m_applicationwrapper->GetVersion().toChar());
    
    for (unsigned int i=0;i<m_applicationwrapper->GetParams().size();i++)
    {
      g_parameters->add(m_applicationwrapper->GetParams()[i].GetName().toChar());
      if (m_applicationwrapper->GetParams()[i].GetType() == 1) //flag
        g_parent->add(m_applicationwrapper->GetParams()[i].GetName().toChar());
    }

    DisplayExample();  
    OnSelectParameters();
  }
}


void ApplicationWrapperGUIControls::DisplayExample()
{
  Fl_Text_Buffer* m_buffer = new Fl_Text_Buffer();
  g_commandline->buffer(m_buffer);
  m_buffer->text( m_applicationwrapper->GetExampleLine().toChar());
}


void ApplicationWrapperGUIControls::OnDownParam()
{
  int m_offset = g_parameters->value();
  if (m_offset != g_parameters->size()-2)
  {
    char m_tempname[200];
    strcpy(m_tempname,g_parameters->text(m_offset+1));
    g_parameters->replace(m_offset+1,g_parameters->text(m_offset));
    g_parameters->replace(m_offset,m_tempname);
    m_applicationwrapper->DownParam(m_currentname);
    g_parameters->value(m_offset+1);
  }
  DisplayExample();
}

void ApplicationWrapperGUIControls::OnUpParam()
{
  int m_offset = g_parameters->value();
  if (m_offset > 1)
  {
    char m_tempname[200];
    strcpy(m_tempname,g_parameters->text(m_offset-1));
    g_parameters->replace(m_offset-1,g_parameters->text(m_offset));
    g_parameters->replace(m_offset,m_tempname);
    m_applicationwrapper->UpParam(m_currentname);
    g_parameters->value(m_offset-1);
  }
  DisplayExample();
}

void ApplicationWrapperGUIControls::OnReject()
{
  g_Applicationwrappergui->hide();
  m_applicationlistguicontrols->g_Applicationlistgui->show();
}

/** Automatic command line parsing. If the current pointed program 
 *  supports -vxml option */
void ApplicationWrapperGUIControls::AutomaticCommandLineParsing()
{

  // Run the application
  std::string program = g_path->value();
  m_applicationwrapper->AutomaticCommandLineParsing( program.c_str() );
  
  g_moduleversion->value(m_applicationwrapper->GetVersion().toChar());
  g_modulename->value(m_applicationwrapper->GetName().toChar());
  
  this->DisplayExample();
}

} // end namespace bm
