/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptExtractSliceAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptExtractSliceAction.h"
#include "SliceExtractor.h"

namespace bm {

ScriptExtractSliceAction::ScriptExtractSliceAction()
: ScriptAction()
{
}

ScriptExtractSliceAction::~ScriptExtractSliceAction()
{
}

bool ScriptExtractSliceAction::TestParam(ScriptError* error,int linenumber)
{
  for (unsigned int i=0;i<m_parameters.size();i++)
      m_manager->TestConvert(m_parameters[i],linenumber);
    
  if (m_parameters.size() < 4)
  {
    error->SetError(MString("No enough parameter for ExtractSlice"),linenumber);
    return false;
  }   

  return true;
}

MString ScriptExtractSliceAction::Help()
{
  return "ExtractSlice(<3D image filename> <2D image filename> <orientation> <slice>)";
}


void ScriptExtractSliceAction::Execute()
{
  MString m_input = m_manager->Convert(m_parameters[0]).removeChar('\'').latin1();
  MString m_output = m_manager->Convert(m_parameters[1]).removeChar('\'').latin1();
  MString m_orientation = m_manager->Convert(m_parameters[2]).removeChar('\'').latin1();
  MString m_slice = m_manager->Convert(m_parameters[3]).removeChar('\'').latin1();

  SliceExtractor m_sliceextractor;
  m_sliceextractor.SetOrientation(m_orientation.toInt());
  m_sliceextractor.SetSlice(m_slice.toInt());
  m_sliceextractor.Extract(m_input.toChar(),m_output.toChar());
}

} // end namespace bm
