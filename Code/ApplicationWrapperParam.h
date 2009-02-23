/*=========================================================================

  Program:   BatchMake
  Module:    ApplicationWrapperParam.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef __ApplicationWrapperParam_h_
#define __ApplicationWrapperParam_h_

#include <vector>
#include <iostream>
#include "MString.h"

#include "ApplicationWrapperParamSub.h"

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

class ApplicationWrapperParam : public ApplicationWrapperParamSub 
{
public:
  ApplicationWrapperParam();
  ~ApplicationWrapperParam();

  const std::vector<ApplicationWrapperParamSub>& GetParamsSub() const {return m_Params;}
  void AddParamSub(ApplicationWrapperParamSub param){m_Params.push_back(param);}
  void RemoveParamSub(MString name);
  int GetParamsSubSize() const {return m_Params.size();}
  ApplicationWrapperParamSub* GetParamSub(MString name);
  void ClearParamSubValues();
  bool ParamSubExists( const std::string& first)const;
  bool CheckSubValueDefined(bool relativePath, std::string* line)const;
  void SetParamSubValue(std::string first, std::string second, std::string value, bool boolean);
  std::vector<std::string> ShowApplicationOptionsSubParams(std::string parent) const;
 

private:
  std::vector<ApplicationWrapperParamSub> m_Params;

};

#endif
