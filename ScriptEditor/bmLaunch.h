/*=========================================================================

  Program:   BatchMake
  Module:    bmLaunch.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __Launch_h_
#define __Launch_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include "MString.h"
#include "bmProgressManager.h"

namespace bm {

class Launch
{
public:
  Launch();
  ~Launch();
  void Execute(MString m_command);
  void SetProgressManager(ProgressManager* progressmanager);
  MString GetOutput();
  MString GetError();

protected:
 ProgressManager* m_progressmanager;
 MString m_output;
 MString m_error;

};

} // end namespace bm

#endif
