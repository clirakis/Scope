/**
 ******************************************************************
 *
 * Module Name : debug.h
 *
 * Author/Date : C.B. Lirakis / 05-Feb-03
 *
 * Description : 
 * 
 * 
 *
 * Restrictions/Limitations :
 *
 * Change Descriptions :
 *
 * Classification : Unclassified
 *
 * References :
 *
 *
 * RCS header info.
 * ----------------
 * $RCSfile$
 * $Author$
 * $Date$
 * $Locker$
 * $Name$
 * $Revision$
 *
 * $Log: $
 *
 *
 * Copyright (C) BBNT Solutions LLC  2001
 *******************************************************************
 */

#ifndef __DEBUG_h_
#define __DEBUG_h_

/* CBL 23-Jul-02 Debugging tools. */
extern int LastLine;
extern char *LastFile;
#define SET_DEBUG_STACK LastLine=__LINE__; LastFile=__FILE__;
#endif
