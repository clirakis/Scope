##################################################################
#
#	Makefile for testing compass concepts using gcc on Linux. 
#
#
#	Modified	by	Reason
# 	--------	--	------
#	31-Oct-04       CBL     Original
#
# BBN Technologies
# 127 John Clarke Rd.
# Middletown, R.I. 02842
# (401) - 849 - 2543 
######################################################################
# Machine specific stuff
#
#
TARGET = ipendulum
COMMON = $(DRIVE)/common
#
# Compile time resolution.
#
INCLUDE =  -I$(COMMON)/utility -I$(COMMON)/iolib -I$(ROOT_INC) 

LIBS    =  -lio -lutility $(ROOT_GLIBS) 

# Rules to make the object files depend on the sources.
SRC     = 
SRCCPP  = main.cpp  ipend.cpp

SRCS    = $(SRC) $(SRCCPP)

HEADERS = ipend.hh

# When we build all, what do we build?
all:      $(TARGET) make.depend

CINTINC = -I../lib
#guiDict.cpp: $(HEADERS)
#	@echo "Generating dictionary $@..."
#	@rootcint  -f $@ -c $^


include $(DRIVE)/common/makefiles/makefile.inc

#dependencies
include make.depend 


# DO NOT DELETE
