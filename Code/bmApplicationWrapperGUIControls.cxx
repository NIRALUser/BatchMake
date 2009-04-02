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
#include <itksys/Directory.hxx>
#include <itksys/SystemTools.hxx>

#include "MString.h"

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
  g_inputData->value(0);
  g_outputData->value(0);
  g_moduleversion->value("1.0");
  g_modulerequirements->value("");
  m_Applicationwrapper = new ApplicationWrapper();

  m_Currentfilename="";

  m_WrappedApplicationsPath = "/Applications";
}

ApplicationWrapperGUIControls::~ApplicationWrapperGUIControls()
{
}

void ApplicationWrapperGUIControls::SetApplicationListGUIControls(ApplicationListGUIControls* applicationlistguicontrols)
{
  m_Applicationlistguicontrols = applicationlistguicontrols;
}

void ApplicationWrapperGUIControls::SetWrappedApplicationsPath(MString applicationpath)
{  
  m_WrappedApplicationsPath = applicationpath;
}

void ApplicationWrapperGUIControls::SetApplicationWrapper(ApplicationWrapper* applicationwrapper)
{
  m_Applicationwrapper = applicationwrapper;
  // @todo are you sure you want to remove spaces ? 
  m_Currentfilename = MString(m_Applicationwrapper->GetName()).removeChar(' ');
}

void ApplicationWrapperGUIControls::Show()
{
  if(!g_Applicationwrappergui->shown())
    {
    g_Applicationwrappergui->show();
    }
}

void ApplicationWrapperGUIControls::OnSelectPath()
{
  char* fName = fl_file_chooser(
    "Select program", "Program (*.*)", NULL); //Fl_File_Chooser::MULTI);

  if(fName)
    {
    g_path->value(fName);
    // Try to parse the commande line arguments automatically
    // this requires the current program to support the option
    // -vxml
    this->AutomaticCommandLineParsing();
    }

 
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
    g_inputData->value(0);
    g_outputData->value(0);
    g_enumlist->clear();
    g_enumlist->add("New...");
    g_enumlist->redraw();
    }
  else
    {
	ApplicationWrapperParamSub* m_param;
	ApplicationWrapperParam* m_parentParam;
	bool is_parent = false;
	int parent = 0;
	//int parentValue = 0;
    g_add->label("Change");
    g_delete->show();

	for(unsigned int i=0 ; i<m_Applicationwrapper->GetParams().size() ; i++)
	  {
	  if(MString(g_parameters->text()) == m_Applicationwrapper->GetParams()[i].GetName())
	    {
		is_parent = true;
	    }
	  }

	if(is_parent)
	  {
	  m_param = m_Applicationwrapper->GetParam( g_parameters->text() );
	  }
	else
	  {
	  std::string childrenName = "";
	  bool found = false;
	  for(unsigned int i=0 ; i<m_Applicationwrapper->GetParams().size() ; i++)
	    {
	    for(unsigned int j=0 ; j<m_Applicationwrapper->GetParams()[i].GetParamsSub().size() ; j++)
	      {
		  std::string name = m_Applicationwrapper->GetParams()[i].GetName().toChar();
		  name += ".";
		  childrenName = m_Applicationwrapper->GetParams()[i].GetParamsSub()[j].GetName().toChar();
		  name += childrenName;
		  if(MString(g_parameters->text()) == name)
	        {
			parent = i;
			found = true;
			break;
	        }
	      }
		if(found)
			break;
	    }
	  m_parentParam = m_Applicationwrapper->GetParam(parent);
	  m_param = m_parentParam->GetParamSub(childrenName.c_str());
	  }

    if (m_param != 0)
      {
      g_name->value(m_param->GetName().latin1());
      if (m_param->GetType() != 5)
        g_value->value(m_param->GetValue().latin1());
      else
        g_value->value("");

      g_type->value(m_param->GetType());
	  if(is_parent)
		{
		g_parent->value(0);
		}
	  else
		{
		for(int i=0 ; i<g_parent->size() ; i++)
		  {
		  if(strcmp(m_Applicationwrapper->GetParams()[parent].GetName().toChar(), g_parent->text(i)) == 0)
		    {
			g_parent->value(i);
			break;
		    }
		  }
		}
      g_optional->value(m_param->GetOptional());
      // input data is 1 and output data is 2
      if(m_param->GetExternalData()==1)
        {
        g_inputData->value(true);
        }
      else
        {
        g_inputData->value(false);
        }
      if(m_param->GetExternalData()==2)
        {
        g_outputData->value(true);
        }
      else
        {
        g_outputData->value(false);
        }

      g_enumlist->clear();
      g_enumlist->add("New...");
      for (unsigned int i=0;i<m_param->GetEnum().size();i++)
        g_enumlist->add(m_param->GetEnum()[i].latin1());

      g_enumlist->value(m_param->GetValue().toInt());
      m_Currentname = m_param->GetName();
    }
  }

  OnSelectType();
}

