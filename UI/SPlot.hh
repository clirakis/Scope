/**
 ******************************************************************
 *
 * Module Name : SPlot.hh
 *
 * Author/Date : C.B. Lirakis / 10-Nov-05
 *
 * Description : Plot scope data or anything else for that matter. 
 *
 * Restrictions/Limitations :
 *
 * Change Descriptions :
 *
 * Classification : Unclassified
 *
 * References :
 *
 *******************************************************************
 */
#ifndef __SPLOT_hh_
#define __SPLOT_hh_
#  include <TGFrame.h>
#  include <TPoint.h>

class TLegend;
class TMultiGraph;
class TObjString;
class TGCheckButton;
class TGRadioButton;
class TGStatusBar;
class TGToolBar;
class TGraph;
class TRootEmbeddedCanvas;
class TMarker;
class TAxis;

enum PlotStateVals {PLOT_STATE_NORMAL, PLOT_STATE_ZOOM };

class SPlot : public TGMainFrame {
    ClassDef(SPlot, 0);

public:
    SPlot(const TGWindow *p, UInt_t w = 1000, UInt_t h = 400);
    ~SPlot();

    static SPlot* GetThis(void)    {return fSPlot;};

    void CloseWindow();
    void ProcessedEvent(Int_t event, Int_t x, Int_t y, TObject *selected);
    void HandleMenu(Int_t id);
    void HandleToolBar(Int_t id);
    void SetVerbose(Int_t value) {fVerbose = value;};
    bool OpenAndParseFile(const char *file);

    // slots
    void GetData(void);
    void SetTrace(void);

    // Quick access methods. 
    inline TGraph* GetGraph(void) {return fGraph;}


private:
    static SPlot        *fSPlot;         // This pointer

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
    TGraph*             fGraph;
    //TMultiGraph         *ftmg;
    //TLegend             *fLegend;

    // Things necessary for rubber band zoom.
    Bool_t              SelectOrZoom;   // True if in Zoom mode.
    Double_t            t1, y1, t2, y2;
    TMarker             *tm1, *tm2;
    // Regular zoom
    Double_t            fZoomLevel;
    // Verbosity of printout.
    Int_t               fVerbose;
    void*               fScope;
    TGRadioButton*      fTrace[8];

    void AddEmbeddedCanvas(UInt_t w, UInt_t h);
    void AddControls(void);
    void CreateStatusBar();
    void CreateMenuBar();
    void CreateToolBar();
    void DoLoad();
    void PlotMe(Int_t);
    void UnZoom();
    void Zoom();
    void ZoomAxis(TAxis *a);
    void DoSaveAs();

    void UpdateTraceButtons(void);

    // Open and parse utilities
    bool CreateGraphObjects();
    bool CleanGraphObjects();
    void SetCurrentFileName(const char *File);

    bool SaveWaveform(void);

    bool SaveParameters(void);


};
#endif
