/*=========================================================================

  Program:   BatchMake
  Module:    bmGrid.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#include "bmGrid.h"
#include <stdio.h>
#include <ctime>
#include <itksys/SystemTools.hxx>
#include <sstream>

namespace bm {

/** Constructor */
Grid::Grid()
{
  m_FileName = "";
  m_CurrentScopeFile = "";
  m_DataHost = "";
  m_OutputHost = "";
  m_Distributed = false; // by default everything is sequential
  m_SingleNode = false;
  m_SingleNodeTransition = false;
  m_DistributedTransition = false;
  m_MaxNodes = -1;
  m_Grouping = true;
  m_NextAppIsAfterEndBarrier = false;
  m_CurrentScope = 0;
  m_TransferFiles = ALL;
}

/** Destructor */
Grid::~Grid()
{
}

void Grid::SetFileName( const std::string& filename )
{
  m_FileName = filename;
}

const std::string& Grid::GetFileName( )const
{
  return m_FileName;
}

/** Return true if the current scope file exists */
bool Grid::HasCurrentScopeFile()
{
  if(m_CurrentScopeFile.size() == 0)
    {
    return false;
    }
  return true;
}

/** Return the current scope file */
const char* Grid::GetCurrentScopeFile()
{
  if( m_CurrentScopeFile.empty() )
    {
    std::stringstream ss;
    ss << time(NULL) << ".bms.tmp";
    m_CurrentScopeFile = ss.str();
    /*
    char* tempchar = new char[25];
    sprintf(tempchar,"%zu.bms.tmp",);
    m_CurrentScopeFile = tempchar;
    delete [] tempchar;
    */
    }
  return m_CurrentScopeFile.c_str();
}

/** Add quotes if necessary */
std::string Grid::AddQuotes(std::string value)
{
  std::string temp;
  // Remove the last space of the string
  if(value[value.size()-1] == ' ')
    {
    value.resize(value.size()-1);
    }

  if((int)value.find(' ') != -1)
    {
    temp = "'";
    temp += value;
    temp += "'";
    }
  else
    {
    temp = value;
    }

  return temp;
}

/** */
void Grid::SetGridBarrier()
{
  m_DistributedSyncBarrier.push_back(m_ApplicationsList.size()-1);
  m_CurrentScope++;
}

void Grid::SetRequirements( const std::string& requirements )
{
  m_Requirements = requirements;
}

const std::string& Grid::GetRequirements()const
{
  return m_Requirements;
}

/** Remove a grid barrier */
void Grid::SetDistributed(bool val)
{
  m_DistributedTransition = true;
  m_Distributed = val;
  if(m_Distributed)
    {
    // We add a virtual node
    ApplicationWrapper virtualApp;
    virtualApp.SetName("BatchMakeVirtualNode");
    this->AddApplication(&virtualApp);
    }
}

/** Remove a grid barrier */
void Grid::RemoveGridBarrier()
{
  m_DistributedSyncBarrier.pop_back();
  m_NextAppIsAfterEndBarrier = true;
  m_CurrentScope--;
}

/** Add an application to the list of applications to generate */
void Grid::AddApplication(ApplicationWrapper* app,const char* datadir,const char* outputdir)
{
  if(m_DataHost.size()>0)
    {
    app->SetDataHost(m_DataHost.c_str());
    }

  if(m_OutputHost.size()>0)
    {
    app->SetOutputHost(m_OutputHost.c_str());
    }

  if(m_SingleNodeTransition)
    {
   app->SetSingleNode(2);
    m_SingleNodeTransition = false;
    }
  else if(m_SingleNode)
    {
    app->SetSingleNode(1);
    }
  else
    {
    app->SetSingleNode(0);
    }

  if(datadir)
    {
    app->SetDataDirectory(datadir);
    }
  else
    {
    app->SetDataDirectory(m_InputDirectory.c_str());
    }

   if(outputdir)
    {
    app->SetOutputDirectory(outputdir);
    }
  else
    {
    app->SetOutputDirectory(m_OutputDirectory.c_str());
    }

  if(m_DistributedTransition && m_Distributed)
    {
    if(m_DistributedSyncBarrier.size()>0)
      {
      app->SetDependsOn(m_DistributedSyncBarrier[m_DistributedSyncBarrier.size()-1]);
      }
    else
      {
      app->SetDependsOn(-1);
      }
    m_DistributedTransition = false;
    }
  else
    {
    app->SetDependsOn(-1);
    }
  app->SetExecutionBlockNumber(m_CurrentScope);

  if(m_NextAppIsAfterEndBarrier)
    {
    app->SetAfterEndBarrier(true);
    m_NextAppIsAfterEndBarrier = false;
    }

  m_ApplicationsList.push_back(*app);
}

