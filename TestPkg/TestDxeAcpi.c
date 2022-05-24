#include "TestDxe.h"

VOID
DumpACPIHeader(EFI_ACPI_DESCRIPTION_HEADER *Entry)
{
    UINT32 Temp = 0, i = 0;
    DEBUG((DEBUG_INFO, "Signature:"));
    for(i = 0; i < 4; i++) {
        Temp = (Entry->Signature >> (i * 8)) & 0xff;
        DEBUG((DEBUG_INFO, "%c", Temp));
    }
    DEBUG((DEBUG_INFO, "\n"));
    DEBUG((DEBUG_INFO, "Length:0x%x\n", Entry->Length));
    DEBUG((DEBUG_INFO, "Revision:0x%02x\n", Entry->Revision));
    DEBUG((DEBUG_INFO, "Checksum:0x%02x\n", Entry->Checksum));
    DEBUG((DEBUG_INFO, "OemID:"));
    for (i = 0; i < 6; i++) {
        DEBUG((DEBUG_INFO, "%c", Entry->OemId[i]));
    }
    DEBUG((DEBUG_INFO, "\n"));
    DEBUG((DEBUG_INFO, "OemTableId:0x%p\n", Entry->OemTableId));
    DEBUG((DEBUG_INFO, "OemRevision:0x%x\n", Entry->OemRevision));  
    DEBUG((DEBUG_INFO, "CreatorId:0x%x\n", Entry->CreatorId));  
    DEBUG((DEBUG_INFO, "CreatorRevision:0x%x\n", Entry->CreatorRevision));  
}

VOID
DumpGenericAddress(char *Name, EFI_ACPI_6_3_GENERIC_ADDRESS_STRUCTURE *Addr)
{
    DEBUG((DEBUG_INFO, "%a: AddressSpaceId:0x%02x, RegisterBitWidth:0x%02x, RegisterBitOffset:0x%02x, AccessSize:0x%02x, Address:0x%11p\n",
        Name,
        Addr->AddressSpaceId, 
        Addr->RegisterBitWidth,
        Addr->RegisterBitOffset,
        Addr->AccessSize,
        Addr->Address));
}

