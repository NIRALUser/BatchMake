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
#include "BMString.h"

namespace bm {

ProgressManagerGUI::ProgressManagerGUI()
:ProgressManager()
{
  m_ProgressGUI = 0;
  m_Progress = 0;
  m_CurrentNode = NULL;
  m_OutputNode = NULL;
  m_ErrorNode = NULL;
  m_Offset = 1;
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

void ProgressManagerGUI::SetStatus(const MString& status)
{
  //m_ProgressGUI->g_Progressgui->show();
}


void ProgressManagerGUI::IsRunning()
{
  if (m_Progress)
    {
    m_Progress->value(m_Progress->value()+1);
    if (m_Progress->value() == m_Progress->maximum()+1)
      { 
      m_Progress->value(0);
      }
    }

  if (m_CurrentNode)
    {
    m_CurrentNode->branch_icon( &image_running );
    }
  m_ProgressGUI->g_Progressgui->redraw();
  Fl::check();
}

void ProgressManagerGUI::AddAction(const MString& name)
{
  Fl_Group *m_group = new Fl_Group( 0, 0, 400, 20);
  m_group->resizable( NULL );
  m_group->end();

  std::string title = name.GetConstRefValue();

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

  // @todo Memory leak ?
  m_Progress = new Fl_Progress(int(sizechar*title.size()),5,150,10);
  m_Progress->box(FL_PLASTIC_UP_BOX);
  m_Progress->selection_color((Fl_Color)180);
  m_Progress->maximum(10);
  m_Progress->value(0);

  m_group->add(m_label);
  m_group->add(m_Progress);

  // by default, there is no Output or Error branch created
  m_OutputNode = NULL;
  m_ErrorNode = NULL;

  //(MString("%1 - ").arg(m_Offset++) + name).toChar();
  MString m_index;
  if (m_Offset <10)
     m_index = MString("0%1 -").arg(m_Offset++);
  else
     m_index = MString("%1 -").arg(m_Offset++);

  if( m_CurrentNode )
    {
    m_CurrentNode  =  m_CurrentNode->add_branch(m_index.toChar() , m_group );
    }
  else
    {
    m_CurrentNode  =  m_ProgressGUI->g_progress->add_branch(m_index.toChar() , m_group );
    }
  m_CurrentNode->branch_icon( &image_waiting );
  m_ProgressGUI->g_Progressgui->redraw();
  Fl::check();
  m_ProgressGUI->g_progress->set_hilighted(m_CurrentNode);
}

void ProgressManagerGUI::FinishAction(const MString& output)
{ 
  m_Progress->value(m_Progress->maximum()+1);
  if (m_CurrentNode)
    {
    m_CurrentNode->add_leaf(output.toChar());
    m_CurrentNode->branch_icon( &image_execute );
    // Go up in the tree
    m_CurrentNode = m_CurrentNode->parent();
    m_ErrorNode = m_CurrentNode->first();
    while( m_ErrorNode && m_ErrorNode->is_branch() && 
         m_ErrorNode->label() != "Error" )
      {
      m_ErrorNode = m_ErrorNode->next_sibling();
      }
    m_OutputNode = m_CurrentNode->first();
    while( m_OutputNode && m_OutputNode->is_branch() && 
         m_OutputNode->label() != "Output" )
      {
      m_OutputNode = m_OutputNode->next_sibling();
      }
    }
  else
    {
    //@todo make sure it never happens
    m_ErrorNode = NULL;
    m_OutputNode = NULL;
    std::cerr << "Debug: An action has been finished but never created ?!?!" 
              << std::endl;
    }

  m_ProgressGUI->g_Progressgui->redraw();
  Fl::check();
}

void ProgressManagerGUI::AddOutput(const MString& output)
{ 
  if (output == "")
    {
    return;
    }

  if( !m_OutputNode )
    {
    if( m_CurrentNode )
      {
      m_OutputNode = m_CurrentNode->add_branch("Output");
      }
    else
      {
      m_OutputNode = m_ProgressGUI->g_progress->add_branch("Output");
      }
    m_OutputNode->branch_icon( &image_output );
    }

  if (m_OutputNode)
    {
    m_OutputNode->add_leaf(BMString(output).replaceAllChars('/','\\').toChar());
    }
  m_ProgressGUI->g_Progressgui->redraw();
  Fl::check();
  m_ProgressGUI->g_progress->set_hilighted( 
    m_CurrentNode ? m_CurrentNode : m_OutputNode );
}

void ProgressManagerGUI::AddError(const MString& error)
{ 
  if (error == "")
    {
    this->ProgressManager::AddError(error);
    return;
    }

  if ( !m_ErrorNode )
    {
    // maybe AddAction has not been called yet, in that case,
    // create an error branch at the root.
    if( m_CurrentNode )
      {
      m_ErrorNode = m_CurrentNode->add_branch("Error");
      m_CurrentNode->branch_icon( &image_problem );
      }
    else
      {
      m_ErrorNode = m_ProgressGUI->g_progress->add_branch("Error");
      }
    m_ErrorNode->branch_icon( &image_error );
    }

  if (m_ErrorNode)
    {
    m_ErrorNode->add_leaf(BMString(error).replaceAllChars('/','\\').toChar());
    }
 
  m_ProgressGUI->g_Progressgui->redraw();

  Fl::check();
  
  this->ProgressManager::AddError(error);
}

void ProgressManagerGUI::Stop()
{
  m_Stop = true;
  m_ProgressGUI->g_Progressgui->redraw();
  m_ProgressGUI->g_cancel->label("Ok");
  Fl::check();
}

void ProgressManagerGUI::SetFinished(const MString& message)
{
  m_CurrentNode  =  m_ProgressGUI->g_progress->add_branch("Processing finished !");
  m_CurrentNode->branch_icon( &image_output );
  m_CurrentNode->add_leaf(message.toChar());
  m_OutputNode = NULL;
  m_ErrorNode = NULL;
  m_ProgressGUI->g_cancel->label("Ok");
  m_ProgressGUI->g_Progressgui->redraw();
  Fl::check();
  m_ProgressGUI->g_progress->set_hilighted(m_CurrentNode);
}

void ProgressManagerGUI::DisplayOutput(const MString& message)
{
  m_ProgressGUI->m_Buffer->append(message.toChar());
  m_ProgressGUI->g_output->scroll(m_ProgressGUI->m_Buffer->line_start(m_ProgressGUI->m_Buffer->length()),0); // m_ProgressGUI->m_Buffer->line_start(m_ProgressGUI->m_Buffer->length()),0);
  Fl::check();
}

void ProgressManagerGUI::DisplayError(const MString& message)
{
  m_ProgressGUI->m_Buffer->append(message.toChar());
  m_ProgressGUI->g_output->scroll(m_ProgressGUI->m_Buffer->line_start(m_ProgressGUI->m_Buffer->length()),0); // m_ProgressGUI->m_Buffer->line_start(m_ProgressGUI->m_Buffer->length()),0);
  Fl::check();
}

void ProgressManagerGUI::DisplayInfo(const MString& message)
{
  m_ProgressGUI->m_Buffer->append(message.toChar());
  m_ProgressGUI->g_output->scroll(m_ProgressGUI->m_Buffer->line_start(m_ProgressGUI->m_Buffer->length()),0); // m_ProgressGUI->m_Buffer->line_start(m_ProgressGUI->m_Buffer->length()),0);
  Fl::check();
}

} // end namespace bm