void ApplicationWrapperGUIControls::OnAddParameters()
{
  ApplicationWrapperParam m_param;
  ApplicationWrapperParamSub m_paramSub;

  if (g_parameters->value() == 0)
    {
    if (g_type->value() == 1) //flag
      {
      g_parent->add(g_name->value());
	  }
   
    //Add Param
    m_param.SetName(MString(g_name->value()));
    m_param.SetType(g_type->value()); //ApplicationWrapperParam::Float
    m_param.SetValue(MString(g_value->value()));
    m_param.SetOptional(MString(g_optional->value()).toBool());
    if(g_inputData->value()>0)
      {
      m_param.SetExternalData(1);
      }
    else if(g_outputData->value()>0)
      {
      m_param.SetExternalData(2);
      }
    else
      {
      m_param.SetExternalData(0);
      }

    std::vector<MString> m_list;
    if (g_type->value() == 5)
      {
      for (int i=1;i<g_enumlist->size()-1;i++)
        {
        m_list.push_back(MString(g_enumlist->text(i)));
        }
      m_param.SetValue(MString((int)g_enumlist->value()));
      }
    m_param.SetEnum(m_list);

    if(g_parent->value() == 0)
	  {
	  m_Applicationwrapper->AddParam(m_param);
	  }
	else
	  {
	  for(unsigned int i=0 ; i<m_Applicationwrapper->GetParams().size() ; i++)
	    {
		if(strcmp(m_Applicationwrapper->GetParams()[i].GetName().toChar(), g_parent->text(g_parent->value())) == 0)
		  {
		  m_Applicationwrapper->GetParam(i)->AddParamSub(m_param);
		  break;
		  }
	    }
	  
	  }
    }
  else
    {
    ApplicationWrapperParam* m_param = m_Applicationwrapper->GetParam( m_Currentname.GetConstRefValue() );
    g_parameters->replace(g_parameters->value(),g_name->value());

    unsigned int m_offset = 0;
    for (int i=1 ; i<g_parent->size()-1 ; i++)
	  {
      if (m_Currentname == g_parent->text(i))
	    {
        m_offset = i;
	    }
	  }
	

    if (g_type->value() == 1) //flag
	  {
	  if (m_offset == 0)
	    {
        g_parent->add(g_name->value());
	    }
      else
	    {
        g_parent->replace(m_offset,g_name->value());
	    }
	  }
    else
      {
      if (m_offset != 0)
	    {
        g_parent->remove(m_offset);
	    }
      }

	if(!m_param)
	  {
	  ApplicationWrapperParamSub* m_paramSub = m_Applicationwrapper->GetParam( m_Currentname.GetConstRefValue() );
	  for(unsigned int i=0 ; i<m_Applicationwrapper->GetParams().size() ; i++)
	    {
	    m_paramSub = m_Applicationwrapper->GetParam(i)->GetParamSub(m_Currentname);
	    if(m_paramSub)
	      {
		    break;
	      }
	    }

	  m_Applicationwrapper->DeleteParam( m_Currentname.GetConstRefValue() );
	  MString test = g_name->value();
	  m_paramSub->SetName(test);
	  m_paramSub->SetType(g_type->value());
      m_paramSub->SetValue(MString(g_value->value()));
      m_paramSub->SetOptional(MString(g_optional->value()).toBool());

	  // input data is 1 and output data is 2
      if(g_inputData->value() > 0)
        {
        m_paramSub->SetExternalData(1);
        }
      else if(g_outputData->value() > 0)
        {
        m_paramSub->SetExternalData(2);
        }
      else
        {
        m_paramSub->SetExternalData(0);
        }

	  std::vector<MString> m_list;
      if (g_type->value() == 5)
        {
        for (int i=1;i<g_enumlist->size()-1;i++)
          {
          m_list.push_back(MString(g_enumlist->text(i)));
          }
        m_paramSub->SetValue(MString((int)g_enumlist->value()));
        }
      m_paramSub->SetEnum(m_list);
	  
	  if(g_parent->value() != 0)
	    {
	    m_Applicationwrapper->GetParam(g_parent->text(g_parent->value()))->AddParamSub(*m_paramSub);
	    }
	  else
	    {
		ApplicationWrapperParam m_newParam ;
		m_newParam.SetName(m_paramSub->GetName());
		m_newParam.SetEnum(m_paramSub->GetEnum());
		m_newParam.SetExternalData(m_paramSub->GetExternalData());
		m_newParam.SetOptional(m_paramSub->GetOptional());
		m_newParam.SetValue(m_paramSub->GetValue());
		m_newParam.SetType(m_paramSub->GetType());
		m_newParam.SetValueDefined(m_paramSub->IsValueDefined());
		m_Applicationwrapper->AddParam(m_newParam);
	    }
	  }

	else
	  {
      m_param->SetName(MString(g_name->value()));
      m_param->SetType(g_type->value());
      m_param->SetValue(MString(g_value->value()));
      m_param->SetOptional(MString(g_optional->value()).toBool());

      // input data is 1 and output data is 2
      if(g_inputData->value() > 0)
        {
        m_param->SetExternalData(1);
        }
      else if(g_outputData->value() > 0)
        {
        m_param->SetExternalData(2);
        }
      else
        {
        m_param->SetExternalData(0);
        }

      std::vector<MString> m_list;
      if (g_type->value() == 5)
        {
        for (int i=1;i<g_enumlist->size()-1;i++)
          {
          m_list.push_back(MString(g_enumlist->text(i)));
          }
        m_param->SetValue(MString((int)g_enumlist->value()));
        }
      m_param->SetEnum(m_list);

	  if(m_param->GetParamsSubSize() != 0 && g_parent->value() != 0)
        {
		fl_alert("This parameter cannot be move as a child.");
		return;
	    }
	  }
	}
  
  m_Applicationwrapper->SetApplicationPath( g_path->value() );
  m_Applicationwrapper->SetName( g_modulename->value() );
  m_Applicationwrapper->SetVersion( g_moduleversion->value() );
  m_Applicationwrapper->SetRequirements( g_modulerequirements->value() );

  DisplayExample();

  this->Refresh();
  g_parameters->value(0);
  g_parameters->redraw();
  OnSelectParameters();
}


