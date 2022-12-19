#include <TROOT.h>
#include <TSystem.h>
#include <TGeoManager.h>
#include <TView.h>
#include <TGeoPatternFinder.h>

#include "compass.hh"

ClassImp(Compass)

Compass::Compass(int stuff): TTimer(500, kTRUE)
{
    c = new TCanvas("box shape", "A simple box", 700,800);
    tv = new TView(2);
    c->SetFillColor(14);

    new TGeoManager("world","the simplest geometry");
    mat = new TGeoMaterial("Al", 26.98,13,2.7);
    med = new TGeoMedium("Al",1,mat);
    top = gGeoManager->MakeSphere("Top", med,   10., 11.,
				  0., 180., 0., 360.);
    gGeoManager->SetTopVolume(top);
    vol = gGeoManager->MakeSphere("Sphere",med, 24., 25.,
				  85.0, 95.0, 0., 360.0);
    vol->SetLineColor(kBlue);
    vol->SetLineWidth(2);
    TGeoPatternSphTheta( vol, 10);
    TGeoPatternSphPhi( vol, 60);

    r1.SetAngles(45.,0.,0.);
    t1.SetTranslation(10.,0.,0.);

    //    TGeoCombiTrans c1(t1,r1);
    //c1.Print();

    top->AddNode(vol,1, &r1);
    gGeoManager->CloseGeometry();

    top->Draw();
    TView *view = gPad->GetView();
    if (0)
    {
	view->SetSystem(4);
    }
    //      view->RotateView(248,66);
    //    view->ShowAxis();
    view->ToggleRulers(gPad);
    theta = 0.0;

    SetBit(kCanDelete);
    Draw();
    Animate();
    gSystem->AddTimer(this);        // start timer = start animation
}
Compass::~Compass()
{
    gSystem->RemoveTimer(this);
    delete vol;
    delete med;
    delete mat;
    delete c;
}
void Compass::Paint(Option_t *t)
{
#if 0
    printf("Paint %f\n", theta);
    c->Modified();
    c->Update();
#endif
}
void Compass::Animate()
{
    r1.SetAngles(theta,0,0);
    theta += 5.0;
    if (theta > 270.0)
    {
	gSystem->RemoveTimer(this);
    }
}
Bool_t Compass::Notify()
{
    printf("Timeout %f\n", theta);
    Animate();
    c->Modified();
    c->Update();
    TTimer::Reset();
    return kFALSE;
}
