/**
 ******************************************************************
 *
 * Module Name : USER.h
 *
 * Author/Date : C.B. Lirakis / 14-Sep-03
 *
 * Description : Function prototypes for user input. 
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
 * $RCSfile: User.h,v $
 * $Author: clirakis $
 * $Date: 2003/08/08 17:26:28 $
 * $Locker:  $
 * $Name:  $
 * $Revision: 1.1 $
 *
 * $Log: User.h,v $
 * Revision 1.1  2003/08/08 17:26:28  clirakis
 * Initial revision
 *
 *
 *
 * Copyright (C) BBNT Solutions LLC  2001
 *******************************************************************
 */
#ifndef __USER_h_
#define __USER_h_
# ifdef __cplusplus
  extern          "C"
  {
# endif

int  InitializeLogData();
void LogData(const float *data, int nchannel);
void FinalizeLogData();

# ifdef __cplusplus
  }
# endif
#endif
