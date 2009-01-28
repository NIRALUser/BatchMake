/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptRandomizeAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptRandomizeAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"
#include <time.h>

namespace bm {

ScriptRandomizeAction::ScriptRandomizeAction()
: ScriptAction()
{
}

ScriptRandomizeAction::~ScriptRandomizeAction()
{
}

bool ScriptRandomizeAction::TestParam(ScriptError* error,int linenumber)
{
  m_Manager->SetTestVariable(m_Parameters[0]);

  if (m_Parameters.size() <4)
    {
    error->SetError(MString("No enough parameter for Randomize"),linenumber);
    return false;
    }

  if ((m_Parameters[1].toLower() != "gaussian")\
      && (m_Parameters[1].toLower() != "uniform")\
      && (m_Parameters[1].toLower() != "categorical"))
    {
    error->SetError(MString("No valid parameters for Randomize [") 
                    + m_Parameters[1] + "]",linenumber);
    return false;
    }

  if ((m_Parameters[1].toLower() == "gaussian") \
      && (m_Parameters.size() != 4))
    {
    error->SetError(MString("Incorret number of parameters for Randomize [")
                    + m_Parameters[1] + "]",linenumber);
    return false;
    }

  if ((m_Parameters[1].toLower()== "uniform") \
       && (m_Parameters.size() != 4))
    {
    error->SetError(MString("Incorret number of parameters for Randomize [")
                    + m_Parameters[1] + "]",linenumber);
    return false;
    }

  if ((m_Parameters[1].toLower() == "categorical") \
       && (m_Parameters.size() < 4))
    {
    error->SetError(MString("Incorret number of parameters for Randomize"),
                    linenumber);
    return false;
    }

  return true;
} 

MString ScriptRandomizeAction::Help()
{
  return "Randomize(<outputvariale> <uniform|gaussian> <min|mean> <max>)";
}


void ScriptRandomizeAction::Execute()
{
  MString m_value;
  if (m_Parameters[1].toLower() == "uniform")
    {
    float m_min = m_Parameters[2].toFloat();
    float m_max = m_Parameters[3].toFloat();
    //sprintf(m_text,"'%f'",(((float)rand()*(m_max-m_min))/(float)RAND_MAX)+m_min);
    m_value = MString( ( ( (float)rand() * ( m_max - m_min ) ) / 
                           (float)RAND_MAX ) + m_min ).toVariable();
    }
  
  m_Manager->SetVariable(m_Parameters[0],m_value);
}

} // end namespace bm
