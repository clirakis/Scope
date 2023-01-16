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
    /// Default Constructor
    Butterworth(uint16_t FilterOrder, double Lcutoff, double Ucutoff);
    /// Default destructor
    ~Butterworth();
    /// Butterworth function
    vector<double> filter(vector<double>x);
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


    uint16_t       fFilterOrder;
    double         fLowerCutoff;
    double         fUpperCutoff;

    vector<double> fDenomCoeffs;
    vector<double> fNumCoeffs;
};
#endif
