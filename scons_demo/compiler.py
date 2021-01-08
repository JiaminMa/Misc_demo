COMPILER_PREFIX = 'arm-none-eabi-'
DEF_LINK = COMPILER_PREFIX + 'ld'
DEF_AS = COMPILER_PREFIX + 'as'
DEF_GCC = COMPILER_PREFIX + 'gcc'
DEF_OBJCOPY = COMPILER_PREFIX + 'objcopy'
DEF_LINKFLAGS = '-T cm4.ld'

DEF_CFLAGS = [
       '-std=c11',
       '-g',
       '-mcpu=cortex-m3',
       '-mthumb',
       '-nostartfiles',
       '--specs=nosys.specs'
        ]
