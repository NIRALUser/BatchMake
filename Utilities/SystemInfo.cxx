/*=========================================================================

  Program:   BatchMake
  Module:    SystemInfo.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2005 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#include "SystemInfo.h"
#include <vector>
#include <itksys/Process.h>

#ifndef WIN32
  #include <sys/utsname.h> // int uname(struct utsname *buf);
#endif

SystemInfo::SystemInfo ()
{
#ifdef WIN32
  // Check to see if this processor supports CPUID.
  if (DoesCPUSupportCPUID()) 
    {
    // Retrieve the CPU details.
    RetrieveCPUIdentity();
    RetrieveCPUFeatures();
    if (!RetrieveCPUClockSpeed())
      {
      RetrieveClassicalCPUClockSpeed();
      }

    // Attempt to retrieve cache information.
    if (!RetrieveCPUCacheDetails()) 
      {
      RetrieveClassicalCPUCacheDetails();
      }
    // Retrieve the extended CPU details.
    if (!RetrieveExtendedCPUIdentity()) 
      {
      RetrieveClassicalCPUIdentity();
      }
    RetrieveExtendedCPUFeatures();

    // Now attempt to retrieve the serial number (if possible).
    RetrieveProcessorSerialNumber();
    }
  this->CPUCount();
  this->QueryMemory();
#elif defined(__APPLE__)
  this->ParseSysCtl();
#elif defined (__SVR4) && defined (__sun)
  this->QuerySolarisInfo();
#else
  this->RetreiveInformationFromCpuInfoFile();
  this->QueryMemory();
#endif

  m_OSName = "";
  m_Hostname = "";
  m_OSRelease = "";
  m_OSVersion = "";
  m_OSPlatform = "";

  this->QueryOSInformation();

}

SystemInfo::~SystemInfo()
{
}

/** Get the vendor string */
const char * SystemInfo::GetVendorString()
{
  return m_ChipID.Vendor;
}

/** Get the OS Name */
const char * SystemInfo::GetOSName()
{
  return m_OSName.c_str();
}

/** Get the hostname */
const char* SystemInfo::GetHostname()
{
  return m_Hostname.c_str();
}

/** Get the OS release */
const char* SystemInfo::GetOSRelease()
{
  return m_OSRelease.c_str();
}

/** Get the OS version */
const char* SystemInfo::GetOSVersion()
{
  return m_OSVersion.c_str();
}

/** Get the OS platform */
const char* SystemInfo::GetOSPlatform()
{
  return m_OSPlatform.c_str();
}

/** Get the vendor ID */
const char * SystemInfo::GetVendorID()
{
  // Return the vendor ID.
  switch (m_ChipManufacturer) 
    {
    case Intel:
      return "Intel Corporation";
    case AMD:
      return "Advanced Micro Devices";
    case NSC:
      return "National Semiconductor";
    case Cyrix:
      return "Cyrix Corp., VIA Inc.";
    case NexGen:
      return "NexGen Inc., Advanced Micro Devices";
    case IDT:
      return "IDT\\Centaur, Via Inc.";
    case UMC:
      return "United Microelectronics Corp.";
    case Rise:
      return "Rise";
    case Transmeta:
      return "Transmeta";
    case Sun:
      return "Sun Microelectronics";
    default:
      return "Unknown Manufacturer";
    }
}

/** Return the type ID of the CPU */
const char * SystemInfo::GetTypeID()
{
  char * temp = new char[32];
  sprintf(temp,"%d",m_ChipID.Type);
  return temp;
}

/** Return the family of the CPU present */
const char * SystemInfo::GetFamilyID()
{
  char * temp = new char[32];
  sprintf(temp,"%d",m_ChipID.Family);
  return temp;
}

// Return the model of CPU present */
const char * SystemInfo::GetModelID()
{
  char * temp = new char[32];
  sprintf(temp,"%d",m_ChipID.Model);
  return temp;
}

/** Return the stepping code of the CPU present. */
const char * SystemInfo::GetSteppingCode()
{
  char * temp = new char[32];
  sprintf(temp,"%d",m_ChipID.Revision);
  return temp;
}

/** Return the stepping code of the CPU present. */
const char * SystemInfo::GetExtendedProcessorName()
{
  return m_ChipID.ProcessorName;
}
  
/** Return the serial number of the processor 
 *  in hexadecimal: xxxx-xxxx-xxxx-xxxx-xxxx-xxxx. */
const char * SystemInfo::GetProcessorSerialNumber()
{
  return m_ChipID.SerialNumber;
}

/** Return the logical processors per physical */
int SystemInfo::GetLogicalProcessorsPerPhysical()
{
  return m_Features.ExtendedFeatures.LogicalProcessorsPerPhysical;
}

/** Return the processor clock frequency. */
float SystemInfo::GetProcessorClockFrequency()
{
  return m_CPUSpeedInMHz;
}

/**  Return the APIC ID. */
int SystemInfo::GetProcessorAPICID()
{
  return m_Features.ExtendedFeatures.APIC_ID;
}

/** Return the L1 cache size. */
int SystemInfo::GetProcessorCacheSize()
{
  return m_Features.L1CacheSize;
}

/** Return the chosen cache size. */
int SystemInfo::GetProcessorCacheXSize(long int dwCacheID)
{
  switch (dwCacheID) 
    {
    case L1CACHE_FEATURE:
      return m_Features.L1CacheSize;
    case L2CACHE_FEATURE:
      return m_Features.L2CacheSize;
    case L3CACHE_FEATURE:
      return m_Features.L3CacheSize;
    }
  return -1;
}

bool SystemInfo::DoesCPUSupportFeature(long int dwFeature)
{
  bool bHasFeature = false;

  // Check for MMX instructions.
  if (((dwFeature & MMX_FEATURE) != 0) && m_Features.HasMMX) bHasFeature = true;

  // Check for MMX+ instructions.
  if (((dwFeature & MMX_PLUS_FEATURE) != 0) && m_Features.ExtendedFeatures.HasMMXPlus) bHasFeature = true;

  // Check for SSE FP instructions.
  if (((dwFeature & SSE_FEATURE) != 0) && m_Features.HasSSE) bHasFeature = true;

  // Check for SSE FP instructions.
  if (((dwFeature & SSE_FP_FEATURE) != 0) && m_Features.HasSSEFP) bHasFeature = true;

  // Check for SSE MMX instructions.
  if (((dwFeature & SSE_MMX_FEATURE) != 0) && m_Features.ExtendedFeatures.HasSSEMMX) bHasFeature = true;

  // Check for SSE2 instructions.
  if (((dwFeature & SSE2_FEATURE) != 0) && m_Features.HasSSE2) bHasFeature = true;

  // Check for 3DNow! instructions.
  if (((dwFeature & AMD_3DNOW_FEATURE) != 0) && m_Features.ExtendedFeatures.Has3DNow) bHasFeature = true;

  // Check for 3DNow+ instructions.
  if (((dwFeature & AMD_3DNOW_PLUS_FEATURE) != 0) && m_Features.ExtendedFeatures.Has3DNowPlus) bHasFeature = true;

  // Check for IA64 instructions.
  if (((dwFeature & IA64_FEATURE) != 0) && m_Features.HasIA64) bHasFeature = true;

  // Check for MP capable.
  if (((dwFeature & MP_CAPABLE) != 0) && m_Features.ExtendedFeatures.SupportsMP) bHasFeature = true;

  // Check for a serial number for the processor.
  if (((dwFeature & SERIALNUMBER_FEATURE) != 0) && m_Features.HasSerial) bHasFeature = true;

  // Check for a local APIC in the processor.
  if (((dwFeature & APIC_FEATURE) != 0) && m_Features.HasAPIC) bHasFeature = true;

  // Check for CMOV instructions.
  if (((dwFeature & CMOV_FEATURE) != 0) && m_Features.HasCMOV) bHasFeature = true;

  // Check for MTRR instructions.
  if (((dwFeature & MTRR_FEATURE) != 0) && m_Features.HasMTRR) bHasFeature = true;

  // Check for L1 cache size.
  if (((dwFeature & L1CACHE_FEATURE) != 0) && (m_Features.L1CacheSize != -1)) bHasFeature = true;

  // Check for L2 cache size.
  if (((dwFeature & L2CACHE_FEATURE) != 0) && (m_Features.L2CacheSize != -1)) bHasFeature = true;

  // Check for L3 cache size.
  if (((dwFeature & L3CACHE_FEATURE) != 0) && (m_Features.L3CacheSize != -1)) bHasFeature = true;

  // Check for ACPI capability.
  if (((dwFeature & ACPI_FEATURE) != 0) && m_Features.HasACPI) bHasFeature = true;

  // Check for thermal monitor support.
  if (((dwFeature & THERMALMONITOR_FEATURE) != 0) && m_Features.HasThermal) bHasFeature = true;

  // Check for temperature sensing diode support.
  if (((dwFeature & TEMPSENSEDIODE_FEATURE) != 0) && m_Features.ExtendedFeatures.PowerManagement.HasTempSenseDiode) bHasFeature = true;

  // Check for frequency ID support.
  if (((dwFeature & FREQUENCYID_FEATURE) != 0) && m_Features.ExtendedFeatures.PowerManagement.HasFrequencyID) bHasFeature = true;

  // Check for voltage ID support.
  if (((dwFeature & VOLTAGEID_FREQUENCY) != 0) && m_Features.ExtendedFeatures.PowerManagement.HasVoltageID) bHasFeature = true;

  return bHasFeature;
}

void SystemInfo::Delay(unsigned int uiMS)
{
#ifdef WIN32
  LARGE_INTEGER Frequency, StartCounter, EndCounter;
  __int64 x;

  // Get the frequency of the high performance counter.
  if (!QueryPerformanceFrequency (&Frequency)) return;
  x = Frequency.QuadPart / 1000 * uiMS;

  // Get the starting position of the counter.
  QueryPerformanceCounter (&StartCounter);

  do {
    // Get the ending position of the counter.  
    QueryPerformanceCounter (&EndCounter);
    } while (EndCounter.QuadPart - StartCounter.QuadPart < x);
#endif
}

bool SystemInfo::DoesCPUSupportCPUID()
{
  int CPUIDPresent = 0;

#ifdef _WIN32 
  // Use SEH to determine CPUID presence
    __try {
        _asm {
#ifdef CPUID_AWARE_COMPILER
       ; we must push/pop the registers <<CPUID>> writes to, as the
      ; optimiser doesn't know about <<CPUID>>, and so doesn't expect
      ; these registers to change.
      push eax
      push ebx
      push ecx
      push edx
#endif
      ; <<CPUID>> 
            mov eax, 0
      CPUID_INSTRUCTION

#ifdef CPUID_AWARE_COMPILER
      pop edx
      pop ecx
      pop ebx
      pop eax
#endif
        }
    }
  __except(1) 
    {
    // Stop the class from trying to use CPUID again!
    CPUIDPresent = false;
    return false;
    }
#else
   CPUIDPresent = false;
#endif

  // Return true to indicate support or false to indicate lack.
  return (CPUIDPresent == 0) ? true : false;
}

