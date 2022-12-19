##################################################################
#
#	Makefile for hytek using gcc on Linux. 
#
#
#	Modified	by	Reason
# 	--------	--	------
#	10-Oct-08       CBL     Original
#
# RCS header info.
# ----------------
# $RCSfile$
# $Author$
# $Date$
# $Locker$
# $Name$
# $Revision$
#
# $Log: $
#
#
######################################################################
# Machine specific stuff
#
#
TARGET = hytek
#
# Compile time resolution.
#
INCLUDE =  -I$(DRIVE)/common/utility -I$(ROOT_INC)
LIBS = -lutility $(ROOT_LIBS)

# Rules to make the object files depend on the sources.
SRC     = 
SRCCPP  = main.cpp IIR.cpp
SRCS    = $(SRC) $(SRCCPP)

HEADERS = IIR.hh

# When we build all, what do we build?
all:      $(TARGET)

include $(DRIVE)/common/makefiles/makefile.inc


#dependencies
include make.depend 
# DO NOT DELETE
