/**
 ******************************************************************
 *
 * Module Name : Polar.cpp
 *
 * Author/Date : C.B. Lirakis / 03-Jul-08
 *
 * Description : This dialog displays polar plot
 * the vessel.
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
 * $RCSfile: Met.cpp,v $
 * $Author:   clirakis  $
 * $Date:   23 Sep 2005 11:57:32  $
 * $Locker: clirakis $
 * $Name:  $
 * $Revision:   1.2  $
 *
 * $Log:   /usr/local/iss60/library/iss60/CCS/RTD/Met.cpv  $

      Rev 1.2   23 Sep 2005 11:57:32   clirakis
   Modified to use CMG, Ship heading, black background and the Doug Cronin
   way of displaying data. Plus the color textual output of data for wind and
   current

      Rev 1.1   20 Sep 2005 10:35:48   clirakis
   Modified with Doug Cronin's suggestions from the 07-Jul-05 Pathfinder trip.

      Rev 1.0   09 Jul 2005 13:07:58   clirakis
   Initial revision.
 * Revision 1.1  2004/09/08 20:00:56  clirakis
 * Initial revision
 *
 *
 *
 *******************************************************************
 */
#ifndef lint
/// RCS Information
static char rcsid[]="$Header:   /usr/local/iss60/library/iss60/CCS/RTD/Met.cpv   1.2   23 Sep 2005 11:57:32   clirakis  $";
#endif
// System includes
#include <iostream>
#include <cmath>
using namespace std;

// Root specific includes
#include <TROOT.h>
#include <TRootEmbeddedCanvas.h>
#include <TCanvas.h>
#include <TEllipse.h>
#include <TText.h>
#include <TView.h>
#include <TView3D.h>
#include <TMath.h>
#include <TLine.h>
#include <TEllipse.h>
#include <TPolyMarker.h>
#include <TMarker.h>

// Local Includes
#include "Polar.hh"


