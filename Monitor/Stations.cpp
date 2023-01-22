/********************************************************************
 *
 * Module Name : Stations.cpp
 *
 * Author/Date : C.B. Lirakis / 23-May-21
 *
 * Description : Generic Stations
 *
 * Restrictions/Limitations :
 *
 * Change Descriptions :
 *
 * Classification : Unclassified
 *
 * References :
 *
 ********************************************************************/
// System includes.
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <cmath>
using namespace std;

// Local Includes.
#include "debug.h"
#include "Stations.hh"

/**
 ******************************************************************
 *
 * Function Name : StationValue constructor
 *
 * Description : Input a line containing station data in the format of
 * text, frequency(hz), day power, night power, lat, lon, distance
 * and fill the fields. # is a comment
 *
 * Inputs : line to parse
 *
 * Returns : NONE
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on: 21-Jan-23
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
StationValue::StationValue(string &line)
{
    SET_DEBUG_STACK;
    const string   delimiter = ",";
    string   token;
    size_t   pos = 0;
    uint8_t  index;

    fSuccess   = false;   // unsuccessful parse
    fLabel     = "none";  // Station text information
    fFrequency = 0.0;     // Station frequency in Hz
    memset(fPower,0, 2*sizeof(double));  // Day/Night power in watts
    fLatitude  = 0.0;     // Antenna Latitude, decimal degrees
    fLongitude = 0.0;     // Antenna Longitude, decimal degrees
    fDistance  = 0.0;     // Range from provided location to antenna (m)

    index = 0;
    // Comments start with # sign. Strip out everything following #
    pos = line.find("#");
    if (pos != string::npos) 
    {
	line.erase(pos);
    }
    // Did the line turn to zero size??
    if (line.size()> 0)
    {
	/*
	 * The original code of just looping while a delimeter
	 * is found is flawed. There is always left at 
	 * the end of the line. 
	 */
	while(line.size() > 0) 
	{
	    pos = line.find(delimiter);
	    if (pos != string::npos) 
	    {
		token = line.substr(0, pos);
	    }
	    else
	    {
		// Get the bit after the last delimiter
		pos   = line.size() - token.length(); 
		token = line;
	    }
	    //cout << "Index: " << (int) index << " Token: " << token << endl;

	    switch(index)
	    {
	    case 0:
		// String label
		fLabel = token;
		break;
	    case 1:
		fFrequency = stod(token);
		break;
	    case 2:
		fPower[0]  = stod(token);
		break;
	    case 3:
		fPower[1]  = stod(token);
		break;
	    case 4:
		fLatitude  = stod(token);
		break;
	    case 5:
		fLongitude = stod(token);
		break;
	    case 6:
		fDistance  = stod(token);
		break;
	    }

	    line.erase(0, pos + delimiter.length());
	    index++;
	}
	fSuccess = true;
    }
    else
    {
	fSuccess = false;
    }
    SET_DEBUG_STACK;
}
#if 0
/**
 ******************************************************************
 *
 * Function Name : StationValue destructor
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
StationValue::~StationValue (void)
{
    SET_DEBUG_STACK;
    SET_DEBUG_STACK;
}


/**
 ******************************************************************
 *
 * Function Name : StationValue function
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
void StationValue::ParseLine(const char *Name)
{
    SET_DEBUG_STACK;
    SET_DEBUG_STACK;
}
#endif

/**
 ******************************************************************
 *
 * Function Name : Description destructor
 *
 * Description : make a formatted output line of StationValue data
 *
 * Inputs : output stream to push the data into. 
 *          n - StationValue data to format
 *
 * Returns : Full filled ostream
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on: 21-Jan-23
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
ostream& operator<<(ostream& output, const StationValue &n)
{

    output << n.fLabel     << ", "
	   << n.fFrequency << ", "
	   << n.fPower[0]  << ", " << n.fPower[1]  << ", "
	   << n.fLatitude  << ", " << n.fLongitude << ", "
	   << n.fDistance << ", " << n.fAzimuth; 

    return output;
}

/**
 ******************************************************************
 *
 * Function Name : Stations constructor
 *
 * Description : Store the antenna location, read the station file
 * 
 *
 * Inputs : RxLat, RxLon - Latitude and Longitude of recieve antenna
 *          in decimal degrees
 *
 * Returns : NONE
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on: 21-Jan-23
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
Stations::Stations(double RxLat, double RxLon)
{
    SET_DEBUG_STACK;
    fRxLatitude  = RxLat;
    fRxLongitude = RxLon;
    ReadFile("Stations.txt");
    CalculateDistance();
    SET_DEBUG_STACK;
}

/**
 ******************************************************************
 *
 * Function Name : Stations destructor
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
Stations::~Stations (void)
{
    SET_DEBUG_STACK;
    StationValue *p;
    for(list<StationValue*>::iterator it=fStations.begin(); it!=fStations.end(); it++)
    {
	p = (StationValue *) *it;
	delete p;
    }
    SET_DEBUG_STACK;
}

/**
 ******************************************************************
 *
 * Function Name : CalculateDistance
 *
 * Description : calculate the range from the source to destination
 * antennas. Loop over all values. 
 *
 * Inputs : StationValue to calculte. 
 *
 * Returns : true on success
 *
 * Error Conditions : Could not initialize the projection
 * 
 * Unit Tested on: 22-Jan-23
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
bool Stations::CalculateDistance(void)
{
    SET_DEBUG_STACK;
    PJ_CONTEXT   *C;
    PJ           *P;
    PJ_COORD     source, destination;
    PJ_COORD     result;
    double       range = 0.0;           // return value
    double       azimuth = 0.0;
    StationValue *p;
    char         ZoneString[64];

    // Setup projection to be used. 
    C = proj_context_create();

    // Compute zone
    double Zone = fRxLongitude + 180.0;
    Zone = ceil(Zone/6.0);

    memset(ZoneString, 0, sizeof(ZoneString));
    sprintf( ZoneString, "+proj=utm +zone=%d +datum=WGS84", (int)Zone);
    //cout << "ZONE: " << ZoneString << endl;

    // "+proj=utm +zone=32 +datum=WGS84", /* or EPSG:32632 */
    // Python code
    // 32618 - UTM zone 18
    // crs_in    = 32600 + self.__Zone
    // crs_32618 = CRS.from_epsg(crs_in)

    P = proj_create_crs_to_crs (C, "EPSG:4326", ZoneString, NULL);
    if (0 == P) 
    {
	cout << "Failed to create transformation object." << endl;
	return false;
    }

    /*
     * Set the destination coordinates. 
     * https://proj.org/development/reference/functions.html#c.proj_coord
     */
    destination.lp.lam = proj_torad(fRxLongitude);
    destination.lp.phi = proj_torad(fRxLatitude);

    // Loop over all stations and calculate the distances. 
    list<StationValue*>::const_iterator it;
    it=fStations.begin(); 
    do
    {
	p = (StationValue *) *it;
	/* set the source coordinates. */
	source.lp.lam = proj_torad(p->Longitude());
	source.lp.phi = proj_torad(p->Latitude());
	/*
	 * PJ_COORD where the first value is the distance between a and b 
	 * in meters, the second value is the forward azimuth and the 
	 * third value is the reverse azimuth. The fourth coordinate value 
	 * is unused.
	 * https://proj.org/development/reference/functions.html#c.proj_coord
	 */
	//result  = proj_geod( P, source, destination);
	result  = proj_geod( P, destination, source);
	range   = result.geod.s;
	azimuth = result.geod.a1;
	if (azimuth<0.0)
	{
	    azimuth += 360.0;
	}
#if 0
	cout << "Results: " << range  << " Expected: "
	     << p->Distance() << " Delta: " 
	     << p->Distance() - range
	     << " Azimuth: " << azimuth
	     << endl;
#endif
	p->Distance(range);
	p->Azimuth(azimuth);

	it++;
    } while(it!= fStations.end());

    SET_DEBUG_STACK;
    return true;
}



