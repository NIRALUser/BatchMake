// Id

/***************************************************************
 *                FLU - FLTK Utility Widgets 
 *  Copyright (C) 2002 Ohio Supercomputer Center, Ohio State University
 *
 * This file and its content is protected by a software license.
 * You should have received a copy of this license with this file.
 * If not, please contact the Ohio Supercomputer Center immediately:
 * Attn: Jason Bryan Re: FLU 1224 Kinnear Rd, Columbus, Ohio 43212
 * 
 ***************************************************************/



#include "FLU/Flu_Output.h"

Flu_Output :: Flu_Output( int X,int Y,int W,int H,const char *l )
  : Fl_Output(X,Y,W,H,l)
{
  color( FL_GRAY );
}

Flu_Output :: ~Flu_Output()
{
}
