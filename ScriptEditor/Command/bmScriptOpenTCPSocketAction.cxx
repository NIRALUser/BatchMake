/*=========================================================================

  Program:   BatchMake
  Module:    bmScriptOpenTCPSocketAction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmScriptOpenTCPSocketAction.h"

namespace bm {

/** */
ScriptOpenTCPSocketAction::ScriptOpenTCPSocketAction()
: ScriptAction()
{
}

/** */
ScriptOpenTCPSocketAction::~ScriptOpenTCPSocketAction()
{
}

/** */
bool ScriptOpenTCPSocketAction::TestParam(ScriptError* error,int linenumber)
{
  if (m_parameters.size() <3)
    {
    error->SetError(MString("No enough parameter for OpenTCPSocket"),linenumber);
    return false;
    }
  if (m_parameters.size() >3)
    {
    error->SetError(MString("Too much parameters for OpenTCPSocket"),linenumber);
    return false;
    }

  m_manager->SetSocketVariable(m_parameters[0]);

  return true;
}

/** */
MString ScriptOpenTCPSocketAction::Help()
{
  return "OpenTCPSocket(SocketName IPadresse TCPport)";
}

/** */
void ScriptOpenTCPSocketAction::Execute()
{
  TCPSocket* socket = m_manager->GetVariableSocket(m_parameters[0]);
  int err = socket->OpenSocket(m_parameters[1].toChar(),m_parameters[2].toInt());
  
  if( err == -1 )
    {
    std::cout<<"error creating the TCPsocket"<<std::endl;
    }
}

} // end namespace bm
