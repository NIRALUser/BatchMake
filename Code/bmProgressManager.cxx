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


void ProgressManager::SetStatus(const BMString& status)
{
  std::cout << "Status: " << status.toChar() << std::endl;
}


void ProgressManager::IsRunning()
{
}

void ProgressManager::AddAction(const BMString& )
{
}

void ProgressManager::FinishAction(const BMString& )
{
}

void ProgressManager::AddOutput(const BMString& )
{
  // AddOutput is typically called by the Run action.
  // After an external application is executed, Run() calls
  // AddOutput with the outputs of the external application
  // While a GUI Progress Manager would like to display
  // these outputs, we probably don't want them outputed
  // in a standard stream. If the user want them, he should
  // use Echo( ${output} ) after calling Run( output my_prog )
  // if( output.length() )
  //  {
  //  std::cerr << output.toChar() << std::endl;
  //  }
}

void ProgressManager::AddError(const BMString& error)
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

void ProgressManager::SetFinished(const BMString& )
{
}

void ProgressManager::DisplayOutput(const BMString& message)
{
  std::cout << message.toChar();
}

void ProgressManager::DisplayError(const BMString& message)
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
