Int_t i;
Float_t ratio;
TSlider *slider;
TCanvas *c1;
TH2C *world;
Int_t *BackupBuffer, BufferSize;

void conway(Int_t niterations = 100000)
{
//
// Simple example illustrating how to use the C++ interpreter	
// to fill histograms in a loop and show the graphics results
// This program is a variant of the tutorial "hsum".
// It illustrates the use of Timers.

  c1 = new TCanvas("c1","Conway's Game of Life",200,10,600,400);
  c1->SetGrid();
  gStyle->SetOptStat(0000000);

// Create some histograms.
  world = new TH2C("world","playing ground1", 100, 0., 100.,
			 100, 0.,100.);
  world->Draw();
  BufferSize = world->GetBufferLength();
  printf("Buffersize = %d\n", BufferSize);

 

  slider = new TSlider("slider","test",0.92, 0.1, 0.97, 0.9, 38);
  slider->SetFillColor(46);
  c1->Update();
#if 0
  c1->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", 0, "",
               "exec3event(Int_t,Int_t,Int_t,TObject*)");
#endif
// Create a TTimer
  TTimer timer("conwayUpdate()",300); // update every 300 msec
  timer.TurnOn();

  for (Int_t i=0; i<niterations; i++) 
  {
      ratio = Float_t(i)/Float_t(niterations);
      UpdatePlayingField();
      if (gSystem->ProcessEvents()) break;
  }
  timer.TurnOff();
  conwayUpdate();
}
void UpdatePlayingField()
{
    int ix, iy, jx,jy;
    Int_t Nx = world->GetNbinsX();
    Int_t Ny = world->GetNbinsY();

    world->Reset();

    for (ix=0; ix<Nx; ix++)
    {
	for (iy=0; iy<Ny; iy++)
	{
	    
	}
    }

}
void conwayUpdate()
{
// called when Timer times out
   if (slider) 
   {
       slider->SetRange(0,ratio);
   }
   c1->Modified();
   c1->Update();
}

void exec3event(Int_t event, Int_t x, Int_t y, TObject *selected)
{
    // Trap events in the canvas here!
    TCanvas *c = (TCanvas *) gTQSender;
    printf("Canvas %s: event=%d, x=%d, y=%d, selected=%s\n", c->GetName(),
	   event, x, y, selected->IsA()->GetName());
}

