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
  m_TextDisplay = 0;
}

ScriptErrorGUI::~ScriptErrorGUI()
{
}

void ScriptErrorGUI::SetTextDisplay(Fl_Text_Display* textdisplay)
{
  m_TextDisplay = textdisplay;
}

void ScriptErrorGUI::SetError( const BMString& error,int linenumber)
{
   if (m_TextDisplay)
   {
    if (linenumber != -1)
     m_TextDisplay->buffer()->append((BMString(" Error (%1): ").arg(linenumber) + error + "\n").toChar());
    else
     m_TextDisplay->buffer()->append((BMString(" Error: ").arg(linenumber) + error + "\n").toChar());
   }

    nb_error++;
}

void ScriptErrorGUI::SetWarning(const BMString& warning,int linenumber)
{
  if (m_TextDisplay)
    {
    m_TextDisplay->buffer()->append(
      (BMString(" Warning (%1): ").arg(linenumber) + warning + "\n").toChar() );
    }
  nb_warning++;
}

void ScriptErrorGUI::SetStatus(const BMString& status)
{
  if (m_TextDisplay)
    {
    m_TextDisplay->buffer()->append( (status + "\n").toChar() );
    }
}

void ScriptErrorGUI::DisplaySummary()
{
  if (m_TextDisplay)
    {
    m_TextDisplay->buffer()->append(
      (BMString("%1 error(s), ").arg(nb_error) +  
       BMString("%1 warning(s)").arg(nb_warning) + "\n").toChar());
    }

}

} // end namespace bm
