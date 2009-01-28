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
  m_Stop = false;
  m_StopOnError = false;
}

ProgressManager::~ProgressManager()
{
}


void ProgressManager::SetStatus(const MString& status)
{
  std::cout << "Status: " << status.toChar() << std::endl;
}


void ProgressManager::IsRunning()
{
}

void ProgressManager::AddAction(const MString& name)
{
}

void ProgressManager::FinishAction(const MString& output)
{
}

void ProgressManager::AddOutput(const MString& output)
{
  if( output.length() )
    {
    std::cerr << output.toChar() << std::endl;
    }
}

void ProgressManager::AddError(const MString& error)
{
  if( error.length() )
    {
    std::cerr << "Error: " << error.toChar() << std::endl;
    }
  ++m_ErrorCount;
  if( m_StopOnError )
    {
    this->Stop();
    }
}

bool ProgressManager::IsStop()
{
  return m_Stop;
}

void ProgressManager::Stop()
{
  m_Stop = true;
}

void ProgressManager::Start(bool reset)
{
  m_Stop = false;
  if( reset )
    {
    this->ResetErrorCount();
    }
}

void ProgressManager::SetFinished(const MString& message)
{
}

void ProgressManager::DisplayOutput(const MString& message)
{
  std::cout << message.toChar();
}

void ProgressManager::DisplayError(const MString& message)
{
  std::cout << message.toChar();
}

void ProgressManager::SetStopOnError( bool stopOnError )
{
  m_StopOnError = stopOnError;
}

unsigned int ProgressManager::GetErrorCount()const
{
  return m_ErrorCount;
}

void ProgressManager::ResetErrorCount()
{
  m_ErrorCount = 0;
}

} // end namespace bm
