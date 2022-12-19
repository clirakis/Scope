Bool_t comments = kFALSE;
Bool_t raytracing = kFALSE;
Bool_t rotate = kFALSE;
Bool_t axis = kTRUE;
void box1(Int_t iaxis=1, Int_t ndiv=8, Double_t start=0, Double_t step=0)
{
   TCanvas *c = new TCanvas("box shape", "A simple box", 700,800);
   c->Divide(1,2,0,0);
   c->cd(1);

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
      TGeoVolume *slice = vol->Divide("SLICE",iaxis,ndiv,start,step);
      if (!slice) return;
      slice->SetLineColor(2);
   }
   gGeoManager->CloseGeometry();
   top->Draw();
   MakePicture();

   c->cd(2);
   TPaveText *pt = new TPaveText(0.01,0.01,0.99,0.99);
   pt->SetLineColor(1);
   TGeoBBox *box = (TGeoBBox*)(vol->GetShape());
   TText *text = pt->AddText("TGeoBBox - box class");
   text->SetTextColor(2);
   AddText(pt,"fDX",box->GetDX(),"half length in X");
   AddText(pt,"fDY",box->GetDY(),"half length in Y");
   AddText(pt,"fDZ",box->GetDZ(),"half length in Z");
   AddText(pt,"fOrigin[0]",(box->GetOrigin())[0],"box origin on X");
   AddText(pt,"fOrigin[1]",(box->GetOrigin())[1],"box origin on Y");
   AddText(pt,"fOrigin[2]",(box->GetOrigin())[2],"box origin on Z");
   if (iaxis) AddText(pt, vol->GetFinder(), iaxis);
   pt->AddText("Execute: box(iaxis, ndiv, start, step) to divide this.");
   pt->AddText("----- IAXIS can be 1, 2 or 3 (X, Y, Z)");
   pt->AddText("----- NDIV must be a positive integer");
   pt->AddText("----- START must be a valid axis offset within shape range on divided axis");
   pt->AddText("----- STEP is the division step. START+NDIV*STEP must be in range also");
   pt->AddText("----- If START and STEP are omitted, all range of the axis will be divided");
   pt->AddText(" ");
   pt->SetTextSize(0.044);
   pt->SetAllWith("-----","color",2);
   pt->SetAllWith("-----","font",72);
   pt->SetAllWith("-----","size",0.04);
   pt->SetAllWith("Execute","color",4);
   pt->SetTextAlign(12);
   pt->Draw();
   c->cd(1);
}
void MakePicture()
{
//   gGeoManager->SetNsegments(80);
   TView *view = gPad->GetView();
   if (view) {
      view->SetSystem(2);
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