VOID
DumpFADT(EFI_ACPI_6_3_FIXED_ACPI_DESCRIPTION_TABLE *FADT)
{
    char *PMProfiles[] = {
        "Unspecified",
        "Desktop",
        "Mobile",
        "WorkStation",
        "Enterprise Server",
        "SOHO Server",
        "Applicance PC",
        "Performance Server",
        "Tablet"
    };

    DEBUG((DEBUG_INFO, "\n#########Start Dump FADT##########\n"));

    DumpACPIHeader(&FADT->Header);

    DEBUG((DEBUG_INFO, "FACS:0x%x\n", FADT->FirmwareCtrl));
    DEBUG((DEBUG_INFO, "Dsdt:0x%x\n", FADT->Dsdt));
    DEBUG((DEBUG_INFO, "", FADT->PreferredPmProfile));

    if (FADT->PreferredPmProfile > 8) {
        DEBUG((DEBUG_INFO, "PreferredPmProfile: Reserved\n"));
    } else {
        DEBUG((DEBUG_INFO, "PreferredPmProfile: %a\n", PMProfiles[FADT->PreferredPmProfile]));
    }

    DEBUG((DEBUG_INFO, "SCI INT:0x%04x\n", FADT->SciInt));
    DEBUG((DEBUG_INFO, "SMI CMD:0x%08x\n", FADT->SmiCmd));
    DEBUG((DEBUG_INFO, "ACPI ENABLE:0x%02x\n", FADT->AcpiEnable));
    DEBUG((DEBUG_INFO, "ACPI DISABLE:0x%02x\n", FADT->AcpiDisable));
    DEBUG((DEBUG_INFO, "S4BIOS REQ:0x%02x\n", FADT->S4BiosReq));
    DEBUG((DEBUG_INFO, "PSTATE CNT:0x%02x\n", FADT->PstateCnt));
    DEBUG((DEBUG_INFO, "PM1a EVT BLK:0x%08x\n", FADT->Pm1aEvtBlk));
    DEBUG((DEBUG_INFO, "PM1b EVT BLK:0x%08x\n", FADT->Pm1bEvtBlk));
    DEBUG((DEBUG_INFO, "PM1a CNT BLK:0x%08x\n", FADT->Pm1aCntBlk));
    DEBUG((DEBUG_INFO, "PM1b CNT BLK:0x%08x\n", FADT->Pm1bCntBlk));
    DEBUG((DEBUG_INFO, "PM TMR BLK:0x%08x\n", FADT->PmTmrBlk));
    DEBUG((DEBUG_INFO, "GPE0 BLK:0x%08x\n", FADT->Gpe0Blk));
    DEBUG((DEBUG_INFO, "GPE1 BLK:0x%08x\n", FADT->Gpe1Blk));
    DEBUG((DEBUG_INFO, "PM1 EVT LEN:0x%02x\n", FADT->Pm1EvtLen));
    DEBUG((DEBUG_INFO, "PM1 CNT LEN:0x%02x\n", FADT->Pm1CntLen));
    DEBUG((DEBUG_INFO, "PM2 CNT LEN:0x%02x\n", FADT->Pm2CntLen));
    DEBUG((DEBUG_INFO, "PM TMR LEN:0x%02x\n", FADT->PmTmrLen));
    DEBUG((DEBUG_INFO, "GPE0 BLK LEN:0x%02x\n", FADT->Gpe0BlkLen));
    DEBUG((DEBUG_INFO, "GPE1 BLK LEN:0x%02x\n", FADT->Gpe1BlkLen));
    DEBUG((DEBUG_INFO, "GPE1 BASE:0x%02x\n", FADT->Gpe1Base));
    DEBUG((DEBUG_INFO, "CST CNT:0x%02x\n", FADT->CstCnt));
    DEBUG((DEBUG_INFO, "P LVL2 LAT:0x%04x\n", FADT->PLvl2Lat));
    DEBUG((DEBUG_INFO, "P LVL3 LAT:0x%04x\n", FADT->PLvl3Lat));
    DEBUG((DEBUG_INFO, "Flush Size:0x%04x\n", FADT->FlushSize));
    DEBUG((DEBUG_INFO, "FLUSH STRIDE:0x%04x\n", FADT->FlushStride));
    DEBUG((DEBUG_INFO, "DUTY OFFSET:0x%02x\n", FADT->DutyOffset));
    DEBUG((DEBUG_INFO, "DUTY WIDTH:0x%02x\n", FADT->DutyWidth));
    DEBUG((DEBUG_INFO, "DAY ALARM:0x%02x\n", FADT->DayAlrm));
    DEBUG((DEBUG_INFO, "MON ALARM:0x%02x\n", FADT->MonAlrm));
    DEBUG((DEBUG_INFO, "CENTURY:0x%02x\n", FADT->Century));
    DEBUG((DEBUG_INFO, "IAPC_BOOT_ARCH:0x%04x\n", FADT->IaPcBootArch));
    DEBUG((DEBUG_INFO, "Flags:0x%08x\n", FADT->Flags));
    DumpGenericAddress("Reset Reg", &FADT->ResetReg);
    DEBUG((DEBUG_INFO, "Reset Value:0x%02x\n", FADT->ResetValue));
    DEBUG((DEBUG_INFO, "ARM BOOT ARCH:0x%04x\n", FADT->ArmBootArch));
    DEBUG((DEBUG_INFO, "FADT MINOR VERSION:0x%02x\n", FADT->MinorVersion));
    DEBUG((DEBUG_INFO, "X FIRMWARE CTRL:0x%p", FADT->XFirmwareCtrl));
    DEBUG((DEBUG_INFO, "X DSDT:0x%p\n", FADT->XDsdt));
    DumpGenericAddress("X PM1a EVT BLK", &FADT->XPm1aEvtBlk);
    DumpGenericAddress("X PM1b EVT BLK", &FADT->XPm1bEvtBlk);
    DumpGenericAddress("X PM1a CNT BLK", &FADT->XPm1aCntBlk);
    DumpGenericAddress("X PM1b CNT BLK", &FADT->XPm1bCntBlk);
    DumpGenericAddress("X PM TMR BLK", &FADT->XPmTmrBlk);
    DumpGenericAddress("GPE0 BLK", &FADT->XGpe0Blk);
    DumpGenericAddress("GPE1 BLK", &FADT->XGpe1Blk);
    DumpGenericAddress("SLEEP CONTROL", &FADT->SleepControlReg);
    DumpGenericAddress("SLEEP STATUS", &FADT->SleepStatusReg);

    DEBUG((DEBUG_INFO, "###########End Dump FADT###########\n\n"));
}

