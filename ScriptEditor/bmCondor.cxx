/*=========================================================================

  Program:   BatchMake
  Module:    bmCondor.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmCondor.h"
#include <stdio.h>
  
namespace bm {

Condor::Condor()
{
  m_FileName = "";
}

Condor::~Condor()
{
}

/** Add a command to the condor script 
 *  The command is parsed to produce a valid line */
void Condor::AddCommand(const char* command)
{
  m_CommandList.push_back(command);
}

/** Write the condor script */
void Condor::Write()
{
  if(m_FileName.size() == 0)
    {
    std::cout << "Condor::Write() : Please set a filename" << std::endl;
    return;
    }
  FILE* fic = fopen(m_FileName.c_str(),"wb");
  
  if(!fic)
    {
    std::cout << "Condor::Write() : Cannot create condor script" << std::endl;
    return;
    }
  
  // Write the header
  fprintf(fic,"# Script generated by BatchMake (c) InsightSoftwareConsortium\n");
  fprintf(fic,"# CADDLab @ UNC Chapel Hill 2004-2005\n");
  fprintf(fic,"# More information at: http://caddlab.rad.unc.edu\n");
  
  fprintf(fic,"universe       = vanilla\n");
  fprintf(fic,"output         = bmCondor.out.txt\n");             
  fprintf(fic,"error          = bmCondor.error.txt\n");            
  fprintf(fic,"log            = bmCondor.log.txt\n");
  
  // Add the executable
  std::vector<std::string>::const_iterator it = m_CommandList.begin();
  while(it != m_CommandList.end())
    {
    fprintf(fic,"executable     = %s\n",(*it).c_str());
    fprintf(fic,"queue 1\n");
    it++;
    }
  fclose(fic);
}

} // end namespace bm
