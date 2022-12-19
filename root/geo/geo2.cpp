
#include <TROOT.h>
#include <TSystem.h>
#include "geo2.hh"


ClassImp(Geo2)



Geo2::Geo2(Int_t csize) : TTimer(500, kTRUE)
{
   // Create a clock in a new canvas.

   fPad = new TCanvas("Geo2:canvas","xclock",-csize,csize);

   fPad->SetFillColor(14);     // grey

   SetBit(kCanDelete);
   Draw();                         // append this Geo2 to fPad
   Animate();
   gSystem->AddTimer(this);        // start timer = start animation
}

Geo2::~Geo2()
{
}

void Geo2::Paint(Option_t *)
{
}

void Geo2::Animate()
{
   fPad->Modified();               //  drawing ...
   fPad->Update();
}

Bool_t Geo2::Notify()
{
   // Actions after timer's time-out
    printf("Timeout\n");
   Animate();
   TTimer::Reset();
   return kFALSE;
}
