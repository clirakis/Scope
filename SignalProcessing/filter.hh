/**
 ******************************************************************
 *
 * Module Name : filter.h
 *
 * Author/Date : C.B. Lirakis / 22-Jan-23
 *
 * Description : Implement IIR filter
 *
 * Restrictions/Limitations :
 *
 * Change Descriptions :
 *
 * Classification : Unclassified
 *
 * References :
 *
 *******************************************************************
 */
#ifndef __FILTER_hh_
#define __FILTER_hh_
#  include "Filt.hh"

/// Filter documentation here. 
class Filter : public Filt
{
public:
    /// Default Constructor
    Filter(const Filt &p);

    /*!
     * Description: 
     *   Apply the filter with the coefficients in the class. 
     *  This filter behaves a lot like sipy.signal.lfilt
     *
     * Arguments:
     *   
     *
     * Returns:
     *
     * Errors:
     *
     */
    vector<double> Do(vector<double>x);

private:

};
#endif