VOID
DumpSPCR(
    IN EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE *SPCR
)
{
    char *TempStr;
    DEBUG((DEBUG_INFO, "\n#########Start Dump SPCR##########\n"));
    DumpACPIHeader(&SPCR->Header);
    switch(SPCR->InterfaceType) {
    case EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_INTERFACE_TYPE_16550:
        TempStr = "16550";
        break;
    case EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_INTERFACE_TYPE_16450:
        TempStr = "16450";
        break;
    case EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_INTERFACE_TYPE_ARM_PL011_UART:
        TempStr = "ARM PL011 UART";
        break;
    case EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_INTERFACE_TYPE_NVIDIA_16550_UART:
        TempStr = "NVIDIA 16550 UART";
        break;
    case EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_INTERFACE_TYPE_ARM_SBSA_GENERIC_UART_2X:
        TempStr = "ARM SBSA Generic UART 2.x";
        break;
    case EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_INTERFACE_TYPE_ARM_SBSA_GENERIC_UART:
        TempStr = "ARM SBSA Generic UART";
        break;
    case EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_INTERFACE_TYPE_DCC:
        TempStr = "ARM DCC";
        break;
    case EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_INTERFACE_TYPE_BCM2835_UART:
        TempStr = "BCM2835";
        break;
    case EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_INTERFACE_TYPE_16550_WITH_GAS:
        TempStr = "16550-compatible";
        break;
    default:
        TempStr = "Reserved";
        break;
    }
    DEBUG((DEBUG_INFO, "Interface:%a\n", TempStr));

    DumpGenericAddress("Base Address", (EFI_ACPI_6_3_GENERIC_ADDRESS_STRUCTURE *)&SPCR->BaseAddress);
    
    switch (SPCR->InterruptType)
    {
    case EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_INTERRUPT_TYPE_8259:
        TempStr = "8259";
        break;
    case EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_INTERRUPT_TYPE_APIC:
        TempStr = "APIC";
        break;
    case EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_INTERRUPT_TYPE_SAPIC:
        TempStr = "SAPIC";
        break;
    case EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_INTERRUPT_TYPE_GIC:
        TempStr = "GIC";
        break;
    default:
        
        TempStr = "Not Supported";
        break;
    }
    DEBUG((DEBUG_INFO, "Interrupt Type: %a\n", TempStr));

    DEBUG((DEBUG_INFO, "IRQ:%d\n", SPCR->Irq));
    DEBUG((DEBUG_INFO, "Global System Interrupt:0x%x\n", SPCR->GlobalSystemInterrupt));
   
    switch (SPCR->BaudRate)
    {
    case EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_BAUD_RATE_9600: 
        TempStr = "9600";
        break;
    case EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_BAUD_RATE_19200:
        TempStr = "19200";
        break;
    case EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_BAUD_RATE_57600:
        TempStr = "57600";
        break;
    case EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_BAUD_RATE_115200:
        TempStr = "115200";
        break;
    default:
        TempStr = "Reserved";
        break;
    }
    DEBUG((DEBUG_INFO, "Baud Rate:%a\n", TempStr));

    switch (SPCR->Parity)
    {
    case EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_PARITY_NO_PARITY:
        DEBUG((DEBUG_INFO, "Parity: No Parity\n"));
        break;
    default:
        DEBUG((DEBUG_INFO, "Parity: Reserved\n"));
        break;
    }
    
    switch (SPCR->StopBits)
    {
    case EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_STOP_BITS_1:
        DEBUG((DEBUG_INFO, "Stop Bits:%d\n", SPCR->StopBits));
        break;
    default:
        DEBUG((DEBUG_INFO, "Stop Bits: No Stop Bits\n"));
        break;
    }


    DEBUG((DEBUG_INFO, "Flow Control: \n"));
    if (SPCR->FlowControl & EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_FLOW_CONTROL_DCD) {
        DEBUG((DEBUG_INFO, "  Support DCD\n"));
    }

    if (SPCR->FlowControl & EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_FLOW_CONTROL_RTS_CTS) {
        DEBUG((DEBUG_INFO, "  Support RTS/CTS\n"));
    }

    if (SPCR->FlowControl & EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_FLOW_CONTROL_XON_XOFF) {
        DEBUG((DEBUG_INFO, "  Support XON/XOFF\n"));
    }
    
    switch (SPCR->TerminalType)
    {
    case EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_TERMINAL_TYPE_VT100:
        TempStr = "VT100";
        break;
    case EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_TERMINAL_TYPE_VT100_PLUS:
        TempStr = "VT100+";
        break;
    case EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_TERMINAL_TYPE_VT_UTF8:
        TempStr = "VI-UTF8";
        break;
    case EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_TERMINAL_TYPE_ANSI:
        TempStr = "ANSI";
        break;
    default:
        
        TempStr = "Reserved";
        break;
    }
    DEBUG((DEBUG_INFO, "Termial Type:%a\n", TempStr));
    
    DEBUG((DEBUG_INFO, "PCI Device ID:0x%04x\n", SPCR->PciDeviceId));
    DEBUG((DEBUG_INFO, "PCI Vendor ID:0x%04x\n", SPCR->PciVendorId));
    DEBUG((DEBUG_INFO, "PCI Bus Number:0x%02x\n", SPCR->PciBusNumber));
    DEBUG((DEBUG_INFO, "PCI Device Number:0x%02x\n", SPCR->PciDeviceNumber));
    DEBUG((DEBUG_INFO, "PCI Function Number:0x%02x\n", SPCR->PciFunctionNumber));
    DEBUG((DEBUG_INFO, "PCI Flags:0x%08x\n", SPCR->PciFlags));
    DEBUG((DEBUG_INFO, "PCI Segment:0x%02x\n", SPCR->PciSegment));
    DEBUG((DEBUG_INFO, "###########End Dump SPCR###########\n\n"));
}