/**
 ******************************************************************
 *
 * Function Name : Polar
 *
 * Description : Create a canvas that will display
 *               allow display of objects in a polar form. 
 *
 * Inputs: metFrame - pointer to the frame in which to draw the MET
 *
 * Returns : fully constructed MET display.
 *
 * Error Conditions :
 *
 * Unit Tested on: 22-Sep-05
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
Polar::Polar(TGCompositeFrame* metFrame) : TObject()
{
    Int_t i;

    fPolarFrame = metFrame;
    for (i=0;i<MaxPoints;i++)
    {
	fPointID[i] = NULL;
    }
    fPoints = NULL;
    fMTest = new TMarker(0.0,0.0,8);
    fMTest->SetMarkerColor(kGreen);
    fMTest->SetMarkerSize(2);

    CreateGraphicsFrame();
    MakePlot();
    Draw();
    Resize();

    fPoints = new TPolyMarker(MaxPoints);
    fPoints->SetMarkerColor(kGreen);
    fPoints->SetMarkerStyle(8);
    fPoints->SetMarkerSize(2);

}
/**
 ******************************************************************
 *
 * Function Name : MakePlot 
 *
 * Description : Create the lines, circles and text that
 *               make up the polar plot. 
 *
 * Inputs: NONE
 *
 * Returns : NONE
 *
 * Error Conditions :
 *
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void Polar::MakePlot()
{
    Int_t    i;
    Double_t R, dR, Theta, dTheta;
    Double_t x, y;
    Double_t Azimuth, dAzimuth;
    Char_t   Values[8];

    // Don't quite want to go to full size. 
    dR = RMax/((Double_t) NCircles);
    dTheta = TMath::Pi()/((Double_t) NLines);
    dAzimuth = 90.0/((Double_t) NCircles);

    R = dR;
    Theta = 0.0;
    Azimuth = 90.0 - dAzimuth;
    for (i=0; i<NCircles;i++)
    {
	fCircles[i] = new TEllipse(0.0, 0.0, R, R);
	fCircles[i]->SetLineColor(2);
	fCircles[i]->SetLineWidth(2);
	fCircles[i]->SetFillStyle(0);
	sprintf( Values,"%0.0f", floor(Azimuth));
	fCircleText[i] = new TText( R, 0.0, Values);
	fCircleText[i]->SetTextColor(kGreen);
	R += dR;
	Azimuth -= dAzimuth;
    }
    for (i=0; i<NLines;i++)
    {
	x = RMax * cos(Theta);
	y = RMax * sin(Theta);
	fLines[i] = new TLine( -x, -y, x, y);
	fLines[i]->SetLineColor(2);
	fLines[i]->SetLineWidth(2);
	Theta += dTheta;
    }
    Theta = 0.0;
    for (i=0; i<2*NLines;i++)
    {
	x = RMax * cos(Theta);
	y = RMax * sin(Theta);
	sprintf( Values,"%0.0f", floor(Theta*180.0/TMath::Pi()));
	fLineText[i] = new TText( x, y, Values);
	if (Theta <= TMath::Pi()/2.0)
	{
	    // Do nothing
	}
	else if (Theta<= TMath::Pi())
	{
	    fLineText[i]->SetTextAlign(31);
	}
	else if (Theta <= 3.0*TMath::Pi()/2.0)
	{
	    fLineText[i]->SetTextAlign(33);
	}
	else
	{
	    fLineText[i]->SetTextAlign(13);
	}
	fLineText[i]->SetTextColor(kGreen);
	Theta += dTheta;
    }   
}
/**
 ******************************************************************
 *
 * Function Name : MakePlot 
 *
 * Description : Create the lines, circles and text that
 *               make up the polar plot. 
 *
 * Inputs: NONE
 *
 * Returns : NONE
 *
 * Error Conditions :
 *
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void Polar::DrawPlot()
{
    Int_t    i;

    for (i=0; i<NCircles;i++)
    {
	fCircles[i]->Draw();
	fCircleText[i]->Draw();
    }
    for (i=0; i<NLines;i++)
    {
	fLines[i]->Draw();
    }
    for (i=0; i<2*NLines;i++)
    {
	fLineText[i]->Draw();
    }
    if (fPoints != NULL)
    {
	cout << "Fpoints" << endl;
	fPoints->Draw();
    }
    for (i=0;i<MaxPoints;i++)
    {
	if (fPointID[i] != NULL)
	{
	    fPointID[i]->Draw();
	}
    }
    fMTest->Draw();
}
/**
 ******************************************************************
 *
 * Function Name : Polar Destructor
 *
 * Description : Clean up our resources
 *
 * Inputs : none
 *
 * Returns : none
 *
 * Error Conditions : none
 *
 * Unit Tested on: 01-Sep-05
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
Polar::~Polar()
{
    // All GUI components are deleted when the RTDWindow is closed
    // since they are drawn in a frame belonging to RTDWindow and
    // handled by ROOT's kDeepCleanup mechanism

}
/**
 ******************************************************************
 *
 * Function Name : CreateGraphicsFrame
 *
 * Description : Create the frame that holds the compass and arrows.
 *
 * Inputs : none
 *
 * Returns : None
 *
 * Error Conditions :
 * 
 * Unit Tested on: 14-Nov-05
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void Polar::CreateGraphicsFrame()
{
    TGHorizontalFrame *graphicsFrame;

    Double_t rmin[] = {0.0, 0.0, 0.0};
    Double_t rmax[] = {1.0, 1.0, 1.0};
    const Int_t GraphicX = 600, GraphicY = 600;
    /*
     * We have a transient frame, How do we want to divide this up?
     * For the moment, use 2 frames, the top containing the graphic
     * and the bottom containg the text data.
     *
     * Make the upper graphics frame first.
     */
    graphicsFrame = new TGHorizontalFrame(fPolarFrame, 10, 10);

    fPolarFrame->AddFrame(graphicsFrame,
            new TGLayoutHints(kLHintsTop | kLHintsExpandX | kLHintsExpandY,
                              1, 1, 1, 1));

    // Want a drawing canvas in this frame.
    fEmbeddedCanvas = new TRootEmbeddedCanvas("Polar_EC",
        graphicsFrame, GraphicX, GraphicY);

    graphicsFrame->AddFrame(fEmbeddedCanvas,
        new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1));

    fEmbeddedCanvas->GetCanvas()->SetFillColor(kBlack);
    fEmbeddedCanvas->GetCanvas()->SetBit(kNoContextMenu);

    TView *tv = fEmbeddedCanvas->GetCanvas()->GetView();
    if (tv == NULL)
    {
        TView3D *tv3d = new TView3D(1, rmin, rmax);
        fEmbeddedCanvas->GetCanvas()->SetView(tv3d);
    }
}
/**
 ******************************************************************
 *
 * Function Name : Polar::Draw()
 *
 * Description : Draw either a supplied image or compass rose as
 * a background on the embedded canvas.
 *
 * Inputs : none
 *
 * Returns : none
 *
 * Error Conditions : none
 *
 * Unit Tested on:  23-Sep-05
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void Polar::Draw()
{
    fEmbeddedCanvas->GetCanvas()->SetEditable(kTRUE);
    DrawPlot();
    fEmbeddedCanvas->GetCanvas()->SetEditable(kFALSE);
    fEmbeddedCanvas->GetCanvas()->Modified();
    fEmbeddedCanvas->GetCanvas()->Update();
}
/**
 ******************************************************************
 *
 * Function Name : Resize
 *
 * Description : Resize the text labels based on frame width
 *
 * Inputs : None
 *
 * Returns : None
 *
 * Error Conditions : None
 *
 * Unit Tested on:
 *
 * Unit Tested by:
 *
 *******************************************************************
 */
