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
#include "bmScriptError.h"
#include "bmScriptActionManager.h"
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
  for (unsigned int i=0;i<m_Parameters.size();i++)
    {
    m_Manager->TestConvert(m_Parameters[i],linenumber);
    }

  if (m_Parameters.size() < 2)
  {
    error->SetError(MString("No enough parameter for ExtractSlice"),linenumber);
    return false;
  }

  return true;
}

MString ScriptExtractSliceAction::Help()
{
  return "ExtractSlice(<3D image filename> <2D image filename> [orientation] [slice] [FROM_MIDDLE])";
}

void ScriptExtractSliceAction::Execute()
{
#ifdef BM_GRID
  if(m_GridModule)
    {
    this->GenerateGrid();
    return;
    }
#endif

  BMString m_input = m_Manager->Convert(m_Parameters[0]).removeAllChars('\'');
  BMString m_Output = m_Manager->Convert(m_Parameters[1]).removeAllChars('\'');

  SliceExtractor m_sliceextractor;

  if(m_Parameters.size()>2)
    {
    BMString m_orientation =
      m_Manager->Convert(m_Parameters[2]).removeAllChars('\'');
    m_sliceextractor.SetOrientation(m_orientation.toInt());
    }

  if(m_Parameters.size()>3)
    {
    BMString m_slice = m_Manager->Convert(m_Parameters[3]).removeAllChars('\'');
    m_sliceextractor.SetSlice(m_slice.toInt());
    }

  if(m_Parameters.size()>4 && !strcmp(m_Parameters[4].toChar(),"FROM_MIDDLE"))
    {
    m_sliceextractor.SetFromMiddleSlice(true);
    }

  m_sliceextractor.Extract(m_input.GetValue(),m_Output.GetValue());
}

} // end namespace bm
