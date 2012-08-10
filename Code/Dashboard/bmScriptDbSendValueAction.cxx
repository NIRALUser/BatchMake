/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptDbSendValueAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptDbSendValueAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"
#include "HttpRequest.h"

namespace bm {

ScriptDbSendValueAction::ScriptDbSendValueAction()
: ScriptAction()
{
}

ScriptDbSendValueAction::~ScriptDbSendValueAction()
{
}

bool ScriptDbSendValueAction::TestParam(ScriptError* error,int linenumber)
{
  for (unsigned int i=0;i<m_Parameters.size();i++)
      m_Manager->TestConvert(m_Parameters[i],linenumber);

  if (m_Parameters.size() != 6)
  {
    error->SetError(MString("No enough parameter for DbSendValue"),linenumber);
    return false;
  }

  return true;
}

MString ScriptDbSendValueAction::Help()
{
  return "DbSendValue(<host> <account> <password> <project> <caption> <value>)";
}


void ScriptDbSendValueAction::Execute()
{
  m_ProgressManager->AddAction("Send data to Dashboard");
  m_ProgressManager->IsRunning();
  HttpRequest m_request;

  BMString m_host = m_Manager->Convert(m_Parameters[0]).removeAllChars('\'');
  BMString m_account = m_Manager->Convert(m_Parameters[1]).removeAllChars('\'');
  BMString m_password = m_Manager->Convert(m_Parameters[2]).removeAllChars('\'');
  BMString m_project = m_Manager->Convert(m_Parameters[3]).removeAllChars('\'');
  BMString m_caption = m_Manager->Convert(m_Parameters[4]).removeAllChars('\'');
  BMString m_value = m_Manager->Convert(m_Parameters[5]).removeAllChars('\'');

  m_request.AddParam("hostname",m_request.GetHostName().c_str());
  m_request.AddParam("hostip",m_request.GetHostIp().c_str());
  m_request.AddParam("project",m_project.toChar());
  m_request.AddParam("account",m_account.toChar());
  m_request.AddParam("password",m_password.toChar());
  m_request.AddParam("caption",m_caption.toChar());
  m_request.AddParam("value",m_value.toChar());

  //m_request.AddParam("clear","1");
  //m_request.AddParam("type","1");
  //m_request.SetFile("main.htm");
  //"www.ia.unc.edu/dev/batchmake/transfert.php"
  MString m_Output = m_request.Send(m_host.toChar());

  if (m_Output.length()>3)
    {
    m_ProgressManager->AddError("Bad Host or connexion problem");
    return;
    }

  if (m_Output.toInt() != 0)
    {
    m_ProgressManager->FinishAction("Dashboard problem when sending data");
    switch(m_Output.toInt())
      {
      case 1 :  m_ProgressManager->AddError("Bad account name"); break;
      case 2 :  m_ProgressManager->AddError("Bad password"); break;
      case 3 :  m_ProgressManager->AddError("Bad project name"); break;
      case 4 :  m_ProgressManager->AddError("Over quota: please use DbClear function first"); break;
      case 5 :  m_ProgressManager->AddError("Host Database error"); break;
      case -1 : m_ProgressManager->AddError("Connexion problem"); break;
      }
    return;
    }

  m_ProgressManager->FinishAction("Data sent");
  return;
}

} // end namespace bm
