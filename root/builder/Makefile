##################################################################
#
#	Makefile for builder using gcc on Linux. 
#
#
#	Modified	by	Reason
# 	--------	--	------
#	10-Feb-06       CBL     Original
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
# Chris Lirakis
# BBN Technologies
# 127 John Clarke Rd.
# Middletown, R.I. 02842
# (401) - 849 - 2543 
######################################################################
# Machine specific stuff
#
#
TARGET = builder
#
# Compile time resolution.
#
INCLUDE = -I$(DRIVE)/common/utility -I$(ROOT_INC)
LIBS = -L$(HOME)/lib_linux -lutility $(ROOT_GLIBS) 

# Rules to make the object files depend on the sources.
SRC     = 
SRCCPP  = Build.cpp BuildDialog.cpp builder_Dict.cpp
SRCS    = $(SRC) $(SRCCPP)

HEADERS = BuildDialog.hh Build_Linkdef.hh

# When we build all, what do we build?
all:      $(TARGET)


#
# The dictionaries are generated automatically using the rootcint
# program. They are based on the headers and do some magic
# with respect to the ability of root to call my local classes.
# This is a Qt thing, so if you want to know more about it
# look at the Qt documentation.
#
builder_Dict.cpp: 	$(HEADERS)
	@echo "Generating builder root dictionary $@..."
	rootcint -f $@ -c $(HEADERS)

include $(DRIVE)/common/makefiles/makefile.inc

#dependencies
#include make.depend 
