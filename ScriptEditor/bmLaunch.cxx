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

#ifdef WIN32
  #include <Windows.h>
#else
  #include <unistd.h>
  #include <sys/wait.h>
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
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
 
  SECURITY_ATTRIBUTES tmpSec;
  ZeroMemory( &tmpSec, sizeof(tmpSec) );
  tmpSec.nLength = sizeof(tmpSec);
  tmpSec.bInheritHandle = true;

  HANDLE output = CreateFile("output.tmp",
           GENERIC_WRITE, FILE_SHARE_WRITE,
           &tmpSec, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);


  HANDLE error = CreateFile("error.tmp",
           GENERIC_WRITE, FILE_SHARE_WRITE,
           &tmpSec, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
   
  ZeroMemory( &si, sizeof(si) );
  si.cb = sizeof(si);
  si.dwFlags = STARTF_USESTDHANDLES;
  si.hStdOutput = output;
  si.hStdError = error;
  ZeroMemory( &pi, sizeof(pi) );

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
    m_error = "BatchMake - CreateProcess failed!";
    return;
  }

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


    if (WaitForSingleObject( pi.hProcess, 500 )  == 0)
      m_run = false;
  } 

  CloseHandle(output);
  CloseHandle(error);

  //Terminate Process
  TerminateProcess(pi.hProcess,0);
  
  CloseHandle( pi.hProcess );
  CloseHandle( pi.hThread );

  std::ifstream m_file;
  char* data = (char*)malloc(1000);
  m_file.open("output.tmp",std::ifstream::binary);
  while (m_file.getline(data,1000))
  {
    data[strlen(data)-1] = '\n';
    m_output += MString(data);
  }
  m_file.close();

  data = (char*)malloc(1000);
  std::ifstream m_file2;
  m_file2.open("error.tmp",std::ifstream::binary);
  while (m_file2.getline(data,1000))
  {
    data[strlen(data)-1] = '\n';
    m_error += MString(data);
  }
  m_file2.close();

  unlink("output.tmp");
  unlink("error.tmp");
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
      if (m_param.length() != 0)
        m_param = m_param + 1;
      execlp(m_prog.toChar(),m_prog.toChar(),m_param.toChar(),NULL);   
      exit(EXIT_FAILURE);
    } 
    else   
    { 
      // This is the parent
      close(stdin_pipe[0]);
      close(stdin_pipe[1]);
      close(stderr_pipe[1]);
      while(1)   
      {       
        data_processed_err = read(stderr_pipe[0],buffer_err,BUFSIZ);
        for (unsigned int k=0;k<strlen(buffer_err);k++)
          m_error += buffer_err[k];
       
         memset(buffer_err,'\0',sizeof(buffer));
        if (data_processed_err == 0) break;
      }
      close(stderr_pipe[0]);
      close(stdout_pipe[1]);  
 
     while(1)
     {
       data_processed = read(stdout_pipe[0],buffer,BUFSIZ);
       for (unsigned int k=0;k<strlen(buffer);k++)
          m_output += buffer[k];
       
       memset(buffer,'\0',sizeof(buffer));
       if (data_processed == 0) break;
     }
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
