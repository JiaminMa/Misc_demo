#ifndef _TEST_DXE_H
#define _TEST_DXE_H

#include <Uefi.h> 
#include <Library/UefiLib.h> 
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/Smbios.h>
#include <Guid/SmBios.h>
#include <IndustryStandard/SerialPortConsoleRedirectionTable.h>

typedef struct _EFI_TESTDXE_PROTOCOL EFI_TESTDXE_PROTOCOL; 

typedef EFI_STATUS (EFIAPI *EFI_DUMP_SMBIOS)(VOID);
typedef EFI_STATUS (EFIAPI *EFI_DUMP_SMBIOS_BY_EPS)(IN EFI_SYSTEM_TABLE  *SystemTable);
typedef EFI_STATUS (EFIAPI *EFI_DUMP_ACPI)(VOID);

struct _EFI_TESTDXE_PROTOCOL {
    UINT64 Revision;
    EFI_DUMP_SMBIOS DumpSmbios;
    EFI_DUMP_SMBIOS_BY_EPS DumpBiosByEps;
    EFI_DUMP_ACPI DumpAcpi;
};

#endif

