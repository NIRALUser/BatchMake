/*=========================================================================

  Program:   BatchMake
  Module:    bmProgressGUIControls.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmProgressGUIControls.h"
#include <iostream>
#include <FL/Fl_File_Chooser.H>
#include "ApplicationWrapperParam.h"
#include <FL/Fl_Check_Button.H>

#include "MString.h"

namespace bm {

ProgressGUIControls::ProgressGUIControls():ProgressGUI()
{
  make_window();
}

ProgressGUIControls::~ProgressGUIControls()
{
}

void ProgressGUIControls::Show()
{
  if(!g_Progressgui->shown())
    g_Progressgui->show();
}

void ProgressGUIControls::OnCancel()
{
  if (MString(g_cancel->label()) == "Ok")
    g_Progressgui->hide();
  
  if (m_progressmanagergui)
    m_progressmanagergui->Stop();

}

void ProgressGUIControls::SetProgressManagerGUI(ProgressManagerGUI* progressmanagergui)
{
  m_progressmanagergui = progressmanagergui;
}

} // end namespace bm