bool SystemInfo::RetrieveCPUFeatures()
{
  int CPUFeatures = 0;
  int CPUAdvanced = 0;

#ifdef WIN32

  // Use assembly to detect CPUID information...
  __try {
    _asm {
#ifdef CPUID_AWARE_COMPILER
       ; we must push/pop the registers <<CPUID>> writes to, as the
      ; optimiser doesn't know about <<CPUID>>, and so doesn't expect
      ; these registers to change.
      push eax
      push ebx
      push ecx
      push edx
#endif
      ; <<CPUID>> 
      ; eax = 1 --> eax: CPU ID - bits 31..16 - unused, bits 15..12 - type, bits 11..8 - family, bits 7..4 - model, bits 3..0 - mask revision
      ;        ebx: 31..24 - default APIC ID, 23..16 - logical processsor ID, 15..8 - CFLUSH chunk size , 7..0 - brand ID
      ;        edx: CPU feature flags
      mov eax,1
      CPUID_INSTRUCTION
      mov CPUFeatures, edx
      mov CPUAdvanced, ebx

#ifdef CPUID_AWARE_COMPILER
      pop edx
      pop ecx
      pop ebx
      pop eax
#endif
    }
  }
  __except(1) 
    {
    return false;
    }

  // Retrieve the features of CPU present.
  m_Features.HasFPU =    ((CPUFeatures & 0x00000001) != 0);    // FPU Present --> Bit 0
  m_Features.HasTSC =    ((CPUFeatures & 0x00000010) != 0);    // TSC Present --> Bit 4
  m_Features.HasAPIC =    ((CPUFeatures & 0x00000200) != 0);    // APIC Present --> Bit 9
  m_Features.HasMTRR =    ((CPUFeatures & 0x00001000) != 0);    // MTRR Present --> Bit 12
  m_Features.HasCMOV =    ((CPUFeatures & 0x00008000) != 0);    // CMOV Present --> Bit 15
  m_Features.HasSerial =  ((CPUFeatures & 0x00040000) != 0);    // Serial Present --> Bit 18
  m_Features.HasACPI =    ((CPUFeatures & 0x00400000) != 0);    // ACPI Capable --> Bit 22
  m_Features.HasMMX =    ((CPUFeatures & 0x00800000) != 0);    // MMX Present --> Bit 23
  m_Features.HasSSE =    ((CPUFeatures & 0x02000000) != 0);    // SSE Present --> Bit 25
  m_Features.HasSSE2 =    ((CPUFeatures & 0x04000000) != 0);    // SSE2 Present --> Bit 26
  m_Features.HasThermal =  ((CPUFeatures & 0x20000000) != 0);    // Thermal Monitor Present --> Bit 29
  m_Features.HasIA64 =    ((CPUFeatures & 0x40000000) != 0);    // IA64 Present --> Bit 30

  // Retrieve extended SSE capabilities if SSE is available.
  if (m_Features.HasSSE) {
    
    // Attempt to __try some SSE FP instructions.
    __try 
      {
      // Perform: orps xmm0, xmm0
      _asm 
        {
        _emit 0x0f
        _emit 0x56
        _emit 0xc0  
        }

      // SSE FP capable processor.
      m_Features.HasSSEFP = true;
      }   
    __except(1) 
      {
      // bad instruction - processor or OS cannot handle SSE FP.
      m_Features.HasSSEFP = false;
      }
    } 
  else 
    {
    // Set the advanced SSE capabilities to not available.
    m_Features.HasSSEFP = false;
    }

  // Retrieve Intel specific extended features.
  if (m_ChipManufacturer == Intel) 
    {
    m_Features.ExtendedFeatures.SupportsHyperthreading =  ((CPUFeatures &  0x10000000) != 0);  // Intel specific: Hyperthreading --> Bit 28
    m_Features.ExtendedFeatures.LogicalProcessorsPerPhysical = (m_Features.ExtendedFeatures.SupportsHyperthreading) ? ((CPUAdvanced & 0x00FF0000) >> 16) : 1;
    
    if ((m_Features.ExtendedFeatures.SupportsHyperthreading) && (m_Features.HasAPIC))
      {
      // Retrieve APIC information if there is one present.
      m_Features.ExtendedFeatures.APIC_ID = ((CPUAdvanced & 0xFF000000) >> 24);
      }
    }
#endif
  return true;
}


/** Find the manufacturer given the vendor id */
void SystemInfo::FindManufacturer()
{
  if (strcmp (m_ChipID.Vendor, "GenuineIntel") == 0)    m_ChipManufacturer = Intel;        // Intel Corp.
  else if (strcmp (m_ChipID.Vendor, "UMC UMC UMC ") == 0)  m_ChipManufacturer = UMC;          // United Microelectronics Corp.
  else if (strcmp (m_ChipID.Vendor, "AuthenticAMD") == 0)  m_ChipManufacturer = AMD;          // Advanced Micro Devices
  else if (strcmp (m_ChipID.Vendor, "AMD ISBETTER") == 0)  m_ChipManufacturer = AMD;          // Advanced Micro Devices (1994)
  else if (strcmp (m_ChipID.Vendor, "CyrixInstead") == 0)  m_ChipManufacturer = Cyrix;        // Cyrix Corp., VIA Inc.
  else if (strcmp (m_ChipID.Vendor, "NexGenDriven") == 0)  m_ChipManufacturer = NexGen;        // NexGen Inc. (now AMD)
  else if (strcmp (m_ChipID.Vendor, "CentaurHauls") == 0)  m_ChipManufacturer = IDT;          // IDT/Centaur (now VIA)
  else if (strcmp (m_ChipID.Vendor, "RiseRiseRise") == 0)  m_ChipManufacturer = Rise;        // Rise
  else if (strcmp (m_ChipID.Vendor, "GenuineTMx86") == 0)  m_ChipManufacturer = Transmeta;      // Transmeta
  else if (strcmp (m_ChipID.Vendor, "TransmetaCPU") == 0)  m_ChipManufacturer = Transmeta;      // Transmeta
  else if (strcmp (m_ChipID.Vendor, "Geode By NSC") == 0)  m_ChipManufacturer = NSC;          // National Semiconductor
  else if (strcmp (m_ChipID.Vendor, "Sun") == 0)  m_ChipManufacturer = Sun;          // Sun Microelectronics
  else                          m_ChipManufacturer = UnknownManufacturer;  // Unknown manufacturer
}

/** */
bool SystemInfo::RetrieveCPUIdentity()
{
  int CPUVendor[3];
  int CPUSignature;

#ifdef WIN32
  // Use assembly to detect CPUID information...
  __try 
    {
    _asm 
      {
#ifdef CPUID_AWARE_COMPILER
       ; we must push/pop the registers <<CPUID>> writes to, as the
      ; optimiser doesn't know about <<CPUID>>, and so doesn't expect
      ; these registers to change.
      push eax
      push ebx
      push ecx
      push edx
#endif
      ; <<CPUID>>
      ; eax = 0 --> eax: maximum value of CPUID instruction.
      ;        ebx: part 1 of 3; CPU signature.
      ;        edx: part 2 of 3; CPU signature.
      ;        ecx: part 3 of 3; CPU signature.
      mov eax, 0
      CPUID_INSTRUCTION
      mov CPUVendor[0 * TYPE int], ebx
      mov CPUVendor[1 * TYPE int], edx
      mov CPUVendor[2 * TYPE int], ecx

      ; <<CPUID>> 
      ; eax = 1 --> eax: CPU ID - bits 31..16 - unused, bits 15..12 - type, bits 11..8 - family, bits 7..4 - model, bits 3..0 - mask revision
      ;        ebx: 31..24 - default APIC ID, 23..16 - logical processsor ID, 15..8 - CFLUSH chunk size , 7..0 - brand ID
      ;        edx: CPU feature flags
      mov eax,1
      CPUID_INSTRUCTION
      mov CPUSignature, eax

#ifdef CPUID_AWARE_COMPILER
      pop edx
      pop ecx
      pop ebx
      pop eax
#endif
    }
  }
  __except(1) 
    {
    return false;
    }

  // Process the returned information.
  memcpy (m_ChipID.Vendor, &(CPUVendor[0]), sizeof (int));
  memcpy (&(m_ChipID.Vendor[4]), &(CPUVendor[1]), sizeof (int));
  memcpy (&(m_ChipID.Vendor[8]), &(CPUVendor[2]), sizeof (int));
  m_ChipID.Vendor[12] = '\0';

  this->FindManufacturer();

  // Retrieve the family of CPU present.
  m_ChipID.ExtendedFamily =    ((CPUSignature & 0x0FF00000) >> 20);  // Bits 27..20 Used
  m_ChipID.ExtendedModel =    ((CPUSignature & 0x000F0000) >> 16);  // Bits 19..16 Used
  m_ChipID.Type =        ((CPUSignature & 0x0000F000) >> 12);  // Bits 15..12 Used
  m_ChipID.Family =        ((CPUSignature & 0x00000F00) >> 8);    // Bits 11..8 Used
  m_ChipID.Model =        ((CPUSignature & 0x000000F0) >> 4);    // Bits 7..4 Used
  m_ChipID.Revision =      ((CPUSignature & 0x0000000F) >> 0);    // Bits 3..0 Used
#endif

  return true;
}

/** */
bool SystemInfo::RetrieveCPUCacheDetails()
{
  int L1Cache[4] = { 0, 0, 0, 0 };
  int L2Cache[4] = { 0, 0, 0, 0 };

#ifdef WIN32
  // Check to see if what we are about to do is supported...
  if (RetrieveCPUExtendedLevelSupport (0x80000005)) 
    {
    // Use assembly to retrieve the L1 cache information ...
    __try 
      {
      _asm 
        {
#ifdef CPUID_AWARE_COMPILER
         ; we must push/pop the registers <<CPUID>> writes to, as the
        ; optimiser doesn't know about <<CPUID>>, and so doesn't expect
        ; these registers to change.
        push eax
        push ebx
        push ecx
        push edx
#endif
        ; <<CPUID>>
        ; eax = 0x80000005 --> eax: L1 cache information - Part 1 of 4.
        ;             ebx: L1 cache information - Part 2 of 4.
        ;             edx: L1 cache information - Part 3 of 4.
        ;              ecx: L1 cache information - Part 4 of 4.
        mov eax, 0x80000005
        CPUID_INSTRUCTION
        mov L1Cache[0 * TYPE int], eax
        mov L1Cache[1 * TYPE int], ebx
        mov L1Cache[2 * TYPE int], ecx
        mov L1Cache[3 * TYPE int], edx

#ifdef CPUID_AWARE_COMPILER
        pop edx
        pop ecx
        pop ebx
        pop eax
#endif
        }
      }
    __except(1) 
      {
      return false;
      }
    // Save the L1 data cache size (in KB) from ecx: bits 31..24 as well as data cache size from edx: bits 31..24.
    m_Features.L1CacheSize = ((L1Cache[2] & 0xFF000000) >> 24);
    m_Features.L1CacheSize += ((L1Cache[3] & 0xFF000000) >> 24);
    } 
  else 
    {
    // Store -1 to indicate the cache could not be queried.
    m_Features.L1CacheSize = -1;
    }

  // Check to see if what we are about to do is supported...
  if (RetrieveCPUExtendedLevelSupport (0x80000006)) 
    {
    // Use assembly to retrieve the L2 cache information ...
    __try 
      {
      _asm 
        {
#ifdef CPUID_AWARE_COMPILER
         ; we must push/pop the registers <<CPUID>> writes to, as the
        ; optimiser doesn't know about <<CPUID>>, and so doesn't expect
        ; these registers to change.
        push eax
        push ebx
        push ecx
        push edx
#endif
        ; <<CPUID>>
        ; eax = 0x80000006 --> eax: L2 cache information - Part 1 of 4.
        ;             ebx: L2 cache information - Part 2 of 4.
        ;             edx: L2 cache information - Part 3 of 4.
        ;              ecx: L2 cache information - Part 4 of 4.
        mov eax, 0x80000006
        CPUID_INSTRUCTION
        mov L2Cache[0 * TYPE int], eax
        mov L2Cache[1 * TYPE int], ebx
        mov L2Cache[2 * TYPE int], ecx
        mov L2Cache[3 * TYPE int], edx

#ifdef CPUID_AWARE_COMPILER
        pop edx
        pop ecx
        pop ebx
        pop eax
#endif
        }
      }
    __except(1) 
      {
      return false;
      }
    // Save the L2 unified cache size (in KB) from ecx: bits 31..16.
    m_Features.L2CacheSize = ((L2Cache[2] & 0xFFFF0000) >> 16);
    } 
  else
    {
    // Store -1 to indicate the cache could not be queried.
    m_Features.L2CacheSize = -1;
    }
  
  // Define L3 as being not present as we cannot test for it.
  m_Features.L3CacheSize = -1;

#endif

  // Return failure if we cannot detect either cache with this method.
  return ((m_Features.L1CacheSize == -1) && (m_Features.L2CacheSize == -1)) ? false : true;
}

