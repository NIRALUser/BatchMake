/*=========================================================================

  Program:   BatchMake
  Module:    ApplicationWrapper.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __ApplicationWrapper_h_
#define __ApplicationWrapper_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <vector>
#include <iostream>
#include "ApplicationWrapperParam.h"
#include "MString.h"
#include "XMLReader.h"
#include "bmModuleDescription.h"
#include "bmModuleDescriptionParser.h"

class ApplicationWrapper
{
public:
  ApplicationWrapper();
  ~ApplicationWrapper();

  void                SetApplicationPath( const std::string& applicationpath );
  const std::string&  GetApplicationPath( )const;
  void                SetName( const std::string& name );
  const std::string&  GetName( )const;
  
  void                SetVersion( const std::string& version );
  const std::string&  GetVersion( )const;
  void                AddParam( ApplicationWrapperParam param );
  ApplicationWrapperParam* GetParam( const std::string& name);
  const ApplicationWrapperParam* GetParamByFlag( const std::string& flag)const;
  ApplicationWrapperParam* GetParam( int index );
  void                Save( const std::string& filename );
  void                Load( const std::string& filename );
  MString             GetExampleLine()const;
  void                ReadModule( XMLReader& m_reader, bool newVersion );
  void                ReadParam( XMLReader& m_reader, bool newVersion );
  std::vector< ApplicationWrapperParam >& GetParams();
  const std::vector< ApplicationWrapperParam >& GetParams()const;
  void                DisplayParam( MString& m_line, int offset )const;
  void                DeleteParam( const std::string& name );
  void                UpParam( const std::string& name);
  void                DownParam( const std::string& name );
  bool                AutomaticCommandLineParsing( const std::string& _path );
  bool                AutomaticCommandLineParsingSlicer( 
                                                      const std::string& _path );

  /** Return the current command line arguments */
  std::string         GetCurrentCommandLineArguments( 
    bool relativePath=true, 
    const std::string& inputDirectory = std::string(), 
    const std::string& outputDirectory = std::string() )const ;

  /** Set the parameter value 
   *  Return true if the parameter is set, false otherwise.
   */
  bool SetParameterValue( const std::string& first, 
                          const std::string& second, 
                          const std::string& value );

  /** Return true if a parameter exists */
  bool ParameterExists( const std::string& first )const;

  /** Set/Get if the application uses sequential arguments */
  void SetSequentialParsing( bool val )
    {
    m_Sequential = val; 
    m_SequentialParams.clear();
    }
  bool GetSequentialParsing() {return m_Sequential;}

  void                SetDataHost(const std::string& hostname) 
    {m_DataHost = hostname;}
  const std::string&  GetDataHost()const 
    {return m_DataHost;}
  
  void SetOutputHost(const std::string& hostname) 
    {m_OutputHost = hostname;}
  const std::string& GetOutputHost()const 
    {return m_OutputHost;}
 
  void SetOutputDirectory(const std::string& directory)
    {m_OutputDirectory = directory;}
  const std::string& GetOutputDirectory()const 
    {return m_OutputDirectory;}

  void SetDataDirectory(const std::string& directory) 
    {m_DataDirectory = directory;}
  const std::string& GetDataDirectory()const 
    {return m_DataDirectory;}

  void SetDependsOn(int val) 
    {m_DependsOn = val;}
  int  GetDependsOn()const 
    {return m_DependsOn;}

  void SetAfterEndBarrier(bool val)
    {m_AfterEndBarrier = val;}
  bool  GetAfterEndBarrier()const
    {return m_AfterEndBarrier;}

  void SetExecutionBlockNumber(int val)
    {m_ExecutionBlockNumber = val;}
  unsigned long GetExecutionBlockNumber()const
    {return m_ExecutionBlockNumber;}

  void SetDependencyTag(const std::string& tag)
    {m_DependencyTag = tag;}
  const std::string& GetDependencyTag()const
    {return m_DependencyTag;}

  void SetSingleNode(int single) 
    {m_SingleNode = single;}
  int GetSingleNode()const
    {return m_SingleNode;}

  const std::vector<std::string> & GetInputDataToTransfer()const
    {
    return m_InputDataToTransfer;
    }

  void AddInputDataToTransfer(const std::string& filename)
    {
    m_InputDataToTransfer.push_back(filename);
    }

  void ClearParameterValues();
  
  void SetRequirements( const std::string& requirements )
    {
    m_Requirements = requirements;
    }
  
  const std::string& GetRequirements() const
    {
    return m_Requirements;
    }

protected:
  
  
private:
  std::string                           m_name;
  std::string                           m_applicationpath;
  std::string                           m_version;
  // Set if we should use the sequential application
  bool                                  m_Sequential; 
  std::vector<ApplicationWrapperParam>  m_params;
  std::vector<ApplicationWrapperParam>  m_SequentialParams;
  std::string                           m_DataHost;
  std::string                           m_OutputHost;
  std::string                           m_OutputDirectory;
  std::string                           m_DataDirectory;
  int                                   m_DependsOn;
  std::string                           m_DependencyTag;
  int                                   m_SingleNode;
  bool                                  m_AfterEndBarrier;
  unsigned long                         m_ExecutionBlockNumber;
  std::vector<std::string>              m_InputDataToTransfer;
  std::string                           m_Requirements;

  bool AddSlicerModuleDescription(bm::ModuleDescription* moduleDescription);
  
  //friend class bm::ApplicationWrapperGUIControls;
};

#endif
