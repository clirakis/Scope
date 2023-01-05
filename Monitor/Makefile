##################################################################
#
#	Makefile for Monitor using gcc on Linux. 
#
#
#	Modified	by	Reason
# 	--------	--	------
#	04-Jan-23       CBL     Original
#
#
######################################################################
# Machine specific stuff
#
#
TARGET = Monitor
#
# Compile time resolution.
#
EXT_CFLAGS = -g
INCLUDE =  -I$(COMMON)/GPIB -I$(COMMON)/DSA602 -I$(COMMON)/utility \
	-I/usr/local/include -I$(ROOT_INC)
LIBS = -L$(DRIVE)/lib_linux -lDSA602 -lmygpib -lutility -L/usr/local/lib \
	-lgpib $(ROOT_LIBS)

# Rules to make the object files depend on the sources.
SRC     = 
SRCCPP  = main.cpp Monitor.cpp UserSignals.cpp
SRCS    = $(SRC) $(SRCCPP)

HEADERS = 

# When we build all, what do we build?
all:      $(TARGET)

include $(DRIVE)/common/makefiles/makefile.inc


#dependencies
include make.depend 
# DO NOT DELETE
