/*=========================================================================

  Program:   BatchMake
  Module:    bmProgressManagerGUI.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __ProgressManagerGUI_h_
#define __ProgressManagerGUI_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include "MString.h"
#include "bmProgressGUIControls.h"
#include "bmProgressManager.h"
#include "FLU/Flu_Tree_Browser.h"
#include "FL/Fl_Progress.H"

namespace bm {

class ProgressGUIControls;

class ProgressManagerGUI : public ProgressManager
{
public:
  ProgressManagerGUI();
  ~ProgressManagerGUI();
  void SetStatus(const BMString& status);
  void IsRunning();
  void SetProgressGUI(ProgressGUIControls* ProgressGUI);
  void AddAction(const BMString& name);
  void FinishAction(const BMString& output);
  void AddOutput(const BMString& output);
  void AddError(const BMString& output);
  void Stop();
  void SetFinished(const BMString& message);
  void DisplayOutput(const BMString& message);
  void DisplayError(const BMString& message);
  void DisplayInfo(const BMString& message);

protected:
  ProgressGUIControls* m_ProgressGUI;
  Flu_Tree_Browser::Node* m_CurrentNode;
  Flu_Tree_Browser::Node* m_OutputNode;
  Flu_Tree_Browser::Node* m_ErrorNode;
  Fl_Progress* m_Progress;
  int m_Offset;


};


} // end namespace bm

#endif
