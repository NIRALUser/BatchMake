/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    BatchMakeMemoryTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include <iostream>
#include <string.h>
#include <cstring>
#include <itksys/SystemTools.hxx>
#include <itksys/SystemInformation.hxx>
#include <itkMultiThreader.h>
#include "metaCommand.h"

int main(int argc, char * argv[]) 
{ 
  if(argc<2)
    {
    std::cout << "Please specify the memory to use (in MB) as argument" << std::endl;
    return 0;
    }

  // Consume XXMB of memory
  unsigned long n = 1024*1024*atoi(argv[1]);
  char* test = new char[n];
  memset(test,0,n);
  itksys::SystemTools::Delay(1000);
  delete [] test;
  return 1;
}