/** */
bool SystemInfo::RetrieveClassicalCPUCacheDetails()
{
  int TLBCode = -1, TLBData = -1, L1Code = -1, L1Data = -1, L1Trace = -1, L2Unified = -1, L3Unified = -1;
  int TLBCacheData[4] = { 0, 0, 0, 0 };
  int TLBPassCounter = 0;
  int TLBCacheUnit = 0;

#ifdef WIN32

  do {
    // Use assembly to retrieve the L2 cache information ...
    __try {
      _asm {
#ifdef CPUID_AWARE_COMPILER
         ; we must push/pop the registers <<CPUID>> writes to, as the
        ; optimiser doesn't know about <<CPUID>>, and so doesn't expect
        ; these registers to change.
        push eax
        push ebx
        push ecx
        push edx
#endif
        ; <<CPUID>>
        ; eax = 2 --> eax: TLB and cache information - Part 1 of 4.
        ;        ebx: TLB and cache information - Part 2 of 4.
        ;        ecx: TLB and cache information - Part 3 of 4.
        ;        edx: TLB and cache information - Part 4 of 4.
        mov eax, 2
        CPUID_INSTRUCTION
        mov TLBCacheData[0 * TYPE int], eax
        mov TLBCacheData[1 * TYPE int], ebx
        mov TLBCacheData[2 * TYPE int], ecx
        mov TLBCacheData[3 * TYPE int], edx

#ifdef CPUID_AWARE_COMPILER
        pop edx
        pop ecx
        pop ebx
        pop eax
#endif
        }
      }
    __except(1)
      {
      return false;
      }

    int bob = ((TLBCacheData[0] & 0x00FF0000) >> 16);

    // Process the returned TLB and cache information.
    for (int nCounter = 0; nCounter < TLBCACHE_INFO_UNITS; nCounter ++) 
      {
      // First of all - decide which unit we are dealing with.
      switch (nCounter) 
        {
        // eax: bits 8..15 : bits 16..23 : bits 24..31
        case 0: TLBCacheUnit = ((TLBCacheData[0] & 0x0000FF00) >> 8); break;
        case 1: TLBCacheUnit = ((TLBCacheData[0] & 0x00FF0000) >> 16); break;
        case 2: TLBCacheUnit = ((TLBCacheData[0] & 0xFF000000) >> 24); break;

        // ebx: bits 0..7 : bits 8..15 : bits 16..23 : bits 24..31
        case 3: TLBCacheUnit = ((TLBCacheData[1] & 0x000000FF) >> 0); break;
        case 4: TLBCacheUnit = ((TLBCacheData[1] & 0x0000FF00) >> 8); break;
        case 5: TLBCacheUnit = ((TLBCacheData[1] & 0x00FF0000) >> 16); break;
        case 6: TLBCacheUnit = ((TLBCacheData[1] & 0xFF000000) >> 24); break;

        // ecx: bits 0..7 : bits 8..15 : bits 16..23 : bits 24..31
        case 7: TLBCacheUnit = ((TLBCacheData[2] & 0x000000FF) >> 0); break;
        case 8: TLBCacheUnit = ((TLBCacheData[2] & 0x0000FF00) >> 8); break;
        case 9: TLBCacheUnit = ((TLBCacheData[2] & 0x00FF0000) >> 16); break;
        case 10: TLBCacheUnit = ((TLBCacheData[2] & 0xFF000000) >> 24); break;

        // edx: bits 0..7 : bits 8..15 : bits 16..23 : bits 24..31
        case 11: TLBCacheUnit = ((TLBCacheData[3] & 0x000000FF) >> 0); break;
        case 12: TLBCacheUnit = ((TLBCacheData[3] & 0x0000FF00) >> 8); break;
        case 13: TLBCacheUnit = ((TLBCacheData[3] & 0x00FF0000) >> 16); break;
        case 14: TLBCacheUnit = ((TLBCacheData[3] & 0xFF000000) >> 24); break;

        // Default case - an error has occured.
        default: return false;
        }

      // Now process the resulting unit to see what it means....
      switch (TLBCacheUnit) 
        {
        case 0x00: break;
        case 0x01: STORE_TLBCACHE_INFO (TLBCode, 4); break;
        case 0x02: STORE_TLBCACHE_INFO (TLBCode, 4096); break;
        case 0x03: STORE_TLBCACHE_INFO (TLBData, 4); break;
        case 0x04: STORE_TLBCACHE_INFO (TLBData, 4096); break;
        case 0x06: STORE_TLBCACHE_INFO (L1Code, 8); break;
        case 0x08: STORE_TLBCACHE_INFO (L1Code, 16); break;
        case 0x0a: STORE_TLBCACHE_INFO (L1Data, 8); break;
        case 0x0c: STORE_TLBCACHE_INFO (L1Data, 16); break;
        case 0x10: STORE_TLBCACHE_INFO (L1Data, 16); break;      // <-- FIXME: IA-64 Only
        case 0x15: STORE_TLBCACHE_INFO (L1Code, 16); break;      // <-- FIXME: IA-64 Only
        case 0x1a: STORE_TLBCACHE_INFO (L2Unified, 96); break;    // <-- FIXME: IA-64 Only
        case 0x22: STORE_TLBCACHE_INFO (L3Unified, 512); break;
        case 0x23: STORE_TLBCACHE_INFO (L3Unified, 1024); break;
        case 0x25: STORE_TLBCACHE_INFO (L3Unified, 2048); break;
        case 0x29: STORE_TLBCACHE_INFO (L3Unified, 4096); break;
        case 0x39: STORE_TLBCACHE_INFO (L2Unified, 128); break;
        case 0x3c: STORE_TLBCACHE_INFO (L2Unified, 256); break;
        case 0x40: STORE_TLBCACHE_INFO (L2Unified, 0); break;    // <-- FIXME: No integrated L2 cache (P6 core) or L3 cache (P4 core).
        case 0x41: STORE_TLBCACHE_INFO (L2Unified, 128); break;
        case 0x42: STORE_TLBCACHE_INFO (L2Unified, 256); break;
        case 0x43: STORE_TLBCACHE_INFO (L2Unified, 512); break;
        case 0x44: STORE_TLBCACHE_INFO (L2Unified, 1024); break;
        case 0x45: STORE_TLBCACHE_INFO (L2Unified, 2048); break;
        case 0x50: STORE_TLBCACHE_INFO (TLBCode, 4096); break;
        case 0x51: STORE_TLBCACHE_INFO (TLBCode, 4096); break;
        case 0x52: STORE_TLBCACHE_INFO (TLBCode, 4096); break;
        case 0x5b: STORE_TLBCACHE_INFO (TLBData, 4096); break;
        case 0x5c: STORE_TLBCACHE_INFO (TLBData, 4096); break;
        case 0x5d: STORE_TLBCACHE_INFO (TLBData, 4096); break;
        case 0x66: STORE_TLBCACHE_INFO (L1Data, 8); break;
        case 0x67: STORE_TLBCACHE_INFO (L1Data, 16); break;
        case 0x68: STORE_TLBCACHE_INFO (L1Data, 32); break;
        case 0x70: STORE_TLBCACHE_INFO (L1Trace, 12); break;
        case 0x71: STORE_TLBCACHE_INFO (L1Trace, 16); break;
        case 0x72: STORE_TLBCACHE_INFO (L1Trace, 32); break;
        case 0x77: STORE_TLBCACHE_INFO (L1Code, 16); break;      // <-- FIXME: IA-64 Only
        case 0x79: STORE_TLBCACHE_INFO (L2Unified, 128); break;
        case 0x7a: STORE_TLBCACHE_INFO (L2Unified, 256); break;
        case 0x7b: STORE_TLBCACHE_INFO (L2Unified, 512); break;
        case 0x7c: STORE_TLBCACHE_INFO (L2Unified, 1024); break;
        case 0x7e: STORE_TLBCACHE_INFO (L2Unified, 256); break;
        case 0x81: STORE_TLBCACHE_INFO (L2Unified, 128); break;
        case 0x82: STORE_TLBCACHE_INFO (L2Unified, 256); break;
        case 0x83: STORE_TLBCACHE_INFO (L2Unified, 512); break;
        case 0x84: STORE_TLBCACHE_INFO (L2Unified, 1024); break;
        case 0x85: STORE_TLBCACHE_INFO (L2Unified, 2048); break;
        case 0x88: STORE_TLBCACHE_INFO (L3Unified, 2048); break;  // <-- FIXME: IA-64 Only
        case 0x89: STORE_TLBCACHE_INFO (L3Unified, 4096); break;  // <-- FIXME: IA-64 Only
        case 0x8a: STORE_TLBCACHE_INFO (L3Unified, 8192); break;  // <-- FIXME: IA-64 Only
        case 0x8d: STORE_TLBCACHE_INFO (L3Unified, 3096); break;  // <-- FIXME: IA-64 Only
        case 0x90: STORE_TLBCACHE_INFO (TLBCode, 262144); break;  // <-- FIXME: IA-64 Only
        case 0x96: STORE_TLBCACHE_INFO (TLBCode, 262144); break;  // <-- FIXME: IA-64 Only
        case 0x9b: STORE_TLBCACHE_INFO (TLBCode, 262144); break;  // <-- FIXME: IA-64 Only
        
        // Default case - an error has occured.
        default: return false;
        }
      }

    // Increment the TLB pass counter.
    TLBPassCounter ++;
    } while ((TLBCacheData[0] & 0x000000FF) > TLBPassCounter);

  // Ok - we now have the maximum TLB, L1, L2, and L3 sizes...
  if ((L1Code == -1) && (L1Data == -1) && (L1Trace == -1)) 
    {
    m_Features.L1CacheSize = -1;
    }
  else if ((L1Code == -1) && (L1Data == -1) && (L1Trace != -1)) 
    {
    m_Features.L1CacheSize = L1Trace;
    }
  else if ((L1Code != -1) && (L1Data == -1)) 
    {
    m_Features.L1CacheSize = L1Code;
    }
  else if ((L1Code == -1) && (L1Data != -1)) 
    {
    m_Features.L1CacheSize = L1Data;
    }
  else if ((L1Code != -1) && (L1Data != -1)) 
    {
    m_Features.L1CacheSize = L1Code + L1Data;
    }
  else 
    {
    m_Features.L1CacheSize = -1;
    }

  // Ok - we now have the maximum TLB, L1, L2, and L3 sizes...
  if (L2Unified == -1) 
    {
    m_Features.L2CacheSize = -1;
    }
  else 
    {
    m_Features.L2CacheSize = L2Unified;
    }

  // Ok - we now have the maximum TLB, L1, L2, and L3 sizes...
  if (L3Unified == -1) 
    {
    m_Features.L3CacheSize = -1;
    }
  else 
    {
    m_Features.L3CacheSize = L3Unified;
    }

#endif
  return true;
}

/** */
bool SystemInfo::RetrieveCPUClockSpeed()
{
#ifdef WIN32
  // First of all we check to see if the RDTSC (0x0F, 0x31) instruction is supported.
  if (!m_Features.HasTSC) 
    {
    return false;
    }

  unsigned int uiRepetitions = 1;
  unsigned int uiMSecPerRepetition = 50;
  __int64  i64Total = 0;
  __int64 i64Overhead = 0;

  for (unsigned int nCounter = 0; nCounter < uiRepetitions; nCounter ++) 
    {
    i64Total += GetCyclesDifference (SystemInfo::Delay, uiMSecPerRepetition);
    i64Overhead += GetCyclesDifference (SystemInfo::DelayOverhead, uiMSecPerRepetition);
    }

  // Calculate the MHz speed.
  i64Total -= i64Overhead;
  i64Total /= uiRepetitions;
  i64Total /= uiMSecPerRepetition;
  i64Total /= 1000;

  // Save the CPU speed.
  m_CPUSpeedInMHz = (float) i64Total;

  return true;
#endif
  return false;
}

