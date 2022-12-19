
#include <TROOT.h>
#include <TSystem.h>

#include "Meter.hh"


ClassImp(Meter)


Float_t MinuteHand::fgMinuteHandX[] = { -0.05, 0, 0.05 };
Float_t MinuteHand::fgMinuteHandY[] = { -0.04, 0.625, -0.04 };

Float_t HourHand::fgHourHandX[] = { -0.05, 0, 0.05 };
Float_t HourHand::fgHourHandY[] = { -0.04, 0.4, -0.04 };

Float_t SecondHand::fgSecondHandX[] = { 0., 0.035, 0., -0.035 };
Float_t SecondHand::fgSecondHandY[] = { 0.78, 0.73, 0.68, 0.73 };

TDatime *ClockHand::fgTime = new TDatime;


TPolygon::TPolygon(Int_t n, Float_t *x, Float_t *y) : TPolyLine(n+1,x,y)
{
   // Create filled polygon. Polygon will be added to current pad.

   if (!TVirtualPad::Pad()) { Error("Constructor","Create TPad first!"); return; }

   fPad  = (TPad*)TVirtualPad::Pad();
   fX[n] = x[0];             // create an extra point to connect polyline ends
   fY[n] = y[0];
   Draw();                   // append to fPad
}

void TPolygon::Paint(Option_t *)
{
   // Paint filled polygon.

   TAttLine::Modify();
   TAttFill::Modify();
   fPad->PaintFillArea(fN-1, fX, fY);
   fPad->PaintPolyLine(fN, fX, fY, "");
}



ClockHand::ClockHand(Int_t n, Float_t *x, Float_t *y) : TPolygon(n,x,y)
{
   // Create clockhand

   fPrevTimeValue = 0;
   fX0 = new Float_t[GetN()];   // initial shape and position
   fY0 = new Float_t[GetN()];   // initial shape and position

   for (int i = 0; i < GetN(); i++) {
     fX0[i] = fX[i];
     fY0[i] = fY[i];
   }
   SetFillColor(9);            // default fill color
}

void ClockHand::Update()
{
   // Update hand position

   if (!IsModified()) return;

   fPrevTimeValue = GetTimeValue();
   Move(GetHandAngle());
}

void ClockHand::Move(Float_t clock_angle)
{
   // Move clockhand.

  Int_t n = GetN();

  // ClockPoints used to rotate,scale and shift initial points
  static ClockPoints *points = new ClockPoints();

  Float_t wh = (Float_t)fPad->XtoPixel(fPad->GetX2());
  Float_t hh = (Float_t)fPad->YtoPixel(fPad->GetY1());

  for (int i = 0; i < n; i++) {
     points->SetXY(fX0[i],fY0[i]);
     points->Rotate(clock_angle);

     if (wh < hh) 
     {
	 points->Scale(0.5,0.5*wh/hh);    //  scale to have a circular clock
     }
     else         
     {
	 points->Scale(0.5*hh/wh,0.5);
     }

     points->Shift(0.5,0.5);                      // move to the center of pad
     fX[i] = points->GetX();
     fY[i] = points->GetY();
   }
}
ArrowHand::ArrowHand(Float_t size) : TArrow( 0.5, 0.5, 0.5, 0.95, size)
{
   fPad  = (TPad*)TVirtualPad::Pad();
   Draw();                   // append to fPad
}
ArrowHand::~ArrowHand()
{
}
void ArrowHand::Move(Float_t angle)
{
    const Float_t deg2rad = .017453292519943295769F;  // pi/180
    const Float_t r=0.25;
    Float_t A = deg2rad *angle;
    rX = r*TMath::Cos(A)+r*TMath::Sin(A) + 0.5;
    rY = r*TMath::Cos(A)-r*TMath::Sin(A) + 0.5;
}
void ArrowHand::Paint(Option_t *)
{
   TAttLine::Modify();
   TAttFill::Modify();
   PaintArrow( 0.5, 0.5, rX, rY);
}

Meter::Meter(Int_t csize) : TTimer(500, kTRUE)
{
   // Create a clock in a new canvas.

   fPad = new TCanvas("Meter:canvas","xclock",-csize,csize);

   fPad->SetFillColor(14);     // grey
   Pointer = new ArrowHand();

   SetBit(kCanDelete);
   Draw();                         // append this Meter to fPad
   Animate();
   gSystem->AddTimer(this);        // start timer = start animation
}

Meter::~Meter()
{
   // Clean up the clock.

}

void Meter::Paint(Option_t *)
{
   // Just draw clock scale (time and minutes ticks)

   static ClockPoints *point1 = new ClockPoints();
   static ClockPoints *point2 = new ClockPoints();

   Float_t wh = (Float_t)fPad->XtoPixel(fPad->GetX2());
   Float_t hh = (Float_t)fPad->YtoPixel(fPad->GetY1());

   for (int i = 0; i < 60; i++ ) {             // draw minute/hour ticks
      point1->SetXY(0.,0.9);

      if (!(i%5)) point2->SetXY(0.,0.8);       // hour  ticks  are longer
      else        point2->SetXY(0.,0.87);

      Float_t angle = 6.*i;
      point1->Rotate(angle);
      point2->Rotate(angle);

      if (wh < hh) {                   // scale in oder to draw circle scale
         point1->Scale(0.5,0.5*wh/hh);
         point2->Scale(0.5,0.5*wh/hh);
      } else {
         point1->Scale(0.5*hh/wh,0.5);
         point2->Scale(0.5*hh/wh,0.5);
      }

      point1->Shift(0.5,0.5);              // move to center of pad
      point2->Shift(0.5,0.5);

      fPad->PaintLine(point1->GetX(),point1->GetY(),point2->GetX(),point2->GetY());
   }
}

void Meter::Animate()
{
    static Float_t angle = 0.0;
    Pointer->Move(angle);
    angle += 5.0;

   fPad->Modified();               //  drawing ...
   fPad->Update();
}

Bool_t Meter::Notify()
{
   // Actions after timer's time-out

   Animate();
   TTimer::Reset();
   return kFALSE;
}
