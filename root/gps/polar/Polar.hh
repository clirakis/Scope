/**
 ******************************************************************
 *
 * Module Name : Polar.hh
 *
 * Author/Date : C.B. Lirakis / 03-Jul-08
 *
 * Description : Display data in a polar plot
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
 * $RCSfile: Met.hh,v $
 * $Author:   clirakis  $
 * $Date:   23 Sep 2005 11:56:48  $
 * $Locker: clirakis $
 * $Name:  $
 * $Revision:   1.2  $
 *
 * $Log:   /usr/local/iss60/library/iss60/CCS/RTD/Met.hhv  $
   
      Rev 1.2   23 Sep 2005 11:56:48   clirakis
   Modified to use CMG, Ship heading, black background and the Doug Cronin
   way of displaying data. Plus the color textual output of data for wind and 
   current
   
      Rev 1.1   20 Sep 2005 10:36:14   clirakis
   Modified with Doug Cronin's suggestions from the 07-Jul-05 Pathfinder trip.
   
      Rev 1.0   09 Jul 2005 13:08:00   clirakis
   Initial revision.
 * Revision 1.1  2004/09/08 20:00:56  clirakis
 * Initial revision
 *
 *
 *
 *
 *******************************************************************
 */
#ifndef __POLAR_hh_
#define __POLAR_hh_

#  include <TROOT.h>
#  include <RQ_OBJECT.h>
class TEllipse;
class TLine;
class TText;
class TPolyMarker;
class TMarker;

const Int_t NCircles  = 4;
const Int_t NLines    = 4;
const Int_t MaxPoints = 8;
const Double_t RMax = 0.75;

class Polar : public TObject
{
    ClassDef(Polar, 0);
public:
    /// Polar Constructor
    Polar(TGCompositeFrame* metFrame);

    /// Polar Destructor
    virtual ~Polar();

    /// Draw the metrological display
    void Draw();

    /// Update data
    //void Update();

    /// Resize the Polar display
    void Resize();
    void SetPoint(Int_t i, Double_t Theta, Double_t Phi, Int_t status=GOOD);
    void SetPointText(Int_t i, const Char_t *text);
    void SetPointStatus(Int_t point, Int_t status);

    enum {GOOD, OK, BAD};

private:

    TGCompositeFrame    *fPolarFrame;      ///< Frame to draw our components in
    TRootEmbeddedCanvas *fEmbeddedCanvas;  ///< Pointer to canvas on which the Polar is drawn

    TEllipse*     fCircles[NCircles];
    TLine*        fLines[NLines];
    TText*        fCircleText[NCircles];
    TText*        fLineText[2*NLines];
    TPolyMarker*  fPoints;
    TText*        fPointID[MaxPoints];
    TMarker*      fMTest;
    // Private Functions

    /// Draw the GUI wind indicator arrow
     /// Create the GUI text frame
    void CreateTextFrame();
    /// Create the GUI graphics frame
    void CreateGraphicsFrame();
    void CreatePolarLabels();
    void MakePlot();
    void DrawPlot();
};
#endif
