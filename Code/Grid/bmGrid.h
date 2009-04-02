/*=========================================================================

  Program:   BatchMake
  Module:    bmGrid.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __Grid_h_
#define __Grid_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include <vector>
#include <ApplicationWrapper.h>

namespace bm {

class Grid
{
public:

  typedef enum 
    {ALL,
    EXECUTABLE,
    INPUT_FILES,
    OUTPUT_FILES,
    NONE} TransfertType;

  Grid();
  ~Grid();

  /** The file name shall be a relative path with regards to OutputDirectory
   *  If you never set OutputDirectory, then the file name can be an absolute
   *  name.
   */
  void SetFileName( const std::string& filename );
  const std::string& GetFileName( )const;
    
  /** Add an application to the list of applications to generate */
  void AddApplication(ApplicationWrapper* app,
                      const char* datadir=NULL,const char* outputdir=NULL);
  
  unsigned long GetNumberOfApplications()
    {
    return m_ApplicationsList.size();
    }

  void WriteCondor();
  void WriteGAD();
  void WriteShell();
  bool HasCurrentScopeFile();
  const char* GetCurrentScopeFile();

  void SetDataHost(const char* hostname) {m_DataHost = hostname;}
  void SetOutputHost(const char* hostname) {m_OutputHost = hostname;}
  void SetDataDirectory(const char* directory) {m_DataDirectory = directory;}
  void SetOutputDirectory(const char* directory) {m_OutputDirectory = directory;}
  void SetGridTempDirectory(const char* directory) {m_GridTempDirectory = directory;}
  void SetGridExecutableDirectory(const char* directory) {m_ExecutableDirectory = directory;}
  void SetTransferFile(TransfertType transferType) {m_TransferFiles = transferType;}
  void SetGridOwner( const std::string owner ) { m_GridOwner = owner; }
 
  const std::string& GetDataDirectory()const {return m_DataDirectory;}
  const std::string& GetOutputDirectory()const {return m_OutputDirectory;}
  const std::string& GetGridTempDirectory()const {return m_GridTempDirectory;}
  const std::string& GetGridExecutableDirectory()const {return m_ExecutableDirectory;}
  const std::string& GetGridOwner()const { return m_GridOwner; }
  /** Set/Get the requirements of the whole script. Application specific 
   *  requirements will be concatenated to it.
  */
  void SetRequirements( const std::string& requirements );
  const std::string& GetRequirements( )const;
 
  void SetDistributed(bool val);

  void SetGridBarrier();
  void RemoveGridBarrier();

  void SetSingleNode(bool single) 
    {
    if(single != m_SingleNode)
      {
      m_SingleNodeTransition = true;
      }
    m_SingleNode = single;
    }

  /** Set the maximum number of nodes to the grid */
  void SetMaxNodes(int nodes) {m_MaxNodes = nodes;}
  int GetMaxNodes() {return m_MaxNodes;}
  
  /** Set if grouping should be used for GAD generation */
  void SetGrouping(bool group) {m_Grouping = group;}
  bool GetGrouping() {return m_Grouping;}


protected:

  std::string Strip(std::string value);
  std::string AddQuotes(std::string value);

  /** Return a filename for a full path */
  std::string GetFilename(const char* fullpath); 

  std::string m_FileName;
  std::vector<ApplicationWrapper> m_ApplicationsList; // we create a copy of the app
  std::string m_CurrentScopeFile;
  std::string m_DataHost;
  std::string m_OutputHost;
  std::string m_DataDirectory;
  std::string m_OutputDirectory;
  std::string m_GridTempDirectory;
  std::string m_ExecutableDirectory;
  std::string m_Requirements;
  std::string m_GridOwner;

  bool             m_Distributed;
  bool             m_DistributedTransition;
  std::vector<int> m_DistributedSyncBarrier;
  bool             m_SingleNode;
  bool             m_SingleNodeTransition;
  int              m_MaxNodes;
  bool             m_Grouping;
  bool             m_NextAppIsAfterEndBarrier;
  unsigned long    m_CurrentScope;
  TransfertType    m_TransferFiles;

  struct DAGnode
    {
    int id;
    std::vector<int> parents;
    bool isAfterbarrier;
    bool isBarrier;
    unsigned long scope;
    bool isVirtual;
    };

  void AddDAGParent(FILE* fic,std::vector<DAGnode>* dag,int parentid);
  void ConsolidateDAG(std::vector<DAGnode>* dag);
  void AddLeavesAsParent(std::vector<DAGnode>* dag,int node,DAGnode* nodeToAdd);

  typedef std::pair<int,int> ParentChildType;
  std::vector<ParentChildType> m_ParentChildList;

};

} // end namespace bm

#endif
