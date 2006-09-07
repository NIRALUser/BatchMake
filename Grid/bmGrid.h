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
  void AddApplication(ApplicationWrapper* app,
                      const char* datadir=NULL,const char* outputdir=NULL);
  
  unsigned long GetNumberOfApplications()
    {
    return m_ApplicationsList.size();
    }

  void WriteCondor();
  void WriteGAD();
  void WriteShell();
  const char* GetCurrentScopeFile();

  void SetDataHost(const char* hostname) {m_DataHost = hostname;}
  void SetOutputHost(const char* hostname) {m_OutputHost = hostname;}
  void SetDataDirectory(const char* directory) {m_DataDirectory = directory;}
  void SetOutputDirectory(const char* directory) {m_OutputDirectory = directory;}
  const char* GetDataDirectory() {return m_DataDirectory.c_str();}
  const char* GetOutputDirectory() {return m_OutputDirectory.c_str();}

  void SetDistributed(bool val) 
    {
    //if(val != m_Distributed)
    // / {
      m_DistributedTransition = true;
    //  }
    m_Distributed = val;
    }

  void SetGridBarrier();
  void RemoveGridBarrier(){m_DistributedSyncBarrier.pop_back();}

  void SetSingleNode(bool single) 
    {
    if(single != m_SingleNode)
      {
      m_SingleNodeTransition = true;
      }
    m_SingleNode = single;
    }

protected:

  /** Return a filename for a full path */
  std::string GetFilename(const char* fullpath); 

  std::string m_FileName;
  std::vector<ApplicationWrapper> m_ApplicationsList; // we create a copy of the app
  std::string m_CurrentScopeFile;
  std::string m_DataHost;
  std::string m_OutputHost;
  std::string m_DataDirectory;
  std::string m_OutputDirectory;

  bool             m_Distributed;
  bool             m_DistributedTransition;
  std::vector<int> m_DistributedSyncBarrier;
  bool             m_SingleNode;
  bool             m_SingleNodeTransition;
};

} // end namespace bm

#endif
