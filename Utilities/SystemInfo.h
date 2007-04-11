/*=========================================================================

  Program:   BatchMake
  Module:    SystemInfo.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#ifndef _SystemInfo_h
#define _SystemInfo_h

#define STORE_TLBCACHE_INFO(x,y)  x = (x < y) ? y : x
#define VENDOR_STRING_LENGTH    (12 + 1)
#define CHIPNAME_STRING_LENGTH    (48 + 1)
#define SERIALNUMBER_STRING_LENGTH  (29 + 1)
#define TLBCACHE_INFO_UNITS      (15)
#define CLASSICAL_CPU_FREQ_LOOP    10000000
#define RDTSC_INSTRUCTION      _asm _emit 0x0f _asm _emit 0x31

#define CPUID_AWARE_COMPILER
#ifdef CPUID_AWARE_COMPILER
  #define CPUID_INSTRUCTION    cpuid
#else
  #define CPUID_INSTRUCTION    _asm _emit 0x0f _asm _emit 0xa2
#endif

#define MMX_FEATURE          0x00000001
#define MMX_PLUS_FEATURE      0x00000002
#define SSE_FEATURE          0x00000004
#define SSE2_FEATURE        0x00000008
#define AMD_3DNOW_FEATURE      0x00000010
#define AMD_3DNOW_PLUS_FEATURE    0x00000020
#define IA64_FEATURE        0x00000040
#define MP_CAPABLE          0x00000080
#define HYPERTHREAD_FEATURE      0x00000100
#define SERIALNUMBER_FEATURE    0x00000200
#define APIC_FEATURE        0x00000400
#define SSE_FP_FEATURE        0x00000800
#define SSE_MMX_FEATURE        0x00001000
#define CMOV_FEATURE        0x00002000
#define MTRR_FEATURE        0x00004000
#define L1CACHE_FEATURE        0x00008000
#define L2CACHE_FEATURE        0x00010000
#define L3CACHE_FEATURE        0x00020000
#define ACPI_FEATURE        0x00040000
#define THERMALMONITOR_FEATURE    0x00080000
#define TEMPSENSEDIODE_FEATURE    0x00100000
#define FREQUENCYID_FEATURE      0x00200000
#define VOLTAGEID_FREQUENCY      0x00400000

// Status Flag
#define HT_NOT_CAPABLE           0
#define HT_ENABLED               1
#define HT_DISABLED              2
#define HT_SUPPORTED_NOT_ENABLED 3
#define HT_CANNOT_DETECT         4

#define HT_BIT             0x10000000     // EDX[28]  Bit 28 is set if HT is supported
#define FAMILY_ID          0x0F00         // EAX[11:8] Bit 8-11 contains family processor ID.
#define PENTIUM4_ID        0x0F00         
#define EXT_FAMILY_ID      0x0F00000      // EAX[23:20] Bit 20-23 contains extended family processor ID
#define NUM_LOGICAL_BITS   0x00FF0000     // EBX[23:16] Bit 16-23 in ebx contains the number of logical
                                          // processors per physical processor when execute cpuid with 
                                          // eax set to 1

#define INITIAL_APIC_ID_BITS  0xFF000000  // EBX[31:24] Bits 24-31 (8 bits) return the 8-bit unique 
                                          // initial APIC ID for the processor this code is running on.
                                          // Default value = 0xff if HT is not supported


#ifdef _WIN32
  #include <windows.h>
#endif

#ifdef __linux
#include <sys/procfs.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/utsname.h> // int uname(struct utsname *buf);
#include <ctype.h> // int isdigit(int c);
#include <errno.h> // extern int errno;
#include <sys/time.h>
#elif __hpux
#include <sys/param.h>
#include <sys/pstat.h>
#endif

#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

typedef  void (*DELAY_FUNC)(unsigned int uiMS);

class SystemInfo 
{

public:
  SystemInfo ();
  ~SystemInfo ();

  const char * GetVendorString();
  const char * GetVendorID();
  const char * GetTypeID();
  const char * GetFamilyID();
  const char * GetModelID();
  const char * GetSteppingCode();
  const char * GetExtendedProcessorName();
  const char * GetProcessorSerialNumber();
  int GetProcessorCacheSize();
  int GetLogicalProcessorsPerPhysical();
  float GetProcessorClockFrequency();
  int GetProcessorAPICID();
  int GetProcessorCacheXSize(long int);
  bool DoesCPUSupportFeature(long int);

  unsigned int GetNumberOfLogicalCPU(); // per physical cpu
  unsigned int GetNumberOfPhysicalCPU();

  bool DoesCPUSupportCPUID();

  // Retrieve memory information in kilobytes.
  long GetTotalVirtualMemory();
  long GetAvailableVirtualMemory();
  long GetTotalPhysicalMemory();
  long GetAvailablePhysicalMemory();  

protected:

  typedef struct tagID 
    {
    int Type;
    int Family;
    int Model;
    int Revision;
    int ExtendedFamily;
    int ExtendedModel;
    char ProcessorName[CHIPNAME_STRING_LENGTH];
    char Vendor[VENDOR_STRING_LENGTH];
    char SerialNumber[SERIALNUMBER_STRING_LENGTH];
    } ID;

  typedef struct tagCPUPowerManagement 
    {
    bool HasVoltageID;
    bool HasFrequencyID;
    bool HasTempSenseDiode;
    } CPUPowerManagement;

  typedef struct tagCPUExtendedFeatures 
    {
    bool Has3DNow;
    bool Has3DNowPlus;
    bool SupportsMP;
    bool HasMMXPlus;
    bool HasSSEMMX;
    bool SupportsHyperthreading;
    int LogicalProcessorsPerPhysical;
    int APIC_ID;
    CPUPowerManagement PowerManagement;
    } CPUExtendedFeatures;  
  
  typedef struct CPUtagFeatures 
    {
    bool HasFPU;
    bool HasTSC;
    bool HasMMX;
    bool HasSSE;
    bool HasSSEFP;
    bool HasSSE2;
    bool HasIA64;
    bool HasAPIC;
    bool HasCMOV;
    bool HasMTRR;
    bool HasACPI;
    bool HasSerial;
    bool HasThermal;
    int CPUSpeed;
    int L1CacheSize;
    int L2CacheSize;
    int L3CacheSize;
    CPUExtendedFeatures ExtendedFeatures;
    } CPUFeatures;
    
  enum Manufacturer 
    {
    AMD, Intel, NSC, UMC, Cyrix, NexGen, IDT, Rise, Transmeta, UnknownManufacturer
    };

  // Functions.
  bool RetrieveCPUFeatures();
  bool RetrieveCPUIdentity();
  bool RetrieveCPUCacheDetails();
  bool RetrieveClassicalCPUCacheDetails();
  bool RetrieveCPUClockSpeed();
  bool RetrieveClassicalCPUClockSpeed();
  bool RetrieveCPUExtendedLevelSupport(int);
  bool RetrieveExtendedCPUFeatures();
  bool RetrieveProcessorSerialNumber();
  bool RetrieveCPUPowerManagement();
  bool RetrieveClassicalCPUIdentity();
  bool RetrieveExtendedCPUIdentity();
  
  Manufacturer  m_ChipManufacturer;
  CPUFeatures   m_Features;
  ID            m_ChipID;
  float         m_CPUSpeedInMHz;
  unsigned int  m_NumberOfLogicalCPU;
  unsigned int  m_NumberOfPhysicalCPU;

  int CPUCount();
  unsigned char LogicalCPUPerPhysicalCPU();
  unsigned char GetAPICId();
  unsigned int IsHyperThreadingSupported();
  long long GetCyclesDifference(DELAY_FUNC, unsigned int);

  // For Linux
  int RetreiveInformationFromCpuInfoFile();
  std::string ExtractValueFromCpuInfoFile(std::string buffer,
                                          const char* word, int init=0);

  static void Delay (unsigned int);
  static void DelayOverhead (unsigned int);

  void FindManufacturer();

  // Evaluate the memory information.
  int QueryMemory();
  long m_TotalVirtualMemory;
  long m_AvailableVirtualMemory;
  long m_TotalPhysicalMemory;
  long m_AvailablePhysicalMemory;

  long int m_CurrentPositionInFile;
};

#endif