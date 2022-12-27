##################################################################
#
#	Makefile for Scope code using gcc on Linux. 
#
#
#	Modified	by	Reason
# 	--------	--	------
#	19-Jan-14       CBL     Original
#	18-Dec-22       CBL     Most base DSA602 functionality in library now
#       27-Dec-22       CBL     MValue and MeasurementA are gone/condensed 
#                               and in the library. 
#
######################################################################
# Machine specific stuff
#
COMMON = $(HOME)/common
TARGET = Scope
#
# Compile time resolution.
#
EXT_CFLAGS = -g
INCLUDE =  -I$(COMMON)/GPIB -I$(COMMON)/DSA602 -I$(COMMON)/utility \
	-I/usr/local/include -I$(ROOT_INC)
LIBS = -L$(DRIVE)/lib_linux -lDSA602 -lmygpib -lutility -L/usr/local/lib \
	-lgpib $(ROOT_GLIBS)

# Rules to make the object files depend on the sources.
SRC     = 
SRCCPP  = main.cpp FFTDlg.cpp TimeDlg.cpp ChannelFrame.cpp \
	ModuleDlg.cpp TraceDlg.cpp TraceFrame.cpp DescDlg.cpp \
	Description.cpp Input.cpp MeasDlg.cpp SPlot.cpp \
	WaveDlg.cpp UserSignals.cpp S_Dict.cpp

SRCS    = $(SRC) $(SRCCPP)

HEADERS = FFTDlg.hh TraceDlg.hh TraceFrame.hh TimeDlg.hh ModuleDlg.hh \
	DescDlg.hh ChannelFrame.hh SPlot.hh MeasDlg.hh WaveDlg.hh


# When we build all, what do we build?
all:      $(TARGET)

#
# The dictionaries are generated automatically using the rootcint
# program. They are based on the headers and do some magic
# with respect to the ability of root to call my local classes.
# This is a Qt thing, so if you want to know more about it
# look at the Qt documentation.
#
S_Dict.cpp: 	$(HEADERS)
	@echo "Generating builder root dictionary $@..."
	rootcint -f $@ -c $(HEADERS)

include $(DRIVE)/common/makefiles/makefile.inc


#dependencies
include make.depend 
# DO NOT DELETE
