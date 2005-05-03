/*=========================================================================

  Program:   BatchMake
  Module:    bmApplicationWrapperGUIControls.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmApplicationWrapperGUIControls.h"
#include <iostream>
#include <FL/Fl_File_Chooser.H>
#include "ApplicationWrapperParam.h"
#include "metaCommand.h"

#include "MString.h"

#ifndef WIN32
  #include <unistd.h>
#endif

namespace bm {

ApplicationWrapperGUIControls::ApplicationWrapperGUIControls():ApplicationWrapperGUI()
{
    make_window();
    g_type->add("filepath");
    g_type->add("flag");
    g_type->add("int");
    g_type->add("float");
    g_type->add("string");
    g_type->add("enum");
    g_type->value(0);

    g_parent->add("no parent");
    g_parent->value(0);

    g_enumlist->add("New...");
    g_enumlist->value(0);
    g_addenum->hide();
    g_delenum->hide();
    g_enumlist->hide();

    g_parameters->add("New...");
    g_parameters->value(0);
    g_add->label("Add");
    g_delete->hide();

    g_optional->value(0);
    g_moduleversion->value("1.0");
    m_applicationwrapper = new ApplicationWrapper();

    m_currentfilename="";
}

ApplicationWrapperGUIControls::~ApplicationWrapperGUIControls()
{
}

void ApplicationWrapperGUIControls::SetApplicationListGUIControls(ApplicationListGUIControls* applicationlistguicontrols)
{
  m_applicationlistguicontrols = applicationlistguicontrols;
}

void ApplicationWrapperGUIControls::SetApplicationPath(MString applicationpath)
{
  m_applicationpath = applicationpath;
}

void ApplicationWrapperGUIControls::SetApplicationWrapper(ApplicationWrapper* applicationwrapper)
{
  m_applicationwrapper = applicationwrapper;
  m_currentfilename = m_applicationwrapper->GetName().removeChar(' ');
}

void ApplicationWrapperGUIControls::Show()
{
  if(!g_Applicationwrappergui->shown())
    g_Applicationwrappergui->show();
}

void ApplicationWrapperGUIControls::OnSelectPath()
{
  char* fName = fl_file_chooser(
    "Select program", "Program (*.*)", NULL,Fl_File_Chooser::MULTI);
 
  if(fName)
    {
    g_path->value(fName);
    }

  // Try to parse the commande line arguments automatically
  // this requires the current program to support the option
  // -vxml
  this->AutomaticCommandLineParsing();
}

void ApplicationWrapperGUIControls::OnSelectType()
{
  switch(g_type->value())
  {
    //enum
    case 1: g_value->label("Flag:");
            g_addenum->hide();
            g_enumlist->hide();
            break;

    //enum
    case 5: g_value->label("Value:");
            g_addenum->show();
            g_enumlist->show();
            break;

    default: g_value->label("Default value:");
             g_addenum->hide();
             g_enumlist->hide();
  }

  g_Applicationwrappergui->redraw();
}

void ApplicationWrapperGUIControls::OnSelectEnumList()
{
  if (g_enumlist->value() != 0)
  {
    g_value->value(g_enumlist->text());
    g_addenum->label("Change");
  }
  else
  {
    g_value->value("");
    g_addenum->label("Add");
  }

  g_value->redraw();
}

void ApplicationWrapperGUIControls::OnAddEnum()
{
  if (g_enumlist->value() == 0)
  {
    g_enumlist->add(g_value->value());
  }
  else
  {
    g_enumlist->replace(g_enumlist->value(),g_value->value());
  }

  g_enumlist->value(0);
  g_enumlist->redraw();
  OnSelectEnumList();
}


void ApplicationWrapperGUIControls::OnSelectParameters()
{
  if (g_parameters->value() == 0)
  {
    g_add->label("Add");
    g_delete->hide();
    g_type->value(0);
    g_parent->value(0);
    g_value->value("");
    g_name->value("");
    g_optional->value(0);
    g_enumlist->clear();
    g_enumlist->add("New...");
    g_enumlist->redraw();
  }
  else
  {
    g_add->label("Change");
    g_delete->show();
    ApplicationWrapperParam* m_param = m_applicationwrapper->GetParam(MString(g_parameters->text()));
    if (m_param != 0)
    {
      g_name->value(m_param->GetName().latin1());
      if (m_param->GetType() != 5)
        g_value->value(m_param->GetValue().latin1());
      else
        g_value->value("");

      g_type->value(m_param->GetType());
      g_parent->value(m_param->GetParent());
      g_optional->value(m_param->GetOptional());
      g_enumlist->clear();
      g_enumlist->add("New...");
      for (unsigned int i=0;i<m_param->GetEnum().size();i++)
        g_enumlist->add(m_param->GetEnum()[i].latin1());

      g_enumlist->value(m_param->GetValue().toInt());
      m_currentname = m_param->GetName();
    }
  }
  OnSelectType();
}

void ApplicationWrapperGUIControls::OnAddParameters()
{
  if (g_parameters->value() == 0)
  {
    g_parameters->add(g_name->value());
    if (g_type->value() == 1) //flag
      g_parent->add(g_name->value());
   
    //Add Param
    ApplicationWrapperParam m_param;
    m_param.SetName(MString(g_name->value()));
    m_param.SetType(g_type->value()); //ApplicationWrapperParam::Float
    m_param.SetValue(MString(g_value->value()));
    m_param.SetParent(g_parent->value());
    m_param.SetOptional(MString(g_optional->value()).toBool());
   
    std::vector<MString> m_list;
    if (g_type->value() == 5)
    {

      for (unsigned int i=1;i<g_enumlist->size()-1;i++)
      {
        m_list.push_back(MString(g_enumlist->text(i)));
      }
      m_param.SetValue(MString((int)g_enumlist->value()));
    }
    m_param.SetEnum(m_list);
    m_applicationwrapper->AddParam(m_param);
  }
  else
  {
    ApplicationWrapperParam* m_param = m_applicationwrapper->GetParam(m_currentname); 
    g_parameters->replace(g_parameters->value(),g_name->value());
    
    unsigned int m_offset = 0;
    for (unsigned int i=1;i<g_parent->size()-1;i++)
      if (m_currentname == g_parent->text(i))
        m_offset = i;
    
    if (g_type->value() == 1) //flag
      if (m_offset == 0)
        g_parent->add(g_name->value());
      else
        g_parent->replace(m_offset,g_name->value());

    else
    {
      if (m_offset != 0)
        g_parent->remove(m_offset);
    }

    m_param->SetName(MString(g_name->value()));
    m_param->SetType(g_type->value());
    m_param->SetValue(MString(g_value->value()));
    m_param->SetParent(g_parent->value());
    m_param->SetOptional(MString(g_optional->value()).toBool());

    std::vector<MString> m_list;
    if (g_type->value() == 5)
    {
      for (unsigned int i=1;i<g_enumlist->size()-1;i++)
      {
        m_list.push_back(MString(g_enumlist->text(i)));
      }
      m_param->SetValue(MString((int)g_enumlist->value()));
    }
    m_param->SetEnum(m_list);
  }

 
  m_applicationwrapper->SetApplicationPath(MString(g_path->value()));
  m_applicationwrapper->SetName(MString(g_modulename->value()));
  m_applicationwrapper->SetVersion(MString(g_moduleversion->value()));

  DisplayExample();

  g_parameters->value(0);
  g_parameters->redraw();
  OnSelectParameters();
}


void ApplicationWrapperGUIControls::OnDeleteParameters()
{
  m_applicationwrapper->DeleteParam(m_currentname);
  g_parameters->remove(g_parameters->value());
  unsigned int m_offset = 0;
  for (unsigned int i=1;i<g_parent->size()-1;i++)
    if (m_currentname == g_parent->text(i))
      m_offset = i;
  
  if (g_type->value() == 1)
    if (m_offset != 0)
      g_parent->remove(m_offset);

  m_applicationwrapper->SetApplicationPath(MString(g_path->value()));
  m_applicationwrapper->SetName(MString(g_modulename->value()));
  m_applicationwrapper->SetVersion(MString(g_moduleversion->value()));
  DisplayExample();
  g_parameters->value(0);
  g_parameters->redraw();
  OnSelectParameters();
}


void ApplicationWrapperGUIControls::OnSaveModule()
{
  m_applicationwrapper->SetApplicationPath(MString(g_path->value()));
  m_applicationwrapper->SetName(MString(g_modulename->value()));
  m_applicationwrapper->SetVersion(MString(g_moduleversion->value()));

  if (m_applicationwrapper->GetName() == "")
  {
    fl_alert("Please set a module name!");
    return;
  }

  if (m_applicationwrapper->GetName().removeChar(' ') == m_currentfilename)
  {
    m_applicationwrapper->Save(m_applicationpath + "/Applications/" + m_applicationwrapper->GetName().removeChar(' ') + ".bmm");
    g_Applicationwrappergui->hide();
     if (m_currentfilename == "")
        m_applicationlistguicontrols->UpdateNew(m_applicationwrapper);
      else
        m_applicationlistguicontrols->Update(m_applicationwrapper->GetName());
    m_applicationlistguicontrols->g_Applicationlistgui->show();
  }
  else
  {
    FILE* m_file = fopen((m_applicationpath + "/Applications/" + m_applicationwrapper->GetName().removeChar(' ') + ".bmm").toChar(),"rb");
    if (m_file == 0)
    {
      m_applicationwrapper->Save(m_applicationpath + "/Applications/" + m_applicationwrapper->GetName().removeChar(' ') + ".bmm");
      unlink((m_applicationpath + "/Applications/" + m_currentfilename + ".bmm").toChar());
      g_Applicationwrappergui->hide();
      if (m_currentfilename == "")
        m_applicationlistguicontrols->UpdateNew(m_applicationwrapper);
      else
        m_applicationlistguicontrols->Update(m_applicationwrapper->GetName());

      m_applicationlistguicontrols->g_Applicationlistgui->show();
    }
    else
    {
      fl_alert("Module name already exists!\nPlease modify module name.");
      fclose(m_file);
    }
  }
}

void ApplicationWrapperGUIControls::Refresh()
{
  g_path->value(m_applicationwrapper->GetApplicationPath().toChar());
  g_modulename->value(m_applicationwrapper->GetName().toChar());
  g_moduleversion->value(m_applicationwrapper->GetVersion().toChar());
  
  for (unsigned int i=0;i<m_applicationwrapper->GetParams().size();i++)
  {
    g_parameters->add(m_applicationwrapper->GetParams()[i].GetName().toChar());
    if (m_applicationwrapper->GetParams()[i].GetType() == 1) //flag
      g_parent->add(m_applicationwrapper->GetParams()[i].GetName().toChar());
  }

  DisplayExample();  
  OnSelectParameters();
}

void ApplicationWrapperGUIControls::OnLoadModule()
{
  char* fName = fl_file_chooser(
    "Load module", "Image Files (*.bmm)", NULL);
 
  if(fName)
  {
    m_applicationwrapper->Load(MString(fName));
    g_path->value(m_applicationwrapper->GetApplicationPath().toChar());
    g_modulename->value(m_applicationwrapper->GetName().toChar());
    g_moduleversion->value(m_applicationwrapper->GetVersion().toChar());
    
    for (unsigned int i=0;i<m_applicationwrapper->GetParams().size();i++)
    {
      g_parameters->add(m_applicationwrapper->GetParams()[i].GetName().toChar());
      if (m_applicationwrapper->GetParams()[i].GetType() == 1) //flag
        g_parent->add(m_applicationwrapper->GetParams()[i].GetName().toChar());
    }

    DisplayExample();  
    OnSelectParameters();
  }
}


void ApplicationWrapperGUIControls::DisplayExample()
{
  Fl_Text_Buffer* m_buffer = new Fl_Text_Buffer();
  g_commandline->buffer(m_buffer);
  m_buffer->text( m_applicationwrapper->GetExampleLine().toChar());
}


void ApplicationWrapperGUIControls::OnDownParam()
{
  int m_offset = g_parameters->value();
  if (m_offset != g_parameters->size()-2)
  {
    char m_tempname[200];
    strcpy(m_tempname,g_parameters->text(m_offset+1));
    g_parameters->replace(m_offset+1,g_parameters->text(m_offset));
    g_parameters->replace(m_offset,m_tempname);
    m_applicationwrapper->DownParam(m_currentname);
    g_parameters->value(m_offset+1);
  }
  DisplayExample();
}

void ApplicationWrapperGUIControls::OnUpParam()
{
  int m_offset = g_parameters->value();
  if (m_offset > 1)
  {
    char m_tempname[200];
    strcpy(m_tempname,g_parameters->text(m_offset-1));
    g_parameters->replace(m_offset-1,g_parameters->text(m_offset));
    g_parameters->replace(m_offset,m_tempname);
    m_applicationwrapper->UpParam(m_currentname);
    g_parameters->value(m_offset-1);
  }
  DisplayExample();
}

void ApplicationWrapperGUIControls::OnReject()
{
  g_Applicationwrappergui->hide();
  m_applicationlistguicontrols->g_Applicationlistgui->show();
}

/** Automatic command line parsing. If the current pointed program 
 *  supports -vxml option */
