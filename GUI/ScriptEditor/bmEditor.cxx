/*=========================================================================

  Program:   BatchMake
  Module:    bmEditor.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmEditor.h"
#include "MString.h"
#include "bmScriptActionManager.h"

namespace bm {

  Fl_Text_Display::Style_Table_Entry styletable[] = {  // Style table
         { FL_BLACK,      FL_COURIER,        12 }, // A - Plain
         { FL_DARK_GREEN, FL_COURIER_ITALIC, 12 }, // B - Line comments
         { FL_RED,        FL_COURIER,        12 }, // C - Variables
         { FL_BLUE,       FL_COURIER,        12 }, // D - Strings
         { FL_DARK_RED,   FL_COURIER,        12 }, // E - Directives
         { FL_DARK_RED,   FL_COURIER_BOLD,   12 }, // F - Types
         { FL_BLUE,       FL_COURIER_BOLD,   12 }  // G - Keywords
};


/** The list of keywords recognized. SHOULD BE IN ALPHABETICAL ORDER */
const char         *code_keywords[] = {  // List of known C/C++ keywords...     
         "echo",  
         "getparam",
         "listdirindir",
         "listfileindir",
         "randomize",
         "run",
         "sequence",  
         "set",
         "validationdashboard"
       };

  const char         *code_types[] = {  // List of known C/C++ types...
         "else",
         "endforeach",
         "endif",  
         "foreach",
         "if",
         "null",
       };



Editor::Editor(int X, int Y, int W, int H, const char* l)
:Fl_Text_Editor(X, Y, W, H, l)
{
  m_drawbrowser = false;
  m_browser = new Flu_Tree_Browser(0, 0,200,80);
  m_browser->box( FL_DOWN_BOX );
  m_browser->show_root(false);
  m_browser->branch_text(m_browser->branch_color(),m_browser->branch_font(),10);
  m_browser->leaf_text(m_browser->leaf_color(),m_browser->leaf_font(),10);
  m_browser->vertical_gap(-2);
  m_browser->add("echo");
  m_browser->add("getparam");
  m_browser->add("randomize");
  m_browser->add("run");
  m_browser->add("sequence");
  m_browser->add("set");
  m_browser->add("validationdashboard");
  m_browser->selection_color(87);
  m_browser->show_leaves( true );
  m_browser->show_branches( true );
  m_browser->get_root()->always_open( true );
  m_browser->insertion_mode(FLU_INSERT_BACK);
  m_browser->selection_follows_hilight(true);
  m_browser->selection_mode(FLU_SINGLE_SELECT);
  m_browser->callback((Fl_Callback*)SelectOption,this);
  Flu_Tree_Browser::Node* m_node = m_browser->first();
  m_node = m_node->next();
  m_node = m_node->next();
  m_node->select_only();
  m_browser->set_hilighted(m_node);
  m_browser->resize(0,0,200,80);
  m_browser->end();

  m_drawhelper = false;
  m_helper = new Fl_Output(0, 0,200,15);
  m_helper->resize(0,0,200,15);
  m_helper->box(FL_BORDER_BOX);
  m_helper->color((Fl_Color)215);
  m_helper->labelsize(10);
  m_helper->textsize(10);

  m_buffer = new Fl_Text_Buffer();
  char *style = new char[3];
  char *text = m_buffer->text();
  style[0] = '\0';
  stylebuf = new Fl_Text_Buffer(m_buffer->length());
  stylebuf->text(style);
  delete[] style;
  free(text);


  buffer(m_buffer);
 // m_buffer->tab_distance(3);

  highlight_data(stylebuf, styletable,
                          sizeof(styletable) / sizeof(styletable[0]),
                          'A', style_unfinished_cb, 0);

  m_buffer->add_modify_callback(style_update, this);
  m_buffer->call_modify_callbacks();

  m_currentword = "";
}

Editor::~Editor()
{
}


int Editor::compare_keywords(const void *a,const void *b) 
{
  return (strcmp(*((const char **)a), *((const char **)b)));
}

void Editor::style_unfinished_cb(int, void*) {
}

