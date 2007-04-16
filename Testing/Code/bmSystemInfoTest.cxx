/*=========================================================================

Program:   Insight Segmentation & Registration Toolkit
Module:    bmSystemInfoTest.cxx
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

#include "SystemInfo.h"
#include <iostream>

int SystemInfoTest(int argc, char* argv[])
{
  SystemInfo info;

  std::cout << "Vendor = " << info.GetVendorID() << std::endl;
  std::cout << "Model = " <<info.GetModelID() << std::endl;
  std::cout << "Extended ProcessorName = " << info.GetExtendedProcessorName() << std::endl;
  std::cout << "Processor Speed = " << info.GetProcessorClockFrequency() << " Mhz" << std::endl;
  std::cout << "Number of physical CPUs = " << info.GetNumberOfPhysicalCPU() << std::endl;
  std::cout << "Number of logical CPUs = " << info.GetNumberOfLogicalCPU() << std::endl;
  std::cout << "Cache Size = " << info.GetProcessorCacheSize() << std::endl;

  std::cout << "Total Virtual Memory = "  << info.GetTotalVirtualMemory() << " b" << std::endl;
  std::cout << "Available Virtual Memory = "  << info.GetAvailableVirtualMemory() << " b" << std::endl;
  std::cout << "Total Physical Memory = "  << info.GetTotalPhysicalMemory() << " b" << std::endl;
  std::cout << "AvailablePhysicalMemory = "  << info.GetAvailablePhysicalMemory() << " b" << std::endl;
    
  std::cout << "Hostname = " << info.GetHostname() << std::endl;
  std::cout << "Operating System Platform = " << info.GetOSPlatform() << std::endl;
  std::cout << "Operating System Name = " << info.GetOSName() << std::endl;
  std::cout << "Operating System Release = " << info.GetOSRelease() << std::endl;
  std::cout << "Operating System Version = " << info.GetOSVersion() << std::endl;

  return EXIT_SUCCESS;
}