/** */
bool SystemInfo::RetrieveClassicalCPUClockSpeed()
{
#ifdef WIN32
  LARGE_INTEGER liStart, liEnd, liCountsPerSecond;
  double dFrequency, dDifference;

  // Attempt to get a starting tick count.
  QueryPerformanceCounter (&liStart);

  __try 
    {
    _asm 
      {
      mov eax, 0x80000000
      mov ebx, CLASSICAL_CPU_FREQ_LOOP
      Timer_Loop: 
      bsf ecx,eax
      dec ebx
      jnz Timer_Loop
      }  
    }
  __except(1) 
    {
    return false;
    }

  // Attempt to get a starting tick count.
  QueryPerformanceCounter (&liEnd);

  // Get the difference...  NB: This is in seconds....
  QueryPerformanceFrequency (&liCountsPerSecond);
  dDifference = (((double) liEnd.QuadPart - (double) liStart.QuadPart) / (double) liCountsPerSecond.QuadPart);

  // Calculate the clock speed.
  if (m_ChipID.Family == 3) 
    {
    // 80386 processors....  Loop time is 115 cycles!
    dFrequency = (((CLASSICAL_CPU_FREQ_LOOP * 115) / dDifference) / 1048576);
    } 
  else if (m_ChipID.Family == 4) 
    {
    // 80486 processors....  Loop time is 47 cycles!
    dFrequency = (((CLASSICAL_CPU_FREQ_LOOP * 47) / dDifference) / 1048576);
    } 
  else if (m_ChipID.Family == 5) 
    {
    // Pentium processors....  Loop time is 43 cycles!
    dFrequency = (((CLASSICAL_CPU_FREQ_LOOP * 43) / dDifference) / 1048576);
    }
  
  // Save the clock speed.
  m_Features.CPUSpeed = (int) dFrequency;
#endif
  return true;
}

/** */
bool SystemInfo::RetrieveCPUExtendedLevelSupport(int CPULevelToCheck)
{
  int MaxCPUExtendedLevel = 0;

  // The extended CPUID is supported by various vendors starting with the following CPU models: 
  //
  //    Manufacturer & Chip Name      |    Family     Model    Revision
  //
  //    AMD K6, K6-2                  |       5       6      x
  //    Cyrix GXm, Cyrix III "Joshua" |       5       4      x
  //    IDT C6-2                      |       5       8      x
  //    VIA Cyrix III                 |       6       5      x
  //    Transmeta Crusoe              |       5       x      x
  //    Intel Pentium 4               |       f       x      x
  //

  // We check to see if a supported processor is present...
  if (m_ChipManufacturer == AMD) 
    {
    if (m_ChipID.Family < 5) return false;
    if ((m_ChipID.Family == 5) && (m_ChipID.Model < 6)) return false;
    } 
  else if (m_ChipManufacturer == Cyrix) 
    {
    if (m_ChipID.Family < 5) return false;
    if ((m_ChipID.Family == 5) && (m_ChipID.Model < 4)) return false;
    if ((m_ChipID.Family == 6) && (m_ChipID.Model < 5)) return false;
    } 
  else if (m_ChipManufacturer == IDT) 
    {
    if (m_ChipID.Family < 5) return false;
    if ((m_ChipID.Family == 5) && (m_ChipID.Model < 8)) return false;
    } 
  else if (m_ChipManufacturer == Transmeta) 
    {
    if (m_ChipID.Family < 5) return false;
    } 
  else if (m_ChipManufacturer == Intel) 
    {
    if (m_ChipID.Family < 0xf)
      {
      return false;
      }
    }
    
#ifdef WIN32

  // Use assembly to detect CPUID information...
  __try {
    _asm {
#ifdef CPUID_AWARE_COMPILER
       ; we must push/pop the registers <<CPUID>> writes to, as the
      ; optimiser doesn't know about <<CPUID>>, and so doesn't expect
      ; these registers to change.
      push eax
      push ebx
      push ecx
      push edx
#endif
      ; <<CPUID>> 
      ; eax = 0x80000000 --> eax: maximum supported extended level
      mov eax,0x80000000
      CPUID_INSTRUCTION
      mov MaxCPUExtendedLevel, eax

#ifdef CPUID_AWARE_COMPILER
      pop edx
      pop ecx
      pop ebx
      pop eax
#endif
    }
  }
  __except(1) 
    {
    return false;
    }
#endif

  // Now we have to check the level wanted vs level returned...
  int nLevelWanted = (CPULevelToCheck & 0x7FFFFFFF);
  int nLevelReturn = (MaxCPUExtendedLevel & 0x7FFFFFFF);

  // Check to see if the level provided is supported...
  if (nLevelWanted > nLevelReturn)
    {
    return false;
    }

  return true;
}

/** */
bool SystemInfo::RetrieveExtendedCPUFeatures()
{
  int CPUExtendedFeatures = 0;

  // Check that we are not using an Intel processor as it does not support this.
  if (m_ChipManufacturer == Intel) 
    {
    return false;
    }

  // Check to see if what we are about to do is supported...
  if (!RetrieveCPUExtendedLevelSupport (0x80000001)) 
    {
    return false;
    }
#ifdef WIN32

  // Use assembly to detect CPUID information...
  __try 
    {
    _asm 
      {
#ifdef CPUID_AWARE_COMPILER
       ; we must push/pop the registers <<CPUID>> writes to, as the
      ; optimiser doesn't know about <<CPUID>>, and so doesn't expect
      ; these registers to change.
      push eax
      push ebx
      push ecx
      push edx
#endif
      ; <<CPUID>> 
      ; eax = 0x80000001 --> eax: CPU ID - bits 31..16 - unused, bits 15..12 - type, bits 11..8 - family, bits 7..4 - model, bits 3..0 - mask revision
      ;             ebx: 31..24 - default APIC ID, 23..16 - logical processsor ID, 15..8 - CFLUSH chunk size , 7..0 - brand ID
      ;             edx: CPU feature flags
      mov eax,0x80000001
      CPUID_INSTRUCTION
      mov CPUExtendedFeatures, edx

#ifdef CPUID_AWARE_COMPILER
      pop edx
      pop ecx
      pop ebx
      pop eax
#endif
      }
    }
  __except(1) 
    {
    return false;
    }

  // Retrieve the extended features of CPU present.
  m_Features.ExtendedFeatures.Has3DNow = ((CPUExtendedFeatures & 0x80000000) != 0);  // 3DNow Present --> Bit 31.
  m_Features.ExtendedFeatures.Has3DNowPlus = ((CPUExtendedFeatures & 0x40000000) != 0);  // 3DNow+ Present -- > Bit 30.
  m_Features.ExtendedFeatures.HasSSEMMX = ((CPUExtendedFeatures & 0x00400000) != 0);  // SSE MMX Present --> Bit 22.
  m_Features.ExtendedFeatures.SupportsMP = ((CPUExtendedFeatures & 0x00080000) != 0);  // MP Capable -- > Bit 19.
  
  // Retrieve AMD specific extended features.
  if (m_ChipManufacturer == AMD) 
    {
    m_Features.ExtendedFeatures.HasMMXPlus = ((CPUExtendedFeatures &  0x00400000) != 0);  // AMD specific: MMX-SSE --> Bit 22
    }

  // Retrieve Cyrix specific extended features.
  if (m_ChipManufacturer == Cyrix) 
    {
    m_Features.ExtendedFeatures.HasMMXPlus = ((CPUExtendedFeatures &  0x01000000) != 0);  // Cyrix specific: Extended MMX --> Bit 24
    }
#endif

  return true;
}

/** */
bool SystemInfo::RetrieveProcessorSerialNumber()
{
  int SerialNumber[3];

  // Check to see if the processor supports the processor serial number.
  if (!m_Features.HasSerial)
    {
    return false;
    }

#ifdef WIN32

    // Use assembly to detect CPUID information...
  __try {
    _asm {
#ifdef CPUID_AWARE_COMPILER
       ; we must push/pop the registers <<CPUID>> writes to, as the
      ; optimiser doesn't know about <<CPUID>>, and so doesn't expect
      ; these registers to change.
      push eax
      push ebx
      push ecx
      push edx
#endif
      ; <<CPUID>>
      ; eax = 3 --> ebx: top 32 bits are the processor signature bits --> NB: Transmeta only ?!?
      ;        ecx: middle 32 bits are the processor signature bits
      ;        edx: bottom 32 bits are the processor signature bits
      mov eax, 3
      CPUID_INSTRUCTION
      mov SerialNumber[0 * TYPE int], ebx
      mov SerialNumber[1 * TYPE int], ecx
      mov SerialNumber[2 * TYPE int], edx

#ifdef CPUID_AWARE_COMPILER
      pop edx
      pop ecx
      pop ebx
      pop eax
#endif
    }
  }
  __except(1) 
    {
    return false;
    }

  // Process the returned information.
  sprintf (m_ChipID.SerialNumber, "%.2x%.2x-%.2x%.2x-%.2x%.2x-%.2x%.2x-%.2x%.2x-%.2x%.2x",
       ((SerialNumber[0] & 0xff000000) >> 24),
       ((SerialNumber[0] & 0x00ff0000) >> 16),
       ((SerialNumber[0] & 0x0000ff00) >> 8),
       ((SerialNumber[0] & 0x000000ff) >> 0),
       ((SerialNumber[1] & 0xff000000) >> 24),
       ((SerialNumber[1] & 0x00ff0000) >> 16),
       ((SerialNumber[1] & 0x0000ff00) >> 8),
       ((SerialNumber[1] & 0x000000ff) >> 0),
       ((SerialNumber[2] & 0xff000000) >> 24),
       ((SerialNumber[2] & 0x00ff0000) >> 16),
       ((SerialNumber[2] & 0x0000ff00) >> 8),
       ((SerialNumber[2] & 0x000000ff) >> 0));
#endif

  return true;
}

/** */
bool SystemInfo::RetrieveCPUPowerManagement()
{  
  int CPUPowerManagement = 0;

  // Check to see if what we are about to do is supported...
  if (!RetrieveCPUExtendedLevelSupport (0x80000007)) 
    {
    m_Features.ExtendedFeatures.PowerManagement.HasFrequencyID = false;
    m_Features.ExtendedFeatures.PowerManagement.HasVoltageID = false;
    m_Features.ExtendedFeatures.PowerManagement.HasTempSenseDiode = false;
    return false;
    }

#ifdef WIN32

  // Use assembly to detect CPUID information...
  __try {
    _asm {
#ifdef CPUID_AWARE_COMPILER
       ; we must push/pop the registers <<CPUID>> writes to, as the
      ; optimiser doesn't know about <<CPUID>>, and so doesn't expect
      ; these registers to change.
      push eax
      push ebx
      push ecx
      push edx
#endif
      ; <<CPUID>> 
      ; eax = 0x80000007 --> edx: get processor power management
      mov eax,0x80000007
      CPUID_INSTRUCTION
      mov CPUPowerManagement, edx
      
#ifdef CPUID_AWARE_COMPILER
      pop edx
      pop ecx
      pop ebx
      pop eax
#endif
    }
  }
  __except(1) 
    {
    return false;
    }

  // Check for the power management capabilities of the CPU.
  m_Features.ExtendedFeatures.PowerManagement.HasTempSenseDiode =  ((CPUPowerManagement & 0x00000001) != 0);
  m_Features.ExtendedFeatures.PowerManagement.HasFrequencyID =    ((CPUPowerManagement & 0x00000002) != 0);
  m_Features.ExtendedFeatures.PowerManagement.HasVoltageID =    ((CPUPowerManagement & 0x00000004) != 0);

#endif

  return true;
}