void Polar::Resize()
{
    Double_t width, height;

    if (fPolarFrame != NULL)
    {
        width = fPolarFrame->GetWidth();
        height = fPolarFrame->GetHeight();

    }
}
/**
 ******************************************************************
 *
 * Function Name : SetPoint
 *
 * Description : 
 *
 * Inputs : Theta and Phi in degrees!
 *
 * Returns : None
 *
 * Error Conditions : None
 *
 * Unit Tested on:
 *
 * Unit Tested by: CBL
 *
 *******************************************************************
 */
void Polar::SetPoint(Int_t i, Double_t Theta, Double_t Phi, Int_t status)
{
    Double_t x,y;
    Double_t R, theta;
    R  = RMax * (90.0-Phi)/90.0;
    theta = TMath::Pi()/180.0 * Theta;
    x = R*cos(theta);
    y = R*sin(theta);
    cout << "Theta: " << Theta
	 << " Phi: "  << Phi
	 << " x: " << x
	 << " y: " << y
	 <<endl;
    if (i>=0 && i<MaxPoints)
    {
	cout << "DRAW" << i << endl;
	fPoints->SetPoint(i, x, y);
	if (fPointID[i] != NULL)
	{
	    fPointID[i]->SetX(x);
	    fPointID[i]->SetY(y);
	    SetPointStatus(i, status);
	    //fPointID[i]->Draw();
	}
	fEmbeddedCanvas->GetCanvas()->SetEditable(kTRUE);
	fPoints->Draw();
	fEmbeddedCanvas->GetCanvas()->SetEditable(kFALSE);
	fEmbeddedCanvas->GetCanvas()->Modified();
	fEmbeddedCanvas->GetCanvas()->Update();
    }
}
/**
 ******************************************************************
 *
 * Function Name : SetPointText
 *
 * Description : 
 *
 * Inputs : 
 *
 * Returns : None
 *
 * Error Conditions : None
 *
 * Unit Tested on:
 *
 * Unit Tested by: CBL
 *
 *******************************************************************
 */
void Polar::SetPointText(Int_t i, const Char_t *text)
{
    if (i>=0 && i<MaxPoints)
    {
	if (fPointID[i] == NULL)
	{
	    fPointID[i] = new TText(0.0, 0.0, text);
	    fPointID[i]->SetTextColor(kGreen);
	    fEmbeddedCanvas->GetCanvas()->SetEditable(kTRUE);
	    fPointID[i]->Draw();
	    fEmbeddedCanvas->GetCanvas()->SetEditable(kFALSE);
	    fEmbeddedCanvas->GetCanvas()->Modified();
	    fEmbeddedCanvas->GetCanvas()->Update();
	}
    }
}
/**
 ******************************************************************
 *
 * Function Name : SetPointStatus
 *
 * Description : Change color based on status
 *
 * Inputs : 
 *
 * Returns : None
 *
 * Error Conditions : None
 *
 * Unit Tested on:
 *
 * Unit Tested by: CBL
 *
 *******************************************************************
 */
void Polar::SetPointStatus(Int_t point, Int_t status)
{
    if (point>=0 && point<MaxPoints)
    {
	if (fPointID[point] != NULL)
	{
	    cout << "Set point status " << status << endl;
	    switch (status)
	    {
	    case Polar::GOOD:
		fPointID[point]->SetTextColor(kGreen);
		break;
	    case Polar::OK:
		fPointID[point]->SetTextColor(kYellow);
		break;
	    case Polar::BAD:
		fPointID[point]->SetTextColor(kRed);
		break;
	    }
	}
    }
}
