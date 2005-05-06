/*=========================================================================

  Program:   BatchMake
  Module:    bmCondor.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __Condor_h_
#define __Condor_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include <vector>
#include <ApplicationWrapper.h>

namespace bm {

class Condor
{
public:
  Condor();
  ~Condor();

  void SetFileName(const char* filename) {m_FileName = filename;}
    
  /** Add an application to the list of applications to generate */
  void AddApplication(ApplicationWrapper* app);
  
  void Write();

protected:

  std::string m_FileName;
  std::vector<ApplicationWrapper> m_ApplicationsList; // we create a copy of the app
};

} // end namespace bm

#endif
