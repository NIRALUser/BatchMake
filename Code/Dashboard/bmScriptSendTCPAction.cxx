/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptSendTCPAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptSendTCPAction.h"
#include "bmScriptError.h"
#include "bmScriptActionManager.h"

namespace bm {

/** */
ScriptSendTCPAction::ScriptSendTCPAction()
: ScriptAction()
{
}

/** */
ScriptSendTCPAction::~ScriptSendTCPAction()
{
}

/** */
bool ScriptSendTCPAction::TestParam(ScriptError* error,int linenumber)
{
  if (m_Parameters.size() <2)
    {
    error->SetError(MString("No enough parameter for SendTCP"),linenumber);
    return false;
    }
  if (m_Parameters.size() >2)
    {
    error->SetError(MString("Too much parameters for SendTCP"),linenumber);
    return false;
    }

  return true;
}

/** */
MString ScriptSendTCPAction::Help()
{
  return "SendTCP(SocketName Message)";
}

/** */
void ScriptSendTCPAction::Execute()
{
  BMString m_value = m_Manager->Convert(m_Parameters[1]);
  m_value.removeAllChars('\'');

  TCPSocket* socket = m_Manager->GetVariableSocket(m_Parameters[0]);
  int err = socket->SendMessage(m_value.toChar());

  if( err != 0 )
    {
    //std::cout<<"error sending the message"<<std::endl;
    this->m_ProgressManager->AddError("SendTCP: Error sending the message");
    return;
    }
}

} // end namespace bm
