/*=========================================================================

  Program:   BatchMake
  Module:    bmLaunch.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmLaunch.h"
#include "bmOutput.h"
  
#ifdef WIN32
  #include <Windows.h>
  #include <fcntl.h>
  #include <errno.h>
  #include <io.h>
  #include <stdio.h>
  #include <process.h>
#else
  #include <unistd.h>
  #include <sys/wait.h>
  #include <fcntl.h>
#endif

#include <fstream>

namespace bm {

Launch::Launch()
{
  m_progressmanager = 0;
  m_output = "";
  m_error = "";
}

Launch::~Launch()
{
}
  
void Launch::SetProgressManager(ProgressManager* progressmanager)
{
 m_progressmanager = progressmanager;
}

void Launch::Execute(MString m_command)
{
  m_output = "";
  m_error = "";

#ifdef WIN32

  char buffer[BUFSIZ+1];
  char buffer_err[BUFSIZ+1];

  STARTUPINFO si;
  PROCESS_INFORMATION pi;
 
  SECURITY_ATTRIBUTES tmpSec;
  ZeroMemory( &tmpSec, sizeof(tmpSec) );
  tmpSec.nLength = sizeof(tmpSec);
  tmpSec.bInheritHandle = true;
  HANDLE hReadPipe;
  HANDLE hWritePipe;
  HANDLE hReadErrorPipe;
  HANDLE hWriteErrorPipe;

  SECURITY_ATTRIBUTES  sa;
  ZeroMemory( &sa, sizeof(sa) );
  sa.nLength = sizeof(sa);
  sa.bInheritHandle = true;


  SECURITY_ATTRIBUTES  sa2;
  ZeroMemory( &sa2, sizeof(sa2) );
  sa2.nLength = sizeof(sa);
  sa2.bInheritHandle = true;


  CreatePipe(&hReadPipe,&hWritePipe,&sa,0);
  CreatePipe(&hReadErrorPipe,&hWriteErrorPipe,&sa2,0);


  ZeroMemory( &si, sizeof(si) );
  si.cb = sizeof(si);
  si.dwFlags = STARTF_USESTDHANDLES;
  si.hStdOutput = hWritePipe; //output;
  si.hStdError = hWriteErrorPipe; //error;
  ZeroMemory( &pi, sizeof(pi) );

  memset(buffer,'\0',sizeof(buffer)); 
  memset(buffer_err,'\0',sizeof(buffer_err)); 
  
  // Start the child process. 
  if( !CreateProcess( NULL,       // No module name (use command line). 
      (char*)m_command.latin1(),  // Command line. 
      NULL,                       // Process handle not inheritable. 
      NULL,                       // Thread handle not inheritable. 
      TRUE,                       // Set handle inheritance to FALSE. 
      0,                          //CREATE_NEW_PROCESS_GROUP,  // No creation flags. 
      NULL,                       // Use parent's environment block. 
      NULL,                       // Use parent's starting directory. 
      &si,                        // Pointer to STARTUPINFO structure.
      &pi )                       // Pointer to PROCESS_INFORMATION structure.
  ) 
  {
    std::cout << "BatchMake - CreateProcess failed!" << std::endl;
    m_error = "BatchMake - CreateProcess failed!";
    return;
  }

  CloseHandle(hWritePipe);
  CloseHandle(hWriteErrorPipe);

   // Wait until child process exits.
  bool m_run = true;



  while(m_run)
  {
    if (m_progressmanager)
    {
      m_progressmanager->IsRunning();
      if (m_progressmanager->IsStop())
        m_run = false;
    
    }

 
  unsigned long m_nbread = 0;
  unsigned long m_nberrorread = 0;
  unsigned long m_nbtoread = 512;
  unsigned long m_nbtoreaderror = 512;
  int m_read = 0;
  int m_readerror = 0;
  unsigned long m_nbreaded = 0;
  unsigned long m_nberrorreaded = 0;

  if (WaitForSingleObject( pi.hProcess, 500 )  == 0)
      m_run = false;

  PeekNamedPipe(hReadPipe,buffer,sizeof(buffer),&m_nbtoread,&m_nbread,NULL); 
  PeekNamedPipe(hReadErrorPipe,buffer_err,sizeof(buffer_err),&m_nbtoreaderror,&m_nberrorread,NULL);

  if (m_nbread != 0)
      ReadFile(hReadPipe, buffer,512,&m_nbreaded,NULL); 


   if (m_nberrorread != 0)
       ReadFile(hReadErrorPipe, buffer_err,512,&m_nberrorreaded,NULL); 
  
    if (m_nbreaded != 0)
    {
      
      if (m_progressmanager)
        m_progressmanager->DisplayOutput(MString(buffer).removeChar('\r'));

      for (unsigned int k=0;k<strlen(buffer);k++)
      {
        if (buffer[k] != '\r')
           m_output += buffer[k];
      }

      memset(buffer,'\0',sizeof(buffer)); 
    }

    if (m_nberrorreaded != 0)
    {
      if (m_progressmanager)
        m_progressmanager->DisplayError(MString(buffer_err).removeChar('\r'));

      for (unsigned int k=0;k<strlen(buffer_err);k++)
      {
        if (buffer[k] != '\r') 
          m_error += buffer_err[k];
      }

      memset(buffer_err,'\0',sizeof(buffer_err)); 
    }

  } 

  //Terminate Process
  TerminateProcess(pi.hProcess,0);

  CloseHandle(hReadPipe);
  CloseHandle(hReadErrorPipe);

  CloseHandle( pi.hProcess );
  CloseHandle( pi.hThread );

#else  
  int stdin_pipe[2];
  int stdout_pipe[2];
  int stderr_pipe[2];
  char buffer[BUFSIZ+1];
  char buffer_err[BUFSIZ+1];
  int fork_result;
  int data_processed;
  int data_processed_err;
  int nchars = 0;
int status = 0;

  memset(buffer,'\0',sizeof(buffer));
memset(buffer_err,'\0',sizeof(buffer)); 

   if ( (pipe(stdin_pipe)==0)   
        && (pipe(stdout_pipe)==0)
        && (pipe(stderr_pipe)==0))
   {
     fork_result = fork();
     if (fork_result == -1)
     {
       std::cerr << "Create Process failed (Pipe error) ! " << std::endl;   
       m_error = "Create Process failed (Pipe error) ! "; 
      exit(EXIT_FAILURE);
     }  
     else if (fork_result == 0)
     { 
       // This is the child
       close(0);
      dup(stdin_pipe[0]);
      close(stdin_pipe[0]);
      close(stdin_pipe[1]);
      close(1);      
      dup(stdout_pipe[1]);     
      close(stdout_pipe[0]); 
      close(stdout_pipe[1]);      
      close(2);
      dup(stderr_pipe[1]);     
      close(stderr_pipe[0]);      
      close(stderr_pipe[1]);   
      MString m_prog = m_command.begin(" ");
      MString m_param = m_command.end(" ");   
   

      fcntl(stdout_pipe[1], F_SETFL, O_NONBLOCK);
      fcntl(stderr_pipe[1], F_SETFL, O_NONBLOCK);

     if (m_param == m_prog)
       m_param = "";

     if (m_param.length() != 0)
        m_param = m_param + 1;


    if (execlp(m_prog.toChar(),(m_prog.rend("/")+1).toChar(),m_param.toChar(),NULL) == -1)
    {         
  if (errno == 2)
  {
      std::cerr << (MString("Program (") + m_prog + ") not found!").toChar() << std::endl;      
  }
    }

      exit(EXIT_FAILURE);
    } 
    else   
    { 
      // This is the parent
      close(stdin_pipe[0]);
      close(stdin_pipe[1]);
      close(stderr_pipe[1]);
      close(stdout_pipe[1]);  

      fcntl(stdout_pipe[0], F_SETFL, O_NONBLOCK);
      fcntl(stderr_pipe[0], F_SETFL, O_NONBLOCK);

      while(1)   
      {       
        data_processed_err = read(stderr_pipe[0],buffer_err,BUFSIZ);
        if (data_processed_err != -1)
        {
          for (unsigned int k=0;k<strlen(buffer_err);k++)
            m_error += buffer_err[k];
       
          if (m_progressmanager)
            m_progressmanager->DisplayError(MString(buffer_err));
       
    memset(buffer_err,'\0',sizeof(buffer));
        }

 
       data_processed = read(stdout_pipe[0],buffer,BUFSIZ);
       if (data_processed != -1)
       {
   for (unsigned int k=0;k<strlen(buffer);k++)
           m_output += buffer[k];
       
         if (m_progressmanager)
           m_progressmanager->DisplayOutput(MString(buffer));

         memset(buffer,'\0',sizeof(buffer));
       }
      
  if (m_progressmanager)
     {
          m_progressmanager->IsRunning();
          if (m_progressmanager->IsStop())
          break;
      }

       if ((data_processed == 0) && (data_processed_err == 0)) break;
     }

     close(stderr_pipe[0]);
     close(stdout_pipe[0]);
    }
  }
#endif
}


MString Launch::GetOutput()
{
  return m_output;
}


MString Launch::GetError()
{
  return m_error;
}

} // end namespace bm
