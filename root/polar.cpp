/**
 ******************************************************************
 *
 * Module Name : 
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

#ifndef lint
/// RCS Information
static char rcsid[]="$Header$";
#endif

// System includes.
#include <iostream>
using namespace std;
#include <string>
#include <cmath>

// Root includes
#include <TSystem.h>
#include <TROOT.h>
#include <TText.h>
#include <TEllipse.h>
#include <TVirtualPad.h>
#include <TPoints.h>

/// Local Includes.
#include "polar.hh"


/**
 ******************************************************************
 *
 * Function Name :
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
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
CBLPolar::CBLPolar(Double_t Rmaximum)
{
    const double marks[] = {0.4, 0.80, -1.0};
    TEllipse *ellipse;
    TText *txt;
    int i;
    double theta, x, y, rmarker;
    char text[32];
    double pi = 4.0 * atan(1.0);

    Rmax = Rmaximum;
    Xmax = 1.10 *Rmax;
    Ymax = 1.10 *Rmax;
    X0 = 0.0;
    Y0 = 0.0;

    // Crosshairs
    xaxis = new TLine ( -Xmax,   0.0, Xmax,  0.0);
    yaxis = new TLine (   0.0, -Ymax,  0.0, Ymax);

    // Determine how many circles there are going to be for axes.
    // Start with static 2.
    circles = new TList();
    circles->SetOwner();

    OtherGraphics = new TList;
    OtherGraphics->SetOwner();

    for (i=0; marks[i] >0.0; i++)
    {
        ellipse = new TEllipse(0.0, 0.0, marks[i], 0.0, 0.0, 360.0, 0.0);
        ellipse->SetLineColor(2);
        circles->Add(ellipse);

	// Add the text associated with these circles etc. 
	// Along X axis
	sprintf(text,"%4.2f", Rmaximum * marks[i]);
	txt = new TText( marks[i], 0.0, text);
	OtherGraphics->Add(txt);
    }
    // Add degree markers around the edge of the graph
    rmarker = 0.9;
    for (theta = pi/4.0; theta < 2.0 * pi; theta += pi/4.0)
    {
	sprintf(text,"%4.1f", theta*180.0/pi);
	x = rmarker * cos(theta);
	y = rmarker * sin(theta);
	txt = new TText( x, y, text);
	OtherGraphics->Add(txt);
    }
    arrow = new TArrow(0.0, 0.0, 1.0, 0.0);
    arrow->SetLineWidth(3);
    marker = new TMarker(0.0,0.0, 29);
    marker->SetMarkerSize(2.0);

    MyPoints = new TList;
    MyPoints->SetOwner();
}
/**
 ******************************************************************
 *
 * Function Name :
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
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
CBLPolar::~CBLPolar()
{
    delete circles;
    delete OtherGraphics;
    delete arrow;
    delete marker;
    delete MyPoints;
}
/**
 ******************************************************************
 *
 * Function Name :
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
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
void CBLPolar::Draw()
{
    TEllipse *el;
    TListIter next(circles);
    TText *txt;
    TListIter ntext(OtherGraphics);
    TPoints *pts;
    TListIter pnext(MyPoints);

    // Assume we are on some canvas etc. 
    TVirtualPad *current = gROOT->GetSelectedPad();
    if (current != 0)
    {
	current->Range(-Xmax, -Ymax, Xmax, Ymax);
    }

    // Make crosshair at middle. 
    xaxis->Draw();
    yaxis->Draw();

    while((el = (TEllipse *)next()))
    {
	el->Draw();
    }

    while ((txt = (TText *) ntext()))
    {
	txt->Draw();
    }
    while((pts = (TPoints *) pnext()))
    {
	if (MarkerSet)
	{
	    marker->Draw();
	}
	else
	{
	}
    }


}
