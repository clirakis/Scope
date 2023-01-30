{
    TCanvas *Hobbes = new TCanvas("Radio Stations","Time Varying",5,5,1200,600);
    Hobbes->cd();
    TPad    *Calvin = new TPad("Calvin","Silly",0.02,0.02,0.99,0.99,10);
    Calvin->Draw();

    // Set the grid on. 
    Hobbes->SetGrid(); 
#if 1
    //char filename[128];
    //memset(filename, 0, sizeof(filename));
    //cout << "Enter file name: ";
    //getline(filename, sizeof(filename), cin);
    //cin >> filename;
    const char *filetypes[] = { 
	"ROOT files",    "*.root",
	0,               0 };

    TGFileInfo fi;
    char *InitialDir = (char *)"/home/clirakis/Data/";
    fi.fFileTypes = filetypes;
    fi.fIniDir    = InitialDir;

    new TGFileDialog( gClient->GetRoot(), 0, kFDOpen, &fi);

    // Open the file with the data in it. 
    TFile *tf = new TFile(fi.fFilename);
#else
    // Open the file with the data in it. 
    TFile *tf = new TFile("/home/clirakis/Data/2023AMonitor007_09.root");
#endif
    /*
     * BC Band Setup
     * Index:Time:nTime:FMax:dBMax:dBArea:WFAN:WCBS:WRVP:WLNA
     * 
     * Low frequency setup
     * Index:Time:nTime:FMax:dBMax:dBArea:TEN:TWE:THIT:FOUR";
     */
    Stations->Draw("dBMax:Time","","groff");
    Int_t     N = Stations->GetSelectedRows();
    Double_t *X = Stations->GetV2();
    Double_t *Y = Stations->GetV1();
    TGraph *tgmax = new TGraph(N, X, Y);
    tgmax->SetMarkerStyle(22);
    tgmax->Draw("ALP");

    tgmax->SetTitle(fi.fFilename);
    tgmax->GetXaxis()->SetTimeDisplay(1);
    tgmax->GetXaxis()->SetNdivisions(503);
    tgmax->GetXaxis()->SetTimeFormat("%H:%M");
    tgmax->GetXaxis()->SetTimeOffset(-5, "EST");

    TH1 *h = tgmax->GetHistogram();
    h->SetXTitle("Time");
    h->SetYTitle("dBm");
    h->SetLabelSize(0.03,"X");
    h->SetLabelSize(0.03,"Y");


    cout << Comments->GetString() << endl;
    cout << FFT->GetString() << endl;
    cout << "===========================================" << endl;

}
