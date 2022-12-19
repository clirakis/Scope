/**
 ******************************************************************
 *
 * Module Name : polar.hh
 *
 * Author/Date : C.B. Lirakis / 30-Oct-04
 *
 * Description :
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
 * Copyright (C) BBNT Solutions LLC  2004
 *******************************************************************
 */

#ifndef __CBLPOLAR_h_
#define __CBLPOLAR_h_

#  include <TLine.h>
#  include <TArrow.h>
#  include <TMarker.h>
#  include <TList.h>

class CBLPolar {
    ClassDef(CBLPolar,1);
public:
    CBLPolar(Double_t Rmaximum = 1.0);
    ~CBLPolar();
    void Draw();
    inline void SetMarker(bool onoff) {MarkerSet = onoff;};

private:
    Double_t X0, Y0, Xmax, Ymax, Rmax;
    TLine *xaxis, *yaxis;
    TList *circles, *OtherGraphics, *MyPoints;
    TArrow *arrow;
    TMarker *marker;
    bool MarkerSet;
};

#endif

