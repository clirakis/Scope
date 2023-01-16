/**
 ******************************************************************
 *
 * Module Name : Butterworth.hh
 *
 * Author/Date : C.B. Lirakis / 15-Jan-23
 *
 * Description : Implementation of butterworth low pass filter
 *
 * Restrictions/Limitations : NONE
 *
 * Change Descriptions :
 *
 * Classification : Unclassified
 *
 * References :
 * https://github.com/nxsEdson/Butterworth-Filter/blob/master/butterworth.cpp
 * and a bunch more
 *
 *******************************************************************
 */
#ifndef __BUTTERWORTH_hh_
#define __BUTTERWORTH_hh_
#  include <vector>
#  include <complex>
#  include <stdint.h>

/// Create a butterworth filter
class Butterworth {
public:

    // What type of filter is it? 
    enum FilterType {kBANDPASS=0, kLOWPASS, kHIPASS, kALOWPASS};


    /*!
     * Description: 
     *  Butterworth constructor.   
     *
     * Arguments:
     *   
     *
     * returns:
     *    
     */
    Butterworth(uint16_t FilterOrder, double Lcutoff, double Ucutoff, 
		FilterType Type=kBANDPASS);


    /// Default destructor
    /*!
     * Description: 
     *  
     *
     * Arguments:
     *   
     *
     * returns:
     *    
     */
    ~Butterworth();


    /// Butterworth function
    vector<double> filter(vector<double>x);


    /*!
     * Description: 
     *  Compute lowpass component values for an analog filter. 
     *  Based on code from:
     *  https://exstrom.com/journal/sigproc/alpbw.c
     *  Schematics and write up here: 
     *  https://exstrom.com/journal/sigproc/asigproc.html
     *
     * Arguments:
     *   Frequency - 3dB point in Hz
     *   Termination - true equal source and load resistances
     *
     * returns:
     *    
     */
    void ALowPass(double Frequency, bool Termination);


    /*!
     * Description: 
     *   print out the entire data about this class. 
     *
     * Arguments:
     *   
     *
     * returns:
     *    
     */
    friend ostream& operator<<(ostream& output, const Butterworth &n); 


private:
    /*!
     * Description: 
     *   
     *
     * Arguments:
     *   
     *
     * Returns:
     *
     * Errors:
     *
     */
    vector<double> TrinomialMultiply(vector<double> b, vector<double> c);

    /*!
     * Description: 
     *   
     *
     * Arguments:
     *   
     *
     * Returns:
     *
     * Errors:
     *
     */
    vector<double> ComputeHP(void);
    /*!
     * Description: 
     *   
     *
     * Arguments:
     *   
     *
     * Returns:
     *
     * Errors:
     *
     */
    vector<double> ComputeLP(void);

    /*!
     * Description: 
     *   
     *
     * Arguments:
     *   
     *
     * Returns:
     *
     * Errors:
     *
     */

    void ComputeNumCoeffs(void);

    /*!
     * Description: 
     *   
     *
     * Arguments:
     *   
     *
     * Returns:
     *
     * Errors:
     *
     */
    void ComputeDenCoeffs(void);



    /*!
     * Description: 
     *   ClearAll -- Clear the vectors for a recalculation
     *
     * Arguments:
     *   NONE
     *
     * Returns: NONE
     *
     * Errors: NONE
     *
     */
    void ClearAll(void); 


    uint16_t       fFilterOrder;
    double         fLowerCutoff;
    double         fUpperCutoff;

    vector<double> fDenomCoeffs;
    vector<double> fNumCoeffs;

    FilterType     fType; 
};
#endif
