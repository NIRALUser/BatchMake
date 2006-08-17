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
    
  if (m_parameters.size() < 2)
  {
    error->SetError(MString("No enough parameter for ExtractSlice"),linenumber);
    return false;
  }   

  return true;
}

MString ScriptExtractSliceAction::Help()
{
  return "ExtractSlice(<3D image filename> <2D image filename> [orientation] [slice])";
}

void ScriptExtractSliceAction::GenerateGrid()
{
  // We create the bmSliceExtractor application and send it to grid
  ApplicationWrapper app;
  MString appName = "bmSliceExtractor";
  bool appFound = false;
  ScriptActionManager::ApplicationWrapperListType::iterator itApp = m_manager->GetApplicationWrapperList()->begin();
  while (itApp != m_manager->GetApplicationWrapperList()->end())
    {
    if(!strcmp((*itApp)->GetName().toChar(),appName.toChar()))
      {
      app = *(*itApp);
      appFound = true;
      break;
      }
    itApp++;
  }

  if(!appFound)
    {
    std::cout << "ScriptExtractSliceAction::GenerateGrid : Cannot find bmSliceExtractor " 
              << appName.toChar() << std::endl;
    return;
    }

  MString m_input = m_manager->Convert(m_parameters[0]).removeChar('\'').latin1();
  MString m_output = m_manager->Convert(m_parameters[1]).removeChar('\'').latin1();
  
  MString m_orientation = "-1";
  if(m_parameters.size()>2)
    {
    m_orientation = m_manager->Convert(m_parameters[2]).removeChar('\'').latin1();
    }

  MString m_slice = "-1";
  if(m_parameters.size()>3)
    {
    m_slice = m_manager->Convert(m_parameters[2]).removeChar('\'').latin1();
    }
   m_manager->Convert(m_parameters[3]).removeChar('\'').latin1();

  std::string withslash = "\"";
  withslash += m_input.toChar();
  withslash += "\"";
  app.SetParameterValue("volume","",withslash);
  withslash = "\"";
  withslash += m_output.toChar();
  withslash += "\"";
  app.SetParameterValue("slice","",withslash);
  app.SetParameterValue("orientation","",m_orientation.toChar());
  app.SetParameterValue("sliceNumber","",m_slice.toChar());

  m_GridModule->AddApplication(&app);
}

void ScriptExtractSliceAction::Execute()
{
  if(m_GridModule)
    {
    this->GenerateGrid();
    return;
    }

  MString m_input = m_manager->Convert(m_parameters[0]).removeChar('\'').latin1();
  MString m_output = m_manager->Convert(m_parameters[1]).removeChar('\'').latin1();
   
  SliceExtractor m_sliceextractor;
 
  if(m_parameters.size()>2)
    {
    MString m_orientation = m_manager->Convert(m_parameters[2]).removeChar('\'').latin1();
    m_sliceextractor.SetOrientation(m_orientation.toInt());
    }
  
  if(m_parameters.size()>3)
    {
    MString m_slice = m_manager->Convert(m_parameters[3]).removeChar('\'').latin1();
    m_sliceextractor.SetSlice(m_slice.toInt());
    }

  m_sliceextractor.Extract(m_input.toChar(),m_output.toChar());
}

} // end namespace bm
