/*=========================================================================

  Program:   BatchMake
  Module:    bmApplicationWrapperGUIControls.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __bmApplicationWrapperGUIControls_h_
#define __bmApplicationWrapperGUIControls_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "bmApplicationWrapperGUI.h"
#include "bmApplicationListGUIControls.h"
#include "ApplicationWrapper.h"
#include <vector>

namespace bm {

class ApplicationListGUIControls;

class ApplicationWrapperGUIControls : public ApplicationWrapperGUI
{
public:
  ApplicationWrapperGUIControls();
  ~ApplicationWrapperGUIControls();
  void Show();
  void OnReject();
  void OnSelectPath();
  void OnSelectType();
  void OnAddEnum();
  void OnSelectEnumList();
  void OnSelectParameters();
  void OnAddParameters();
  void OnSaveModule();
  void OnLoadModule();
  void OnDeleteParameters();
  void OnUpParam();
  void OnDownParam();
  void DisplayExample();
  void Refresh();
  void SetApplicationWrapper(ApplicationWrapper*);
  void SetApplicationPath(MString applicationpath);
  void SetApplicationListGUIControls(ApplicationListGUIControls* applicationlistguicontrols);


private:
  ApplicationWrapper* m_applicationwrapper;
  MString m_currentname;
  MString m_currentfilename;
  MString m_applicationpath;
  ApplicationListGUIControls* m_applicationlistguicontrols;
};

} // end namespace bm

#endif
