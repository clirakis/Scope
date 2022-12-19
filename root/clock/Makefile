############################################################
#
# $Date: 2004/09/01 17:03:09 $
# $Id: Makefile,v 1.3 2004/09/01 17:03:09 clirakis Exp $
# $Revision: 1.3 $
# $Source: /home/clirakis/Work/Navo/2004/RTD/integrate/RCS/Makefile,v $
# $Author: clirakis $
# Description: Linux Makefile
# Project: Makefile for multithreaded GUI
#
############################################################

CC  = gcc
CPP = g++
LD  = g++
AR  = ar

# Includes
ROOTINC     := $(shell root-config --incdir)
INCS = -I$(ROOTINC)
# Libraries

ROOTLIBS     := $(shell root-config --glibs)

LIBS = $(ROOTLIBS)

CFLAGS = $(INCS) -g -Dlinux
# Targets
TARGET      = Mine.so
TARGET_OBJS = Aclock.o My_Dict.o

My_Dict.cpp: Aclock.hh My_LinkDef.hh 
	@echo "Generating RTD dictionary $@..."
	@rootcint -f $@ -c $^
# Rules-

.cpp.o:
	$(CPP) $(CFLAGS) -c $<
.c.o:
	$(CC) $(CFLAGS) -c $<


all: 

$(TARGET): $(TARGET_OBJS)
	g++ -shared -o $@ $(TARGET_OBJS) $(LIBS)

clean: 
	rm -f *.o *.d *~ ../*~ $(TARGET) 



