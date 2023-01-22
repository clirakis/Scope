{
    TCanvas *Hobbes = new TCanvas("Radio Stations","Time Varying",5,5,1200,600);
    Hobbes->cd();
    TPad    *Calvin = new TPad("Calvin","Silly",0.02,0.02,0.99,0.99,10);
    Calvin->Draw();
    Calvin->cd();
    // Set the grid on. 
    Calvin->SetGrid(); 

    TGraph *tgf = new TGraph("filtered.txt");
    tgf->SetMarkerStyle(22);
    tgf->Draw("ALP");

    tgf->GetXaxis()->SetTimeDisplay(1);
    tgf->GetXaxis()->SetNdivisions(503);
    tgf->GetXaxis()->SetTimeFormat("%H:%M");
    tgf->GetXaxis()->SetTimeOffset(-5, "EST");

    TH1 *h = tgf->GetHistogram();
    h->SetXTitle("Time");
    h->SetYTitle("dBm");
    h->SetLabelSize(0.03,"X");
    h->SetLabelSize(0.03,"Y");



}
