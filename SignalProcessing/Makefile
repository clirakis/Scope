##################################################################
#
#	Makefile for SP using gcc on Linux. 
#       testing out signal processing ideas. 
#
#
#	Modified	by	Reason
# 	--------	--	------
#       15-Jan-23	CBL     Original
#       22-Jan-23       CBL     Divided up the classes
#                               Filt - Base class containing the coefficients
#                               Butterworth - class to calculate the filter
#                               Filter - forward only filter. 
#                               FiltFilt - forward/backward filter
#
#
######################################################################
# Machine specific stuff
#
#
TARGET = SP
#
# Compile time resolution.
#
EXT_CFLAGS = -g
INCLUDE =  -I$(COMMON)/utility -I/usr/local/include -I$(ROOT_INC)
LIBS = -L$(DRIVE)/lib_linux -lutility -L/usr/local/lib $(ROOT_LIBS)

# Rules to make the object files depend on the sources.
SRC     = 
SRCCPP  = main.cpp Butterworth.cpp Filt.cpp filter.cpp FiltFilt.cpp \
	UserSignals.cpp
SRCS    = $(SRC) $(SRCCPP)

HEADERS = 

# When we build all, what do we build?
all:      $(TARGET)

include $(DRIVE)/common/makefiles/makefile.inc


#dependencies
include make.depend 
# DO NOT DELETE