void Editor::style_update(int        pos,          // I - Position of update
             int        nInserted,    // I - Number of inserted chars
             int        nDeleted,     // I - Number of deleted chars
             int        nRestyled,    // I - Number of restyled chars
             const char *deletedText, // I - Text that was deleted
             void       *cbArg) {     // I - Callback data
  int  start,                         // Start of text
       end;                           // End of text
  char last,                          // Last style on line
       *style,                        // Style data
       *text;                         // Text data


  Fl_Text_Buffer* stylebuf = ((Editor *)cbArg)->stylebuf;
  Fl_Text_Buffer* textbuf = ((Editor *)cbArg)->m_buffer;

 /* if (nInserted == 1)
    if (MString(textbuf->text_range(pos, pos+1)) == "\t")
  {
    TabPressed(cbArg);
    //textbuf->replace(pos,pos+1,"  ");
    return;
  }*/

 // If this is just a selection change, just unselect the style buffer...
  if (nInserted == 0 && nDeleted == 0) {
    stylebuf->unselect();
    return;
  }

  // Track changes in the text buffer...
  if (nInserted > 0) {
    // Insert characters into the style buffer...
    style = new char[nInserted + 1];
    memset(style, 'A', nInserted);
    style[nInserted] = '\0';

    stylebuf->replace(pos, pos + nDeleted, style);
    delete[] style;
  } else {
    // Just delete characters in the style buffer...
    stylebuf->remove(pos, pos + nDeleted);
  }

  // Select the area that was just updated to avoid unnecessary
  // callbacks...
  stylebuf->select(pos, pos + nInserted - nDeleted);

  // Re-parse the changed region; we do this by parsing from the
  // beginning of the line of the changed region to the end of
  // the line of the changed region...  Then we check the last
  // style character and keep updating if we have a multi-line
  // comment character...
  start = textbuf->line_start(pos);
  end   = textbuf->line_end(pos + nInserted);
  text  = textbuf->text_range(start, end);
  style = stylebuf->text_range(start, end);

  last  = style[end - start - 1];

  if ((pos == start))
  {
    style[0] = stylebuf->text_range(start-1,start)[0];
    style[1] = '\0';
    //std::cout << "Style start:" << style << std::endl;
   //int value = style[0];
    //style = new char[2];
    //memset(style,value, 1);
    //
  }


 /* printf("start = %d, end = %d, text = \"%s\", style = \"%s\"...\n",
         start, end, text, style);*/

  if ((end - start) != 0)
  {
   style_parse(text, style, end - start);

  //printf("new style = \"%s\"...\n", style);

  stylebuf->replace(start, end, style);
  ((Editor *)cbArg)->redisplay_range(start, end);
  }

  if (last != style[end - start - 1]) {
    // The last character on the line changed styles, so reparse the
    // remainder of the buffer...
    free(text);
    free(style);

    end   = textbuf->length();
    text  = textbuf->text_range(start, end);
    style = stylebuf->text_range(start, end);

    style_parse(text, style, end - start);

    stylebuf->replace(start, end, style);
    ((Editor *)cbArg)->redisplay_range(start, end);
  }

  free(text);
  free(style);

  int m_x;
  int m_y;
 ((Editor *)cbArg)->position_to_xy(pos,&m_x,&m_y);
 ((Editor *)cbArg)->ShowOptionFinder(m_x,m_y);
}

