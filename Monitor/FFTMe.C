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
    TFile *tf = new TFile("/home/clirakis/Data/2023AMonitor014_07.root");

    /*
     * Index:Time:nTime:FMax:dBMax:dBArea:WFAN:WCBS:WRVP:WLNA
     */
    Stations->Draw("dBMax:Time","", "goff");
    Int_t     N = Stations->GetSelectedRows();
    Double_t *X = Stations->GetV2();
    Double_t *Y = Stations->GetV1();

    SaveData( N, X, Y);

    /*
     * https://root.cern/doc/master/FFT_8C.html
     * https://root.cern.ch/doc/master/classTVirtualFFT.html
     * inputs 
     *   N Dimensions
     *   number of points per dimension (could be array)
     *   options
     *     "C2CFORWARD" - a complex input/output discrete Fourier transform 
     *                    (DFT) in one or more dimensions, -1 in the exponent
     * 
     *      "C2CBACKWARD"- a complex input/output discrete Fourier transform 
     *                     (DFT) in one or more dimensions, +1 in the exponent
     * 
     *      "R2C" - a real-input/complex-output discrete Fourier transform 
     *              (DFT) in one or more dimensions,
     *
     *      "C2R" - inverse transforms to "R2C", taking complex input 
     *              (storing the non-redundant half of a logically 
     *               Hermitian array) to real output
     *
     *      "R2HC" - a real-input DFT with output in halfcomplex format, 
     *               i.e. real and imaginary parts for a transform of 
     *               size n stored as: 
     *                    r0, r1, r2, ..., rn/2, i(n+1)/2-1, ..., i2, i1
     *
     *      "HC2R" - computes the reverse of FFTW_R2HC, above
     *
     *       DHT" - computes a discrete Hartley transform
     */

    TVirtualFFT *fftr2c = TVirtualFFT::FFT(1, &N, "R2C");
    fftr2c->SetPoints(Y);
    fftr2c->Transform();

    //Use the following method to get the full output:
    Double_t *re_full = new Double_t[N];
    Double_t *im_full = new Double_t[N];
    Double_t *mag     = new Double_t[N];
    Double_t *XT      = new Double_t[N];

    fftr2c->GetPointsComplex( re_full, im_full);

    // Supress DC component
    for (Int_t i=1;i<N/2;i++)
    {
	XT[i-1]  = (Double_t) 2*i;  // Sample time is every 2 seconds. 
	mag[i-1] = sqrt(pow(re_full[i],2.0)+pow(im_full[i],2.0));
    }
 
    TGraph *fftg = new TGraph(N/2-1, XT, mag);
    fftg->Draw("ALP");

    delete[] re_full;
    delete[] im_full;
    delete[] mag;
    delete[] XT;
}
void SaveData(Int_t N, Double_t *X, Double_t *Y)
{
    // Input X is in epoch seconds
    ofstream saveme("WFAN.txt");

    if (saveme.is_open())
    {
	for (Int_t i=0;i<N;i++)
	{
	    saveme << X[i]-X[0] << "," << Y[i] << endl;
	}
	saveme.flush();
	saveme.close();
    }
}
