/**
 ******************************************************************
 *
 * Module Name : Stations.hh
 *
 * Author/Date : C.B. Lirakis / 21-Jan-22
 *
 * Description : Methods to read the plain text file Stations.txt
 * with radio data in it. 
 *
 * Restrictions/Limitations : NONE
 *
 * Change Descriptions :
 *
 * Classification : Unclassified
 *
 * References :
 *
 *******************************************************************
 */
#ifndef __STATIONS_hh_
#define __STATIONS_hh_
#  include <string>
#  include <list> 
#  include <proj.h>   // Include file for proj8

class StationValue {
public:
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
    StationValue(string &line);

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
    inline const string &Label(void) {return fLabel;};
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
    inline double Frequency(void) {return fFrequency;};
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
    inline double Power(bool day) {if(day)return fPower[0];return fPower[1];};

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
    inline double Distance(void) {return fDistance;};

    inline void Distance (double x) {fDistance = x;};

    inline double Azimuth(void) {return fAzimuth;};

    inline void   Azimuth(double x) {fAzimuth = x;};

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
    inline bool Success(void) {return fSuccess;};


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
    inline double Latitude(void) {return fLatitude;};

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
    inline double Longitude(void) {return fLongitude;};



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
    friend ostream& operator<<(ostream& output, const StationValue &n);
 

private:
    string fLabel;     // Station text information
    double fFrequency; // Station frequency in Hz
    double fPower[2];  // Day/Night power in watts
    double fLatitude;  // Antenna Latitude, decimal degrees
    double fLongitude; // Antenna Longitude, decimal degrees
    double fDistance;  // Range from provided location to antenna
    double fAzimuth;   // Azimuth to source. 
    bool   fSuccess;   // Successful parse. 
};

/// Stations documentation here. 
class Stations {
public:
    /// Default Constructor
    Stations(double RxLat, double RxLon);
    /// Default destructor
    ~Stations(void);
    /// Stations function



    /*!
     * Description: 
     *   Find all the information about a radio station
     *   based on its frequency.
     *
     * Arguments:
     *   Freq - Frequency in Hz to serach for
     *   range - window to provide match in Hz
     *
     * Returns:
     *   pointer to StationValue information that matches frequency
     *
     * Errors: NONE
     *
     */
    StationValue *FindByFrequency(double Freq, double range);

    /*!
     * Description: 
     *   Find all the information about a radio station
     *   based on its frequency.
     *
     * Arguments:
     *   text - text that is used for the label, such as WCBS
     *
     * Returns:
     *   pointer to StationValue information that matches label
     *
     * Errors: NONE
     *
     */
    StationValue *FindByText(const char* text);

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
    friend ostream& operator<<(ostream& output, const Stations &n);
 
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
    bool ReadFile(const char *Filename);

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
    bool CalculateDistance(void);

#if 0
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
    void SetupProjection(void);
#endif
    double fRxLatitude;
    double fRxLongitude;

    list <StationValue *> fStations;

};
#endif