void ApplicationWrapperGUIControls::OnDeleteParameters()
{
  unsigned int m_offset = 0;
  for (int i=1 ; i<g_parent->size()-1 ; i++)
    {
    if (m_Currentname == g_parent->text(i))
	  {
      m_offset = i;
	  }
    }

  if (g_type->value() == 1)
    {
    if (m_offset != 0)
	  {
      g_parent->remove(m_offset);
      }
    }

  m_Applicationwrapper->SetApplicationPath( g_path->value() );
  m_Applicationwrapper->SetName( g_modulename->value() );
  m_Applicationwrapper->SetVersion( g_moduleversion->value() );
  m_Applicationwrapper->SetRequirements( g_modulerequirements->value() );

  const int size = g_parameters->size();
  for(int i=0 ; i<size ; i++)
    {
    g_parameters->remove(g_parameters->value());
    }
  m_Applicationwrapper->DeleteParam( m_Currentname.GetConstRefValue() );
  this->Refresh();
  DisplayExample();
  g_parameters->value(0);
  g_parameters->redraw();
  OnSelectParameters();
}


void ApplicationWrapperGUIControls::OnSaveModule()
{
  m_Applicationwrapper->SetApplicationPath( g_path->value() );
  m_Applicationwrapper->SetName( g_modulename->value() );
  m_Applicationwrapper->SetVersion( g_moduleversion->value() );
  m_Applicationwrapper->SetRequirements( g_modulerequirements->value() );

  if (m_Applicationwrapper->GetName() == "")
    {
    fl_alert("Please set a module name!");
    return;
    }

  // Check if the Applications directory exists
  itksys::Directory directory;
  std::string dirpath = m_WrappedApplicationsPath.toChar();
  dirpath += "/";
  if(!directory.Load(dirpath.c_str()))
    {
    int createdir = fl_ask("The Applications directory doesn't exist. Create it?");
    if(createdir)
      {
      itksys::SystemTools::MakeDirectory(dirpath.c_str());
      }
    else
      {
      return;
      }
    }


  if ( MString(m_Applicationwrapper->GetName()).removeChar(' ') == m_Currentfilename)
    {
    std::string filename = m_WrappedApplicationsPath.GetConstRefValue();
    filename += "/";
    filename += MString( m_Applicationwrapper->GetName() )
      .removeChar(' ').GetConstRefValue();
    filename += ".bmm";
    m_Applicationwrapper->Save( filename );
    g_Applicationwrappergui->hide();
     if (m_Currentfilename == "")
       {
       m_Applicationlistguicontrols->UpdateNew(m_Applicationwrapper);
       }
     else
       {
       m_Applicationlistguicontrols->Update(m_Applicationwrapper->GetName());
       }
    m_Applicationlistguicontrols->g_Applicationlistgui->show();
    }
  else
    {
    std::string filename = m_WrappedApplicationsPath.GetConstRefValue();
    filename += "/";
    filename += MString( m_Applicationwrapper->GetName() )
      .removeChar(' ').GetConstRefValue();
    filename += ".bmm";
    FILE* m_file = fopen(filename.c_str(),"rb");
    if (m_file == 0)
      {
      m_Applicationwrapper->Save(filename);
      itksys::SystemTools::RemoveFile((m_WrappedApplicationsPath + "/" + m_Currentfilename + ".bmm").toChar());
      g_Applicationwrappergui->hide();
      if (m_Currentfilename == "")
        {
        m_Applicationlistguicontrols->UpdateNew(m_Applicationwrapper);
        }
      else
        {
        m_Applicationlistguicontrols->Update(m_Applicationwrapper->GetName());
        }
      m_Applicationlistguicontrols->g_Applicationlistgui->show();
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
  g_path->value( m_Applicationwrapper->GetApplicationPath().c_str() );
  g_modulename->value(m_Applicationwrapper->GetName().c_str());
  g_moduleversion->value(m_Applicationwrapper->GetVersion().c_str());
  g_modulerequirements->value(m_Applicationwrapper->GetRequirements().c_str());
  
  for (unsigned int i=0 ; i<m_Applicationwrapper->GetParams().size() ; i++)
    {
    g_parameters->add(m_Applicationwrapper->GetParams()[i].GetName().toChar());
	if(m_Applicationwrapper->GetParams()[i].GetType() == 1)
      g_parent->add(m_Applicationwrapper->GetParams()[i].GetName().toChar());

	for (unsigned int j=0 ; j<m_Applicationwrapper->GetParams()[i].GetParamsSub().size() ; j++)
	  {
	  std::string name = "";
	  std::string childrenName = m_Applicationwrapper->GetParams()[i].GetParamsSub()[j].GetName().toChar();
	  string::size_type loc = childrenName.find( ".", 0 );
	  if( loc == string::npos )
	    {
		name = m_Applicationwrapper->GetParams()[i].GetName().toChar();
	    name += ".";
		name += m_Applicationwrapper->GetParams()[i].GetParamsSub()[j].GetName().toChar();
	    }
	  else
	    {
	    name = childrenName;
	    }
	  g_parameters->add(name.c_str());
	  }
    }

  DisplayExample();  
  //OnSelectParameters();
}

void ApplicationWrapperGUIControls::OnLoadModule()
{
  char* fName = fl_file_chooser(
    "Load module", "Image Files (*.bmm)", NULL);
 
  if(fName)
  {
    m_Applicationwrapper->Load( fName );
    g_path->value(m_Applicationwrapper->GetApplicationPath().c_str());
    g_modulename->value(m_Applicationwrapper->GetName().c_str());
    g_moduleversion->value(m_Applicationwrapper->GetVersion().c_str());
    g_modulerequirements->value(m_Applicationwrapper->GetRequirements().c_str());
    
    for (unsigned int i=0;i<m_Applicationwrapper->GetParams().size();i++)
    {
      g_parameters->add(m_Applicationwrapper->GetParams()[i].GetName().toChar());
      if (m_Applicationwrapper->GetParams()[i].GetType() == 1) //flag
        g_parent->add(m_Applicationwrapper->GetParams()[i].GetName().toChar());
    }

    DisplayExample();  
    OnSelectParameters();
  }
}


void ApplicationWrapperGUIControls::DisplayExample()
{
  Fl_Text_Buffer* m_buffer = new Fl_Text_Buffer();
  g_commandline->buffer(m_buffer);
  //int lenght = m_Applicationwrapper->GetExampleLine().length();
  std::string Lines = m_Applicationwrapper->GetExampleLine().toChar();

  /*if(lenght >260)
    {
    Fl_Text_Buffer* m_buffer2 = new Fl_Text_Buffer();
	Fl_Text_Buffer* m_buffer3 = new Fl_Text_Buffer();
	Fl_Text_Buffer* m_buffer4 = new Fl_Text_Buffer();
    g_commandline2->buffer(m_buffer2);
	g_commandline3->buffer(m_buffer3);
	g_commandline4->buffer(m_buffer4);

	std::string line4 = Lines.substr(255);
	std::string line3 = Lines.substr(170, 85);
	std::string line2 = Lines.substr(85,85);
	std::string line1 = Lines.substr(0,85);

	m_buffer->text( line1.c_str() );
	m_buffer2->text( line2.c_str() );
	m_buffer3->text( line3.c_str() );
	m_buffer4->text( line4.c_str() );
    }
  else if(lenght >170)
    {
    Fl_Text_Buffer* m_buffer2 = new Fl_Text_Buffer();
	Fl_Text_Buffer* m_buffer3 = new Fl_Text_Buffer();
    g_commandline2->buffer(m_buffer2);
	g_commandline3->buffer(m_buffer3);
	std::string line3 = Lines.substr(170);
	std::string line2 = Lines.substr(85,85);
	std::string line1 = Lines.substr(0,85);

	m_buffer->text( line1.c_str() );
	m_buffer2->text( line2.c_str() );
	m_buffer3->text( line3.c_str() );
    }
  else if(lenght >90)
    {
	Fl_Text_Buffer* m_buffer2 = new Fl_Text_Buffer();
	g_commandline2->buffer(m_buffer2);
	std::string line2 = Lines.substr(85);
	std::string line1 = Lines.substr(0,85);

	m_buffer->text( line1.c_str() );
	m_buffer2->text( line2.c_str() );
    }
  else
  {*/
  m_buffer->text( m_Applicationwrapper->GetExampleLine().toChar());
  //}
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
    m_Applicationwrapper->DownParam(m_Currentname.GetConstRefValue());
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
    m_Applicationwrapper->UpParam(m_Currentname.GetConstRefValue());
    g_parameters->value(m_offset-1);
  }
  DisplayExample();
}

void ApplicationWrapperGUIControls::OnReject()
{
  g_Applicationwrappergui->hide();
  m_Applicationlistguicontrols->g_Applicationlistgui->show();
}

/** Automatic command line parsing. If the current pointed program 
 *  supports -vxml option or --xml */
void ApplicationWrapperGUIControls::AutomaticCommandLineParsing()
{
  std::string program = g_path->value();
  bool appplicationWrapped = false;

  // Run the application
  if(g_runapp->value())
    {
    appplicationWrapped = m_Applicationwrapper->AutomaticCommandLineParsing( program.c_str() );
    }

  // If not previously wrapped
  if(!appplicationWrapped && g_runappxml->value())
    {
    m_Applicationwrapper->AutomaticCommandLineParsingSlicer( program.c_str() );
    }

  g_modulerequirements->value(m_Applicationwrapper->GetRequirements().c_str());
  g_moduleversion->value(m_Applicationwrapper->GetVersion().c_str());
  g_modulename->value(m_Applicationwrapper->GetName().c_str());
  
  this->DisplayExample();
}

} // end namespace bm
