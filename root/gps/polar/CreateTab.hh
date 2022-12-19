/**
 ******************************************************************
 *
 * Module Name : CreateTab.hh
 *
 * Author/Date : C.B. Lirakis / 02-Jul-08
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
 *******************************************************************
 */
#ifndef __CREATETAB_h_
#define __CREATETAB_h_

#include <TGTab.h>
#include <RQ_OBJECT.h>

class TEllipse;
class TText;
class TGraph;
class TRootEmbeddedCanvas;
class TGCompositeFrame;
class Polar;

class CreateTab : public TGTab {
    RQ_OBJECT("CreateTab")
public:
    CreateTab(TGCompositeFrame* p);
    ~CreateTab();
    void DoTab(Int_t i);
    void FillPlots(Double_t X, Double_t Y, Double_t Z);
    void Test();

private:
    void AddTab1();
    void AddTab2();
    void AddTab3();
    void CreateGraphicsFrame(TGCompositeFrame *putHere);
    void CreateLEDs();

    /// Status stuff.
    TEllipse    *fDestPassedLED;            ///< GUI LED for dest passed
    TEllipse    *fEnteredCircleLED;         ///< GUI LED for entered circle
    TEllipse    *fWopLED;                   ///< GUI LED for wop
    TEllipse    *fOnlineLED;                ///< GUI LED for online
    TEllipse    *fOfflineLED;               ///< GUI LED for offline
    TEllipse    *fTurnLED;                  ///< GUI LED for turn

    TText       *fDestPassedText;           ///< GUI Text for dest passed
    TText       *fEnteredCircleText;        ///< GUI Text for entered circle
    TText       *fWopText;                  ///< GUI Text for wop
    TText       *fOnlineText;               ///< GUI Text for online
    TText       *fOfflineText;              ///< GUI Text for offline
    TText       *fTurnModeText;             ///< GUI Text for turn
    Polar*       fPolar;
    // Plotting
    TGraph      *fXY, *fHeight;
    UInt_t      fXYPointer, fHeightPointer;
    Double_t    fAngle;

    TRootEmbeddedCanvas  *fStatus_EC, *fPosPlot;
    TRootEmbeddedCanvas  *fHeight_EC;

};
#endif
