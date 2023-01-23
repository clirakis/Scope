{
    /*
     * Page 197 of root users manual has info on setting up plotting. 
     */
    gStyle->SetOptStat(1100);

    TCanvas *Hobbes = new TCanvas("Radio","Time Varying",5,5,1200,600);
    Hobbes->Draw();
    Hobbes->cd();
    TPad    *Calvin = new TPad("Calvin","Silly",0.02,0.02,0.99,0.99, 33, 4, 1);
    Calvin->Draw();
    Calvin->cd();

    // Set the grid on. 
    Calvin->SetGrid(); 
    // Open the file with the data in it. 
    TFile *tf = new TFile("/home/clirakis/Data/2023AMonitor017_00.root");

    TMultiGraph *tmg = new TMultiGraph();
    TLegend     *tl  = new TLegend(0.1,0.7,0.48,0.9);
    tl->SetHeader("signal strength variation.");
    /*
     * Index:Time:nTime:FMax:dBMax:dBArea:WFAN:WCBS:WRVP:WLNA
     */
    Stations->Draw("dBMax:Time","", "goff");
    Int_t     N = Stations->GetSelectedRows();
    Double_t *X = Stations->GetV2();
    Double_t *Y = Stations->GetV1();
    TGraph *tgmax = new TGraph(N, X, Y);
    tgmax->SetMarkerStyle(20);
    tmg->Add(tgmax);
    tl->AddEntry(tgmax, "WFAN 660");

    Stations->Draw("WLNA:Time", "", "goff");
    N = Stations->GetSelectedRows();
    X = Stations->GetV2();
    Y = Stations->GetV1();
    TGraph *wlna = new TGraph(N, X, Y);
    wlna->SetMarkerStyle(21);
    wlna->SetLineColor(2);
    tmg->Add(wlna);
    tl->AddEntry(wlna, "WLNA 1420");

    Stations->Draw("WCBS:Time", "", "goff");
    N = Stations->GetSelectedRows();
    X = Stations->GetV2();
    Y = Stations->GetV1();
    TGraph *wcbs = new TGraph(N, X, Y);
    wcbs->SetMarkerStyle(22);
    wcbs->SetLineColor(3);
    tmg->Add(wcbs);
    tl->AddEntry(wcbs, "WCBS 880");

    Stations->Draw("WRVP:Time", "", "goff");
    N = Stations->GetSelectedRows();
    X = Stations->GetV2();
    Y = Stations->GetV1();
    TGraph *wrvp = new TGraph(N, X, Y);
    wrvp->SetMarkerStyle(23);
    wrvp->SetLineColor(4);
    tmg->Add(wrvp);
    tl->AddEntry(wrvp, "WRVP 1310");


    // ==================================================================
    tmg->Draw("ALP");
    tl->Draw();

    tmg->GetXaxis()->SetTimeDisplay(1);
    tmg->GetXaxis()->SetNdivisions(503);
    tmg->GetXaxis()->SetTimeFormat("%H:%M");
    tmg->GetXaxis()->SetTimeOffset(-5, "EST");

    TH1 *h = tmg->GetHistogram();
    h->SetXTitle("Time");
    h->SetYTitle("dBm");
    h->SetLabelSize(0.03,"X");
    h->SetLabelSize(0.03,"Y");


    cout << Comments->GetString() << endl;
    cout << "===========================================" << endl;


    TCanvas *Rod  = new TCanvas("Radio Stations","Correlation",5,5,1200,600);
    Rod->Draw();
    Rod->cd();
    TPad    *Noko = new TPad("Noko","Silly",0.04,0.04,0.96,0.96, 21, 5, 1);
    Noko->Draw();
    Noko->cd();
    Noko->SetGrid();

    //Stations->Draw("WFAN:WRVP","(WFAN>-85)&&(WRVP>-85)", "groff");
    Stations->Draw("WFAN:WRVP","", "groff");
    N = Stations->GetSelectedRows();
    X = Stations->GetV2();
    Y = Stations->GetV1();
    TGraph *corr = new TGraph(N, X, Y);
    corr->SetTitle("Correlation between WFAN and WRVP");
    corr->SetMarkerStyle(23);
    corr->SetLineColor(4);
    corr->Draw("AP");
    cout << "Correlation: " << corr->GetCorrelationFactor() << endl;
}
