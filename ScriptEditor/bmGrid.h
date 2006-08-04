/*=========================================================================

  Program:   BatchMake
  Module:    bmGrid.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __Grid_h_
#define __Grid_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include <vector>
#include <ApplicationWrapper.h>

namespace bm {

class Grid
{
public:
  Grid();
  ~Grid();

  void SetFileName(const char* filename) {m_FileName = filename;}
    
  /** Add an application to the list of applications to generate */
  void AddApplication(ApplicationWrapper* app);
  
  unsigned long GetNumberOfApplications()
    {
    return m_ApplicationsList.size();
    }

  void WriteCondor();
  void WriteGAD();
  void WriteShell();
  const char* GetCurrentScopeFile();

protected:

  std::string m_FileName;
  std::vector<ApplicationWrapper> m_ApplicationsList; // we create a copy of the app
  std::string m_CurrentScopeFile;
};

} // end namespace bm

#endif
