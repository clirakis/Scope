 Bool_t comments = kTRUE;
Bool_t raytracing = kFALSE;
Bool_t rotate = kFALSE;
Bool_t axis = kTRUE;

void box3(Int_t iaxis=1, Int_t ndiv=8, Double_t start=0, Double_t step=0)
{
   TCanvas *c = new TCanvas("box shape", "A simple box", 700,800);
   Float_t p[] = {5.0, 5.0, 0.0};
   Float_t x[] = {0.0,10.0,20.0};
   Float_t y[] = {0.0,10.0,10.0};
   Float_t z[] = {0.0,10.0,30.0};

   new TGeoManager("box", "poza1");
   TGeoMaterial *mat = new TGeoMaterial("Al", 26.98,13,2.7);
   TGeoMedium *med = new TGeoMedium("MED",1,mat);
   TGeoVolume *top = gGeoManager->MakeBox("TOP",med,100,100,100);
   gGeoManager->SetTopVolume(top);
   TGeoVolume *vol = gGeoManager->MakeBox("BOX",med, 20,30,40);
   vol->SetLineColor(2);
   vol->SetLineWidth(2);
   top->AddNode(vol,1);
   if (iaxis) {
      TGeoVolume *slice1 = vol->Divide("SLICE",iaxis,ndiv,start,step);
      if (!slice1) return;
      slice1->SetLineColor(2);
   }
   gGeoManager->CloseGeometry();
   top->Draw();
   MakePicture();
   TPolyMarker3D *pm = new TPolyMarker3D( 1, p, 8);
   pm->SetMarkerColor(2);
   pm->Draw();
   TPolyLine3D *pa = new TPolyLine3D(3,x,y,z);
   pa->Draw();
}
void MakePicture()
{
//   gGeoManager->SetNsegments(80);
   TView *view = gPad->GetView();
   if (view) {
      view->RotateView(248,66);
      if (axis) view->ShowAxis();
      if (comments) view->MoveViewCommand('k',1);
   }
   Bool_t is_raytracing = gGeoManager->GetGeomPainter()->IsRaytracing();
   if (is_raytracing != raytracing) {
      gGeoManager->GetGeomPainter()->SetRaytracing(raytracing);
      gPad->Modified();
      gPad->Update();
   }   
}
      
//______________________________________________________________________________
void AddText(TPaveText *pave, const char *datamember, Double_t value, const char *comment)
{
   char line[128];
   for (Int_t i=0; i<128; i++) line[i] = ' ';
   memcpy(&line[0], datamember, strlen(datamember));
   line[10] = '=';
   char number[20];
   sprintf(number, "%5.2f", value);
   memcpy(&line[12], number, strlen(number));
   line[26] = '=';
   line[27] = '>';
   sprintf(&line[30], "%s",comment);
   TText *text = pave->AddText(line);
//   text->SetTextColor(4);
   text->SetTextAlign(12);//12
}

//______________________________________________________________________________
void AddText(TPaveText *pave, const char *datamember, Int_t value, const char *comment)
{
   char line[128];
   for (Int_t i=0; i<128; i++) line[i] = ' ';
   memcpy(&line[0], datamember, strlen(datamember));
   line[10] = '=';
   char number[20];
   sprintf(number, "%5i", value);
   memcpy(&line[12], number, strlen(number));
   line[26] = '=';
   line[27] = '>';
   sprintf(&line[30], "%s",comment);
   TText *text = pave->AddText(line);
//   text->SetTextColor(4);
   text->SetTextAlign(12);
}

//______________________________________________________________________________
void AddText(TPaveText *pave, TObject *pf, Int_t iaxis)
{
   char line[128];
   TGeoPatternFinder *finder = (TGeoPatternFinder*)pf;
   if (!pave || !pf) return;
   for (Int_t i=0; i<128; i++) line[i] = ' ';
   TGeoVolume *volume = finder->GetVolume();
   TGeoShape *sh = volume->GetShape();
   sprintf(line, "Division of %s on axis %d (%s)", volume->GetName(), iaxis,sh->GetAxisName(iaxis));
   TText *text = pave->AddText(line);
   text->SetTextColor(3);
   text->SetTextAlign(12);
   AddText(pave, "fNdiv",finder->GetNdiv(),"number of divisions");
   AddText(pave, "fStart",finder->GetStart(),"start divisioning position");
   AddText(pave, "fStep",finder->GetStep(),"division step");
}
