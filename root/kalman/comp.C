{
    TMultiGraph *tmg = new TMultiGraph();

    TGraph *tg1 = new TGraph("baseline.dat");
    tg1->SetLineColor(2);
    tmg->Add(tg1);

    TGraph *tg2 = new TGraph("test.dat");
    tg2->SetLineColor(3);
    tmg->Add(tg2);
    
    tmg->Draw("ALP");
}