VOID
DumpMadtGIC(EFI_ACPI_6_3_GIC_STRUCTURE *GIC)
{
    DEBUG((DEBUG_INFO, "GIC Distributor Information:\n"));
    DEBUG((DEBUG_INFO, "  CPU Interface Number:%d\n", GIC->CPUInterfaceNumber));
    DEBUG((DEBUG_INFO, "  Acpi Processor Uid:0x%08x\n", GIC->AcpiProcessorUid));
    DEBUG((DEBUG_INFO, "  Flags:0x%08x\n", GIC->Flags));
    DEBUG((DEBUG_INFO, "  Parking Protoco lVersion:0x%08x\n", GIC->ParkingProtocolVersion));
    DEBUG((DEBUG_INFO, "  Performance Interrupt Gsiv:0x%08x\n", GIC->PerformanceInterruptGsiv));
    DEBUG((DEBUG_INFO, "  Parked Address:0x%p\n", GIC->ParkedAddress));
    DEBUG((DEBUG_INFO, "  Physical Base Address:0x%p\n", GIC->PhysicalBaseAddress));
    DEBUG((DEBUG_INFO, "  GICV:0x%11p\n", GIC->GICV));
    DEBUG((DEBUG_INFO, "  GICH:0x%11p\n", GIC->GICH));
    DEBUG((DEBUG_INFO, "  VGIC Maintenance Interrupt:0x%x\n", GIC->VGICMaintenanceInterrupt));
    DEBUG((DEBUG_INFO, "  GICR Base Address:0x%11p\n", GIC->GICRBaseAddress));
    DEBUG((DEBUG_INFO, "  MPIDR:%11p\n", GIC->MPIDR));
    DEBUG((DEBUG_INFO, "  Processor Power Efficiency Class:0x%02x\n", GIC->ProcessorPowerEfficiencyClass));
    DEBUG((DEBUG_INFO, "  Spe Overflow Interrupt:0x%04x\n", GIC->SpeOverflowInterrupt));
}

