{
    TCanvas *Hobbes = new TCanvas("Radio Stations","Time Varying",5,5,1200,600);
    Hobbes->cd();
    TPad    *Calvin = new TPad("Calvin","Silly",0.02,0.02,0.99,0.99,10);
    Calvin->Draw();
    Calvin->cd();
    // Set the grid on. 
    Calvin->SetGrid(); 

    TMultiGraph *tmg = new TMultiGraph();

    TGraph *tgf1 = new TGraph("Raw.txt");
    tgf1->SetMarkerStyle(21);
    tgf1->SetLineColor(2);
    tmg->Add(tgf1);
    //tgf1->Draw("ALP");

    TGraph *tgf2 = new TGraph("filtered.txt");
    tgf2->SetMarkerStyle(22);
    tgf2->SetMarkerColor(3);
    tgf2->SetLineColor(3);
    tmg->Add(tgf2);
    //tgf2->Draw("ALP");

    tmg->Draw("ALP");

    tmg->GetXaxis()->SetTimeDisplay(1);
    tmg->GetXaxis()->SetNdivisions(503);
    tmg->GetXaxis()->SetTimeFormat("%H:%M");
    tmg->GetXaxis()->SetTimeOffset(-5, "EST");

    TH1 *h = tmg->GetHistogram();
    h->SetXTitle("Time");
    h->SetYTitle("dBm");
    h->SetLabelSize(0.03,"X");
    h->SetLabelSize(0.03,"Y");
}
