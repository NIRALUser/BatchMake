/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptDbClearAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptDbClearAction.h"
#include "HttpRequest.h"

namespace bm {

ScriptDbClearAction::ScriptDbClearAction()
: ScriptAction()
{
}

ScriptDbClearAction::~ScriptDbClearAction()
{
}

bool ScriptDbClearAction::TestParam(ScriptError* error,int linenumber)
{
  for (unsigned int i=0;i<m_parameters.size();i++)
      m_manager->TestConvert(m_parameters[i],linenumber);
     
  if (m_parameters.size() != 4)
  {
    error->SetError(MString("No enough parameter for DbClear"),linenumber);
    return false;
  }   

  return true;
}

MString ScriptDbClearAction::Help()
{
  return "DbSendValue(<host> <account> <password> <project>)";
}


void ScriptDbClearAction::Execute()
{
  m_progressmanager->AddAction("Clear Dashboard");
  m_progressmanager->IsRunning();
  HttpRequest m_request;

  MString m_host = m_manager->Convert(m_parameters[0]).removeChar('\'').latin1();
  MString m_account = m_manager->Convert(m_parameters[1]).removeChar('\'').latin1();
  MString m_password = m_manager->Convert(m_parameters[2]).removeChar('\'').latin1(); 
  MString m_project = m_manager->Convert(m_parameters[3]).removeChar('\'').latin1();

  m_request.AddParam("hostname",m_request.GetHostName());
  m_request.AddParam("hostip",m_request.GetHostIp());
  m_request.AddParam("project",m_project.toChar());
  m_request.AddParam("account",m_account.toChar());
  m_request.AddParam("password",m_password.toChar());  
  m_request.AddParam("clear","1");

  MString m_output = m_request.Send(m_host.toChar());
  
  if (m_output.length()>3)
    m_progressmanager->AddError("Bad Host or connexion problem");
  else
  {
    if (m_output.toInt() == 0)
      m_progressmanager->FinishAction(MString("Data sent"));
    else
    {
      m_progressmanager->FinishAction(MString("Dashboard problem when sending data"));
      switch(m_output.toInt())
      {
        case 1 :  m_progressmanager->AddError("Bad account name"); break;
        case 2 :  m_progressmanager->AddError("Bad password"); break;
        case 3 :  m_progressmanager->AddError("Bad project name"); break;
        case 4 :  m_progressmanager->AddError("Over quota: please use DbClear function first"); break;
        case 5 :  m_progressmanager->AddError("Host Database error"); break;
        case -1 : m_progressmanager->AddError("Connexion problem"); break;
      }  
    }
  }
}

} // end namespace bm
