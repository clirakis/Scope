##################################################################
#
#	Makefile for acquiring data from a single channel
#       and logging it to disk
#
#
#	Modified	by	Reason
# 	--------	--	------
#	14-Sep-03       CBL     Original
#
# BBN Technologies
# 127 John Clarke Rd.
# Middletown, R.I. 02842
# (401) - 848 - 3536
######################################################################
# Machine specific stuff
#
#
TARGET = treetest
#
# Compile time resolution.
#

INCLUDE = -I$(ROOT_INC) 
LIBS = $(ROOT_LIBS) -lNew
# Rules to make the object files depend on the sources.
SRC     = 
SRCCPP  = main.cpp debug.cpp logdata.cpp
SRCS    = $(SRC) $(SRCCPP)

HEADERS = debug.h

# When we build all, what do we build?
all:      $(TARGET)

include $(DRIVE)/common/makefiles/makefile.inc


#dependencies
include make.depend 
