/*=========================================================================

  Program:   BatchMake
  Module:    bmProgressManager.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmProgressManager.h"

namespace bm {

ProgressManager::ProgressManager()
{
  m_stop = false;
}

ProgressManager::~ProgressManager()
{
}


void ProgressManager::SetStatus(MString status)
{
  std::cout << " Status: " << status.toChar() << std::endl;
}


void ProgressManager::IsRunning()
{
  std::cout << ".";
}

void ProgressManager::AddAction(MString name)
{
}

void ProgressManager::FinishAction(MString output)
{
}

void ProgressManager::AddOutput(MString output)
{
}

void ProgressManager::AddError(MString output)
{
}

bool ProgressManager::IsStop()
{
  return m_stop;
}

void ProgressManager::SetFinished(MString message)
{

}

} // end namespace bm
