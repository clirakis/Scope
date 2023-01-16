/*
 *  Low pass butterworth filter with real components. 
 *          "Electronics Equations Handbook" by Stephen J Erst
 *              1989 tab books, page 62
 *
 */
{
    Int_t NTaps = 2;
    Int_t    i;
    Double_t Cap, L;
    Double_t Resistance = 1.0; // ohms
    Double_t Frequency  = 60.0; // Hz
    Double_t theta;

    Bool_t odd = true;
    for(i=1;i<=NTaps;i++)
    {
	if (odd)
	{
	    theta = TMath::Pi() * ((Double_t) (2*i-1))/((Double_t) 2*NTaps);
	    Cap = 1.0/(TMath::Pi()*Frequency*Resistance) * sin(theta);
	    cout << "C"<< i << " : " << Cap/2.0 << endl;
	    odd = false;
	}
	else
	{
	    theta = TMath::Pi() * ((Double_t) (2*i-1))/((Double_t) 2*NTaps);
	    L = Resistance/(TMath::Pi()*Frequency) * sin(theta);
	    cout << "L" << i-1 << " : " << L << endl;
	    odd = true;
	}
    }
}
