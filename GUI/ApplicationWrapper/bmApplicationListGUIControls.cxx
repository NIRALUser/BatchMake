/*=========================================================================

  Program:   BatchMake
  Module:    bmApplicationListGUIControls.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmApplicationListGUIControls.h"
#include "bmApplicationWrapperGUIControls.h"
#include <FL/fl_ask.H>

namespace bm {

ApplicationListGUIControls::ApplicationListGUIControls()
:ApplicationListGUI()
{
  make_window();
  g_list->box( FL_DOWN_BOX );
  g_list->show_leaves( true );
  g_list->show_branches( true );
  g_list->get_root()->always_open( true );
  g_list->show_root( false );
  g_list->insertion_mode(FLU_INSERT_BACK);
  g_list->selection_color(97);
}

ApplicationListGUIControls::~ApplicationListGUIControls()
{
}


void ApplicationListGUIControls::SetApplicationPath(MString applicationpath)
{
  m_applicationpath = applicationpath;
}

void ApplicationListGUIControls::SetApplicationList(std::vector<ApplicationWrapper*>* applicationlist)
{
  m_applicationlist = applicationlist;
  //Update list
  for (unsigned int i=0;i<m_applicationlist->size();i++)
  {
    g_list->add((*m_applicationlist)[i]->GetName().toChar());
    if (i ==0)
      g_list->select_all();
  }

}

void ApplicationListGUIControls::Show()
{
  if(!g_Applicationlistgui->shown())
    g_Applicationlistgui->show();
}

void ApplicationListGUIControls::OnAccept()
{
  g_Applicationlistgui->hide();
}

void ApplicationListGUIControls::OnNew()
{
  ApplicationWrapperGUIControls* ui = new ApplicationWrapperGUIControls();
  ui->SetApplicationListGUIControls(this);
  ui->SetApplicationPath(m_applicationpath);
  ui->Show();
}

void ApplicationListGUIControls::OnEdit()
{
  if (g_list->num_selected() == 0)
  {
    fl_alert("Select a module to edit !");
    return;
  }

  int m_index = g_list->get_selected(1)->index();
  if (m_index < 0) m_index = 0;
  ApplicationWrapperGUIControls* ui = new ApplicationWrapperGUIControls();

  ui->SetApplicationListGUIControls(this);
  ui->SetApplicationPath(m_applicationpath);
  ui->SetApplicationWrapper((*m_applicationlist)[m_index]);
  ui->Refresh();
  g_Applicationlistgui->hide();
  ui->Show();
}

void ApplicationListGUIControls::OnRemove()
{
}


void ApplicationListGUIControls::OnSelect()
{
}

void ApplicationListGUIControls::Update(MString name)
{
  g_list->get_selected(1)->label(name.toChar());
}

void ApplicationListGUIControls::UpdateNew(ApplicationWrapper* wrapper)
{
  g_list->add(wrapper->GetName().toChar());
  m_applicationlist->push_back(wrapper);
}

} // end namespace bm
