/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptErrorGUI.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptErrorGUI.h"

namespace bm {

ScriptErrorGUI::ScriptErrorGUI() 
: ScriptError()
{
  m_textdisplay = 0;
}

ScriptErrorGUI::~ScriptErrorGUI()
{
}

void ScriptErrorGUI::SetTextDisplay(Fl_Text_Display* textdisplay)
{
  m_textdisplay = textdisplay;
}

void ScriptErrorGUI::SetError(MString error,int linenumber)
{
   if (m_textdisplay)
   {
    if (linenumber != -1)
     m_textdisplay->buffer()->append((MString(" Error (%1): ").arg(linenumber) + error + "\n").toChar());
    else
     m_textdisplay->buffer()->append((MString(" Error: ").arg(linenumber) + error + "\n").toChar());
   }

    nb_error++;
}

void ScriptErrorGUI::SetWarning(MString warning,int linenumber)
{
   if (m_textdisplay)
      m_textdisplay->buffer()->append((MString(" Warning (%1): ").arg(linenumber) + warning + "\n").toChar());
  nb_warning++;
}

void ScriptErrorGUI::SetStatus(MString status)
{
   if (m_textdisplay)
      m_textdisplay->buffer()->append( (status + "\n").toChar());
}

void ScriptErrorGUI::DisplaySummary()
{
  if (m_textdisplay)
     m_textdisplay->buffer()->append((MString("%1 error(s), ").arg(nb_error) +  MString("%1 warning(s)").arg(nb_warning) + "\n").toChar());

}

} // end namespace bm
