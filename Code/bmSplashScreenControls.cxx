/*=========================================================================

  Program:   BatchMake
  Module:    bmSplashScreenControls.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmSplashScreenControls.h"

namespace bm {

SplashScreenControls::SplashScreenControls()
:SplashScreen()
{
  make_window();
}

SplashScreenControls::~SplashScreenControls()
{
}

void SplashScreenControls::Show()
{
  if(!g_Splashscreen->shown())
    g_Splashscreen->show();
}

} // end namespace bm
