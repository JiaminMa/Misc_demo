#include "TestDxe.h"

static VOID
EFIAPI
TestDxeGetStrArray(UINT8 *StrBegin, UINT8 *StrArr[], UINT32 *StrAreaSize)
{
    UINT8 i = 0;
    StrArr[0] = StrBegin;
    UINT32 StrSize = 0;
    while(!((*StrBegin == 0) && (*(StrBegin + 1) == 0))) {
        StrBegin++;
        StrSize++;
        if (i != 0) {
            StrArr[i] = StrBegin;
        }
        while(*StrBegin != 0) {
            StrBegin++;
            StrSize++;
        }
        i++;
    }
    *StrAreaSize = StrSize + 2;
}

static VOID
EFIAPI
TestDxeDumpSmbiosType1(SMBIOS_TABLE_TYPE1 *Record, UINT32 *StrAreaSizeParam)
{
    UINT8 *String = (UINT8 *)(Record) + Record->Hdr.Length; 
    UINT8 *Strings[16];
    UINT32 StrAreaSize;

    char *WakupTypeStr[] = {
        "Reserved",
        "Other",
        "Unknown",
        "Apm Timer",
        "Modem Ring",
        "Lan Remote",
        "Power Switch",
        "Pci Pme",
        "Ac Power Restored",
    };

    DEBUG((DEBUG_INFO, "System Information\n"));

    TestDxeGetStrArray(String, Strings, &StrAreaSize);

    if (Record->Manufacturer != 0) {
        DEBUG((DEBUG_INFO, "    Manufacture:%a\n", Strings[Record->Manufacturer - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    Manufacture:\n"));
    }

    if (Record->ProductName != 0) {
        DEBUG((DEBUG_INFO, "    Product Name:%a\n", Strings[Record->ProductName - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    Product Name:\n"));
    }

    if (Record->Version != 0) {
        DEBUG((DEBUG_INFO, "    Version: %a\n", Strings[Record->Version - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    Version: None\n"));
    }

    if (Record->SerialNumber != 0) {
        DEBUG((DEBUG_INFO, "    SerialNumber: %a\n", Strings[Record->SerialNumber - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    SerialNumber: None\n"));
    }

    DEBUG((DEBUG_INFO, "    WakeUpType: %x\n", WakupTypeStr[Record->WakeUpType]));

    DEBUG((DEBUG_INFO, "    Uuid: %g\n", Record->Uuid));

    if (Record->SKUNumber != 0) {
        DEBUG((DEBUG_INFO, "    SKUNumber: %a\n", Strings[Record->SKUNumber - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    SKUNumber: None\n"));
    }
    
    if (Record->Family != 0) {
        DEBUG((DEBUG_INFO, "    Family: %a\n", Strings[Record->Family - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    Family: None\n"));
    }
    *StrAreaSizeParam = StrAreaSize;

}

static VOID
EFIAPI
TestDxeDumpSmbiosType3(SMBIOS_TABLE_TYPE3 *Record, UINT32 *StrAreaSizeParam)
{
    UINT8 *String = (UINT8 *)(Record) + Record->Hdr.Length; 
    UINT8 *Strings[16];
    UINT32 StrAreaSize;

    char *ChasisStateStr[] = {
        "None",
        "Other",
        "Unknown",
        "Safe",
        "Warning",
        "Critical",
        "Non Recoverable"
    };

    DEBUG((DEBUG_INFO, "Chassis Information\n"));
    
    TestDxeGetStrArray(String, Strings, &StrAreaSize);
    
    if (Record->Manufacturer != 0) {
        DEBUG((DEBUG_INFO, "    Manufacture:%a\n", Strings[Record->Manufacturer - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    Manufacture: No Enclosure\n"));
    } 

    if (Record->Type != 0) {
        DEBUG((DEBUG_INFO, "    Type:%a\n", Strings[Record->Type - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    Type: Other\n"));
    } 

    if (Record->Version != 0) {
        DEBUG((DEBUG_INFO, "    Version:%a\n", Strings[Record->Version - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    Version: N/A\n"));
    } 
    
    if (Record->SerialNumber != 0) {
        DEBUG((DEBUG_INFO, "    Serial Number:%a\n", Strings[Record->SerialNumber - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    Serial Number: None\n"));
    } 

    if (Record->AssetTag != 0) {
        DEBUG((DEBUG_INFO, "    Asset Tag:%a\n", Strings[Record->AssetTag - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    Asset Tag: No Asset Tag\n"));
    } 

    DEBUG((DEBUG_INFO, "    Boot-up State:%a\n", ChasisStateStr[Record->BootupState]));
    DEBUG((DEBUG_INFO, "    Power Supply State:%a\n", ChasisStateStr[Record->PowerSupplyState]));
    DEBUG((DEBUG_INFO, "    Thermal State:%a\n", ChasisStateStr[Record->ThermalState]));
    DEBUG((DEBUG_INFO, "    Security State:%a\n", ChasisStateStr[Record->SecurityStatus]));

    DEBUG((DEBUG_INFO, "    OEM Information:0x%02x%02x%02x%02x\n", Record->OemDefined[3], Record->OemDefined[2],
                     Record->OemDefined[1], Record->OemDefined[0]));
    DEBUG((DEBUG_INFO, "    Height:%d\n", Record->Height));
    DEBUG((DEBUG_INFO, "    Number Of Power Cords:%d\n", Record->NumberofPowerCords));
    DEBUG((DEBUG_INFO, "    Contained Elements:%d\n", Record->ContainedElements[0].ContainedElementMaximum));
    *StrAreaSizeParam = StrAreaSize;
}

static VOID
EFIAPI
TestDxeDumpSmbiosType4(SMBIOS_TABLE_TYPE4 *Record, UINT32 *StrAreaSizeParam)
{
    UINT8 *String = (UINT8 *)(Record) + Record->Hdr.Length; 
    UINT8 *Strings[16];
    UINT32 StrAreaSize;
    char *ProcessorType[]  = {
        "None",
        "Other",
        "Unknown",
        "Central Processor",
        "Math Processor",
        "Dsp Processor",
        "Video Processor",
    };
        
    DEBUG((DEBUG_INFO, "Processor Information\n"));
    
    TestDxeGetStrArray(String, Strings, &StrAreaSize);
    if (Record->Socket != 0) {
        DEBUG((DEBUG_INFO, "    Socket:%a\n", Strings[Record->Socket - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    Socket: None\n"));
    }      

    DEBUG((DEBUG_INFO, "    Type:%a\n", ProcessorType[Record->ProcessorType]));
    DEBUG((DEBUG_INFO, "    Family:%d\n", Record->ProcessorFamily));
    
    if (Record->ProcessorManufacturer != 0) {
        DEBUG((DEBUG_INFO, "    Processor Manufacturer:%a\n", Strings[Record->ProcessorManufacturer - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    Processor Manufacturer: None\n"));
    }  

    DEBUG((DEBUG_INFO, "    ID: %02x %02x %02x %02x %02x %02x %02x %02x\n",
                Record->ProcessorId.Signature.ProcessorSteppingId,
                Record->ProcessorId.Signature.ProcessorModel,
                Record->ProcessorId.Signature.ProcessorFamily,
                Record->ProcessorId.Signature.ProcessorType,
                Record->ProcessorId.Signature.ProcessorReserved1,
                Record->ProcessorId.Signature.ProcessorXModel,
                Record->ProcessorId.Signature.ProcessorXFamily,
                Record->ProcessorId.Signature.ProcessorReserved2
                ));
    if (Record->ProcessorVersion != 0) {
        DEBUG((DEBUG_INFO, "    Version:%d\n", Strings[Record->ProcessorVersion - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    Version: None\n"));
    }

    DEBUG((DEBUG_INFO, "    External Clock:%d\n", Record->ExternalClock));
    DEBUG((DEBUG_INFO, "    Max Speed:%d\n", Record->MaxSpeed));
    DEBUG((DEBUG_INFO, "    Current Speed:%d\n", Record->CurrentSpeed));
    DEBUG((DEBUG_INFO, "    Status:%d\n", Record->Status));
    DEBUG((DEBUG_INFO, "    Upgrade:%d\n", Record->ProcessorUpgrade));
    DEBUG((DEBUG_INFO, "    L1 Cache Handle:0x%04x\n", Record->L1CacheHandle));
    DEBUG((DEBUG_INFO, "    L2 Cache Handle:0x%04x\n", Record->L2CacheHandle));
    DEBUG((DEBUG_INFO, "    L3 Cache Handle:0x%04x\n", Record->L3CacheHandle));

    if (Record->SerialNumber != 0) {
        DEBUG((DEBUG_INFO, "    Serial Number:%a\n", Strings[Record->SerialNumber - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    Serial Number: None\n"));
    } 

    if (Record->AssetTag != 0) {
        DEBUG((DEBUG_INFO, "    Asset Tag:%a\n", Strings[Record->AssetTag - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    Asset Tag: None\n"));
    } 

    if (Record->PartNumber != 0) {
        DEBUG((DEBUG_INFO, "    Part Number:%a\n", Strings[Record->PartNumber - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    Part Number: None\n"));
    } 

    DEBUG((DEBUG_INFO, "    Core Count:%d\n", Record->CoreCount));
    DEBUG((DEBUG_INFO, "    Core Enabled:%d\n", Record->EnabledCoreCount));
    DEBUG((DEBUG_INFO, "    Characteristics:0x%04x\n", Record->ProcessorCharacteristics));
    *StrAreaSizeParam = StrAreaSize;
}

static VOID
EFIAPI
TestDxeDumpSmbiosType16(SMBIOS_TABLE_TYPE16 *Record, UINT32 *StrAreaSizeParam)
{

    char *MemoryUseStr[] = {
        "None",
        "Other",
        "Unknown",
        "System Memory",
        "Vedio Memory",
        "Flash Memory",
        "Non Volatile Ram",
        "Cache Memory",
    };

    char *MemoryErrorStr[] = {
        "N/A",
        "Other",
        "Unknown",
        "None",
        "Parity",
        "Single Bit ECC",
        "Multi Bit ECC",
        "CRC",
    };

    char *TempStr = NULL;
    DEBUG((DEBUG_INFO, "    Physical Memory Array\n"));
    switch(Record->Location) {
        case MemoryArrayLocationOther:
            TempStr = "Other";
            break;
        case MemoryArrayLocationUnknown:
            TempStr = "Unknown";
            break;
        case MemoryArrayLocationSystemBoard:
            TempStr = "System Board";
            break;
        case MemoryArrayLocationIsaAddonCard:
            TempStr = "Isa Addon Card";
            break;
        case MemoryArrayLocationEisaAddonCard:
            TempStr = "Eisa Addon Card";
            break;
        case MemoryArrayLocationPciAddonCard:
            TempStr = "Pci Addon Card";
            break;
        case MemoryArrayLocationMcaAddonCard:
            TempStr = "Mca Addon Card";
            break;
        case MemoryArrayLocationPcmciaAddonCard:
            TempStr = "Pcmcia Addon Card";
            break;
        case MemoryArrayLocationProprietaryAddonCard:
            TempStr = "Proprietary Addon Card";
            break;
        case MemoryArrayLocationNuBus:
            TempStr = "Nu Bus";
            break;
        case MemoryArrayLocationPc98C20AddonCard:
            TempStr = "Pc98C20 Addon Card";
            break;
        case MemoryArrayLocationPc98C24AddonCard:
            TempStr = "Pc98C24 Addon Card";
            break;
        case MemoryArrayLocationPc98EAddonCard:
            TempStr = "Pc98E Addon Card";
            break;
        case MemoryArrayLocationPc98LocalBusAddonCard:
            TempStr = "Pc98 Local Bus Addon Card";
            break;
        case MemoryArrayLocationCXLAddonCard:
            TempStr = "CXL Addon Card";
            break;
        default:
            TempStr = "None";
            break;
    }
    DEBUG((DEBUG_INFO, "    Location: %a\n", TempStr));
    DEBUG((DEBUG_INFO, "    Use:%a\n", MemoryUseStr[Record->Use]));
    DEBUG((DEBUG_INFO, "    Error Type:%a\n", MemoryErrorStr[Record->MemoryErrorCorrection]));
    DEBUG((DEBUG_INFO, "    Maximum Capacity: %d\n", Record->MaximumCapacity));
    DEBUG((DEBUG_INFO, "    MemoryErrorInformationHandle: 0x%x\n", Record->MemoryErrorInformationHandle));
    DEBUG((DEBUG_INFO, "    Number Of Devices:%d\n", Record->NumberOfMemoryDevices));
    *StrAreaSizeParam = 2;
}

static VOID
EFIAPI
TestDxeDumpSmbiosType17(SMBIOS_TABLE_TYPE17 *Record, UINT32 *StrAreaSizeParam)
{
    
    char *MemoryFormFactorStr[] = {
        "N/A",
        "Other",
        "Unknown",
        "Simm",
        "Sip",
        "Chip",
        "Dip",
        "Zip",
        "Proprietary Card",
        "Dimm",
        "Tsop",
        "Row Of Chips",
        "Rimm",
        "Sodimm",
        "Srimm",
        "Fb Dimm",
        "Die"
    };
 
    char *MemoryTypeStr[] ={
        "N/A",
        "Other",
        "Unknown",
        "Dram",
        "Edram",
        "Vram",
        "Sram",
        "Ram",
        "Rom",
        "Flash",
        "Eeprom",
        "Feprom",
        "Eprom",
        "Cdram",
        "3Dram",
        "Sdram",
        "Sgram",
        "Rdram",
        "Ddr",
        "Ddr2",
        "Ddr2FbDimm",
        "Ddr3",
        "Fbd2",
        "Ddr4",
        "Lpddr",
        "Lpddr2",
        "Lpddr3",
        "Lpddr4",
        "Logical Non Volatile Device",
        "HBM",
        "HBM2",
        "Ddr5",
        "Lpddr5"
    };

    UINT8 *String = (UINT8 *)(Record) + Record->Hdr.Length; 
    UINT8 *Strings[16];
    UINT32 StrAreaSize;

    TestDxeGetStrArray(String, Strings, &StrAreaSize);

    DEBUG((DEBUG_INFO, "Memory Device\n"));
    DEBUG((DEBUG_INFO, "    Array Handle: 0x%04x\n", Record->MemoryArrayHandle));
    DEBUG((DEBUG_INFO, "    Error Information Handle:0x%04x\n", Record->MemoryErrorInformationHandle));
    DEBUG((DEBUG_INFO, "    Total Width:%d \n", Record->TotalWidth));
    DEBUG((DEBUG_INFO, "    Data Width:%d \n", Record->DataWidth));
    DEBUG((DEBUG_INFO, "    Size:%dMB\n", Record->Size));
    DEBUG((DEBUG_INFO, "    Form Factor:%a\n", MemoryFormFactorStr[Record->FormFactor]));
    DEBUG((DEBUG_INFO, "    Set:%d\n", Record->DeviceSet));
    if (Record->DeviceLocator != 0) {
        DEBUG((DEBUG_INFO, "    Locater: %a\n", Strings[Record->DeviceLocator - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    Locater: None\n"));
    }

    if (Record->BankLocator != 0) {
        DEBUG((DEBUG_INFO, "    Bank Locator: %a\n", Strings[Record->BankLocator - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    Bank Locator: None\n"));
    }
    
    DEBUG((DEBUG_INFO, "    Type:%a\n", MemoryTypeStr[Record->MemoryType]));
    DEBUG((DEBUG_INFO, "    Type Detail:%x\n", Record->TypeDetail));
    DEBUG((DEBUG_INFO, "    Speed:%d\n", Record->Speed));

    if (Record->Manufacturer != 0) {
        DEBUG((DEBUG_INFO, "    Manufacturer:%a\n", Strings[Record->Manufacturer - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    Manufacturer: None\n"));
    } 
    
    if (Record->SerialNumber != 0) {
        DEBUG((DEBUG_INFO, "    Serial Number:%a\n", Strings[Record->SerialNumber - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    Serial Number: None\n"));
    } 

    if (Record->AssetTag != 0) {
        DEBUG((DEBUG_INFO, "    Asset Tag:%a\n", Strings[Record->AssetTag - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    Asset Tag: None\n"));
    } 

    if (Record->PartNumber != 0) {
        DEBUG((DEBUG_INFO, "    Part Number:%a\n", Strings[Record->PartNumber - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    Part Number: None\n"));
    }

    DEBUG((DEBUG_INFO, "    Configured  Memory  Clock   Speed:%d\n", Record->ConfiguredMemoryClockSpeed));
    *StrAreaSizeParam = StrAreaSize;
}

static VOID
EFIAPI
TestDxeDumpSmbiosType32(SMBIOS_TABLE_TYPE32 *Record, UINT32 *StrAreaSizeParam)
{
    char *BootStatusStr[] = {
        "No Error",
        "No Boot Media",
        "Normal OS Failed Loading",
        "Firmware Detected Failure ",
        "OS Detected Failure",
        "User Requested Boot",
        "System Security Violation",
        "Previous Requested Image",
        "Watchdog Timer Expired",
        "Start Reserved",
        "Start Oem Specific",
        "Status Start Product Specific"
    };
    DEBUG((DEBUG_INFO, "System Boot Information\n"));
    DEBUG((DEBUG_INFO, "    Boot Status:%a\n", BootStatusStr[Record->BootStatus]));
    *StrAreaSizeParam = 2;
}

static VOID
EFIAPI
TestDxeDumpSmbiosType0(SMBIOS_TABLE_TYPE0 *Record, UINT32 *StrAreaSizeParam)
{
    UINT8 *String = (UINT8 *)(Record) + Record->Hdr.Length; 
    UINT8 *Strings[16];
    UINT32 StrAreaSize;

    DEBUG((DEBUG_INFO, "BIOS Information\n"));
    TestDxeGetStrArray(String, Strings, &StrAreaSize);

    if (Record->Vendor != 0) {
        DEBUG((DEBUG_INFO, "    Vendor:%a\n", Strings[Record->Vendor - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    Vendor:None\n"));
    }
    
    if (Record->BiosVersion != 0) {
        DEBUG((DEBUG_INFO, "    Bios Version:%a\n", Strings[Record->BiosVersion - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    Bios Version:None\n"));
    }

    if (Record->BiosReleaseDate != 0) {
        DEBUG((DEBUG_INFO, "    Bios Release Date:%a\n", Strings[Record->BiosReleaseDate - 1]));
    } else {
        DEBUG((DEBUG_INFO, "    Bios Release Date:None\n"));
    }

    DEBUG((DEBUG_INFO, "    Bios Segment:0x%x\n", Record->BiosSegment));
    DEBUG((DEBUG_INFO, "    Bios Size:0x%x\n", Record->BiosSize));
    DEBUG((DEBUG_INFO, "    Extended Bios Size:0x%x\n", Record->ExtendedBiosSize));
    DEBUG((DEBUG_INFO, "    Bios Characteristics\n"));
    if (Record->BiosCharacteristics.IsaIsSupported == 1) {
        DEBUG((DEBUG_INFO, "        ISA is supported\n"));
    } else {
        DEBUG((DEBUG_INFO, "        ISA is not supported\n"));
    }

    if (Record->BiosCharacteristics.PciIsSupported == 1) {
        DEBUG((DEBUG_INFO, "        PCI is supported\n"));
    } else {
        DEBUG((DEBUG_INFO, "        PCI is not supported\n"));
    }

    if (Record->BiosCharacteristics.PcmciaIsSupported == 1) {
        DEBUG((DEBUG_INFO, "        PC Card (PCMCIA) is supported\n"));
    } else {
        DEBUG((DEBUG_INFO, "        PC Card (PCMCIA) is not supported\n"));
    }

    if (Record->BiosCharacteristics.PlugAndPlayIsSupported == 1) {
        DEBUG((DEBUG_INFO, "        PNP is supported\n"));
    } else {
        DEBUG((DEBUG_INFO, "        PNP is not supported\n"));
    }

    if (Record->BiosCharacteristics.ApmIsSupported == 1) {
        DEBUG((DEBUG_INFO, "        APM is supported\n"));
    } else {
        DEBUG((DEBUG_INFO, "        APM is not supported\n"));
    }

    if (Record->BiosCharacteristics.BiosIsUpgradable == 1) {
        DEBUG((DEBUG_INFO, "        BIOS is upgraedable\n"));
    } else {
        DEBUG((DEBUG_INFO, "        BIOS is not upgraedable\n"));
    }  

    if (Record->BiosCharacteristics.BiosShadowingAllowed == 1) {
        DEBUG((DEBUG_INFO, "        BIOS Shadowing is allowed\n"));
    } else {
        DEBUG((DEBUG_INFO, "        BIOS Shadowing is not allowed\n"));
    }

    if (Record->BiosCharacteristics.EscdSupportIsAvailable == 1) {
        DEBUG((DEBUG_INFO, "        ESCD is available\n"));
    } else {
        DEBUG((DEBUG_INFO, "        ESCD is not available\n"));
    }

    if (Record->BiosCharacteristics.BootFromCdIsSupported == 1) {
        DEBUG((DEBUG_INFO, "        Boot from CD is supported\n"));
    } else {
        DEBUG((DEBUG_INFO, "        Boot from CD is not supported\n"));
    }

    if (Record->BiosCharacteristics.SelectableBootIsSupported == 1) {
        DEBUG((DEBUG_INFO, "        Selectable boot is supported\n"));
    } else {
        DEBUG((DEBUG_INFO, "        Selectable boot is not supported\n"));
    }

    if (Record->BiosCharacteristics.EDDSpecificationIsSupported == 1) {
        DEBUG((DEBUG_INFO, "        EDD is supported\n"));
    } else {
        DEBUG((DEBUG_INFO, "        EDD is not supported\n"));
    }

    if (Record->BiosCharacteristics.PrintScreenIsSupported == 1) {
        DEBUG((DEBUG_INFO, "        Print screen service is supported\n"));
    } else {
        DEBUG((DEBUG_INFO, "        Print screen service is not supported\n"));
    }

    if (Record->BiosCharacteristics.Keyboard8042IsSupported == 1) {
        DEBUG((DEBUG_INFO, "        8042 Keyboard is supported\n"));
    } else {
        DEBUG((DEBUG_INFO, "        8042 Keyboard is not supported\n"));
    }

    if (Record->BiosCharacteristics.SerialIsSupported == 1) {
        DEBUG((DEBUG_INFO, "        Serial service is supported\n"));
    } else {
        DEBUG((DEBUG_INFO, "        Serial service is not supported\n"));
    }

    if (Record->BiosCharacteristics.PrinterIsSupported == 1) {
        DEBUG((DEBUG_INFO, "        Printer service is supported\n"));
    } else {
        DEBUG((DEBUG_INFO, "        Printer service is not supported\n"));
    }
    *StrAreaSizeParam = StrAreaSize;
}

EFI_STATUS
EFIAPI
TestDxeDumpSmbios(VOID)
{
    EFI_STATUS Status = EFI_SUCCESS;
    EFI_SMBIOS_PROTOCOL *Smbios;
    EFI_SMBIOS_HANDLE   SmbiosHandle;
    EFI_SMBIOS_TABLE_HEADER       *Record;
    UINT32 HandleIndex = 0;
    UINT32 StrAreaSize;
    Status = gBS->LocateProtocol (
                  &gEfiSmbiosProtocolGuid,
                  NULL,
                  (VOID**)&Smbios
                  );

    if (EFI_ERROR (Status)) {
        return Status;
    }

    SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    DEBUG((DEBUG_INFO, "[TestDxeDumpSmbios] Called\n"));
    Status = Smbios->GetNext (Smbios, &SmbiosHandle, NULL, &Record, NULL);

    while (!EFI_ERROR(Status)) {
        DEBUG((DEBUG_INFO, "Handle 0x%x DMI type %d, %d bytes..\n", HandleIndex, Record->Type, Record->Length));
   
        if (Record->Type == SMBIOS_TYPE_BIOS_INFORMATION) {
            TestDxeDumpSmbiosType0((SMBIOS_TABLE_TYPE0 *)Record, &StrAreaSize);
         }else if (Record->Type == SMBIOS_TYPE_SYSTEM_INFORMATION){
            TestDxeDumpSmbiosType1((SMBIOS_TABLE_TYPE1 *)Record, &StrAreaSize);
        }else if (Record->Type == SMBIOS_TYPE_SYSTEM_ENCLOSURE) {
            TestDxeDumpSmbiosType3((SMBIOS_TABLE_TYPE3 *)Record, &StrAreaSize);
        } else if (Record->Type == SMBIOS_TYPE_PROCESSOR_INFORMATION) {
            TestDxeDumpSmbiosType4((SMBIOS_TABLE_TYPE4 *)Record, &StrAreaSize);                
        }else if (Record->Type == SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY){    
            TestDxeDumpSmbiosType16((SMBIOS_TABLE_TYPE16 *)Record, &StrAreaSize);  
        }else if (Record->Type == SMBIOS_TYPE_MEMORY_DEVICE){
            TestDxeDumpSmbiosType17((SMBIOS_TABLE_TYPE17 *)Record, &StrAreaSize);
        }else if (Record->Type == SMBIOS_TYPE_SYSTEM_BOOT_INFORMATION){
            TestDxeDumpSmbiosType32((SMBIOS_TABLE_TYPE32 *)Record, &StrAreaSize);
        } else {
            
        }
        Status = Smbios->GetNext (Smbios, &SmbiosHandle, NULL, &Record, NULL);
        HandleIndex++;
        
        DEBUG((DEBUG_INFO, "\n"));
    }
    return Status;
}

EFI_STATUS
EFIAPI
TestDxeDumpSmbiosByEps(IN EFI_SYSTEM_TABLE  *SystemTable)
{
    EFI_STATUS Status = EFI_SUCCESS;

    EFI_CONFIGURATION_TABLE *ConfigureTable = SystemTable->ConfigurationTable;
    UINTN NumberOfTableEntries = SystemTable->NumberOfTableEntries;
    UINTN i = 0;
    SMBIOS_TABLE_3_0_ENTRY_POINT *Eps;
    EFI_SMBIOS_TABLE_HEADER       *Record;
    UINT32 StrAreaSize;
    UINT32 HandleIndex = 0;

    for (i = 0; i < NumberOfTableEntries; i++) {
        if (CompareGuid(&ConfigureTable[i].VendorGuid, &gEfiSmbios3TableGuid) != 0) {
            Eps = (SMBIOS_TABLE_3_0_ENTRY_POINT *)ConfigureTable[i].VendorTable;
            break;
        }
    }

    DEBUG((DEBUG_INFO, "[TestDxeDumpSmbiosByEps]: Eps:0x%11p\n", Eps));
    DEBUG((DEBUG_INFO, "Anchor String:%a\n", Eps->AnchorString));
    DEBUG((DEBUG_INFO, "Entry Point Structure Checksum:0x%x\n", Eps->EntryPointStructureChecksum));
    DEBUG((DEBUG_INFO, "Entry Point Length:0x%x\n", Eps->EntryPointLength));
    DEBUG((DEBUG_INFO, "Version: %d.%d\n", Eps->MajorVersion, Eps->MinorVersion));
    DEBUG((DEBUG_INFO, "Doc Rev:%d\n", Eps->DocRev));
    DEBUG((DEBUG_INFO, "Entry Point Revision:%d\n", Eps->EntryPointRevision));
    DEBUG((DEBUG_INFO, "Table Max Size:%d\n", Eps->TableMaximumSize));
    DEBUG((DEBUG_INFO, "Table Address:0x%11p\n", Eps->TableAddress));

    Record = (EFI_SMBIOS_TABLE_HEADER *)(Eps->TableAddress);

    while(Record->Type != SMBIOS_TYPE_END_OF_TABLE) {
        
        DEBUG((DEBUG_INFO, "Handle 0x%x DMI type %d, %d bytes..\n", HandleIndex, Record->Type, Record->Length));
        if (Record->Type == SMBIOS_TYPE_BIOS_INFORMATION) {
            TestDxeDumpSmbiosType0((SMBIOS_TABLE_TYPE0 *)Record, &StrAreaSize);
         }else if (Record->Type == SMBIOS_TYPE_SYSTEM_INFORMATION){
            TestDxeDumpSmbiosType1((SMBIOS_TABLE_TYPE1 *)Record, &StrAreaSize);
        }else if (Record->Type == SMBIOS_TYPE_SYSTEM_ENCLOSURE) {
            TestDxeDumpSmbiosType3((SMBIOS_TABLE_TYPE3 *)Record, &StrAreaSize);
        } else if (Record->Type == SMBIOS_TYPE_PROCESSOR_INFORMATION) {
            TestDxeDumpSmbiosType4((SMBIOS_TABLE_TYPE4 *)Record, &StrAreaSize);                
        }else if (Record->Type == SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY){    
            TestDxeDumpSmbiosType16((SMBIOS_TABLE_TYPE16 *)Record, &StrAreaSize);  
        }else if (Record->Type == SMBIOS_TYPE_MEMORY_DEVICE){
            TestDxeDumpSmbiosType17((SMBIOS_TABLE_TYPE17 *)Record, &StrAreaSize);
        }else if (Record->Type == SMBIOS_TYPE_SYSTEM_BOOT_INFORMATION){
            TestDxeDumpSmbiosType32((SMBIOS_TABLE_TYPE32 *)Record, &StrAreaSize);
        } else {
            
        }

        UINT8 *Temp = (UINT8 *)(Record);
        Record = (EFI_SMBIOS_TABLE_HEADER *)(Temp + Record->Length + StrAreaSize);
        HandleIndex++;
    }

    return Status;
}


static EFI_TESTDXE_PROTOCOL mTestDxeProtocol = {
    0x10,
    TestDxeDumpSmbios,
    TestDxeDumpSmbiosByEps
};

EFI_STATUS
EFIAPI
TestDriverEntry(
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
)
{ 
    EFI_STATUS Status = EFI_SUCCESS;

    DEBUG ((DEBUG_INFO , "TestDriverEntry Start..\n"));
    Status = SystemTable->BootServices->InstallProtocolInterface(
                        &ImageHandle,
                        &gTestDxeGUID,
                        EFI_NATIVE_INTERFACE,
                        &mTestDxeProtocol
                        );
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_INFO, "[TestDxe] Install Protocol Fail\n"));
        return Status;
    }

    return Status;
}
