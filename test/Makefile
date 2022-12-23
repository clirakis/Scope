##################################################################
#
#	Makefile for xxxx using gcc on Linux. 
#
#
#	Modified	by	Reason
# 	--------	--	------
#	23-Dec-22       CBL     Original, test DSA602 functions
#
######################################################################
# Machine specific stuff
#
#
TARGET = testme
#
# Compile time resolution.
#
EXT_CFLAGS = -g
INCLUDE =  -I$(COMMON)/GPIB -I$(COMMON)/DSA602 -I$(COMMON)/utility \
	-I/usr/local/include 
LIBS = -L$(DRIVE)/lib_linux -lDSA602 -lmygpib -lutility -L/usr/local/lib \
	-lgpib 

# Rules to make the object files depend on the sources.
SRC     = 
SRCCPP  = main.cpp
SRCS    = $(SRC) $(SRCCPP)

HEADERS = 

# When we build all, what do we build?
all:      $(TARGET)

include $(DRIVE)/common/makefiles/makefile.inc


#dependencies
include make.depend 
# DO NOT DELETE
