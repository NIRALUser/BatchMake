/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptError.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __ScriptError_h_
#define __ScriptError_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include "BMString.h"

namespace bm {

class ScriptError
{

public:

  ScriptError();
  virtual ~ScriptError();
  virtual void SetError( const BMString& error, int line=-1 );
  virtual void SetWarning( const BMString& warning, int line=-1 );
  virtual void SetStatus( const BMString& status );
  virtual void DisplaySummary();
  virtual int GetError();
  virtual int GetWarning();
  virtual void Reset();

protected:

  int nb_error;
  int nb_warning;

};

} // end namespace bm

#endif
