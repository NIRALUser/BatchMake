/*=========================================================================

  Program:   BatchMake
  Module:    XMLReader.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/

#ifndef _XMLREADER_H
#define _XMLREADER_H

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <MString.h>

using namespace std;


class XMLReader
{
public:
  XMLReader();
  ~XMLReader();
  int Open(const char* filename);
  MString GetBalise();
  MString GetCurrentBalise();
  MString GetValue();
  void Close();
  bool EndOfFile();

private:
  std::ifstream networkfile;
  MString m_value;
  MString m_balise;
  bool    m_EndOfFile;

};

#endif
