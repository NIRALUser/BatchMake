/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptGridTransferFileAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptGridTransferFileAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

namespace bm {

/** */
ScriptGridTransferFileAction::ScriptGridTransferFileAction()
: ScriptAction()
{
}

/** */
ScriptGridTransferFileAction::~ScriptGridTransferFileAction()
{
}

/** */
bool ScriptGridTransferFileAction::TestParam(ScriptError* error,int linenumber)
{
  if (m_Parameters.size() < 1)
    {
    error->SetError(MString("No enough parameter for GridTransferFile"),linenumber);
    return false;
    }

  for (unsigned int i = 0; i < m_Parameters.size(); ++i )
    {
    m_Manager->TestConvert( m_Parameters[i], linenumber );
    }
  return true;
}

/** */
MString ScriptGridTransferFileAction::Help()
{
  return "GridTransferFile(ALL|EXECUTABLE|INPUT_FILES|OUTPUT_FILES|NONE)";
}

/** */
void ScriptGridTransferFileAction::Execute()
{
  if(!m_GridModule)
    {
    return;
    }

  std::string transfer =
    m_Manager->Convert( m_Parameters[0] ).fromVariable().toChar();
  if( transfer == "ALL")
    {
    m_GridModule->SetTransferFile(Grid::ALL);
    }
  else if( transfer == "EXECUTABLE")
    {
    m_GridModule->SetTransferFile(Grid::EXECUTABLE);
    }
  else if( transfer == "INPUT_FILES")
    {
    m_GridModule->SetTransferFile(Grid::INPUT_FILES);
    }
  else if( transfer == "OUTPUT_FILES")
    {
    m_GridModule->SetTransferFile(Grid::OUTPUT_FILES);
    }
  else if( transfer == "NONE")
    {
    m_GridModule->SetTransferFile(Grid::NONE);
    }
}

} // end namespace bm