/** */
bool SystemInfo::RetrieveExtendedCPUIdentity()
{
  int ProcessorNameStartPos = 0;
  int CPUExtendedIdentity[12];

  // Check to see if what we are about to do is supported...
  if (!RetrieveCPUExtendedLevelSupport(0x80000002)) return false;
  if (!RetrieveCPUExtendedLevelSupport(0x80000003)) return false;
  if (!RetrieveCPUExtendedLevelSupport(0x80000004)) return false;
   
#ifdef WIN32
  // Use assembly to detect CPUID information...
  __try {
    _asm {
#ifdef CPUID_AWARE_COMPILER
       ; we must push/pop the registers <<CPUID>> writes to, as the
      ; optimiser doesn't know about <<CPUID>>, and so doesn't expect
      ; these registers to change.
      push eax
      push ebx
      push ecx
      push edx
#endif
      ; <<CPUID>> 
      ; eax = 0x80000002 --> eax, ebx, ecx, edx: get processor name string (part 1)
      mov eax,0x80000002
      CPUID_INSTRUCTION
      mov CPUExtendedIdentity[0 * TYPE int], eax
      mov CPUExtendedIdentity[1 * TYPE int], ebx
      mov CPUExtendedIdentity[2 * TYPE int], ecx
      mov CPUExtendedIdentity[3 * TYPE int], edx

      ; <<CPUID>> 
      ; eax = 0x80000003 --> eax, ebx, ecx, edx: get processor name string (part 2)
      mov eax,0x80000003
      CPUID_INSTRUCTION
      mov CPUExtendedIdentity[4 * TYPE int], eax
      mov CPUExtendedIdentity[5 * TYPE int], ebx
      mov CPUExtendedIdentity[6 * TYPE int], ecx
      mov CPUExtendedIdentity[7 * TYPE int], edx

      ; <<CPUID>> 
      ; eax = 0x80000004 --> eax, ebx, ecx, edx: get processor name string (part 3)
      mov eax,0x80000004
      CPUID_INSTRUCTION
      mov CPUExtendedIdentity[8 * TYPE int], eax
      mov CPUExtendedIdentity[9 * TYPE int], ebx
      mov CPUExtendedIdentity[10 * TYPE int], ecx
      mov CPUExtendedIdentity[11 * TYPE int], edx

#ifdef CPUID_AWARE_COMPILER
      pop edx
      pop ecx
      pop ebx
      pop eax
#endif
    }
  }
  __except(1) 
    {
    return false;
    }

  // Process the returned information.
  memcpy (m_ChipID.ProcessorName, &(CPUExtendedIdentity[0]), sizeof (int));
  memcpy (&(m_ChipID.ProcessorName[4]), &(CPUExtendedIdentity[1]), sizeof (int));
  memcpy (&(m_ChipID.ProcessorName[8]), &(CPUExtendedIdentity[2]), sizeof (int));
  memcpy (&(m_ChipID.ProcessorName[12]), &(CPUExtendedIdentity[3]), sizeof (int));
  memcpy (&(m_ChipID.ProcessorName[16]), &(CPUExtendedIdentity[4]), sizeof (int));
  memcpy (&(m_ChipID.ProcessorName[20]), &(CPUExtendedIdentity[5]), sizeof (int));
  memcpy (&(m_ChipID.ProcessorName[24]), &(CPUExtendedIdentity[6]), sizeof (int));
  memcpy (&(m_ChipID.ProcessorName[28]), &(CPUExtendedIdentity[7]), sizeof (int));
  memcpy (&(m_ChipID.ProcessorName[32]), &(CPUExtendedIdentity[8]), sizeof (int));
  memcpy (&(m_ChipID.ProcessorName[36]), &(CPUExtendedIdentity[9]), sizeof (int));
  memcpy (&(m_ChipID.ProcessorName[40]), &(CPUExtendedIdentity[10]), sizeof (int));
  memcpy (&(m_ChipID.ProcessorName[44]), &(CPUExtendedIdentity[11]), sizeof (int));
  m_ChipID.ProcessorName[48] = '\0';

  // Because some manufacturers have leading white space - we have to post-process the name.
  if (m_ChipManufacturer == Intel) 
    {
    for (int nCounter = 0; nCounter < CHIPNAME_STRING_LENGTH; nCounter ++) 
      {
      // There will either be NULL (\0) or spaces ( ) as the leading characters.
      if ((m_ChipID.ProcessorName[nCounter] != '\0') && (m_ChipID.ProcessorName[nCounter] != ' ')) 
        {
        // We have found the starting position of the name.
        ProcessorNameStartPos = nCounter;
        // Terminate the loop.
        break;
        }
      }

    // Check to see if there is any white space at the start.
    if (ProcessorNameStartPos == 0) 
      {
      return true;
      }

    // Now move the name forward so that there is no white space.
    memmove(m_ChipID.ProcessorName, &(m_ChipID.ProcessorName[ProcessorNameStartPos]), (CHIPNAME_STRING_LENGTH - ProcessorNameStartPos));
   }
#endif

  return true;
}

/** */
bool SystemInfo::RetrieveClassicalCPUIdentity()
{
  // Start by decided which manufacturer we are using....
  switch (m_ChipManufacturer) 
    {
    case Intel:
      // Check the family / model / revision to determine the CPU ID.
      switch (m_ChipID.Family) {
        case 3:
          sprintf (m_ChipID.ProcessorName, "Newer i80386 family"); 
          break;
        case 4:
          switch (m_ChipID.Model) {
            case 0: sprintf (m_ChipID.ProcessorName,"i80486DX-25/33"); break;
            case 1: sprintf (m_ChipID.ProcessorName,"i80486DX-50"); break;
            case 2: sprintf (m_ChipID.ProcessorName,"i80486SX"); break;
            case 3: sprintf (m_ChipID.ProcessorName,"i80486DX2"); break;
            case 4: sprintf (m_ChipID.ProcessorName,"i80486SL"); break;
            case 5: sprintf (m_ChipID.ProcessorName,"i80486SX2"); break;
            case 7: sprintf (m_ChipID.ProcessorName,"i80486DX2 WriteBack"); break;
            case 8: sprintf (m_ChipID.ProcessorName,"i80486DX4"); break;
            case 9: sprintf (m_ChipID.ProcessorName,"i80486DX4 WriteBack"); break;
            default: sprintf (m_ChipID.ProcessorName,"Unknown 80486 family"); return false;
            }
          break;
        case 5:
          switch (m_ChipID.Model) 
            {
            case 0: sprintf (m_ChipID.ProcessorName,"P5 A-Step"); break;
            case 1: sprintf (m_ChipID.ProcessorName,"P5"); break;
            case 2: sprintf (m_ChipID.ProcessorName,"P54C"); break;
            case 3: sprintf (m_ChipID.ProcessorName,"P24T OverDrive"); break;
            case 4: sprintf (m_ChipID.ProcessorName,"P55C"); break;
            case 7: sprintf (m_ChipID.ProcessorName,"P54C"); break;
            case 8: sprintf (m_ChipID.ProcessorName,"P55C (0.25�m)"); break;
            default: sprintf (m_ChipID.ProcessorName,"Unknown Pentium family"); return false;
            }
          break;
        case 6:
          switch (m_ChipID.Model) 
            {
            case 0: sprintf (m_ChipID.ProcessorName,"P6 A-Step"); break;
            case 1: sprintf (m_ChipID.ProcessorName,"P6"); break;
            case 3: sprintf (m_ChipID.ProcessorName,"Pentium II (0.28 �m)"); break;
            case 5: sprintf (m_ChipID.ProcessorName,"Pentium II (0.25 �m)"); break;
            case 6: sprintf (m_ChipID.ProcessorName,"Pentium II With On-Die L2 Cache"); break;
            case 7: sprintf (m_ChipID.ProcessorName,"Pentium III (0.25 �m)"); break;
            case 8: sprintf (m_ChipID.ProcessorName,"Pentium III (0.18 �m) With 256 KB On-Die L2 Cache "); break;
            case 0xa: sprintf (m_ChipID.ProcessorName,"Pentium III (0.18 �m) With 1 Or 2 MB On-Die L2 Cache "); break;
            case 0xb: sprintf (m_ChipID.ProcessorName,"Pentium III (0.13 �m) With 256 Or 512 KB On-Die L2 Cache "); break;
            default: sprintf (m_ChipID.ProcessorName,"Unknown P6 family"); return false;
            }
          break;
        case 7:
          sprintf (m_ChipID.ProcessorName,"Intel Merced (IA-64)");
          break;
        case 0xf:
          // Check the extended family bits...
          switch (m_ChipID.ExtendedFamily) 
            {
            case 0:
              switch (m_ChipID.Model) 
                {
                case 0: sprintf (m_ChipID.ProcessorName,"Pentium IV (0.18 �m)"); break;
                case 1: sprintf (m_ChipID.ProcessorName,"Pentium IV (0.18 �m)"); break;
                case 2: sprintf (m_ChipID.ProcessorName,"Pentium IV (0.13 �m)"); break;
                default: sprintf (m_ChipID.ProcessorName,"Unknown Pentium 4 family"); return false;
                }
              break;
            case 1:
              sprintf (m_ChipID.ProcessorName,"Intel McKinley (IA-64)");
              break;
            default:
              sprintf (m_ChipID.ProcessorName,"Pentium D");
            }
          break;
        default:
          sprintf (m_ChipID.ProcessorName,"Unknown Intel family");
          return false;
        }
      break;

    case AMD:
      // Check the family / model / revision to determine the CPU ID.
      switch (m_ChipID.Family) 
        {
        case 4:
          switch (m_ChipID.Model) 
            {
            case 3: sprintf (m_ChipID.ProcessorName,"80486DX2"); break;
            case 7: sprintf (m_ChipID.ProcessorName,"80486DX2 WriteBack"); break;
            case 8: sprintf (m_ChipID.ProcessorName,"80486DX4"); break;
            case 9: sprintf (m_ChipID.ProcessorName,"80486DX4 WriteBack"); break;
            case 0xe: sprintf (m_ChipID.ProcessorName,"5x86"); break;
            case 0xf: sprintf (m_ChipID.ProcessorName,"5x86WB"); break;
            default: sprintf (m_ChipID.ProcessorName,"Unknown 80486 family"); return false;
            }
          break;
        case 5:
          switch (m_ChipID.Model) 
            {
            case 0: sprintf (m_ChipID.ProcessorName,"SSA5 (PR75, PR90, PR100)"); break;
            case 1: sprintf (m_ChipID.ProcessorName,"5k86 (PR120, PR133)"); break;
            case 2: sprintf (m_ChipID.ProcessorName,"5k86 (PR166)"); break;
            case 3: sprintf (m_ChipID.ProcessorName,"5k86 (PR200)"); break;
            case 6: sprintf (m_ChipID.ProcessorName,"K6 (0.30 �m)"); break;
            case 7: sprintf (m_ChipID.ProcessorName,"K6 (0.25 �m)"); break;
            case 8: sprintf (m_ChipID.ProcessorName,"K6-2"); break;
            case 9: sprintf (m_ChipID.ProcessorName,"K6-III"); break;
            case 0xd: sprintf (m_ChipID.ProcessorName,"K6-2+ or K6-III+ (0.18 �m)"); break;
            default: sprintf (m_ChipID.ProcessorName,"Unknown 80586 family"); return false;
            }
          break;
        case 6:
          switch (m_ChipID.Model) 
            {
            case 1: sprintf (m_ChipID.ProcessorName,"Athlon� (0.25 �m)"); break;
            case 2: sprintf (m_ChipID.ProcessorName,"Athlon� (0.18 �m)"); break;
            case 3: sprintf (m_ChipID.ProcessorName,"Duron� (SF core)"); break;
            case 4: sprintf (m_ChipID.ProcessorName,"Athlon� (Thunderbird core)"); break;
            case 6: sprintf (m_ChipID.ProcessorName,"Athlon� (Palomino core)"); break;
            case 7: sprintf (m_ChipID.ProcessorName,"Duron� (Morgan core)"); break;
            case 8: 
              if (m_Features.ExtendedFeatures.SupportsMP)
                sprintf (m_ChipID.ProcessorName,"Athlon� MP (Thoroughbred core)"); 
              else sprintf (m_ChipID.ProcessorName,"Athlon� XP (Thoroughbred core)");
              break;
            default: sprintf (m_ChipID.ProcessorName,"Unknown K7 family"); return false;
            }
          break;
        default:
          sprintf (m_ChipID.ProcessorName,"Unknown AMD family");
          return false;
        }
      break;

    case Transmeta:
      switch (m_ChipID.Family) 
        {  
        case 5:
          switch (m_ChipID.Model) 
            {
            case 4: sprintf (m_ChipID.ProcessorName,"Crusoe TM3x00 and TM5x00"); break;
            default: sprintf (m_ChipID.ProcessorName,"Unknown Crusoe family"); return false;
            }
          break;
        default:
          sprintf (m_ChipID.ProcessorName,"Unknown Transmeta family");
          return false;
        }
      break;

    case Rise:
      switch (m_ChipID.Family) 
        {  
        case 5:
          switch (m_ChipID.Model) 
            {
            case 0: sprintf (m_ChipID.ProcessorName,"mP6 (0.25 �m)"); break;
            case 2: sprintf (m_ChipID.ProcessorName,"mP6 (0.18 �m)"); break;
            default: sprintf (m_ChipID.ProcessorName,"Unknown Rise family"); return false;
            }
          break;
        default:
          sprintf (m_ChipID.ProcessorName,"Unknown Rise family");
          return false;
        }
      break;

    case UMC:
      switch (m_ChipID.Family) 
        {  
        case 4:
          switch (m_ChipID.Model) 
            {
            case 1: sprintf (m_ChipID.ProcessorName,"U5D"); break;
            case 2: sprintf (m_ChipID.ProcessorName,"U5S"); break;
            default: sprintf (m_ChipID.ProcessorName,"Unknown UMC family"); return false;
            }
          break;
        default:
          sprintf (m_ChipID.ProcessorName,"Unknown UMC family");
          return false;
        }
      break;

    case IDT:
      switch (m_ChipID.Family) 
        {  
        case 5:
          switch (m_ChipID.Model) 
            {
            case 4: sprintf (m_ChipID.ProcessorName,"C6"); break;
            case 8: sprintf (m_ChipID.ProcessorName,"C2"); break;
            case 9: sprintf (m_ChipID.ProcessorName,"C3"); break;
            default: sprintf (m_ChipID.ProcessorName,"Unknown IDT\\Centaur family"); return false;
            }
          break;
        case 6:
          switch (m_ChipID.Model) 
            {
            case 6: sprintf (m_ChipID.ProcessorName,"VIA Cyrix III - Samuel"); break;
            default: sprintf (m_ChipID.ProcessorName,"Unknown IDT\\Centaur family"); return false;
            }
          break;
        default:
          sprintf (m_ChipID.ProcessorName,"Unknown IDT\\Centaur family");
          return false;
        }
      break;

    case Cyrix:
      switch (m_ChipID.Family) 
        {  
        case 4:
          switch (m_ChipID.Model) 
            {
            case 4: sprintf (m_ChipID.ProcessorName,"MediaGX GX, GXm"); break;
            case 9: sprintf (m_ChipID.ProcessorName,"5x86"); break;
            default: sprintf (m_ChipID.ProcessorName,"Unknown Cx5x86 family"); return false;
            }
          break;
        case 5:
          switch (m_ChipID.Model) 
            {
            case 2: sprintf (m_ChipID.ProcessorName,"Cx6x86"); break;
            case 4: sprintf (m_ChipID.ProcessorName,"MediaGX GXm"); break;
            default: sprintf (m_ChipID.ProcessorName,"Unknown Cx6x86 family"); return false;
            }
          break;
        case 6:
          switch (m_ChipID.Model) 
            {
            case 0: sprintf (m_ChipID.ProcessorName,"6x86MX"); break;
            case 5: sprintf (m_ChipID.ProcessorName,"Cyrix M2 Core"); break;
            case 6: sprintf (m_ChipID.ProcessorName,"WinChip C5A Core"); break;
            case 7: sprintf (m_ChipID.ProcessorName,"WinChip C5B\\C5C Core"); break;
            case 8: sprintf (m_ChipID.ProcessorName,"WinChip C5C-T Core"); break;
            default: sprintf (m_ChipID.ProcessorName,"Unknown 6x86MX\\Cyrix III family"); return false;
            }
          break;
        default:
          sprintf (m_ChipID.ProcessorName,"Unknown Cyrix family");
          return false;
        }
      break;

    case NexGen:
      switch (m_ChipID.Family) 
        {  
        case 5:
          switch (m_ChipID.Model) 
            {
            case 0: sprintf (m_ChipID.ProcessorName,"Nx586 or Nx586FPU"); break;
            default: sprintf (m_ChipID.ProcessorName,"Unknown NexGen family"); return false;
            }
          break;
        default:
          sprintf (m_ChipID.ProcessorName,"Unknown NexGen family");
          return false;
        }
      break;

    case NSC:
      sprintf (m_ChipID.ProcessorName,"Cx486SLC \\ DLC \\ Cx486S A-Step");
      break;
    default:
      sprintf (m_ChipID.ProcessorName,"Unknown family"); // We cannot identify the processor.
      return false;
    }

  return true;
}

