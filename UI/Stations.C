{
    TCanvas *Hobbes = new TCanvas("Radio Stations","FFT Radio Stations",5,5,1200,600);
    Hobbes->cd();
    TPad    *Calvin = new TPad("Calvin","Silly",0.02,0.02,0.99,0.99,10);
    Calvin->Draw();

    // Set the grid on. 
    Hobbes->SetGrid(); 
    // Open the file with the FFT data in it. 
    //TFile *tf = new TFile("FFT10.root");
    TFile *tf = new TFile("FFT_1M_3.root");

    // Draw the FFT. 
    Trace->Draw("ALP");

    TH1 *h = Trace->GetHistogram();
    h->SetXTitle("Frequency (Hz)");
    h->SetYTitle("dBm");
    h->SetLabelSize(0.03,"X");
    h->SetLabelSize(0.03,"Y");


    cout << WFMPRE->GetString() << endl;
    cout << "===========================================" << endl;
    cout << Channel->GetString() << endl;


    MarkStations();

    Double_t pv = GetPeakValue(660.0e3, Trace); // Power in dBm.
    Convert2Volts(pv);

    //CompareStations(Trace);
    ScanForPeaks(Trace);
}
/*
 * https://radio-locator.com/cgi-bin/finder?sr=Y&s=C&call=WMCA&nav=home
 */
struct t_Station {
    const char* text;
    Double_t    Distance;        // miles
    Double_t    Freq;            // kHz
    Double_t    SourcePower[2];  // Watts - Day/night
    Double_t    Distance2Source; // meters
} AMStations[9] = {{"WMCA 570" , 38.2,  570.0, { 5000, 5000}, 64270.0}, 
		   {"WFAN 660" , 30.4,  660.0, {50000,50000}, 50390.0}, 
		   {"WOR 710"  , 35.1,  710.0, {50000,50000}, 59000.0}, 
		   {"WABC 770" , 29.3,  770.0, {50000,50000}, 49720.0}, 
		   {"WNBC 820" , 38.2,  820.0, {50000,50000}, 64270.0}, // ????
		   {"WCBS 880" , 30.4,  880.0, {50000,50000}, 50510.0}, 
		   {"WGNY 1220", 19.5, 1220.0, {10000.0, 180.0}, 30780.0}, 
		   {"WLNA 1420",  1.4, 1420.0, {5000.0,1000.0} ,  2000.0},
		   {NULL, 0.0, 0.0}};

void MarkStations(void)
{
    UInt_t    i = 0;
    TLine     *tl; 
    TPaveText *tpt;
    // Example unused code.
    // TPaveLabel is in user coordnates. 
    //TPaveLabel *PL_WFAN = new TPaveLabel( 660.0e3, -50, 700.0e3, 
    // 					  -55.0, "WFAN 660");
    //TPaveText *PL_WFAN = new TPaveText( 0.35, 0.4, 0.4, 0.45, "NDC");

    Double_t Lower = -90.0;  // -70.0
    Double_t Upper = -40.0;  // -20.0
    Double_t x0, x1, y0, y1;
    //y0 = -35.0;
    y0 = -40.0;
    Double_t ystep = 2.5;

    while (AMStations[i].text != NULL)
    {
	x0 = AMStations[i].Freq*1.0e3;
	x1 = x0 + 160.0e3;
	y1 = y0 + ystep;
	// Put some lines where there are radio stations.
	tpt = new TPaveText( x0, y0, x1, y1);
	tpt->AddText(AMStations[i].text);
	tpt->Draw();
	tl = new TLine ( x0, Lower, x0, Upper);
	tl->SetLineColor(kRed);
	tl->Draw();
	i++;
	y0 -= ystep;
    }
}
Double_t GetPeakValue(Double_t F, TGraph* tg)
{
    Double_t XPTS  = 2048.0;
    Double_t XINCR = 9.765625E+2;
    Double_t dx;

    Int_t N = tg->GetN();
    Int_t index=0;
    Int_t window = 10;
    do 
    {
	dx = F - tg->GetPointX(index);
	index++;
    } while((index<N) && (dx>0.0));

    UInt_t i;
    Double_t MaxdB = -90.0;
    UInt_t   IMax = 0;
    Double_t Fpt, dBm;
    for (i=index-window;i< index+window;i++)
    {
	Fpt = tg->GetPointX(i);
	dBm = tg->GetPointY(i);
// 	cout << "Index: " << i
// 	     << " Freq: " << Fpt
// 	     << " dBm: "  << dBm
// 	     << endl;
	if (dBm > MaxdB)
	{
	    IMax  = i;
	    MaxdB = dBm;
	}
    }
    //cout << "Max: " << MaxdB << endl;
    return MaxdB;
}
Double_t Convert2Volts(Double_t dBm)
{
    Double_t rv = 0.0;
    const Double_t FreeSpaceImpedance = 377.0; // Ohms

    Double_t watts = pow(10.0, dBm/10.0) * 1.0e-3; // Watts. 
    cout << "Watts:" << watts << endl;
    rv = sqrt( watts * FreeSpaceImpedance);
    cout << "Volts/m: " << rv << endl;
    return rv;
}

