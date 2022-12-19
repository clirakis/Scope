
///////////////////////////////////////////////////////////////////
//  ROOT implementation of the X11 xclock.
//
//  To run this example do the following:
//  $ root
//  root [0] gSystem.Load("Aclock")
//  root [1] Aclock a
//  <enjoy>
//  root [2] .q
//
//  Other ROOT fun examples: Tetris, Hello ...
///////////////////////////////////////////////////////////////////

#ifndef GEO2_H
#define GEO2_H

#include <TTimer.h>
#include <TCanvas.h>
#include <TPolyLine.h>
#include <TDatime.h>
#include <TPoints.h>



class Geo2 : public TTimer {

private:
   TPad       *fPad;            // pad where this clock is drawn

public:
   Aclock(Int_t csize=100);
   virtual ~Aclock();

   virtual Bool_t Notify();
   void   Paint(Option_t *option);
   void   Animate();

   ClassDef(Geo2,0) 
};

#endif 