VOID 
DumpMadtGICMSI(EFI_ACPI_6_3_GIC_MSI_FRAME_STRUCTURE *GICMSI)
{
    DEBUG((DEBUG_INFO, "GIC MSI Information:\n"));
    DEBUG((DEBUG_INFO, "  Gic Msi Frame Id:0x%08x\n", GICMSI->GicMsiFrameId));
    DEBUG((DEBUG_INFO, "  Physical Base Address:0x%11p\n", GICMSI->PhysicalBaseAddress));
    DEBUG((DEBUG_INFO, "  Flags:0x%08x\n", GICMSI->Flags));
    DEBUG((DEBUG_INFO, "  SPI Count:0x%04x\n", GICMSI->SPICount));
    DEBUG((DEBUG_INFO, "  SPI Base:0x%04x\n", GICMSI->SPIBase));
}

VOID
DumpMadtGICD(
    IN EFI_ACPI_6_3_GIC_DISTRIBUTOR_STRUCTURE *GICD
)
{
    DEBUG((DEBUG_INFO, "GIC Distributor Information:\n"));
    DEBUG((DEBUG_INFO, "  Gic Id:0x%08x\n", GICD->GicId ));
    DEBUG((DEBUG_INFO, "  Physical Base Address:0x%11p\n", GICD->PhysicalBaseAddress));
    DEBUG((DEBUG_INFO, "  System Vector Base:0x%x\n", GICD->SystemVectorBase));
    DEBUG((DEBUG_INFO, "  Gic Version:0x%02x\n", GICD->GicVersion));
}

VOID
DumpMADT(
    IN EFI_ACPI_6_3_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER *MADT
)
{
    UINT8 *TempPtr; 
    UINT8 Type, Length;
    UINTN Len = 0;
    DEBUG((DEBUG_INFO, "\n###########Start Dump MADT###########\n"));
    DumpACPIHeader(&MADT->Header);
    DEBUG((DEBUG_INFO, "Local APIC Address:0x%08x\n", MADT->LocalApicAddress));
    DEBUG((DEBUG_INFO, "Flags:0x%08x\n", MADT->Flags));

    TempPtr = (UINT8 *)(MADT + 1);
    Len = MADT->Header.Length - sizeof(EFI_ACPI_6_3_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER);
    while (Len > 0) {
        Type = *TempPtr;
        Length = *(TempPtr + 1);
        switch(Type)
        {
        case EFI_ACPI_6_3_PROCESSOR_LOCAL_APIC:
        case EFI_ACPI_6_3_IO_APIC:
        case EFI_ACPI_6_3_INTERRUPT_SOURCE_OVERRIDE:
        case EFI_ACPI_6_3_NON_MASKABLE_INTERRUPT_SOURCE:
        case EFI_ACPI_6_3_LOCAL_APIC_NMI:
        case EFI_ACPI_6_3_LOCAL_APIC_ADDRESS_OVERRIDE:
        case EFI_ACPI_6_3_IO_SAPIC:
        case EFI_ACPI_6_3_LOCAL_SAPIC:
        case EFI_ACPI_6_3_PROCESSOR_LOCAL_X2APIC:
        case EFI_ACPI_6_3_LOCAL_X2APIC_NMI:
        case EFI_ACPI_6_3_GICR:
        case EFI_ACPI_6_3_GIC_ITS:
            DEBUG((DEBUG_INFO, "Parser not implemented for Type:0x%02x\n", Type));
            break;
        case EFI_ACPI_6_3_GIC:
            DumpMadtGIC((EFI_ACPI_6_3_GIC_STRUCTURE *)TempPtr);
            break;
        case EFI_ACPI_6_3_GICD:
            DumpMadtGICD((EFI_ACPI_6_3_GIC_DISTRIBUTOR_STRUCTURE *)TempPtr);
            break;
        case EFI_ACPI_6_3_GIC_MSI_FRAME:
            DumpMadtGICMSI((EFI_ACPI_6_3_GIC_MSI_FRAME_STRUCTURE *)TempPtr);
            break;
        default:
            DEBUG((DEBUG_INFO, "Unknown Type:0x%02x\n", Type));
            break;
        }

        Len = Len - Length;
        TempPtr = TempPtr + Length;
    }
    DEBUG((DEBUG_INFO, "###########End Dump MADT###########\n\n"));
}