/** Extract a value from the CPUInfo file */
std::string SystemInfo::ExtractValueFromCpuInfoFile(std::string buffer,const char* word,int init)
{
  long int pos = buffer.find(word,init);
  if(pos != -1)
    {
    m_CurrentPositionInFile = pos;
    pos = buffer.find(":",pos);
    long int pos2 = buffer.find("\n",pos);
    if(pos!=-1 && pos2!=-1)
      {
      return buffer.substr(pos+2,pos2-pos-2);
      }
    }
  m_CurrentPositionInFile = -1;
  return "";
}

/** Query for the cpu status */
int SystemInfo::RetreiveInformationFromCpuInfoFile()
{
  m_NumberOfLogicalCPU = 0;
  m_NumberOfPhysicalCPU = 0;
  std::string buffer;

  FILE *fd = fopen("/proc/cpuinfo", "r" );
  if ( !fd ) 
    {
    std::cout << "Problem opening /proc/meminfo" << std::endl;
    return 0;
    }
  
  long int fileSize = 0;
  while(!feof(fd))
    {
    buffer += fgetc(fd);
    fileSize++;
    }
  fclose( fd );
  
  buffer.resize(fileSize-2);

  // Number of CPUs
  long int pos = buffer.find("processor");
  while(pos != -1)
    {
    m_NumberOfLogicalCPU++;
    m_NumberOfPhysicalCPU++;
    pos = buffer.find("processor",pos+1);
    }

  // Count the number of physical ids that are the same
  int currentId = -1;
  std::string idc = this->ExtractValueFromCpuInfoFile(buffer,"physical id");

  while(m_CurrentPositionInFile>0)
    {
    int id = atoi(idc.c_str());
    if(id == currentId)
      {
      m_NumberOfPhysicalCPU--;
      }
    currentId = id;
    idc = this->ExtractValueFromCpuInfoFile(buffer,"physical id",m_CurrentPositionInFile+1);
    }

   if(m_NumberOfPhysicalCPU>0)
     {
     m_NumberOfLogicalCPU /= m_NumberOfPhysicalCPU;
     }

  // CPU speed (checking only the first proc
  std::string CPUSpeed = this->ExtractValueFromCpuInfoFile(buffer,"cpu MHz");
  m_CPUSpeedInMHz = (float)atof(CPUSpeed.c_str());

  // Chip family
  m_ChipID.Family = atoi(this->ExtractValueFromCpuInfoFile(buffer,"cpu family").c_str());
 
  // Chip Vendor
  strcpy(m_ChipID.Vendor,this->ExtractValueFromCpuInfoFile(buffer,"vendor_id").c_str());
  this->FindManufacturer();
  
  // Chip Model
  m_ChipID.Model = atoi(this->ExtractValueFromCpuInfoFile(buffer,"model").c_str());
  this->RetrieveClassicalCPUIdentity();

  // L1 Cache size
  std::string cacheSize = this->ExtractValueFromCpuInfoFile(buffer,"cache size");
  pos = cacheSize.find(" KB");
  if(pos!=-1)
    {
    cacheSize = cacheSize.substr(0,pos);
    }
  m_Features.L1CacheSize = atoi(cacheSize.c_str());


  return 1;
}

/** Query for the memory status */
int SystemInfo::QueryMemory()
{
  m_TotalVirtualMemory = 0;
  m_TotalPhysicalMemory = 0;
  m_AvailableVirtualMemory = 0;
  m_AvailablePhysicalMemory = 0;
#ifdef __CYGWIN__
  return 0;
#elif _WIN32
  MEMORYSTATUS ms;
  GlobalMemoryStatus(&ms);

  unsigned long tv = ms.dwTotalVirtual;
  unsigned long tp = ms.dwTotalPhys;
  unsigned long av = ms.dwAvailVirtual;
  unsigned long ap = ms.dwAvailPhys;
  m_TotalVirtualMemory = tv>>10>>10;
  m_TotalPhysicalMemory = tp>>10>>10;
  m_AvailableVirtualMemory = av>>10>>10;
  m_AvailablePhysicalMemory = ap>>10>>10;
  return 1;
#elif __linux
  unsigned long tv=0;
  unsigned long tp=0;
  unsigned long av=0;
  unsigned long ap=0;
  
  char buffer[1024]; // for skipping unused lines
  
  int linuxMajor = 0;
  int linuxMinor = 0;
  
  // Find the Linux kernel version first
  struct utsname unameInfo;
  int errorFlag = uname(&unameInfo);
  if( errorFlag!=0 )
    {
    std::cout << "Problem calling uname(): " << strerror(errno) << std::endl;
    return 0;
    }
 
  if( unameInfo.release!=0 && strlen(unameInfo.release)>=3 )
    {
    // release looks like "2.6.3-15mdk-i686-up-4GB"
    char majorChar=unameInfo.release[0];
    char minorChar=unameInfo.release[2];
    
    if( isdigit(majorChar) )
      {
      linuxMajor=majorChar-'0';
      }
    
    if( isdigit(minorChar) )
      {
      linuxMinor=minorChar-'0';
      }
    }
  
  FILE *fd = fopen("/proc/meminfo", "r" );
  if ( !fd ) 
    {
    std::cout << "Problem opening /proc/meminfo" << std::endl;
    return 0;
    }
  
  if( linuxMajor>=3 || ( (linuxMajor>=2) && (linuxMinor>=6) ) )
    {
    // new /proc/meminfo format since kernel 2.6.x
    // Rigorously, this test should check from the developping version 2.5.x
    // that introduced the new format...
    
    long freeMem;
    long buffersMem;
    long cachedMem;
    
    fscanf(fd,"MemTotal:%ld kB\n", &m_TotalPhysicalMemory);
    fscanf(fd,"MemFree:%ld kB\n", &freeMem);
    fscanf(fd,"Buffers:%ld kB\n", &buffersMem);
    fscanf(fd,"Cached:%ld kB\n", &cachedMem);
    
    m_AvailablePhysicalMemory=freeMem+cachedMem+buffersMem;
    
    // Skip SwapCached, Active, Inactive, HighTotal, HighFree, LowTotal
    // and LowFree.
    int i=0;
    while(i<7)
      {
      fgets(buffer, sizeof(buffer), fd); // skip a line
      ++i;
      }
    
    fscanf(fd,"SwapTotal:%ld kB\n", &m_TotalVirtualMemory);
    fscanf(fd,"SwapFree:%ld kB\n", &m_AvailableVirtualMemory);
    }
  else
    {
    // /proc/meminfo format for kernel older than 2.6.x
    
    unsigned long temp;
    unsigned long cachedMem;
    unsigned long buffersMem;
    fgets(buffer, sizeof(buffer), fd); // Skip "total: used:..."
    
    fscanf(fd, "Mem: %lu %lu %lu %lu %lu %lu\n",
         &tp, &temp, &ap, &temp, &buffersMem, &cachedMem);
    fscanf(fd, "Swap: %lu %lu %lu\n", &tv, &temp, &av);
    
    m_TotalVirtualMemory = tv>>10>>10;
    m_TotalPhysicalMemory = tp>>10>>10;
    m_AvailableVirtualMemory = av>>10>>10;
    m_AvailablePhysicalMemory = (ap+buffersMem+cachedMem)>>10>>10;
    }
  fclose( fd );
  return 1;
#elif __hpux
  unsigned long tv=0;
  unsigned long tp=0;
  unsigned long av=0;
  unsigned long ap=0;
  struct pst_static pst;
  struct pst_dynamic pdy;
     
  unsigned long ps = 0;
  if (pstat_getstatic(&pst, sizeof(pst), (size_t) 1, 0) != -1)
    {
    ps = pst.page_size;
    tp =  pst.physical_memory *ps;
    tv = (pst.physical_memory + pst.pst_maxmem) * ps;
    if (pstat_getdynamic(&pdy, sizeof(pdy), (size_t) 1, 0) != -1)
      {
      ap = tp - pdy.psd_rm * ps;
      av = tv - pdy.psd_vm;
      m_TotalVirtualMemory = tv>>10>>10;
      m_TotalPhysicalMemory = tp>>10>>10;
      m_AvailableVirtualMemory = av>>10>>10;
      m_AvailablePhysicalMemory = ap>>10>>10;
      return 1;
      }
    }
  return 0;
#else
  return 0;
#endif


}