/**
 ******************************************************************
 *
 * Function Name : Stations function
 *
 * Description : Read the file of known stations and populate 
 *               memory with a list. 
 *
 * Inputs : Name - file name to open and parse. 
 *
 * Returns : True on success
 *
 * Error Conditions : Error on open
 * 
 * Unit Tested on: 21-Jan-23
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
bool Stations::ReadFile(const char *Name)
{
    SET_DEBUG_STACK;
    ifstream     myin(Name);   // input stream
    string       line;         // line to read data into
    bool         rv = false;   // return value, assume failure
    StationValue *sv;          // pointer to StationValue. 

    if (!myin.fail())
    {
	while(!myin.eof())
	{
	    // Read a full line
	    getline( myin, line);
	    // Parse the line and return a pointer to the data. 
	    sv = new StationValue(line);
	    if (sv->Success())
	    {
		// Store it in the list. 
		fStations.push_back(sv);
	    }
	    else
	    {
		delete sv;
	    }
	}
	rv = true;
    }
    SET_DEBUG_STACK;
    return rv;
}

/**
 ******************************************************************
 *
 * Function Name : Description destructor
 *
 * Description : Format output of all stations. 
 *
 * Inputs : output stream to populate
 *          n Stations class
 *
 * Returns : Filled output stream
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on: 21-Jan-23
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
ostream& operator<<(ostream& output, const Stations &n)
{
    SET_DEBUG_STACK;
    StationValue *p;

    output << "============================================" << endl
	   << "Stations: " << endl
	   << "                     Power       Source Antenna" << endl
	   << "Text, Frequency(Hz), Day, Night, Latitude, Longitude, Distance(m), Azimuth(deg)"
	   << endl;
    list<StationValue*>::const_iterator it;
    for(it=n.fStations.begin(); it!=n.fStations.end(); it++)
    {
	p = (StationValue *) *it;
	output << *p << endl;
    }
    output << "============================================" << endl;
    return output;
}

/**
 ******************************************************************
 *
 * Function Name : FindByFrequency
 *
 * Description : Find all the information about a radio station
 *               based on its frequency. 
 *
 * Inputs : Approximate frequency to search for. 
 *
 * Returns : pointer to StationValue information that matches frequency
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on: 22-Jan-23
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
StationValue *Stations::FindByFrequency(double Freq, double range)
{
    SET_DEBUG_STACK;
    StationValue *p;
    list<StationValue*>::const_iterator it;
    bool found = false;

    it=fStations.begin(); 
    do
    {
	p = (StationValue *) *it;
	found = ((p->Frequency() >= (Freq-range)) && 
		 (p->Frequency() <= (Freq+range)));
	it++;
    } while((it!= fStations.end()) && !found);

    SET_DEBUG_STACK;
    return p;
}

/**
 ******************************************************************
 *
 * Function Name : FindByText
 *
 * Description : Find all the information about a radio station
 *               based on the provided label
 *
 * Inputs : text to search for
 *
 * Returns : pointer to StationValue information that matches frequency
 *
 * Error Conditions : NONE
 * 
 * Unit Tested on: 22-Jan-23
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
StationValue *Stations::FindByText(const char *text)
{
    SET_DEBUG_STACK;
    StationValue *p;
    list<StationValue*>::const_iterator it;
    bool found = false;

    it=fStations.begin(); 
    do
    {
	p = (StationValue *) *it;
	found = (p->Label().find(text) != string::npos);
	it++;
    } while((it!= fStations.end()) && !found);
    SET_DEBUG_STACK;
    return p;
}
#if 0
/**
 ******************************************************************
 *
 * Function Name : StationValue function
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
void Stations::SetupProjection
{
    SET_DEBUG_STACK;
    SET_DEBUG_STACK;
}
#endif

#ifdef MAIN
int main(int argc, char **argv)
{
    // My location in NY. 
    const static double Lat0 =  41.3082;
    const static double Lon0 = -73.893;

    Stations sta(Lat0, Lon0);
    cout << sta;

    StationValue *p = sta.FindByFrequency(880.1e3, 1.0e3);
    cout << "Found: " << *p << endl;

    p = sta.FindByText("WFAN");
    cout << "Find by text: " << *p << endl;
    exit(0);
}
#endif
