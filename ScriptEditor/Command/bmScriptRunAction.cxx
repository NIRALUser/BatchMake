/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptRunAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptRunAction.h"
#include "bmLaunch.h"
#include "Timer.h"

namespace bm {

ScriptRunAction::ScriptRunAction()
: ScriptAction()
{
}

ScriptRunAction::~ScriptRunAction()
{
}

bool ScriptRunAction::TestParam(ScriptError* error,int linenumber)
{
   if (m_parameters.size() <2)
   {
     error->SetError(MString("No enough parameter for Run"),linenumber);
     return false;
   }

   if (m_parameters.size() >2)
   {
     error->SetWarning(MString("Too much parameter for Run"),linenumber);
     return false;
   }
  
  m_manager->SetTestVariable(m_parameters[0]);


  return (m_manager->TestConvert(m_parameters[1],linenumber));
}

void ScriptRunAction::Execute()
{
  Launch m_launch;
  Timer m_timer;
  m_timer.start();
  m_launch.SetProgressManager(m_progressmanager);

  m_progressmanager->SetStatus(MString("Start ") + m_manager->Convert(m_parameters[1]).removeChar('\''));
  MString m_actioname = m_manager->ConvertExtra(m_parameters[1]);

  m_actioname = m_actioname.removeChar(' ',true);
  m_actioname = m_actioname.removeChar('\'',true);
  m_actioname = m_actioname.begin("'").rend("\\");
  if (m_actioname.length() != 0)
    m_actioname = m_actioname + 1;

  m_progressmanager->AddAction(m_actioname);
  
  m_launch.Execute(m_manager->Convert(m_parameters[1]).removeChar('\''));
  MString m_output = m_launch.GetOutput();
  MString m_error = m_launch.GetError();
  m_manager->SetVariable(m_parameters[0],MString("'") + m_output + "'");
  m_progressmanager->SetStatus(MString("Finish: Execution time %1ms").arg(m_timer.getMilliseconds()) + m_manager->Convert(m_parameters[1]));
  m_progressmanager->FinishAction(MString("Execution time: %1ms").arg(m_timer.getMilliseconds()));
  
  int m_offset = 0;
  while (m_offset != -1)
  {
    m_offset = m_output.find("\n",m_offset);
    m_progressmanager->AddOutput(m_output.begin("\n"));
    if (m_offset != -1)
     m_output = m_output.end("\n")+1;
  }


  m_offset = 0;
  while (m_offset != -1)
  {
    m_offset = m_error.find("\n",m_offset);
    m_progressmanager->AddError(m_error.begin("\n"));
    if (m_offset != -1)
     m_error = m_error.end("\n")+1;
  }



  m_timer.stop();

  // m_manager->GetError()->SetStatus(m_launch.Execute(m_manager->Convert(m_parameters[1])));


  /*MString m_value;
  for (int i=m_parameters[1].toInt();i<m_parameters[2].toInt();i+=m_parameters[3].toInt())
  {
    m_value+= MString("%1 ").arg(i);
  }
  m_value+= m_parameters[2];
  m_manager->SetVariable(m_parameters[0],m_value);*/
}

} // end namespace bm