VOID 
DumpGTDT(
    IN EFI_ACPI_6_3_GENERIC_TIMER_DESCRIPTION_TABLE *GTDT
)
{
    DEBUG((DEBUG_INFO, "\n###########Start Dump GTDT###########\n\n"));
    DumpACPIHeader(&GTDT->Header);
    DEBUG((DEBUG_INFO, "Physical Address: 0x%11p\n", GTDT->CntControlBasePhysicalAddress));
    DEBUG((DEBUG_INFO, "Secure PL1 TimerGSIV:0x%08x\n", GTDT->SecurePL1TimerGSIV));
    DEBUG((DEBUG_INFO, "Secure PL1 TimerFlags:0x%08x\n", GTDT->SecurePL1TimerFlags));
    DEBUG((DEBUG_INFO, "Non Secure PL1 Timer GSIV:0x%08x\n", GTDT->NonSecurePL1TimerGSIV));
    DEBUG((DEBUG_INFO, "Non Secure PL1 Timer Flags:0x%08x\n", GTDT->NonSecurePL1TimerFlags));
    DEBUG((DEBUG_INFO, "Virtual Timer GSIV:0x%08x\n", GTDT->VirtualTimerGSIV));
    DEBUG((DEBUG_INFO, "Virtual Timer Flags:0x%08x\n", GTDT->VirtualTimerFlags));
    DEBUG((DEBUG_INFO, "NonSecure PL2 Timer GSIV:0x%08x\n", GTDT->NonSecurePL2TimerGSIV));
    DEBUG((DEBUG_INFO, "NonSecure PL2 Timer Flags:0x%08x\n", GTDT->NonSecurePL2TimerFlags));

    DEBUG((DEBUG_INFO, "###########End Dump GTDT###########\n\n"));
}

