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
  if (m_parameters.size() <2)
    {
    error->SetError(MString("No enough parameter for SendTCP"),linenumber);
    return false;
    }
  if (m_parameters.size() >2)
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
  TCPSocket* socket = m_manager->GetVariableSocket(m_parameters[0]);
  int err = socket->SendMessage(m_parameters[1].toChar());
  
  if( err == -1 )
    {
    std::cout<<"error sending the message"<<std::endl;
    }
}

} // end namespace bm