void Editor:: style_parse(const char *text,char *style,int length) 
{
  char             current;
  int             col;
  int             last;
  char             buf[255],
             *bufptr;
  const char *temp;

//if (length == 0)
{
  if ((style[0] != 'C') && (style[0] != 'D')) style[0] = 'A';
}

for (current = *style, col = 0, last = 0; length > 0; length --, text ++) {

    if (current == 'B') current = 'A';
    if (current == 'A') 
    {
      // Check for directives, comments, strings, and keywords...
      /*if (col == 0 && *text == '#') 
      {
        // Set style to directive
        current = 'E';
      } 
      else */
      if (strncmp(text, "#", 1) == 0) 
      {
        current = 'B';
          for (; length > 0 && *text != '\n'; length --, text ++) *style++ = 'B';
        if (length == 0) break;
      } 
      else if (strncmp(text, "${", 2) == 0) 
      {
        current = 'C';
      } 
      else if (strncmp(text, "\\'", 2) == 0) 
      {
              // Quoted quote...
        *style++ = current;
        *style++ = current;
        text ++;
        length --;
        col += 2;
        continue;
      } 
      else if (*text == '\'') 
      {
        current = 'D';
      } 
      else 
      { //if (!last && islower(*text)) {
         // Might be a keyword...
       /*for (temp = text, bufptr = buf;
              islower(*temp) && bufptr < (buf + sizeof(buf) - 1);
              *bufptr++ = *temp++);*/

        temp = text;  
        bufptr = buf;

        int offset =0;
        while ((text[offset] != ' ') && (offset<strlen(text)) && (text[offset] != '\n') && (text[offset] != '('))
        {
           *bufptr++ = tolower(*temp++);
           offset++;
        }

       // std::cout << offset << std::endl;
       // bufptr[offset] = '\0';
        //if (!islower(*temp)) {
         {
           *bufptr = '\0';
           bufptr = buf;

           //std::cout << "Buffer: " << bufptr << std::endl;

            if (bsearch(&bufptr, code_types,
                        sizeof(code_types) / sizeof(code_types[0]),
                  sizeof(code_types[0]), compare_keywords)) 
            {
                while (text < temp) 
                {
                  *style++ = 'F';
                  text ++;
                  length --;
                  col ++;
                }

              text --;
              length ++;
              last = 1;
              continue;
            } 
            else if (bsearch(&bufptr, code_keywords,
                     sizeof(code_keywords) / sizeof(code_keywords[0]),
               sizeof(code_keywords[0]), compare_keywords)) 
            {
              while (text < temp) 
              {
                *style++ = 'G';
                text ++;
                length --;
                col ++;
              }

              text --;
              length ++;
              last = 1;
              continue;
            }
        }
     }
    } else if (current == 'C' && strncmp(text, "}", 1) == 0) 
      {
        // Close a C comment...
        *style++ = current;
        current = 'A';
        col += 1;
        continue;
      } else if (current == 'D') 
      {
        // Continuing in string...
        /*if (strncmp(text, "\\\"", 2) == 0) 
        {
          // Quoted end quote...
          *style++ = current;
          *style++ = current;
          text ++;
          length --;
          col += 2;
          continue;
        } 
        else */if (*text == '\'') 
        {
                // End quote...
          *style++ = current;
          col ++;
          current = 'A';
          continue;
        }
      }

    // Copy style info...
    /*if (current == 'A' && (*text == '{' || *text == '}')) *style++ = 'G';
    else *style++ = current;*/
    *style++ = current;
    col ++;

    last = isalnum(*text) || *text == '.';

    /*if (*text == '\n') {
      // Reset column and possibly reset the style
      col = 0;
      if (current == 'B' || current == 'E') current = 'A';
    }*/
  }
}

void Editor::Load(const char* filename)
{
  int r = m_buffer->loadfile(filename);
  if (r)
     fl_alert("Error reading from file \'%s\':\n%s.", filename, strerror(errno));

  int m_foundPos = 0;
  while(m_buffer->findchars_forward(m_foundPos,"\r",&m_foundPos))
    m_buffer->remove(m_foundPos,m_foundPos+1);
  
  m_buffer->call_modify_callbacks();
}

void Editor::Save(const char* filename)
{
 if (m_buffer->savefile(filename))
      fl_alert("Error writing to file \'%s\':\n%s.", filename, strerror(errno));

    m_buffer->call_modify_callbacks();
}

void Editor::ShowOptionFinder(int x,int y)
{
  m_browser->resize(x+13,y+15,200,80);
  m_helper->resize(x+13,y+15,200,15);
}


void Editor::draw()
{
  //((Fl_Text_Display*)this)->draw();
  Fl_Text_Display::draw();

  if (m_drawbrowser)
    m_browser->show();
  else
    m_browser->hide();

  if (m_drawhelper)
    m_helper->show();
  else
    m_helper->hide();

  m_browser->redraw();
  m_helper->redraw();

  redraw();
}

