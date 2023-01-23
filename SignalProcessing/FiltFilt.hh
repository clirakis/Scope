/**
 ******************************************************************
 *
 * Module Name : FiltFilt.h
 *
 * Author/Date : C.B. Lirakis / 22-Jan-23
 *
 * Description : Implement IIR FiltFilt
 *
 * Restrictions/Limitations :
 *
 * Change Descriptions :
 *
 * Classification : Unclassified
 *
 * References :
 *   Loosely based on:
 *   https://github.com/KBaur/FiltFilt/tree/main/src/include
 *
 *******************************************************************
 */
#ifndef __FILTFILT_hh_
#define __FILTFILT_hh_
#  include "Filt.hh"

/// FiltFilt documentation here. 
class FiltFilt : public Filt
{
public:
    /// Default Constructor
    FiltFilt(const Filt &p);

    /*!
     * Description: 
     *   Apply the filter with the coefficients in the class. 
     * Trying to do the same but get a forward/backward filter. 
     * that behaves like filtfilt.
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

#if 0
    /**
     * @brief Constructor
     * @details Filtfilt function from 'Likhterov & Kopeika, 2003. "Hardware-efficient technique for minimizing startup transients in Direct Form II digital filters"'
     * @param p_inputVector 
     * @return std::vector<T>&& 
     */
    std::vector<T>&& ZeroPhaseFiltering(const std::vector<T>& p_inputSignal);
    /**
     * @brief FilterDirectFormII
     * 
     * @param signal1 
     * @return std::vector<double> 
     */
    std::vector<T>&& FilterDirectFormII(const std::vector<T> &p_inputSignal);

#endif
protected:
#if 0
    /**
     * @brief PadSignal
     * @details Pad a signal with a window size p_size and a value p_value
     * @param p_signal 
     * @param p_size 
     * @param p_value 
     */
    void PadSignal(std::vector<T> &p_signal,unsigned int p_size, T p_value = 0.0);
    /**
     * @brief ResizeSignal
     * @details Shrink a signal by a window size p_shrink
     * @param p_output 
     * @param p_shrink 
     * @return std::vector<T> 
     */
    std::vector<T> ResizeSignal(std::vector<T>& p_output,unsigned int p_shrink);
    /**
     * @brief CumulativeSum
     * 
     * @param p_signal 
     * @return std::vector<T> 
     */
    std::vector<T> CumulativeSum(std::vector<T>& p_signal);
    /**
     * @brief InitializeSi
     * @details Initialize Si a factor needed by the filtfilt function
     * @return std::vector<T> 
     */
    std::vector<T> InitializeSi();
    /**
     * @brief FlipLR
     * 
     * @param p_signal 
     */
    void FlipLR(std::vector<T>& p_signal);

#endif
private:

};
#endif
