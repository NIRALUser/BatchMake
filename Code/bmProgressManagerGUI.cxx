/*=========================================================================

  Program:   BatchMake
  Module:    bmProgressManagerGUI.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmProgressManagerGUI.h"
#include "bmIcons.h"

namespace bm {

ProgressManagerGUI::ProgressManagerGUI()
:ProgressManager()
{
  m_ProgressGUI = 0;
  m_progress = 0;
  m_currentnode = 0;
  m_outputnode = 0;
  m_Errornode = 0;
  m_offset = 1;
}

ProgressManagerGUI::~ProgressManagerGUI()
{
}

void ProgressManagerGUI::SetProgressGUI(ProgressGUIControls* ProgressGUI)
{
  m_ProgressGUI = ProgressGUI;
  m_ProgressGUI->SetProgressManagerGUI(this);
  m_ProgressGUI->g_progress->get_root()->branch_icons( &image_server, &image_server );
}

void ProgressManagerGUI::SetStatus(MString status)
{
  //m_ProgressGUI->g_Progressgui->show();
}


void ProgressManagerGUI::IsRunning()
{
  if (m_progress)
  {
    m_progress->value(m_progress->value()+1);
    if (m_progress->value() == m_progress->maximum()+1)
     m_progress->value(0);
  }

  if (m_currentnode)
    m_currentnode->branch_icon( &image_running );

  m_ProgressGUI->g_Progressgui->redraw();
  Fl::check();
}

void ProgressManagerGUI::AddAction(MString name)
{
  Fl_Group *m_group = new Fl_Group( 0, 0, 400, 20);
  m_group->resizable( NULL );
  m_group->end();


  std::string title = name.toChar();
  long int pos = title.find_last_of("/");
  long int pos2 = title.find_last_of("\\");
  if(pos != -1 || pos2 !=-1)
    {
    if(pos2>pos)
      {
      pos = pos2;
      }
    title = title.substr(pos+1,title.size()-pos-1);
    }

  char* title2 = new char[title.size()+1];
  strcpy(title2,title.c_str());
  
  Fl_Box *m_label = new Fl_Box(0, 0, 150, 20);
  m_label->label(title2);
  m_label->labelsize(12);
  m_label->align(FL_ALIGN_INSIDE | FL_ALIGN_LEFT );

  double sizechar = 100/14; // approximate

  m_progress = new Fl_Progress(int(sizechar*title.size()),5,150,10);
  m_progress->box(FL_PLASTIC_UP_BOX);
  m_progress->selection_color((Fl_Color)180);
  m_progress->maximum(10);

  m_group->add(m_label);
  m_group->add(m_progress);

  m_outputnode = 0;
  m_Errornode = 0;

  //(MString("%1 - ").arg(m_offset++) + name).toChar();
  MString m_index;
  if (m_offset <10)
     m_index = MString("0%1 -").arg(m_offset++);
  else
     m_index = MString("%1 -").arg(m_offset++);

  m_currentnode  =  m_ProgressGUI->g_progress->add_branch(m_index.toChar() , m_group );
  m_currentnode->branch_icon( &image_waiting );
  m_ProgressGUI->g_Progressgui->redraw();
  Fl::check();
  m_ProgressGUI->g_progress->set_hilighted(m_currentnode);
}

void ProgressManagerGUI::FinishAction(MString output)
{ 
  m_progress->value(m_progress->maximum()+1);
  if (m_currentnode)
  {
      m_currentnode->add_leaf(output.toChar());
      m_currentnode->branch_icon( &image_execute );
  }

  m_ProgressGUI->g_Progressgui->redraw();
  Fl::check();
}

void ProgressManagerGUI::AddOutput(MString output)
{ 
  if (output == "")
    return;

  if (!m_outputnode)
  {
      m_outputnode = m_currentnode->add_branch("Output");
      m_outputnode->branch_icon( &image_output );
  }

  if (m_outputnode)
      m_outputnode->add_leaf(output.replaceChar('/','\\').toChar());
   m_ProgressGUI->g_Progressgui->redraw();
  Fl::check();
  m_ProgressGUI->g_progress->set_hilighted(m_currentnode);
}

void ProgressManagerGUI::AddError(MString error)
{ 
  if (error == "")
    return;

  if (!m_Errornode)
  {
    m_Errornode = m_currentnode->add_branch("Error");
    m_Errornode->branch_icon( &image_error );
    m_currentnode->branch_icon( &image_problem );
  }

  if (m_Errornode)
  {
      m_Errornode->add_leaf(error.replaceChar('/','\\').toChar());
  }
 
  m_ProgressGUI->g_Progressgui->redraw();

  Fl::check();
}

void ProgressManagerGUI::Stop()
{
  m_stop = true;
   m_ProgressGUI->g_Progressgui->redraw();
  m_ProgressGUI->g_cancel->label("Ok");
  Fl::check();
}

void ProgressManagerGUI::SetFinished(MString message)
{
  m_currentnode  =  m_ProgressGUI->g_progress->add_branch("Processing finished !");
  m_currentnode->branch_icon( &image_output );
  m_currentnode->add_leaf(message.toChar());
  m_ProgressGUI->g_cancel->label("Ok");
  m_ProgressGUI->g_Progressgui->redraw();
  Fl::check();
  m_ProgressGUI->g_progress->set_hilighted(m_currentnode);
}

void ProgressManagerGUI::DisplayOutput(MString message)
{
  m_ProgressGUI->m_buffer->append(message.toChar());
  m_ProgressGUI->g_output->scroll(m_ProgressGUI->m_buffer->line_start(m_ProgressGUI->m_buffer->length()),0); // m_ProgressGUI->m_buffer->line_start(m_ProgressGUI->m_buffer->length()),0);
  Fl::check();

}

void ProgressManagerGUI::DisplayError(MString message)
{
  m_ProgressGUI->m_buffer->append(message.toChar());
  m_ProgressGUI->g_output->scroll(m_ProgressGUI->m_buffer->line_start(m_ProgressGUI->m_buffer->length()),0); // m_ProgressGUI->m_buffer->line_start(m_ProgressGUI->m_buffer->length()),0);
  Fl::check();
}

void ProgressManagerGUI::DisplayInfo(MString message)
{
  m_ProgressGUI->m_buffer->append(message.toChar());
  m_ProgressGUI->g_output->scroll(m_ProgressGUI->m_buffer->line_start(m_ProgressGUI->m_buffer->length()),0); // m_ProgressGUI->m_buffer->line_start(m_ProgressGUI->m_buffer->length()),0);
  Fl::check();
}

} // end namespace bm
