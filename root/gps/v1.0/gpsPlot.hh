/**
 ******************************************************************
 *
 * Module Name : gpsPlot.hh
 *
 * Author/Date : C.B. Lirakis / 29-Jun-08
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
 * $RCSfile: spiceplot.hh,v $
 * $Author: clirakis $
 * $Date: 2008/04/25 11:24:38 $
 * $Locker:  $
 * $Name:  $
 * $Revision: 1.1.1.1 $
 *
 * $Log: spiceplot.hh,v $
 * Revision 1.1.1.1  2008/04/25 11:24:38  clirakis
 * Imported using TkCVS
 *
 * Revision 1.1.1.1  2008/04/23 16:51:41  clirakis
 * Imported using TkCVS
 *
 *******************************************************************
 */
#ifndef __GPSPLOT_hh_
#  define __GPSPLOT_hh_
#  include <TROOT.h>
#  include <TGFrame.h>

class TList;
class TRootEmbeddedCanvas;
class TGStatusBar;
class TGToolBar;
class TGWindow;
class TGTab;
class CreateTab;
class TTimer;

class gpsPlot : public TGMainFrame {
    ClassDef(gpsPlot, 0);

public:
    gpsPlot(const TGWindow *p, UInt_t w = 1200, UInt_t h = 400, Int_t v=0);
    ~gpsPlot();
    void CloseWindow();
    void ProcessedEvent(Int_t event, Int_t x, Int_t y, TObject *selected);
    void HandleMenu(Int_t id);
    void HandleToolBar(Int_t id);
    void SetVerbose(Int_t value) {fVerbose = value;};
    void TimeoutProc();

private:
    TList*               fCleanup;
    TRootEmbeddedCanvas  *fEmbeddedCanvas;
    TGStatusBar*         fStatusBar; 
    TGToolBar*           fToolBar;
    CreateTab*           fTab;
    TTimer*              fTimer;

    // Verbosity of printout.
    Int_t                fVerbose;

    void AddEmbeddedCanvas(UInt_t w, UInt_t h);
    void CreateStatusBar();
    void CreateMenuBar();
    void CreateToolBar();
};
#endif
