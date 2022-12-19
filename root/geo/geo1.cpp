#include <TROOT.h>
#include <TSystem.h>
#include <TGeoManager.h>

#include "geo1.hh"

ClassImp(Geo1)

Geo1::Geo1(int stuff): TTimer(500, kTRUE)
{
    c = new TCanvas("box shape", "A simple box", 700,800);
    c->SetFillColor(14);

    new TGeoManager("world","the simplest geometry");
    mat = new TGeoMaterial("Al", 26.98,13,2.7);
    med = new TGeoMedium("Air",1,mat);
    top = gGeoManager->MakeBox("Top",med, 10.,10.,10.);
    gGeoManager->SetTopVolume(top);
    vol = gGeoManager->MakeBox("BOX",med, 20,30,40);
    vol->SetLineColor(kBlue);
    vol->SetLineWidth(2);

    r1.SetAngles(45.,0.,0.);
    t1.SetTranslation(10.,0.,0.);

    //    TGeoCombiTrans c1(t1,r1);
    //c1.Print();

    top->AddNode(vol,1, &r1);
    gGeoManager->CloseGeometry();
    theta = 0.0;
    top->Draw();

    SetBit(kCanDelete);
    Draw();
    Animate();
    gSystem->AddTimer(this);        // start timer = start animation
}
Geo1::~Geo1()
{
    gSystem->RemoveTimer(this);
    delete vol;
    delete med;
    delete mat;
    delete c;
}
void Geo1::Paint(Option_t *t)
{
#if 0
    printf("Paint %f\n", theta);
    c->Modified();
    c->Update();
#endif
}
void Geo1::Animate()
{
    r1.SetAngles(theta,0,0);
    theta += 5.0;
    if (theta > 270.0)
    {
	gSystem->RemoveTimer(this);
    }
}
Bool_t Geo1::Notify()
{
    printf("Timeout %f\n", theta);
    Animate();
    c->Modified();
    c->Update();
    TTimer::Reset();
    return kFALSE;
}
