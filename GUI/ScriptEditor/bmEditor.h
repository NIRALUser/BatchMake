/*=========================================================================

  Program:   BatchMake
  Module:    bmEditor.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __Editor_h_
#define __Editor_h_

#include "FL/Fl_Text_Editor.H"
#include "FL/fl_ask.H"
#include "FL/Fl_Browser.H"
#include "Fl/Fl_Output.h"
#include "FLU/Flu_Tree_Browser.h"
#include <FL/Fl_Window.H>
#include "MString.h"
#include <list>

namespace bm {

class Editor : public Fl_Text_Editor
{
public:
  Editor(int X, int Y, int W, int H, const char* l=0);
  ~Editor();

  static void style_parse(const char *text, char *style, int length);
  static void style_update(int pos,          // I - Position of update
             int        nInserted,    // I - Number of inserted chars
             int        nDeleted,     // I - Number of deleted chars
             int        nRestyled,    // I - Number of restyled chars
             const char *deletedText, // I - Text that was deleted
             void       *cbArg);
  static int compare_keywords(const void *a,const void *b);
  static void style_unfinished_cb(int, void*);
  static void SelectOption(Flu_Tree_Browser*, void*);

  void Load(const char* filename);
  void Save(const char* filename);
  void ShowOptionFinder(int x,int y);
  void draw();
  static void TabPressed(void*);
  int handle( int event );
  static bool Find(std::list<MString> array,MString key);
  void UpdateKeyword();
  void UpdateVariable();
  void SetParentWindow(Fl_Window* parentwindow);
  static void SetModified(bool flag);

protected:
  Fl_Text_Buffer* stylebuf;
  Fl_Text_Buffer* m_buffer;
  Flu_Tree_Browser* m_browser;
  Fl_Output* m_helper;
  bool m_drawbrowser;
  bool m_drawhelper;
  MString m_currentword;
};

} // end namespace bm

#endif
