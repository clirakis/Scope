{
    TCanvas *Hobbes = new TCanvas("Dist","A test",5,5,1200,600);
    Hobbes->cd();
    TPad    *Calvin = new TPad("Calvin","Silly",0.01,0.01,0.99,0.99,10);
    Calvin->Draw();
    Calvin->cd();
    Calvin->SetGrid();
    /* X here is omega/omega_not where Omega not = 1.0/R*C */
    TF1 *lp = new TF1("LowPass", 
		      "1.0/(1.0+x*x)", 0.0, 1.0);
    lp->Draw();
    
    TH1 *hbs = lp->GetHistogram();
    if (hbs)
    {
	hbs->SetYTitle("Response");
	hbs->SetXTitle("");
	hbs->SetLabelSize(0.03,"X");
	hbs->SetLabelSize(0.03,"Y");
    }
}