void Editor::SelectOption(Flu_Tree_Browser* browser, void* widget)
{
  if (browser->get_selected(1) != 0)
  {
    if (browser->callback_reason() == FLU_DOUBLE_CLICK)
    {
      MString m_text(MString(browser->get_selected(1)->label()) + "()");
      m_text = m_text.mid(((Editor*)widget)->m_currentword.length());
     ((Editor*)widget)->m_buffer->insert(((Editor*)widget)->insert_position(),m_text.toChar());
     ((Editor*)widget)->insert_position(((Editor*)widget)->insert_position()+m_text.length()-1);
     ((Editor*) widget)->m_drawbrowser = false;
     ((Editor*)widget)->m_currentword = "";
    }

   if (browser->callback_reason() == FLU_TAB_KEY)
   {
      MString m_text(MString(browser->get_selected(1)->label()) + "()");
      m_text = m_text.mid(((Editor*)widget)->m_currentword.length());
     ((Editor*)widget)->m_buffer->insert(((Editor*)widget)->insert_position(),m_text.toChar());
     ((Editor*)widget)->insert_position(((Editor*)widget)->insert_position()+m_text.length()-1);
     ((Editor*) widget)->m_drawbrowser = false;
     ((Editor*)widget)->m_currentword = "";

     ScriptActionManager m_manager;
     ScriptAction* m_help = m_manager.CreateAction(MString(browser->get_selected(1)->label()));
     if (m_help)
     {
       ((Editor*)widget)->m_helper->value(m_help->Help().toChar());
       if (MString(((Editor*)widget)->m_helper->value()) != "")
       {
         ((Editor*)widget)->m_helper->resize(((Editor*)widget)->m_helper->x(),((Editor*)widget)->m_helper->y(),(int)(m_help->Help().length()*4.9),15);
         ((Editor*)widget)->m_drawhelper = true;
       }
       else
         ((Editor*)widget)->m_drawhelper = false;
       delete m_help;
     }
     else
      ((Editor*)widget)->m_drawhelper = false;
   }

  }
} 


void Editor::TabPressed(void* widget) 
{
}

int Editor :: handle( int event )
{
  if ( event == FL_KEYDOWN )
  {
    if( Fl::event_key() == FL_Tab)
    {
      if (!m_drawbrowser)
        m_drawbrowser = true;
      else
        m_browser->handle(event);

      take_focus();
      return 1;
    }
    else if( Fl::event_key() == FL_Down)
    {
      if (m_drawbrowser)
      {
        m_browser->handle(event);
        take_focus();
        return 1;  
      }
    }
    else if( Fl::event_key() == FL_Up)
     {
        if (m_drawbrowser)
        {
          m_browser->handle(event);
          take_focus();
          return 1;  
        }
     }
    else
    {
       if ((Fl::event_key() == ' ') || (Fl::event_key() == FL_Enter))
        m_currentword = "";
       else
       if ((Fl::event_key() == FL_BackSpace) || (Fl::event_key() == FL_BackSpace))
       {
          m_currentword = m_currentword.mid(0,m_currentword.length()-1);
       }
       else
       {
         if ((Fl::event_key() != FL_Shift_L) &&  (Fl::event_key() != FL_Shift_R))
            m_currentword +=  Fl::event_key();
       }

      
      int m_offset = -1;
      int currentrating = 0;
      for (unsigned int i=0;i<sizeof(code_keywords)/sizeof(code_keywords[0]);i++)
      {

        int m_rating = 0;
        int m_correctword = true;
       
        for (unsigned int j=0;j<m_currentword.length();j++)
        {
          if (strlen(code_keywords[i]) > j)
          {
            if ((m_currentword.toLower()[j] != code_keywords[i][j]))
              m_correctword = false;
            else
             m_rating++;
          }
          else
          {
            m_correctword = false;
          }
        }
        
         
        if ((m_rating != 0) && (m_rating > currentrating) && (m_correctword))
        {
          m_offset = i;
          currentrating = m_rating;
        }

      }

      if (m_offset != -1)
      {
        m_drawbrowser = true;
        Flu_Tree_Browser::Node* m_node = m_browser->first();
        for (unsigned int k=0;k<=m_offset;k++)
          m_node = m_node->next();

        if(m_node)
          {
          m_node->select_only();
          m_browser->set_hilighted(m_node);
          }
        draw();
      }
      else
      {
        m_drawbrowser = false;
        m_drawhelper = false;
      }
    }

  }

  return   Fl_Text_Editor::handle( event );
}

} // end namespace bm
