/*=========================================================================

  Program:   BatchMake
  Module:    bmApplicationListGUIControls.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __ApplicationListGUIControls_h_
#define __ApplicationListGUIControls_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "bmApplicationListGUI.h"
#include "ApplicationWrapper.h"
#include <vector>

namespace bm {

class ApplicationListGUIControls : public ApplicationListGUI
{

public:
  ApplicationListGUIControls();
  ~ApplicationListGUIControls();

  void SetApplicationPath(MString applicationpath);
  void SetApplicationList(std::vector<ApplicationWrapper*>*);
  void Show();
  void OnAccept();
  void OnEdit();
  void OnNew();
  void OnRemove();
  void OnSelect();
  void Update(MString name);
  void UpdateNew(ApplicationWrapper* wrapper);

private:
  std::vector<ApplicationWrapper*>* m_applicationlist;
  MString m_applicationpath;

};

} // end namepsace bm

#endif
