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
EFI_GUID gTestDxeGUID = { 0xbdb38129, 0x4d65, 0x39f4, { 0x72, 0x12, 0x68, 0xcf, 0x5a, 0x19, 0xa, 0xf8 }};

typedef struct _EFI_TESTDXE_PROTOCOL EFI_TESTDXE_PROTOCOL; 

typedef EFI_STATUS (EFIAPI *EFI_DUMP_SMBIOS)(VOID);
typedef EFI_STATUS (EFIAPI *EFI_DUMP_SMBIOS_BY_EPS)(IN EFI_SYSTEM_TABLE  *SystemTable);

struct _EFI_TESTDXE_PROTOCOL {
    UINT64 Revision;
    EFI_DUMP_SMBIOS DumpSmbios;
    EFI_DUMP_SMBIOS_BY_EPS DumpBiosByEps;
};

#endif

