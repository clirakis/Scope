{
    TCanvas *Hobbes = new TCanvas("Dist","A test",5,5,1200,600);
    Hobbes->cd();
    TPad    *Calvin = new TPad("Calvin","Silly",0.02,0.02,0.99,0.99,10);
    Calvin->Draw();
    Calvin->cd();

    Calvin->SetGrid();	

    const Int_t NX = 100;
    const Int_t NY = 100;
    Int_t i,j;
    Double_t X,Y, W,  dx, dy;
    TH2F *th2 = new TH2F( "sphere", "Sphere", NX, -TMath::Pi(), 
			  TMath::Pi(), NY, 0., TMath::Pi());

    dx = TMath::TwoPi()/NX;
    dy = TMath::Pi()/NY;
    X = -TMath::Pi();
    for (i=0;i<NX;i++)
    {
	X += dx;
	Y = 0.0;
	for(j=0;j<NY;j++)
	{
	    Y+=dy;
	    W = (Double_t) ((i+j)%50);
	    th2->Fill(X,Y,W);
	}
    }
    //    th2->Draw("LEGO:SPH");
    th2->Draw("SURF1:SPH");
    //    th2->Draw("SURF1:POL");

}
