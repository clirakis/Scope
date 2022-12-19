/**
 ******************************************************************
 *
 * Module Name : analshot.hh
 *
 * Author/Date : C.B. Lirakis / 10-Nov-05
 *
 * Description : Analyze Shot data in depth. 
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
 *
 *
 * Copyright (C) BBNT 2005
 *******************************************************************
 */
#ifndef __ANALSHOT_h_
#define __ANALSHOT_h_
#  include <TROOT.h>
#  include <TGFrame.h>
#  include <TList.h>
#  include <TGraph.h>
#  include <TMultiGraph.h>
#  include <TLegend.h>
#  include <TRootEmbeddedCanvas.h>
#  include <TGMenu.h>
#  include <TGButton.h>
#  include <TGStatusBar.h>
#  include <TGToolBar.h>
#  include <TPoint.h>
#  include <Gtypes.h>
#  include <Rtypes.h>
#  include <TMarker.h>

class TLegend;
class TMultiGraph;
class TObjString;

enum PlotStateVals {PLOT_STATE_NORMAL, PLOT_STATE_ZOOM 
};
class SpicePlot : public TGMainFrame {
    ClassDef(SpicePlot, 0);

public:
    SpicePlot(const TGWindow *p, UInt_t w = 1200, UInt_t h = 400, 
	      Int_t v=0);
    ~SpicePlot();
    void CloseWindow();
    void ProcessedEvent(Int_t event, Int_t x, Int_t y, TObject *selected);
    void HandleMenu(Int_t id);
    void HandleToolBar(Int_t id);
    void SetVerbose(Int_t value) {verbose = value;};
    bool OpenAndParseFile(const char *file);

private:
    TList               *fCleanup;
    TRootEmbeddedCanvas *fEmbeddedCanvas;
    TGStatusBar         *fStatusBar; 
    Double_t            down_x, down_y;
    TString             *fLastDir;
    TString             *fCurrentFile;
    TGToolBar           *fToolBar;

    /*
     * For zooming 
     */
    Int_t               PlotState;
    TPoint              screen;
    Double_t            X,Y;
    /*
     * For plotting
     */
    TMultiGraph         *ftmg;
    TLegend             *fLegend;

    // Things necessary for rubber band zoom.
    Bool_t              SelectOrZoom;   // True if in Zoom mode.
    Double_t            t1, y1, t2, y2;
    TMarker             *tm1, *tm2;
    // Regular zoom
    Double_t            fZoomLevel;
    // Verbosity of printout.
    Int_t               verbose;

    void AddEmbeddedCanvas(UInt_t w, UInt_t h);
    void CreateStatusBar();
    void CreateMenuBar();
    void CreateToolBar();
    void DoLoad();
    void PlotMe(Int_t);
    void UnZoom();
    void Zoom();
    void ZoomAxis(TAxis *a);

    // Open and parse utilities
    bool CreateGraphObjects();
    bool CleanGraphObjects();
    void SetCurrentFileName(const char *File);
};
#endif