Double_t Propagate(Int_t index, bool day)
{
    Int_t j;
    if (day)
	j = 0;
    else
	j = 1;
    /*
     * Assume an isotropic pattern. 
     * Use the distance and spread, no gain. 
     * Well assume that none goes into the earth. 
     */
    Double_t r = AMStations[index].Distance2Source;
    Double_t P = AMStations[index].SourcePower[j];

    // Surface Area of sphere 4/3 * pi * r^2
    Double_t A = 4.0/3.0 * TMath::Pi() * r * r;
    // Divide by 2, only upper hemisphere
    A = A/2.0;

    // new Power per unit area
    Double_t rv = P/A;
    return rv;
}  

void CompareStations(TGraph *tg)
{
    Int_t i;
    Double_t Theory, dBm_Theory[8], dBm[8], Diff;

    // Loop over all stations. 
    for (i=0;i<9;i++)
    {
	Theory   = Propagate(i, true); // This gives us watts at destination.
	dBm_Theory[i] = 10.0*TMath::Log10(Theory/1.0e-3);

	// What did we measure on our antenna?
	dBm[i]      = GetPeakValue(AMStations[i].Freq*1.0e3, tg);

	Diff   = dBm[i] - dBm_Theory[i]; 
	cout << AMStations[i].text 
	     << " Theory: " << dBm_Theory[i]
	     << " Measured: " << dBm[i]
	     << " Difference: " << Diff 
	     << endl;
    }
    TF1 *tf = new TF1("Linear", "[0]*x+[1]", -80.0, -10.0);
    TCanvas *Rod = new TCanvas("Power correlation","Correlation",5,5,1200,600);
    TGraph  *corr= new TGraph(8, dBm_Theory, dBm);
    Rod->SetGrid();
    corr->SetMarkerStyle(kStar);
    corr->Draw("AP");
    corr->Fit(tf,"SV");
    //corr->FitPanel();
    cout << "Drawing complete." << endl;
    cout << "Corr factor." << corr->GetCorrelationFactor() << endl;
}
void ScanForPeaks(TGraph *tg)
{
    Double_t XINCR = 9.765625E+2;
    Double_t dy;

    Int_t N = tg->GetN();
    Double_t Freq, dBm, Prev_dBm;
    UInt_t i;
    Double_t X[2048], Y[2048];

    Double_t LowerCutoff = -86.0; // dBm cuttoff. 
    memset(X, 0, 2048*sizeof(Double_t));
    memset(Y, 0, 2048*sizeof(Double_t));
    /*
     *  Not efficient, but works, eliminate everything below the cutoff. 
     */
    for (i=0;i<N;i++)
    {
	Freq = tg->GetPointX(i);
	dBm  = tg->GetPointY(i);
	X[i] = Freq;
	if (dBm>LowerCutoff)
	{
	    Y[i] = dBm;
	}
    }

    /*
     * Now loop over this smaller set. 
     */
    Prev_dBm = -96.0;  // lowest 16 bit value
    for (i=0;i<N;i++)
    {
	Freq = X[i];
	dBm  = Y[i];
	dy   = dBm - Prev_dBm;
	if (dy < 0.0)
	{
	    cout << "Index: " << i
		 << " Freq: " << X[i-1]
		 << " dBm: "  << Y[i-1]
		 << endl;
	}
	Prev_dBm = dBm;
    }
}
