COMPILER_PREFIX = 'arm-none-eabi-'
DEF_LINK = COMPILER_PREFIX + 'ld'
DEF_AS = COMPILER_PREFIX + 'as'
DEF_GCC = COMPILER_PREFIX + 'gcc'
DEF_OBJCOPY = COMPILER_PREFIX + 'objcopy'
DEF_LINKFLAGS = '-T bsp/qemu_cm3/qemu_cm3.ld'

DEF_CFLAGS = [
       '-std=c11',
       '-g',
       '-O0',
       '-mcpu=cortex-m3',
       '-mthumb',
       '-I',
       'include', 
       '-I',
       'bsp/qemu_cm3',
       '-nostartfiles',
       '--specs=nosys.specs'
        ]