EFIAPI
EFI_STATUS 
TestDxeDumpAcpi()
{
    UINTN i, j = 0, EntryCount;
    UINT8 Temp = 0;
    EFI_CONFIGURATION_TABLE *configTab = NULL;
    EFI_ACPI_DESCRIPTION_HEADER           *XSDT, *Entry, *DSDT;
    UINT64 *EntryPtr;
    EFI_ACPI_6_3_FIXED_ACPI_DESCRIPTION_TABLE   *FADT;

    EFI_ACPI_6_3_ROOT_SYSTEM_DESCRIPTION_POINTER  *Root;

    DEBUG((DEBUG_INFO, "Dump ACPI\n"));

    configTab = gST->ConfigurationTable;
    for(i=0; i<gST->NumberOfTableEntries;i++)
    {
        if ((CompareGuid(&configTab->VendorGuid, &gEfiAcpiTableGuid) == TRUE) ||
        (CompareGuid(&configTab->VendorGuid, &gEfiAcpi20TableGuid) == TRUE))
        {
            DEBUG((DEBUG_INFO, "Found ACPI table\n")); 
            DEBUG((DEBUG_INFO, "Address: @[0x%p]\n",configTab));

            Root = configTab->VendorTable;
            DEBUG((DEBUG_INFO, "RSDP @[0x%p]\n", Root));
            DEBUG((DEBUG_INFO, "  Signature:"));
            for(j = 0; j < 8; j++) {
                Temp = (Root->Signature >> (j * 8)) & 0xff;
                DEBUG((DEBUG_INFO, "%c", Temp));
            }
            DEBUG((DEBUG_INFO, "\n"));
        
            DEBUG((DEBUG_INFO, "  Revision:%d\n", Root->Revision));
        
            DEBUG((DEBUG_INFO, "  OEM_ID:"));
            for (j = 0; j < 6; j++) {
                DEBUG((DEBUG_INFO, "%c", Root->OemId[j]));
            }
            DEBUG((DEBUG_INFO, "\n"));

            DEBUG((DEBUG_INFO, "  Length=[0x%x]\n", Root->Length));
            DEBUG((DEBUG_INFO, "  XSDT address=[0x%p]\n", Root->XsdtAddress));
            DEBUG((DEBUG_INFO, "  RSDT address=[0x%p]\n", Root->RsdtAddress));

            if (Root->Revision >= EFI_ACPI_6_3_ROOT_SYSTEM_DESCRIPTION_POINTER_REVISION) {
                XSDT = (EFI_ACPI_DESCRIPTION_HEADER *)(UINTN) Root->XsdtAddress;
                EntryCount = (XSDT->Length - sizeof(EFI_ACPI_DESCRIPTION_HEADER)) 
                      / sizeof(UINT64);
                DEBUG((DEBUG_INFO, "  XSDT @[0x%p]\n", XSDT));
                DEBUG((DEBUG_INFO, "    Signature:"));
                for(j = 0; j < 4; j++) {
                    Temp = (XSDT->Signature >> (j * 8)) & 0xff;
                    DEBUG((DEBUG_INFO, "%c", Temp));
                }
                DEBUG((DEBUG_INFO, "\n"));
          
                DEBUG((DEBUG_INFO, "    Length=[%d]\n", XSDT->Length));
                DEBUG((DEBUG_INFO, "    Entry Count=[%d]\n", EntryCount));
                DEBUG((DEBUG_INFO, "    OEM_ID:"));
                for (j = 0; j < 6; j++) {
                    DEBUG((DEBUG_INFO, "%c", Root->OemId[j]));
                }
                DEBUG((DEBUG_INFO, "\n"));
        
                EntryPtr=(UINT64 *)(XSDT+1);
                for (j = 0; j < EntryCount; j++, EntryPtr++) {
           
                    Entry = (EFI_ACPI_DESCRIPTION_HEADER *)((UINTN)(*EntryPtr));
                    if (Entry == NULL) {
                    continue;
                    }
                    DEBUG((DEBUG_INFO, "    Signature:"));
                    for(j = 0; j < 4; j++) {
                    Temp = (Entry->Signature >> (j * 8)) & 0xff;
                    DEBUG((DEBUG_INFO, "%c", Temp));
                    }
                    DEBUG((DEBUG_INFO, "\n"));
                    switch (Entry->Signature) {
                        case EFI_ACPI_6_3_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE:

                            FADT = (EFI_ACPI_6_3_FIXED_ACPI_DESCRIPTION_TABLE *)(UINTN) Entry;
                            DumpFADT(FADT);
                            DSDT = (EFI_ACPI_DESCRIPTION_HEADER *)(UINTN)(FADT->Dsdt);
                            DEBUG((DEBUG_INFO, "\n#########Start Dump DSDT##########\n"));
                            DumpACPIHeader(DSDT);
                            // UINT32 *temp = (UINT32 *)DSDT;
                            // for (UINTN k = 0; k < DSDT->Length; k += 4) {
                            //     DEBUG((DEBUG_INFO, "0x%08x\n", *temp++));
                            // }
                            DEBUG((DEBUG_INFO, "#########End Dump DSDT##########\n\n"));
                            break;
                        case EFI_ACPI_6_3_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_SIGNATURE:
                            DumpSPCR((EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE *)Entry);
                            break;
                        case EFI_ACPI_6_3_MULTIPLE_APIC_DESCRIPTION_TABLE_SIGNATURE:
                            DumpMADT((EFI_ACPI_6_3_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER *)Entry);
                            break;
                        case EFI_ACPI_6_3_GENERIC_TIMER_DESCRIPTION_TABLE_SIGNATURE:
                            DumpGTDT((EFI_ACPI_6_3_GENERIC_TIMER_DESCRIPTION_TABLE *)Entry);
                            break;
                        case EFI_ACPI_6_4_PROCESSOR_PROPERTIES_TOPOLOGY_TABLE_STRUCTURE_SIGNATURE:
                            DEBUG((DEBUG_INFO, "\n#########Start Dump PPTT##########\n"));
                            DumpACPIHeader(Entry);
                            DEBUG((DEBUG_INFO, "#########End Dump PPTT##########\n\n"));
                            break;
                        default:
                            break;
                    }
                }
            }
        }
        configTab++;
    }
    // while(1) {

    // }
    return EFI_SUCCESS;
}