/** */
unsigned long SystemInfo::GetTotalVirtualMemory() 
{ 
  return m_TotalVirtualMemory; 
}

/** */
unsigned long SystemInfo::GetAvailableVirtualMemory() 
{ 
  return m_AvailableVirtualMemory; 
}

unsigned long SystemInfo::GetTotalPhysicalMemory() 
{ 
  return m_TotalPhysicalMemory; 
}

/** */
unsigned long SystemInfo::GetAvailablePhysicalMemory() 
{ 
  return m_AvailablePhysicalMemory; 
}

/** Get Cycle differences */
long long SystemInfo::GetCyclesDifference (DELAY_FUNC DelayFunction, unsigned int uiParameter)
{
#ifdef WIN32

  unsigned int edx1, eax1;
  unsigned int edx2, eax2;

  // Calculate the frequency of the CPU instructions.
  __try {
    _asm {
      push uiParameter ; push parameter param
      mov ebx, DelayFunction ; store func in ebx

      RDTSC_INSTRUCTION

      mov esi, eax ; esi = eax
      mov edi, edx ; edi = edx

      call ebx ; call the delay functions

      RDTSC_INSTRUCTION

      pop ebx

      mov edx2, edx      ; edx2 = edx
      mov eax2, eax      ; eax2 = eax

      mov edx1, edi      ; edx2 = edi
      mov eax1, esi      ; eax2 = esi
    }
  }
  __except(1) 
    {
    return -1;
    }

  return ((((__int64) edx2 << 32) + eax2) - (((__int64) edx1 << 32) + eax1));

#endif

  return -1;
}

/** Compute the delay overhead */
void SystemInfo::DelayOverhead(unsigned int uiMS)
{
#ifdef WIN32
  LARGE_INTEGER Frequency, StartCounter, EndCounter;
  __int64 x;

  // Get the frequency of the high performance counter.
  if(!QueryPerformanceFrequency (&Frequency)) 
    {
    return;
    }
  x = Frequency.QuadPart / 1000 * uiMS;

  // Get the starting position of the counter.
  QueryPerformanceCounter (&StartCounter);
  
  do {
    // Get the ending position of the counter.  
    QueryPerformanceCounter (&EndCounter);
  } while (EndCounter.QuadPart - StartCounter.QuadPart == x);
#endif
}

/** Return the number of logical CPU per physical CPUs Works only for windows */
unsigned char SystemInfo::LogicalCPUPerPhysicalCPU(void)
{
  unsigned int Regebx = 0;
#ifdef WIN32
  if (!this->IsHyperThreadingSupported()) 
    {
    return (unsigned char) 1;  // HT not supported
    }
  __asm
    {
    mov eax, 1
    cpuid
    mov Regebx, ebx
    }
#endif
  return (unsigned char) ((Regebx & NUM_LOGICAL_BITS) >> 16);
}

/** Works only for windows */
unsigned int SystemInfo::IsHyperThreadingSupported()
{
  unsigned int Regedx    = 0,
             Regeax      = 0,
             VendorId[3] = {0, 0, 0};
#ifdef WIN32
  __try    // Verify cpuid instruction is supported
    {
      __asm
      {
        xor eax, eax          // call cpuid with eax = 0
            cpuid                 // Get vendor id string
        mov VendorId, ebx
        mov VendorId + 4, edx
        mov VendorId + 8, ecx
        
        mov eax, 1            // call cpuid with eax = 1
        cpuid
        mov Regeax, eax      // eax contains family processor type
        mov Regedx, edx      // edx has info about the availability of hyper-Threading
      }
    }
  __except (EXCEPTION_EXECUTE_HANDLER)
    {
    return(0);                   // cpuid is unavailable
    }

  if (((Regeax & FAMILY_ID) == PENTIUM4_ID) || (Regeax & EXT_FAMILY_ID))
    {
    if (VendorId[0] == 'uneG')
      {
      if (VendorId[1] == 'Ieni')
        {
        if (VendorId[2] == 'letn')
          {
          return(Regedx & HT_BIT);    // Genuine Intel with hyper-Threading technology
          }
        }
      }
    }
#endif

  return 0;    // Not genuine Intel processor
}

/** Return the APIC Id. Works only for windows. */
unsigned char SystemInfo::GetAPICId()
{
  unsigned int Regebx = 0;
#ifdef WIN32
  if (!this->IsHyperThreadingSupported()) 
    {
    return (unsigned char) -1;  // HT not supported
    } // Logical processor = 1
  __asm
    {
    mov eax, 1
    cpuid
    mov Regebx, ebx
    }
#endif
  return (unsigned char) ((Regebx & INITIAL_APIC_ID_BITS) >> 24);
}

/** Count the number of CPUs. Works only on windows. */
int SystemInfo::CPUCount()
{
#ifdef WIN32
  unsigned char StatusFlag  = 0;
  SYSTEM_INFO info;

  m_NumberOfPhysicalCPU = 0;
  m_NumberOfLogicalCPU = 0;
  info.dwNumberOfProcessors = 0;
  GetSystemInfo (&info);

  // Number of physical processors in a non-Intel system
  // or in a 32-bit Intel system with Hyper-Threading technology disabled
  m_NumberOfPhysicalCPU = (unsigned char) info.dwNumberOfProcessors;  

  if (this->IsHyperThreadingSupported())
    {
    unsigned char HT_Enabled = 0;
    m_NumberOfLogicalCPU = this->LogicalCPUPerPhysicalCPU();
    if (m_NumberOfLogicalCPU >= 1)    // >1 Doesn't mean HT is enabled in the BIOS
      {
      HANDLE hCurrentProcessHandle;
      DWORD  dwProcessAffinity;
      DWORD  dwSystemAffinity;
      DWORD  dwAffinityMask;

      // Calculate the appropriate  shifts and mask based on the 
      // number of logical processors.
      unsigned char i = 1;
      unsigned char PHY_ID_MASK  = 0xFF;
      unsigned char PHY_ID_SHIFT = 0;

      while (i < m_NumberOfLogicalCPU)
        {
        i *= 2;
         PHY_ID_MASK  <<= 1;
        PHY_ID_SHIFT++;
        }
      
      hCurrentProcessHandle = GetCurrentProcess();
      GetProcessAffinityMask(hCurrentProcessHandle, &dwProcessAffinity,
                                                  &dwSystemAffinity);

      // Check if available process affinity mask is equal to the
      // available system affinity mask
      if (dwProcessAffinity != dwSystemAffinity)
        {
        StatusFlag = HT_CANNOT_DETECT;
        m_NumberOfPhysicalCPU = (unsigned char)-1;
        return StatusFlag;
        }

      dwAffinityMask = 1;
      while (dwAffinityMask != 0 && dwAffinityMask <= dwProcessAffinity)
        {
        // Check if this CPU is available
        if (dwAffinityMask & dwProcessAffinity)
          {
          if (SetProcessAffinityMask(hCurrentProcessHandle,
                                     dwAffinityMask))
            {
            unsigned char APIC_ID, LOG_ID, PHY_ID;
            Sleep(0); // Give OS time to switch CPU

            APIC_ID = GetAPICId();
            LOG_ID  = APIC_ID & ~PHY_ID_MASK;
            PHY_ID  = APIC_ID >> PHY_ID_SHIFT;
 
            if (LOG_ID != 0) 
              {
              HT_Enabled = 1;
              }
            }
          }
        dwAffinityMask = dwAffinityMask << 1;
        }
      // Reset the processor affinity
      SetProcessAffinityMask(hCurrentProcessHandle, dwProcessAffinity);
            
      if (m_NumberOfLogicalCPU == 1)  // Normal P4 : HT is disabled in hardware
        {
        StatusFlag = HT_DISABLED;
        }
      else
        {
        if (HT_Enabled)
          {
          // Total physical processors in a Hyper-Threading enabled system.
          m_NumberOfPhysicalCPU /= (m_NumberOfLogicalCPU);
          StatusFlag = HT_ENABLED;
          }
        else 
          {
          StatusFlag = HT_SUPPORTED_NOT_ENABLED;
          }
        }
      }
    }
  else
    {
    // Processors do not have Hyper-Threading technology
    StatusFlag = HT_NOT_CAPABLE;
    m_NumberOfLogicalCPU = 1;
    }
  return StatusFlag;
#endif
}

/** Return the number of logical CPUs on the system */
unsigned int SystemInfo::GetNumberOfLogicalCPU()
{
  return m_NumberOfLogicalCPU;
}

/** Return the number of physical CPUs on the system */
unsigned int SystemInfo::GetNumberOfPhysicalCPU()
{
  return m_NumberOfPhysicalCPU;
}

/** For Mac we Parse the sysctl -a output */
bool SystemInfo::ParseSysCtl()
{
  // Extract the arguments from the command line
  std::vector<const char*> args;
  args.push_back("sysctl");
  args.push_back("-a");
  args.push_back(0);

  m_SysCtlBuffer = this->RunProcess(args);
   
  // Parse values for Mac
  m_TotalPhysicalMemory = atoi(this->ExtractValueFromSysCtl("hw.memsize:").c_str())/(1024*1024);
  m_TotalVirtualMemory = 0;
  m_AvailablePhysicalMemory = 0;
  m_AvailableVirtualMemory = 0;

  m_NumberOfPhysicalCPU = atoi(this->ExtractValueFromSysCtl("hw.physicalcpu:").c_str());
  m_NumberOfLogicalCPU = atoi(this->ExtractValueFromSysCtl("hw.logicalcpu:").c_str());
  
  if(m_NumberOfPhysicalCPU!=0)
    {
    m_NumberOfLogicalCPU /= m_NumberOfPhysicalCPU;
    }

  m_CPUSpeedInMHz = atoi(this->ExtractValueFromSysCtl("hw.cpufrequency:").c_str()); 
  m_CPUSpeedInMHz /= 1000000;

  // Chip family
  m_ChipID.Family = atoi(this->ExtractValueFromSysCtl("machdep.cpu.family:").c_str()); 
 
  // Chip Vendor
  strcpy(m_ChipID.Vendor,this->ExtractValueFromSysCtl("machdep.cpu.vendor:").c_str());
  this->FindManufacturer();
  
  // Chip Model
  m_ChipID.Model = atoi(this->ExtractValueFromSysCtl("machdep.cpu.model:").c_str());
  this->RetrieveClassicalCPUIdentity();

  // Cache size
  m_Features.L1CacheSize = atoi(this->ExtractValueFromSysCtl("hw.l1icachesize:").c_str());  
  m_Features.L2CacheSize = atoi(this->ExtractValueFromSysCtl("hw.l2cachesize:").c_str());  

  return true;
}

