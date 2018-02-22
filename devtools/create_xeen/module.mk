MODULE := devtools/create_xeen

MODULE_OBJS := \
	create_xeen.o \
	cc.o \
	constants.o \
	hashmap.o \
	memorypool.o \
	str.o

# Set the name of the executable
TOOL_EXECUTABLE := create_xeen

# Include common rules
include $(srcdir)/rules.mk