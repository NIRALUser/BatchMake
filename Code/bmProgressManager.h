/*=========================================================================

  Program:   BatchMake
  Module:    bmProgressManager.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __ProgressManager_h_
#define __ProgressManager_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include "BMString.h"

namespace bm {

class ProgressManager
{
public:
  ProgressManager();
  virtual ~ProgressManager();
  virtual void SetStatus(const BMString& status);
  virtual void Start(bool resetErrors = true);
  virtual void Stop();
  virtual void IsRunning();
  virtual void AddAction(const BMString& name);
  virtual void FinishAction(const BMString& output);
  /** Typically called by Run(...) */
  virtual void AddOutput(const BMString& output);
  virtual void AddError(const BMString& output);
  virtual bool IsStop();
  virtual void SetFinished(const BMString& message);
  /** Typically called by Echo(...) */
  virtual void DisplayOutput(const BMString& message);
  virtual void DisplayError(const BMString& message);
  /** Define the behavior to have when an error is added. If stop is set to 
   *  true, next time AddError() is called, it will automatically call Stop() 
   *  and the next action won't be executed. If stop is set to false, then 
   *  nothing happen when AddError is called.
   */
  void SetStopOnError( bool stopOnError );
  void ResetErrorCount();
  unsigned int GetErrorCount()const;
  
protected:

  bool m_Stop;
  bool m_StopOnError;
  unsigned int m_ErrorCount;
};

} // end namespace bm

#endif