/** Extract a value from sysctl command */
std::string SystemInfo::ExtractValueFromSysCtl(const char* word)
{
  long int pos = m_SysCtlBuffer.find(word);
  if(pos != -1)
    {
    pos = m_SysCtlBuffer.find(": ",pos);
    long int pos2 = m_SysCtlBuffer.find("\n",pos);
    if(pos!=-1 && pos2!=-1)
      {
      return m_SysCtlBuffer.substr(pos+2,pos2-pos-2);
      }
    }
  return "";
}

/** Run a given process */
std::string SystemInfo::RunProcess(std::vector<const char*> args)
{ 
  std::string buffer = "";

  // Run the application
  itksysProcess* gp = itksysProcess_New();
  itksysProcess_SetCommand(gp, &*args.begin());
  itksysProcess_SetOption(gp,itksysProcess_Option_HideWindow,1);

  itksysProcess_Execute(gp);

  char* data = NULL;
  int length;
  double timeout = 255;

  while(itksysProcess_WaitForData(gp,&data,&length,&timeout)) // wait for 1s
    {
    for(int i=0;i<length;i++)
      {
      buffer += data[i];
      }
    }
  itksysProcess_WaitForExit(gp, 0);

  int result = 1;
  switch(itksysProcess_GetState(gp))
    {
    case itksysProcess_State_Exited:
      {
      result = itksysProcess_GetExitValue(gp);
      } break;
    case itksysProcess_State_Error:
      {
      std::cerr << "Error: Could not run " << args[0] << ":\n";
      std::cerr << itksysProcess_GetErrorString(gp) << "\n";
      } break;
    case itksysProcess_State_Exception:
      {
      std::cerr << "Error: " << args[0]
                << " terminated with an exception: "
                << itksysProcess_GetExceptionString(gp) << "\n";
      } break;
    case itksysProcess_State_Starting:
    case itksysProcess_State_Executing:
    case itksysProcess_State_Expired:
    case itksysProcess_State_Killed:
      {
      // Should not get here.
      std::cerr << "Unexpected ending state after running " << args[0]
                << std::endl;
      } break;
    }
  itksysProcess_Delete(gp);

  return buffer;
}
  

std::string SystemInfo::ParseValueFromKStat(const char* arguments)
{
  std::vector<const char*> args;
  args.clear();
  args.push_back("kstat");
  args.push_back("-p");
  
  std::string command = arguments;
  long int start = -1;
  long int pos = command.find(' ',0);
  while(pos!=-1)
    {
    bool inQuotes = false;
    // Check if we are between quotes
    long int b0 = command.find('"',0);
    long int b1 = command.find('"',b0+1);
    while(b0 != -1 && b1 != -1 && b1>b0)
      {
      if(pos>b0 && pos<b1)
        {
        inQuotes = true;
        break;
        }
      b0 = command.find('"',b1+1);
      b1 = command.find('"',b0+1);
      }
    
    if(!inQuotes)
      {
      std::string arg = command.substr(start+1,pos-start-1);

      // Remove the quotes if any
      long int quotes = arg.find('"');
      while(quotes != -1)
        {
        arg.erase(quotes,1);
        quotes = arg.find('"');
        }
      args.push_back(arg.c_str());  
      start = pos;
      }
    pos = command.find(' ',pos+1);
    }
  std::string lastArg = command.substr(start+1,command.size()-start-1);
  args.push_back(lastArg.c_str());

  args.push_back(0);

  std::string buffer = this->RunProcess(args);

  std::string value = "";
  for(unsigned int i=buffer.size()-1;i>0;i--)
    {
    if(buffer[i] == ' ' || buffer[i] == '\t')
      {
      break;
      }   
    if(buffer[i] != '\n' && buffer[i] != '\r')
      {
      std::string val = value;
      value = buffer[i];
      value += val;
      }          
    }
  return value;
}

/** Querying for system information from Solaris */
bool SystemInfo::QuerySolarisInfo()
{
  // Parse values
  m_NumberOfPhysicalCPU = atoi(this->ParseValueFromKStat("-n system_misc -s ncpus").c_str());
  m_NumberOfLogicalCPU = m_NumberOfPhysicalCPU;
  
  if(m_NumberOfPhysicalCPU!=0)
    {
    m_NumberOfLogicalCPU /= m_NumberOfPhysicalCPU;
    }

  m_CPUSpeedInMHz = atoi(this->ParseValueFromKStat("-s clock_MHz").c_str());

  // Chip family
  m_ChipID.Family = 0; 
 
  // Chip Vendor
  strcpy(m_ChipID.Vendor,"Sun");
  this->FindManufacturer();
  
  // Chip Model
  sprintf(m_ChipID.ProcessorName,"%s",this->ParseValueFromKStat("-s cpu_type").c_str());
  m_ChipID.Model = 0;

  // Cache size
  m_Features.L1CacheSize = 0; 
  m_Features.L2CacheSize = 0;  

  char* tail;
  unsigned long totalMemory =
       strtoul(this->ParseValueFromKStat("-s physmem").c_str(),&tail,0);
  m_TotalPhysicalMemory = totalMemory/1024;
  m_TotalPhysicalMemory *= 8192;
  m_TotalPhysicalMemory /= 1024;

  // Undefined values (for now at least)
  m_TotalVirtualMemory = 0;
  m_AvailablePhysicalMemory = 0;
  m_AvailableVirtualMemory = 0;

  return true;
}

/** Query the operating system information */
bool SystemInfo::QueryOSInformation()
{
#ifdef WIN32

  m_OSName = "Windows";

  OSVERSIONINFOEX osvi;
  BOOL bIsWindows64Bit;
  BOOL bOsVersionInfoEx;
  char * operatingSystem = new char [256];

  // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
  ZeroMemory (&osvi, sizeof (OSVERSIONINFOEX));
  osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFOEX);

  if (!(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi))) 
    {
    osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
    if (!GetVersionEx ((OSVERSIONINFO *) &osvi)) 
      {
      return NULL;
      }
    }

  switch (osvi.dwPlatformId) 
    {
    case VER_PLATFORM_WIN32_NT:
      // Test for the product.
      if (osvi.dwMajorVersion <= 4) 
        {
        m_OSRelease = "NT";
        }
      if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0) 
        {
        m_OSRelease = "2000";
        }
      if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1) 
        {
        m_OSRelease = "XP";
        }

      // Test for product type.
      if (bOsVersionInfoEx) 
        {
        if (osvi.wProductType == VER_NT_WORKSTATION) 
          {
          if (osvi.wSuiteMask & VER_SUITE_PERSONAL)
            {
            m_OSRelease += " Personal";
            }
          else 
            {
            m_OSRelease += " Professional";
            }
          } 
        else if (osvi.wProductType == VER_NT_SERVER)
          {
          // Check for .NET Server instead of Windows XP.
          if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1) 
            {
            m_OSRelease = ".NET";
            }

          // Continue with the type detection.
          if (osvi.wSuiteMask & VER_SUITE_DATACENTER) 
            {
            m_OSRelease += " DataCenter Server";
            }
          else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE) 
            {
            m_OSRelease += " Advanced Server";
            }
          else 
            {
            m_OSRelease += " Server";
            }
          }

        sprintf (operatingSystem, "%s(Build %d)", osvi.szCSDVersion, osvi.dwBuildNumber & 0xFFFF);
        m_OSVersion = operatingSystem; 
        } 
      else 
        {
        HKEY hKey;
        char szProductType[80];
        DWORD dwBufLen;

        // Query the registry to retrieve information.
        RegOpenKeyEx (HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\ProductOptions", 0, KEY_QUERY_VALUE, &hKey);
        RegQueryValueEx (hKey, "ProductType", NULL, NULL, (LPBYTE) szProductType, &dwBufLen);
        RegCloseKey (hKey);

        if (lstrcmpi ("WINNT", szProductType) == 0)
          {
          m_OSRelease += " Professional";
          }
        if (lstrcmpi ("LANMANNT", szProductType) == 0)
          {
          // Decide between Windows 2000 Advanced Server and Windows .NET Enterprise Server.
          if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
            {
            m_OSRelease += " Standard Server";
            }
          else 
            {
            m_OSRelease += " Server";
            }
          }
        if (lstrcmpi ("SERVERNT", szProductType) == 0)
          {
          // Decide between Windows 2000 Advanced Server and Windows .NET Enterprise Server.
          if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
            {
            m_OSRelease += " Enterprise Server";
            }
          else 
            {
            m_OSRelease += " Advanced Server";
            }
          }
         }

      // Display version, service pack (if any), and build number.
      if (osvi.dwMajorVersion <= 4) 
        {
        // NB: NT 4.0 and earlier.
        sprintf (operatingSystem, "version %d.%d %s (Build %d)",
                 osvi.dwMajorVersion,
                 osvi.dwMinorVersion,
                 osvi.szCSDVersion,
                 osvi.dwBuildNumber & 0xFFFF);
        m_OSVersion = operatingSystem;
        } 
      else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1) 
        {
        // Windows XP and .NET server.
        typedef BOOL (CALLBACK* LPFNPROC) (HANDLE, BOOL *);
        HINSTANCE hKernelDLL; 
        LPFNPROC DLLProc;
        
        // Load the Kernel32 DLL.
        hKernelDLL = LoadLibrary ("kernel32");
        if (hKernelDLL != NULL)  { 
          // Only XP and .NET Server support IsWOW64Process so... Load dynamically!
          DLLProc = (LPFNPROC) GetProcAddress (hKernelDLL, "IsWow64Process"); 
         
          // If the function address is valid, call the function.
          if (DLLProc != NULL) (DLLProc) (GetCurrentProcess (), &bIsWindows64Bit);
          else bIsWindows64Bit = false;
         
          // Free the DLL module.
          FreeLibrary (hKernelDLL); 
          } 
        } 
      else 
        { 
        // Windows 2000 and everything else.
        sprintf (operatingSystem,"%s(Build %d)", osvi.szCSDVersion, osvi.dwBuildNumber & 0xFFFF);
        m_OSVersion = operatingSystem;
        }
      break;

    case VER_PLATFORM_WIN32_WINDOWS:
      // Test for the product.
      if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0) 
        {
        m_OSRelease = "95";
        if(osvi.szCSDVersion[1] == 'C') 
          {
          m_OSRelease += "OSR 2.5";
          }
        else if(osvi.szCSDVersion[1] == 'B') 
          {
          m_OSRelease += "OSR 2";
          }
      } 

      if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10) 
        {
        m_OSRelease = "98";
        if (osvi.szCSDVersion[1] == 'A' ) 
          {
          m_OSRelease += "SE";
          }
        } 

      if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90) 
        {
        m_OSRelease = "Me";
        } 
      break;

    case VER_PLATFORM_WIN32s:
      m_OSRelease = "Win32s";
      break;

    default:
      m_OSRelease = "Unknown";
      break;
  }


  // Get the hostname
  WORD wVersionRequested;
  WSADATA wsaData;
  char name[255];
  wVersionRequested = MAKEWORD(2,0);

  if ( WSAStartup( wVersionRequested, &wsaData ) == 0 )
    {
    gethostname(name,sizeof(name));
    WSACleanup( );
    }
  m_Hostname = name;

#else

  struct utsname unameInfo;
  int errorFlag = uname(&unameInfo);
  
  m_OSName = unameInfo.sysname;
  m_Hostname = unameInfo.nodename;
  m_OSRelease = unameInfo.release;
  m_OSVersion = unameInfo.version;
  m_OSPlatform = unameInfo.machine;

#endif

  return true;

}

/** Return true if the machine is 64 bits */
bool SystemInfo::Is64Bits()
{
  if(sizeof(long int) == 4)
    {
    return false;
    }
  return true;
}
