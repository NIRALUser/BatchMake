#include <iostream>
// This file is located in BatchMake_SOURCE_DIR/Utilities
#include <itksys/SystemInformation.hxx>

int main(int , char * [])
{ 
  itksys::SystemInformation info;
  info.RunMemoryCheck();
  unsigned long memoryinMB = info.GetTotalPhysicalMemory();
  std::cout << "PHYSICAL_MEMORY_MB=" << memoryinMB << std::endl;
  return 1; 
}
