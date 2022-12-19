/**
 ******************************************************************
 *
 * Module Name : USER.hh
 *
 * Author/Date : C.B. Lirakis / 16-Nov-06
 *
 * Description : Function prototypes for user calls for interfacing to root
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
 *******************************************************************
 */
#ifndef __USER_hh_
#define __USER_hh_

bool UserInitialize();
void UserDo(double x, double y, double z);
void UserFinalize();

#endif
