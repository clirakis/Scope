/**
 ******************************************************************
 *
 * Module Name : 
 *
 * Author/Date : C.B. Lirakis / 02-May-08
 *
 * Description :
 *
 * Restrictions/Limitations :
 *
 * Change Descriptions :
 *
 * Classification : Unclassified
 *
 * References :
 *
 *
 * RCS header info.
 * ----------------
 * $RCSfile$
 * $Author$
 * $Date$
 * $Locker$
 * $Name$
 * $Revision$
 *
 * $Log: $
 *
 *
 *******************************************************************
 */
#include <time.h>

/**
 ******************************************************************
 *
 * Function Name :
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void SetPlot()
{
    TCanvas *Hobbes = new TCanvas("Dist","A test",5,5,1200,600);
    Hobbes->cd();
    TPad    *Calvin = new TPad("Calvin","Silly",0.01,0.01,0.99,0.99,10);
    Calvin->Draw();
    Calvin->cd();
    Calvin->SetGrid();
}
/**
 ******************************************************************
 *
 * Function Name :
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void BioRhythm(Int_t Year, Int_t Month, Int_t Day)
{
    const Double_t Lower = -15.0;
    /*
     * Cycle times (days)
     */
    const Double_t Physical     = 23.0;
    const Double_t Emotional    = 28.0;
    const Double_t Intellectual = 33.0;
    const Double_t Spiritual    = 53.0; 
    const Double_t Awareness    = 48.0; 
    const Double_t Aesthetic    = 43.0; 
    const Double_t Intuition    = 38.0; 
    const Double_t Upper = 15.0;

    Long_t JD_Birth = cvt2jd(Year, Month, Day);
    time_t now = time(NULL);
    struct tm *now_tm = localtime(&now);
    Int_t  year  = now_tm->tm_year + 1900;
    Int_t  month = now_tm->tm_mon + 1; 
    Long_t Today = cvt2jd( year, month, now_tm->tm_mday);
    Long_t Delta = Today - JD_Birth;
    SetPlot();
    
    TLegend *legend = new TLegend(0.8, 0.90, 0.9, 0.99);

    TF1 *physical = new TF1("Physical", "sin(2.0*TMath::Pi()*([0]+x)/[1])", 
			    Lower, Upper);
    physical->SetParameter(0, JD_Birth);
    physical->SetParameter(1, Physical);
    physical->SetLineColor(2);
    physical->Draw();
    legend->AddEntry(physical, "Physical");

    TF1 *emotional = new TF1("Emotional", "sin(2.0*TMath::Pi()*([0]+x)/[1])", 
			     Lower, Upper);
    emotional->SetParameter(0, JD_Birth);
    emotional->SetParameter(1, Emotional);
    emotional->SetLineColor(3);
    emotional->Draw("SAME");
    legend->AddEntry(emotional, "Emotional");

    TF1 *intellectual = new TF1("Intellectual", "sin(2.0*TMath::Pi()*([0]+x)/[1])", 
			     Lower, Upper);
    intellectual->SetParameter(0, JD_Birth);
    intellectual->SetParameter(1, Intellectual);
    intellectual->SetLineColor(4);
    intellectual->Draw("SAME");
    legend->AddEntry(intellectual,"Intellectual");

    legend->Draw();
}
/**
 ******************************************************************
 *
 * Function Name : cvt2jd
 *
 * Description : convert from year month and day to julian day. 
 *
 * Inputs : yr  - year to use. 
 *          mo  - month to use. 
 *          day - day to use 
 *
 * Returns : number of julian days. 
 *
 * Error Conditions :
 *
 *******************************************************************
 */
Long_t cvt2jd(Int_t yr,Int_t mo,Int_t day)
{
    Long_t mjd;  /*holds result*/
    Int_t ilp;       /*number of leap years from 1900 not counting current*/
    static Int_t dmo[12] = {0,31,59,90,120,151,181,212,243,273,304,334};
    /*
     * this subroutine receives a date in the form year, month and day
     * and returns the MJD corresponding to that day.  the year may
     * be specified as 90 or 1990.
     */
    if(yr > 1900) 
    {
	yr -= 1900;    /* convert to years since 1900 */
    }
    ilp=(yr-1)/4;		     /* number of leap years since 1900*/
    /*
     * compute number of days since 1900 + 1 day for each leap year
     * + number of days since start of this year
     */
    mjd = 365*(Long_t) yr + (Long_t) (ilp + dmo[mo-1] + day - 1);
    mjd += 15020;      /* add MJD of 1900 */
    /*
     * if current month is jan or feb then it does not matter if
     * current year is a leap year
     */
    if(mo < 3) 
    {
	return(mjd);
    }
    /*
     *	if current month is march or later then must add 1 day if
     *	current year is divisible by 4
     */
    if( (yr & 3) == 0)
    {
	mjd++;
    }
    return(mjd);
}