/** Return a filename for a full path */
std::string Grid::GetFilename(const char* fullpath)
{
  std::string fullname = fullpath;
  long int pos = fullname.find_last_of("/");
  if(pos != -1)
     {
     return fullname.substr(pos+1,fullname.size()-pos-1);
     }
  pos = fullname.find_last_of("\\");
  if(pos != -1)
     {
     return fullname.substr(pos+1,fullname.size()-pos-1);
     }
  return fullpath;
}

/** Write a kwgrid script */
void Grid::WriteGAD()
{
  if( m_FileName.empty() )
    {
    std::cout << "Grid::WriteGAD() : Please set a filename" << std::endl;
    return;
    }
  std::string filename;
  if( !m_WorkingDirectory.empty() )
    {
    filename = m_WorkingDirectory + "/" + m_FileName;
    }
  else
    {
    filename = m_FileName;
    }
  FILE* fic = fopen( filename.c_str(), "wb" );

  if(!fic)
    {
    std::cout << "Grid::WriteGAD() : Cannot create GAD script" << std::endl;
    return;
    }

  // Write the header
  fprintf(fic,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  fprintf(fic,"<gridApplication\n");
  fprintf(fic,"xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n");
  fprintf(fic,"xsi:noNamespaceSchemaLocation=\"grid-application-description.xsd\"\n");
  fprintf(fic,"name=\"BatchMake Script\"\n");
  fprintf(fic,"description=\"Script generated by BatchMake (c) Kitware Inc.\">\n");

  unsigned int appnum = 1;
  unsigned int inFile = 1;
  unsigned int outFile = 1;
  int applicationComponent = 1;

  int applicationChunckSize = 1;
  if(m_MaxNodes>0)
    {
    double chunckSize = m_ApplicationsList.size();
    chunckSize /= m_MaxNodes;
    applicationChunckSize = (int)chunckSize;
    }

  fprintf(fic,"<applicationComponent name=\"task%d\" remoteExecution=\"true\">\n",appnum);
  fprintf(fic,"<componentActionList>\n");

  // Add the applicationComponent
  std::vector<ApplicationWrapper>::iterator it = m_ApplicationsList.begin();
  while(it != m_ApplicationsList.end())
    {
    // Find the current dependency
    ApplicationWrapper* dependApp = NULL;
    std::string dependencyName = "";
    if((*it).GetDependsOn() == -1 && it!=m_ApplicationsList.begin())
      {
      std::vector<ApplicationWrapper>::iterator itprev = it;
      itprev--;
      dependApp = &(*itprev);
      }
    else if((*it).GetDependsOn() >= 0)
      {
      dependApp = &(m_ApplicationsList[(*it).GetDependsOn()]);
      }


    // If this is not the first application
    if(it != m_ApplicationsList.begin()
       && ((*it).GetSingleNode() == 2)
       && (applicationComponent>applicationChunckSize)
       )
      {
      applicationComponent=0;
      fprintf(fic,"</componentActionList>\n");
      fprintf(fic,"</applicationComponent>\n\n");
      fprintf(fic,"<applicationComponent name=\"task%d\" remoteExecution=\"true\">\n",appnum);
      fprintf(fic,"<componentActionList>\n");
      }

    std::vector<std::string> dependencies;

    const std::vector<ApplicationWrapperParam> & params = (*it).GetParams();
    std::vector<ApplicationWrapperParam>::const_iterator itParams = params.begin();

    // Check if we have external data
    // For now we skip bmGridStore
    if( strcmp((*it).GetName().c_str(),"bmGridStore")
        && strcmp((*it).GetName().c_str(),"bmGridSend"))
      {
      while(itParams != params.end())
        {
        if((*itParams).GetExternalData() == 1
           && (*itParams).GetValue().length() > 0
           && !(*it).GetDataHost().empty() ) // DATA_IN
          {
          fprintf(fic," <componentAction type=\"DataRelocation\" name=\"InputFile%d\">\n",inFile);
          fprintf(fic,"  <parameter name=\"Host\" value=\"%s\"/>\n",(*it).GetDataHost().c_str());
          fprintf(fic,"  <parameter name=\"Description\" value=\"\"/>\n");
          fprintf(fic,"  <parameter name=\"Direction\" value=\"In\"/>\n");
          fprintf(fic,"  <parameter name=\"Protocol\" value=\"gsiftp\"/>\n");
          fprintf(fic,"  <parameter name=\"SourceDataPath\" value=\"%s%s\"/>\n",(*it).GetDataDirectory().c_str(),
                  this->AddQuotes((*itParams).GetValue().toChar()).c_str());
          fprintf(fic,"  <parameter name=\"DestDataPath\" value=\"%s%s\"/>\n",m_GridTempDirectory.c_str(),
                  this->AddQuotes(this->GetFilename((*itParams).GetValue().toChar()).c_str()).c_str());

          if(dependApp)
            {
            fprintf(fic,"  <dependency name=\"%s\" status=\"done\"/>\n",dependApp->GetDependencyTag().c_str());
            }
          fprintf(fic," </componentAction>\n");
          char* dep = new char[255];
          sprintf(dep,"InputFile%d",inFile);
          (*it).SetDependencyTag(dep);
          dependencies.push_back(dep);
          delete [] dep;
          inFile++;
          }
        itParams++;
        }

      } // end bmGridStore

    char* temp = new char[10];
    sprintf(temp,"app%d",appnum);
    (*it).SetDependencyTag(temp);
    delete [] temp;

    fprintf(fic," <componentAction type=\"JobSubmission\" name=\"app%d\">\n",appnum);

    // We assume that the current application is in the path of the node
    std::string applicationName = (*it).GetApplicationPath();
    int pos = applicationName.find_last_of("/");
    int pos2 = applicationName.find_last_of("\\");

    if(pos2 > pos)
      {
      pos = pos2;
      }

    if(pos != -1)
      {
      applicationName = applicationName.substr(pos+1,applicationName.size()-pos-1);
      }

    fprintf(fic,"  <parameter name=\"Executable\" value=\"%s%s\"/>\n",
            m_ExecutableDirectory.c_str(),applicationName.c_str());

    std::string commandline = "";
    itParams = params.begin();
    unsigned int nParams = 0;
    while(itParams != params.end())
      {
      //if((*itParams).IsValueDefined() && (!(*itParams).GetParent() || !m_Grouping))
      if((*itParams).IsValueDefined() && (!m_Grouping))
        {
        if(nParams>0)
          {
          commandline += " ";
          }

        bool hasChildren = false;

        if(!m_Grouping)
          {
          std::string command = (*itParams).GetName().toChar();
          command += ".";
          std::vector<ApplicationWrapperParam>::const_iterator itParamsChildren = params.begin();
          while(itParamsChildren != params.end())
            {
            std::string child = (*itParamsChildren).GetName().toChar();
            if(child.find(command)==0)
              {
              hasChildren = true;
              break;
              }
            itParamsChildren++;
            }
          }

        if(hasChildren && !(*itParams).GetParamsSubSize())
          {
          commandline += " ";
          commandline += (*itParams).GetValue().toChar();
          commandline += " ";
          }
        else
          {
          // If this is an input file we use the correct filename
          // WARNING WORKS ONLY IF ONE INPUT IMAGE
          if((*itParams).GetExternalData() == 1
             && (*itParams).GetValue().length() > 0
             && !(*it).GetDataHost().empty()
             && (strcmp((*it).GetName().c_str(),"bmGridStore")
                 && strcmp((*it).GetName().c_str(),"bmGridSend"))
             ) // DATA_IN
            {
            char* num = new char[10];
            sprintf(num,"%d",inFile-1);
            commandline += "{InputFile";
            commandline += num;
            commandline += "}";
            delete [] num;
            }
          else
            {
            commandline += "{";
            commandline += (*itParams).GetName().replaceChar('.','_').toChar();
            commandline += "}";
            }
          }

        // This is a hack because grouping was not working...
        if(!m_Grouping && !hasChildren)
          {
          std::string value = (*itParams).GetValue().toChar();
          // Extract the values
          std::vector<std::string> values;
          unsigned int startWord = 0;
          for(unsigned int i=0;i<value.size();i++)
            {
            if(value[i]=='\"')
              {
              startWord=i+1;
              // Go to the next '\"' and increase the nValues
              long int local_position = value.find("\"",i+1);
              if(local_position != -1)
                {
                values.push_back(value.substr(startWord,local_position-i-1));
                i=local_position+1;
                }
              }
            else if(value[i] == ' ')
              {
              values.push_back(value.substr(startWord,i-startWord));
              startWord =i+1; // WARNING this is not always true!
              }
            }

          // ignoring the first value
          std::vector<std::string>::const_iterator itValues = values.begin();
          if(values.size()>0)
            {
            itValues++;
            }

          unsigned int numid = 1;
          while(itValues!=values.end())
            {
            char* num = new char[10];
            sprintf(num,"%d",numid);
            commandline += " {";
            commandline += (*itParams).GetName().replaceChar('.','_').toChar();
            commandline += "_";
            commandline += num;
            commandline += "}";
            delete [] num;
            itValues++;
            numid++;
            }
          }
        nParams++;
        }
      itParams++;
      }

    fprintf(fic,"  <parameter name=\"Arguments\"  value=\"%s\"/>\n",commandline.c_str());

    // Write each argument out
    itParams = params.begin();
    nParams = 1;
    while(itParams != params.end())
      {
      //if((*itParams).IsValueDefined() && !(*itParams).GetParent())
      if((*itParams).IsValueDefined() && !(*itParams).GetParamsSubSize())
        {
        if((*itParams).GetType() == ApplicationWrapperParam::Flag)
          {
          std::string optional = "false";
          if((*itParams).GetOptional())
            {
            optional = "true";
            }

          std::string syntax = (*itParams).GetValue().toChar();

          std::vector<ApplicationWrapperParam>::const_iterator itParent = itParams;
          itParent++;

          while(itParent!=params.end())
            {
            const std::vector<ApplicationWrapperParamSub> children = itParent->GetParamsSub();
            std::vector<ApplicationWrapperParamSub>::const_iterator itChildren = children.begin();
            while(itChildren!=children.end())
              {
              syntax += " {";
              syntax += (*itChildren).GetName().toChar();
              syntax += "}";

              // Check if we have only one value
              std::string value = (*itChildren).GetValue().toChar();
              unsigned int nValues = 0;
              for(unsigned int i=0;i<value.size();i++)
                {
                if(value[i]=='\"')
                  {
                  // Go to the next '\"' and increase the nValues
                  long int local_position = value.find("\"",i+1);
                  if(local_position != -1)
                    {
                    i=local_position+1;
                    nValues++;
                    }
                  }
                else if(value[i] == ' ')
                  {
                  nValues++;
                  }
                }

              for(unsigned int j=1;j<nValues;j++)
                {
                syntax += " {";
                syntax += (*itChildren).GetName().toChar();
                char* num = new char[10];
                sprintf(num,"%d",j);
                syntax += "_";
                syntax += num;
                syntax += "}";
                delete [] num;
                }
              itChildren++;
              }
            itParent++;
            }

          itParent = itParams;
          itParent++;


          // If the group has no child we plan accordingly
          if(itParent == params.end())
            {
            fprintf(fic,"  <argument name=\"%s\" value=\"%s\"/>\n"
                    ,(*itParams).GetName().replaceChar('.','_').toChar()
                    ,this->AddQuotes(syntax).c_str());
            }
          else
            {
            if(m_Grouping)
              {
              fprintf(fic,"  <group name=\"%s\" syntax=\"%s\" optional=\"%s\" selected=\"true\">\n"
                      ,(*itParams).GetName().toChar()
                      ,syntax.c_str()
                      ,optional.c_str());
              }
            while(itParent!=params.end())
              {
              const std::vector<ApplicationWrapperParamSub> children = itParent->GetParamsSub();
              std::vector<ApplicationWrapperParamSub>::const_iterator itChildren = children.begin();
              while(itChildren!=children.end())
                {
                std::string value = (*itParent).GetValue().toChar();
                // Extract the values
                std::vector<std::string> values;
                unsigned int startWord = 0;
                unsigned int i;
                for(i=0;i<value.size();i++)
                  {
                  if(value[i]=='\"')
                    {
                    startWord=i+1;
                    // Go to the next '\"' and increase the nValues
                    long int local_position = value.find("\"",i+1);
                    if(local_position != -1)
                      {
                      values.push_back(value.substr(startWord,local_position-i-1));
                      i=local_position+1;
                      }
                    }
                  else if(value[i] == ' ')
                    {
                    values.push_back(value.substr(startWord,i-startWord));
                    startWord =i+1; // WARNING this is not always true!
                    }
                  }

                // If the size of the list is null, something is wrong, well we have only one value
                if(values.size() == 0)
                  {
                  values.push_back(value);
                  }


                std::vector<std::string>::const_iterator itV=values.begin();
                i=0;
                while(itV!=values.end())
                  {
                  if(itV==values.begin())
                    {
                    fprintf(fic,"   <argument name=\"%s\" value=\"%s\" type=\"%s\"/>\n",
                            (*itParent).GetName().replaceChar('.','_').toChar(),this->AddQuotes((*itV)).c_str(),
                            (*itParent).GetTypeAsChar());
                    }
                  else
                    {
                    fprintf(fic,"   <argument name=\"%s_%d\" value=\"%s\" type=\"%s\"/>\n",
                            (*itParent).GetName().replaceChar('.','_').toChar(),i,this->AddQuotes((*itV)).c_str(),
                            (*itParent).GetTypeAsChar());

                    }
                  i++;
                  itV++;
                  }
                itChildren++;
                }
              itParent++;
              }
            if(m_Grouping)
              {
              fprintf(fic,"  </group>\n");
              }
            }
          }
        else
          {
          // If this is an input file we use the correct filename
          // WARNING WORKS ONLY IF ONE INPUT IMAGE
          if((*itParams).GetExternalData() == 1
             && (*itParams).GetValue().length() > 0
             && !(*it).GetDataHost().empty()
             && (strcmp((*it).GetName().c_str(),"bmGridStore")
                 && strcmp((*it).GetName().c_str(),"bmGridSend"))
             ) // DATA_IN
            {
            // Do nothing
            }
          else
            {
            MString value2 = (*itParams).GetValue();
            value2 = value2.removeChar('\"');
            std::string value3 = value2.toChar();
            fprintf(fic,"  <argument name=\"%s\" value=\"%s\" type=\"%s\"/>\n",
                    (*itParams).GetName().toChar(),this->AddQuotes(value3).c_str(),
                    (*itParams).GetTypeAsChar());
            }
          }
        }
      nParams++;
      itParams++;
      }

    std::vector<std::string>::const_iterator itDep = dependencies.begin();
    while(itDep != dependencies.end())
      {
      fprintf(fic,"  <dependency name=\"%s\"/>\n",(*itDep).c_str());
      itDep++;
      }

    if(dependencies.size() == 0 && dependApp)
      {
      fprintf(fic,"  <dependency name=\"%s\"/>\n",dependApp->GetDependencyTag().c_str());
      }

    fprintf(fic," </componentAction>\n");
    dependencies.clear();

    char* appName = new char[255];
    sprintf(appName,"app%d",appnum);

    itParams = params.begin();

    if(strcmp((*it).GetName().c_str(),"bmGridStore")
       && strcmp((*it).GetName().c_str(),"bmGridSend"))
      {
      while(itParams != params.end())
        {
        if((*itParams).GetExternalData() == 2
           && (*itParams).GetValue().length() > 0
           && !(*it).GetOutputHost().empty()) // DATA_OUT
          {
          fprintf(fic," <componentAction type=\"DataRelocation\" name=\"OutputFile%d\">\n",outFile);
          fprintf(fic,"  <parameter name=\"Host\" value=\"%s\"/>\n",(*it).GetOutputHost().c_str());
          fprintf(fic,"  <parameter name=\"Description\" value=\"\"/>\n");
          fprintf(fic,"  <parameter name=\"Direction\" value=\"Out\"/>\n");
          fprintf(fic,"  <parameter name=\"Protocol\" value=\"gsiftp\"/>\n");

          MString valwithoutquote = (*itParams).GetValue().removeChar('"');

          fprintf(fic,"  <parameter name=\"SourceDataPath\" value=\"%s\"/>\n",valwithoutquote.toChar());
          fprintf(fic,"  <parameter name=\"DestDataPath\" value=\"%s%s\"/>\n",(*it).GetOutputDirectory().c_str(),valwithoutquote.toChar());
          fprintf(fic," <dependency name=\"%s\" status=\"done\"/>\n",appName);
          fprintf(fic," </componentAction>\n");
          char* dep = new char[255];
          sprintf(dep,"OutputFile%d",outFile);
          (*it).SetDependencyTag(dep);
          dependencies.push_back(dep);
          delete [] dep;
          outFile++;
          }
        itParams++;
        }
      } // end bmGridStore

    delete [] appName;
    it++;
    appnum++;
    applicationComponent++;
    }

  fprintf(fic,"</componentActionList>\n");
  fprintf(fic,"</applicationComponent>\n\n");

  fprintf(fic,"</gridApplication>\n");
  fclose(fic);
}

/** Write a shell script */
void Grid::WriteShell()
{
  if(m_FileName.size() == 0)
    {
    std::cout << "Grid::WriteShell() : Please set a filename" << std::endl;
    return;
    }
  std::string filename;
  if( !m_WorkingDirectory.empty() )
    {
    filename = m_WorkingDirectory + "/" + m_FileName;
    }
  else
    {
    filename = m_FileName;
    }

  FILE* fic = fopen( filename.c_str(),"wb");
  if(!fic)
    {
    std::cout << "Grid::WriteShell() : Cannot create GAD script" << std::endl;
    return;
    }

  // Write the header
  fprintf(fic,"# Script generated by BatchMake (c) Kitware Inc.\n");
  fprintf(fic,"# (c) Kitware Inc 2009\n");
  fprintf(fic,"# More information at: http://www.batchmake.org\n\n");

  fprintf(fic,"#!/bin/bash\n\n");

  // Add the executable
  std::vector<ApplicationWrapper>::iterator it = m_ApplicationsList.begin();
  while(it != m_ApplicationsList.end())
    {
    fprintf(fic,"%s %s\n",(*it).GetApplicationPath().c_str(),
                          (*it).GetCurrentCommandLineArguments(false).c_str());
    it++;
    }

  fclose(fic);
}

/** Strip a string */
std::string Grid::Strip(std::string value)
{
  std::string stripped = "";
  for(unsigned int i=0;i<value.size();i++)
    {
    if(value[i] != ' ' && value[i] != '\"' && value[i] != '\'')
      {
      stripped += value[i];
      }
    }
  return stripped;
}

/** Write a condor script */
void Grid::WriteCondor()
{
  if(m_FileName.size() == 0)
    {
    std::cout << "Grid::WriteCondor() : Please set a filename" << std::endl;
    return;
    }
  std::string filename;
  if( !m_WorkingDirectory.empty() )
    {
    filename = m_WorkingDirectory + "/" + m_FileName;
    }
  else
    {
    filename = m_FileName;
    }
  std::cout << "Generating Condor File: " << filename << std::endl;

  // Create a DAG
  std::vector<DAGnode> dag;
  m_ParentChildList.clear();

  FILE* dagfile = fopen( filename.c_str(),"wb");
  if( !dagfile )
    {
    std::cerr << "Grid::WriteCondor() : Cannot create dagman file "
              << m_FileName << ". The dagman file path shall be relative "
              << "to the Working directory." << std::endl;
    return;
    }
  // Write the header
  fprintf(dagfile,"# Script generated by BatchMake (c) Insight Software Consortium\n");
  fprintf(dagfile,"# (c) Kitware Inc 2009\n");
  fprintf(dagfile,"# More information at: http://www.batchmake.org\n\n");

  // Add the executables
  unsigned int id = 0;
  std::vector<ApplicationWrapper>::iterator it = m_ApplicationsList.begin();
  while(it != m_ApplicationsList.end())
    {
    char* idStr = new char[10];
    sprintf(idStr,"%d",id);
    std::string jobFileName = itksys::SystemTools::GetFilenamePath( filename );
    if( !jobFileName.empty() )
      {
      jobFileName += "/";
      }
    jobFileName += itksys::SystemTools::GetFilenameWithoutExtension( filename );
    jobFileName += ".";
    jobFileName += idStr;
    jobFileName += itksys::SystemTools::GetFilenameExtension( filename );

    delete [] idStr;

    DAGnode dagNode;
    dagNode.id = id;

    // We have a barrier node
    if((*it).GetDependsOn()!=-1)
      {
      dagNode.isBarrier = true;
      dagNode.parents.push_back((*it).GetDependsOn());
      }
    else
      {
      dagNode.isBarrier = false;
      // This is the previous node
      if(dag.size()>0)
        {
        dagNode.parents.push_back(dag.size()-1);
        }
      }
    dagNode.isAfterbarrier = (*it).GetAfterEndBarrier();
    dagNode.scope = (*it).GetExecutionBlockNumber();

    dagNode.isVirtual = false;
    if((*it).GetName() == "BatchMakeVirtualNode")
      {
      dagNode.isVirtual = true;
      }

    dag.push_back(dagNode);

    //std::cout << id << " : " << (*it).GetCurrentCommandLineArguments(true).c_str() << " ec = " << (*it).GetExecutionBlockNumber() << " depends on " << (*it).GetDependsOn() << std::endl;

    // Add the file to the dag file
    if(!dagNode.isVirtual)
      {
      fprintf(dagfile,"Job job%d %s\n",id, jobFileName.c_str());
      // if there is a Condor Pre Script for this application, add it to the DAG file
      if((*it).GetCondorPreScript() != "")
        {
        fprintf(dagfile,"SCRIPT PRE job%d %s\n",id, (*it).GetCondorPreScript().c_str());
        }
      // if there is a Condor Post Script for this application, add it to the DAG file
      if((*it).GetCondorPostScript() != "")
        {
        fprintf(dagfile,"SCRIPT POST job%d %s\n",id, (*it).GetCondorPostScript().c_str());
        }
      }

    std::cout << "Generating Condor File: " << jobFileName.c_str() << std::endl;

    FILE* fic = fopen( jobFileName.c_str(),"wb");
    if(!fic)
      {
      std::cout << "Grid::WriteCondor() : Cannot create Condor script: " << jobFileName << std::endl;
      return;
      }

    // Write the header
    fprintf(fic,"# Script generated by BatchMake (c) Insight Software Consortium\n");
    fprintf(fic,"# (c) Kitware Inc 2009\n");
    fprintf(fic,"# More information at: http://www.batchmake.org\n\n");

    fprintf(fic,"Universe       = vanilla\n");
    fprintf(fic,"Output         = bmGrid.%d.out.txt\n",id);
    fprintf(fic,"Error          = bmGrid.%d.error.txt\n",id);
    fprintf(fic,"Log            = bmGrid.%d.log.txt\n",id);
    fprintf(fic,"Notification   = NEVER\n");

    if(m_WorkingDirectory.size()>0)
      {
      fprintf(fic,"Initialdir     = %s\n",m_WorkingDirectory.c_str());
      }

    std::string executable = (*it).GetApplicationPath();
    if( m_TransferFiles != ALL &&
        m_TransferFiles != EXECUTABLE &&
        !m_ExecutableDirectory.empty() )
      {
      std::string executableDirectory = m_ExecutableDirectory;
      if( m_ExecutableDirectory[ m_ExecutableDirectory.size() - 1 ] != '/' )
        {
        executableDirectory = m_ExecutableDirectory + "/";
        }
      executable = executableDirectory +
        itksys::SystemTools::GetFilenameName( executable );
      }

    fprintf(fic,"Executable    = %s\n",executable.c_str());

    // We need to escape double-quotes if any and add the
    std::string arguments =
      (*it).GetCurrentCommandLineArguments( true,
                                            m_InputDirectory,
                                            m_OutputDirectory );
    std::size_t posDQ = arguments.find("\"");
    while( posDQ != std::string::npos )
      {
      arguments[posDQ]='\'';
      posDQ = arguments.find("\"", posDQ + 1);
      }

    fprintf(fic,"Arguments     = \"%s\"\n",arguments.c_str());

    std::string requirements = m_Requirements;
    if( !m_Requirements.empty() && !it->GetRequirements().empty() )
      {
      requirements += "&& ";
      }
    requirements += it->GetRequirements();

    if( !requirements.empty() )
      {
      fprintf( fic, "Requirements = %s \n", requirements.c_str() );
      }

    if( !m_Owner.empty() )
      {
      fprintf( fic, "+Owner=\"%s\"\n", m_Owner.c_str() );
      }

    // Check if we have external data
    const std::vector<ApplicationWrapperParam> & params = (*it).GetParams();
    std::vector<ApplicationWrapperParam>::const_iterator itParams = params.begin();

    std::string externalData = "";
    bool gotExternalData = false;
    while(itParams != params.end())
      {
      if((*itParams).GetExternalData()>0)
        {
        gotExternalData = true;
        }
      if((*itParams).GetExternalData() == 1)
        {
        std::string datavalue = this->Strip((*itParams).GetValue().toChar());
        if(externalData.size()>0 && datavalue.size()>0)
          {
          externalData += ",";
          }
        externalData += datavalue;
        }
      itParams++;
      }


    std::vector<std::string> otherinputdata = (*it).GetInputDataToTransfer();
    if(externalData.size() > 0 || otherinputdata.size()>0)
      {
      std::vector<std::string>::const_iterator itExternalData = otherinputdata.begin();
      while(itExternalData != otherinputdata.end())
        {
        std::string datavalue = this->Strip(*itExternalData);
        if(externalData.size()>0 && datavalue.size()>0)
          {
          externalData += ",";
          }
        externalData += datavalue;
        itExternalData++;
        }

      if(m_TransferFiles != NONE)
        {
        fprintf(fic,"transfer_input_files = %s\n",externalData.c_str());
        }
      }


    if(m_TransferFiles!=NONE && gotExternalData)
      {
      fprintf(fic,"should_transfer_files = yes\n");
      fprintf(fic,"when_to_transfer_output = ON_EXIT_OR_EVICT\n");
      }

    fprintf(fic,"Queue 1\n");
    it++;

  fclose(fic);
  id++;
  }// end loop applications


  // Write out the tree in the dag file
  this->ConsolidateDAG(&dag);

  // We start with the root node and go down the tree
  std::vector<DAGnode>::const_iterator itdag = dag.begin();
  while(itdag != dag.end())
    {
    if((*itdag).parents.size()==0)
      {
      this->AddDAGParent(dagfile,&dag,(*itdag).id);
      }
    itdag++;
    }

  // Add the dot generation
  fprintf(dagfile,"DOT %s.dot\n", filename.c_str() );

  fclose(dagfile);
}

/** Consolidtae the Directed Acycle graph
 *  by connecting the nodes when wer are an after barrier node */
void Grid::ConsolidateDAG(std::vector<DAGnode>* dag)
{
  std::vector<DAGnode>::iterator it = dag->begin();
  while(it != dag->end())
    {
    // if this is an after barrier
    if((*it).isAfterbarrier)
      {
      // Find the starting barrer node
      std::vector<DAGnode>::const_iterator itStartBarrier = it;
      int startBarrier = 0;
      while(itStartBarrier != dag->begin())
        {
        if((*itStartBarrier).isBarrier && (*itStartBarrier).scope == (*it).scope)
          {
          startBarrier = (*itStartBarrier).id;
          break;
          }
        itStartBarrier--;
        }

      if(startBarrier<(*it).id)
        {
        // We have the starting barrier node
        //std::cout << "Starting Barrier for node " << (*it).id << " = " << startBarrier << std::endl;
        // We add the leaves except the given node
        this->AddLeavesAsParent(dag,startBarrier,&(*it));
        }
      } // end if this is a afterbarrier
    it++;
    }

  // We remove the virtual nodes
  it = dag->begin();
  while(it != dag->end())
    {
    if((*it).isVirtual)
      {
      if((*it).parents.size()>1)
        {
        std::cout << "ERROR: don't know how to delete virtual node with multiple parents" << std::endl;
        return;
        }

      // 'it' may not have parents if 'it' is root
      // Find all the nodes that are connected and connect to the parent instead
      int parent = (*it).parents.size()? (*it).parents[0] : -1;

      std::vector<DAGnode>::iterator itC = dag->begin();
      while(itC != dag->end())
        {
        std::vector<int>::iterator itP = (*itC).parents.begin();
        while(itP != (*itC).parents.end())
          {
          if(*itP == (*it).id)
            {
            if( parent != -1 )
              {
              // connect the child of 'it' with 'it's parent.
              *itP = parent;
              }
            else
              {
              // it has no parent (root), after it get deleted, its children
              //won't have parents neither
              (*itC).parents.erase( itP );
              }
            break;
            }
          itP++;
          }
        itC++;
        }

      it = dag->erase(it);
      }
    else
      {
      it++;
      }
    }

}

/** Add leaves */
void Grid::AddLeavesAsParent(std::vector<DAGnode>* dag,int node,DAGnode* nodeToAdd)
{
  // if we already have a parent we quit
  std::vector<int>::const_iterator pit = nodeToAdd->parents.begin();
  while(pit != nodeToAdd->parents.end())
    {
    if(node==*pit)
      {
      return;
      }
    pit++;
    }

  // Find the child of the node with the same scope
  std::vector<int> childlist;

  std::vector<DAGnode>::const_iterator it = dag->begin();
  while(it != dag->end())
    {
    std::vector<int>::const_iterator itP = (*it).parents.begin();
    while(itP != (*it).parents.end())
      {
      if(*itP == node)
        {
        childlist.push_back((*it).id);
        break;
        }
      itP++;
      }
    it++;
    }

  if(childlist.size() == 0)
    {
    nodeToAdd->parents.push_back(node);
    return;
    }
  else
    {
    std::vector<int>::const_iterator c = childlist.begin();
    while(c != childlist.end())
      {
      this->AddLeavesAsParent(dag,*c,nodeToAdd);
      c++;
      }
    }
}

/** Add a DAG parent */
void Grid::AddDAGParent(FILE* fic,std::vector<DAGnode>* dag,int parentid)
{
  // Write out the children
  std::vector<int> childrenlist;
  std::vector<DAGnode>::const_iterator it = dag->begin();
  while(it != dag->end())
    {
    std::vector<int>::const_iterator itParent = (*it).parents.begin();
    while(itParent != (*it).parents.end())
      {
      if(*itParent == parentid)
        {
        childrenlist.push_back((*it).id);
        break;
        }
      itParent++;
      }
    it++;
    }

  if(childrenlist.size()>0)
    {
    std::vector<int>::const_iterator itChild = childrenlist.begin();
    while(itChild != childrenlist.end())
      {
      // If not in the list of parent-child we write it out
      std::vector<ParentChildType>::const_iterator pcit = m_ParentChildList.begin();
      bool exists = false;
      while(pcit != m_ParentChildList.end())
        {
        if((*pcit).first == parentid && (*pcit).second == *itChild)
          {
          exists = true;
          break;
          }
        pcit++;
        }

      if(!exists)
        {
        fprintf(fic,"PARENT job%d CHILD job%d \n",parentid,*itChild);
        ParentChildType pc;
        pc.first = parentid;
        pc.second = *itChild;
        m_ParentChildList.push_back(pc);
        }

      itChild++;
      }
    }

  // Now continue with the current list of children
  std::vector<int>::const_iterator itChild = childrenlist.begin();
  while(itChild != childrenlist.end())
    {
    this->AddDAGParent(fic,dag,(*itChild));
    itChild++;
    }
}


} // end namespace bm
