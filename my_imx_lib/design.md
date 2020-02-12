This is a demo project to learn the i.Mx BootROM.

# Memory Map

```c
#if defined(ROM_BUILD)
#define ROM_START   0x0000
#define ROM_SIZE    0x40000
#else
#if defined(IRAM_BUILD)
#define ROM_START   0x007E0000
#define ROM_SIZE    0x40000
#endif

#define RAM_START   0x00900000
#define RAM_SIZE    0x10000

#define HAB_HAL_PERSISTENT_MIN_BYTES    2944
#define HAB_HAL_SCRATCH_MIN_BYTES   7104

#define MAIN_STACK_SZ       0x2000
#define PROCESS_STACK_SZ    0x000
#define MMU_TBL_SIZE        0x5000
#define RAM_PATCH_SZ        0x1000
#define DMA_ZONE_SZ         0x4000

```

