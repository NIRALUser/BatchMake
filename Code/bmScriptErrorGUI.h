/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptErrorGUI.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __ScriptErrorGUI_h_
#define __ScriptErrorGUI_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <bmScriptError.h>
#include <iostream>
#include "BMString.h"
#include <FL/Fl_Text_Display.H>

namespace bm {

class ScriptErrorGUI : public ScriptError
{
public:
  ScriptErrorGUI();
  ~ScriptErrorGUI();

  void SetTextDisplay(Fl_Text_Display* textdisplay);
  void SetError(const BMString& error,int line=-1);
  void SetWarning(const BMString& warning,int line=-1);
  void SetStatus(const BMString& status);
  void DisplaySummary();

protected:
  Fl_Text_Display* m_TextDisplay;
};

} // end namespace bm


#endif