void ApplicationWrapperGUIControls::AutomaticCommandLineParsing()
{

  // Run the application
  std::string program = g_path->value();
  program += " -vxml";
  std::cout << "Running = " << program.c_str() << std::endl;
  std::string m_output = "";

#ifdef WIN32

  char buffer[BUFSIZ+1];

  STARTUPINFO si;
  PROCESS_INFORMATION pi;
 
  SECURITY_ATTRIBUTES tmpSec;
  ZeroMemory( &tmpSec, sizeof(tmpSec) );
  tmpSec.nLength = sizeof(tmpSec);
  tmpSec.bInheritHandle = true;
  HANDLE hReadPipe;
  HANDLE hWritePipe;

  SECURITY_ATTRIBUTES  sa;
  ZeroMemory( &sa, sizeof(sa) );
  sa.nLength = sizeof(sa);
  sa.bInheritHandle = true;


  SECURITY_ATTRIBUTES  sa2;
  ZeroMemory( &sa2, sizeof(sa2) );
  sa2.nLength = sizeof(sa);
  sa2.bInheritHandle = true;

  CreatePipe(&hReadPipe,&hWritePipe,&sa,0);

  ZeroMemory( &si, sizeof(si) );
  si.cb = sizeof(si);
  si.dwFlags = STARTF_USESTDHANDLES;
  si.hStdOutput = hWritePipe; //output;
  ZeroMemory( &pi, sizeof(pi) );

  memset(buffer,'\0',sizeof(buffer)); 
  
  // Start the child process. 
  if( !CreateProcess( NULL,       // No module name (use command line). 
      (char*)program.c_str(),  // Command line. 
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
    std::cout << "AutomaticCommandLineParsing - CreateProcess failed!" << std::endl;
    return;
    }

  CloseHandle(hWritePipe);

  // Wait until child process exits.
  bool m_run = true;

  unsigned int i=0;
  while(m_run)
  {
  unsigned long m_nbread = 0;
  unsigned long m_nberrorread = 0;
  unsigned long m_nbtoread = 512;
  unsigned long m_nbtoreaderror = 512;
  int m_read = 0;
  int m_readerror = 0;
  unsigned long m_nbreaded = 0;
  unsigned long m_nberrorreaded = 0;

  if (WaitForSingleObject( pi.hProcess, 500 )  == 0)
    {
    m_run = false;
    }

  PeekNamedPipe(hReadPipe,buffer,sizeof(buffer),&m_nbtoread,&m_nbread,NULL); 

  int val = ReadFile(hReadPipe,buffer,512,&m_nbreaded,NULL); 
  while (m_nbread > 0 && val)
    {
    for (unsigned int k=0;k<m_nbreaded;k++)
      {
      if (buffer[k] != '\r')
        {
        m_output += buffer[k];
        }
      }
    memset(buffer,'\0',sizeof(buffer));
    val = ReadFile(hReadPipe,buffer,512,&m_nbreaded,NULL);
    }

  } 

  //Terminate Process
  TerminateProcess(pi.hProcess,0);

  CloseHandle(hReadPipe);

  CloseHandle( pi.hProcess );
  CloseHandle( pi.hThread );

#else  
  int stdin_pipe[2];
  int stdout_pipe[2];
  char buffer[BUFSIZ+1];
  int fork_result;
  int data_processed;
  int nchars = 0;
int status = 0;

  memset(buffer,'\0',sizeof(buffer));

   if ( (pipe(stdin_pipe)==0)   
        && (pipe(stdout_pipe)==0)      )
   {
     fork_result = fork();
     if (fork_result == -1)
     {
       std::cerr << "Create Process failed (Pipe error) ! " << std::endl;   
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

      fcntl(stdout_pipe[1], F_SETFL, O_NONBLOCK);

    if (execlp(program.c_str(),program.c_str(),"",NULL) == -1)
    {         
  if (errno == 2)
  {
      std::cout << "Program not found : " << program  << std::endl;
   }
    }

      exit(EXIT_FAILURE);
    } 
    else   
    { 
      // This is the parent
      close(stdin_pipe[0]);
      close(stdin_pipe[1]);
      close(stdout_pipe[1]);  

      fcntl(stdout_pipe[0], F_SETFL, O_NONBLOCK);

      while(1)   
        {      
    
       data_processed = read(stdout_pipe[0],buffer,BUFSIZ);
       if (data_processed != -1)
       {
   for (unsigned int k=0;k<strlen(buffer);k++)
           m_output += buffer[k];
       

         memset(buffer,'\0',sizeof(buffer));
       }
      

       if ((data_processed == 0) ) break;
        }

     close(stdout_pipe[0]);
    }
  } 
#endif
  
  // Analayze the output of the program
  MetaCommand parser;
  parser.ParseXML(m_output.c_str());

  // Convert the metaCommand to ApplicationWrapper
  g_moduleversion->value(parser.GetVersion().c_str());
  m_applicationwrapper->SetVersion(parser.GetVersion().c_str());


  // extract the name from the filename
  std::string revname;
  
  for(i=0;i<g_path->size();i++)
    {
    if(g_path->value()[g_path->size()-1-i] == '/'
      || g_path->value()[g_path->size()-1-i] == '\\')
      {
      break;
      }
    revname += g_path->value()[g_path->size()-1-i];
    }

  std::string name;
  
  int end=0;
  if(revname.find("exe.") != -1)
    {
    end=4;
    }

  for(i=0;i<revname.size()-end;i++)
    {
    name += revname[revname.size()-i-1];
    }

  g_modulename->value(name.c_str());
  m_applicationwrapper->SetName(name.c_str());

  m_applicationwrapper->SetApplicationPath(g_path->value());

  // Now adding the parameters
  const MetaCommand::OptionVector & options = parser.GetOptions();
  MetaCommand::OptionVector::const_iterator it = options.begin();

  unsigned int parentId=0;
  while(it != options.end())
    {
    ApplicationWrapperParam parentParam;
    parentParam.SetName((*it).name);
    parentParam.SetOptional(!(*it).required);
    if((*it).tag != "") // we have one value
      {
      parentParam.SetType(ApplicationWrapperParam::Flag);
      std::string tag = "-";
      tag += (*it).tag;
      parentParam.SetValue(tag);
      parentParam.SetName((*it).name);
      m_applicationwrapper->AddParam(parentParam);
      parentId++;
      }
    else
      {
      parentId = 0;
      }
    
    std::vector<MetaCommand::Field>::const_iterator itField = (*it).fields.begin();
    while(itField != (*it).fields.end())
      {
      ApplicationWrapperParam param;
      param.SetName((*itField).name);
      param.SetValue((*itField).value);    
      if((*itField).type == MetaCommand::TypeEnumType::FLOAT)
        {
        param.SetType(ApplicationWrapperParam::Float);
        }
      else if((*itField).type == MetaCommand::TypeEnumType::INT)
        {
        param.SetType(ApplicationWrapperParam::Int);
        }
      else if((*itField).type == MetaCommand::TypeEnumType::STRING)
        {
        param.SetType(ApplicationWrapperParam::String);
        }
      else if((*itField).type == MetaCommand::TypeEnumType::FLAG)
        {
        itField++;
        continue;
        }
      
      param.SetOptional(!(*itField).required);
      param.SetParent(parentId);
      m_applicationwrapper->AddParam(param);
      itField++; 
      }
    
    it++;
    }

  this->DisplayExample();
}

} // end namespace bm
