
####################################
# STANDARD MAKEFILE INCLUDES (FIRST)
####################################

include $(CODEPATH)/build/make/Makefile.all

CC_ARM_SRCS :=
CC_THUMB_SRCS := $(wildcard *.c)
AS_ARM_SRCS :=
AS_THUMB_SRCS :=

CC_ARM_OBJS := $(patsubst %c,%o,$(CC_ARM_SRCS))
CC_THUMB_OBJS := $(patsubst %c,%o,$(CC_THUMB_SRCS))
AS_ARM_OBJS := $(patsubst %s,%o,$(AS_ARM_SRCS))
AS_THUMB_OBJS := $(patsubst %s,%o,$(AS_THUMB_SRCS))

SUB_DIRS :=

.PHONY: dir $(SUB_DIRS) all

dir:$(SUB_DIRS) all

$(SUB_DIRS):
	$(AT)make $(MK_OPT)-S -C $@ dir

##############################
# STANDARD INCLUDE FILES (END)
##############################

include $(CODEPATH)/build/make/Makefile.rules

