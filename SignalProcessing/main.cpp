/**
 ******************************************************************
 *
 * Module Name : main for SP
 *
 * Author/Date : C.B. Lirakis / 15-Jan-23
 *
 * Description : Testing of signal processing tools
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
// System includes.
#include <iostream>
using namespace std;
#include <unistd.h>
#include <string> 
#include <fstream>


/// Local Includes.
#include "debug.h"
#include "tools.h"
#include "CLogger.hh"
#include "UserSignals.hh"
#include "Version.hh"
#include "Butterworth.hh"

/** Control the verbosity of the program output via the bits shown. */
static unsigned int VerboseLevel = 0;

/** Pointer to the logger structure. */
static CLogger   *logger;

/**
 ******************************************************************
 *
 * Function Name : Help
 *
 * Description : provides user with help if needed.
 *
 * Inputs : none
 *
 * Returns : none
 *
 * Error Conditions : none
 *
 *******************************************************************
 */
static void Help(void)
{
    SET_DEBUG_STACK;
    cout << "********************************************" << endl;
    cout << "* Test file for text Logging.              *" << endl;
    cout << "* Built on "<< __DATE__ << " " << __TIME__ << "*" << endl;
    cout << "* Available options are :                  *" << endl;
    cout << "*                                          *" << endl;
    cout << "********************************************" << endl;
}
/**
 ******************************************************************
 *
 * Function Name :  ProcessCommandLineArgs
 *
 * Description : Loop over all command line arguments
 *               and parse them into useful data.
 *
 * Inputs : command line arguments. 
 *
 * Returns : none
 *
 * Error Conditions : none
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
static void
ProcessCommandLineArgs(int argc, char **argv)
{
    int option;
    SET_DEBUG_STACK;
    do
    {
        option = getopt( argc, argv, "f:hHnv");
        switch(option)
        {
        case 'f':
            break;
        case 'h':
        case 'H':
            Help();
        Terminate(0);
        break;
	case 'v':
	    VerboseLevel = atoi(optarg);
            break;
        }
    } while(option != -1);
}
static void Do(void)
{
    SET_DEBUG_STACK;
    //     Filter Order
    int    FiltOrd;
    double FrequencyBands[2];

    /*
     * https://github.com/nxsEdson/Butterworth-Filter/blob/master/butterworth.cpp
     */
    //double       fps = 100; // Hz

    /*
     * Frequency bands is a vector of values - 
     *   Lower Frequency Band and 
     *   Higher Frequency Band
     *
     * First value is lower cutoff and second value is higher cutoff
     * these values are as a ratio of f/fs, where fs is sampling rate, 
     * and f is cutoff frequency and therefore should lie in the range [0 1]
     * 
     * I can't really replicate the results in scipy.signal. not sure why.
     */
    //double FrequencyBands[2] = { 1.5/fps*2, 2.5/fps*2 };
#if 0
    FiltOrd = 4;
    FrequencyBands[0] = 0.3;
    FrequencyBands[1] = 0.42;
#endif
#if 1
    /*
     * initial test in example found:
     * https://stackoverflow.com/questions/10373184/bandpass-butterworth-filter-implementation-in-c
     */
    FiltOrd = 5;
    FrequencyBands[0] = 0.25;
    FrequencyBands[1] = 0.375;

    /*
     * Results they got: 
     * B= 1,0,-5,0,10,0,-10,0,5,0,-1 
     * A= 1.000000000000000, -4.945988709743181, 13.556489496973796, -24.700711850327743, 32.994881546824828, -33.180726698160655, 25.546126213403539, -14.802008410165968, 6.285430089797051, -1.772929809750849, 0.277753012228403
     *
     * Matlab results
     * B = 0.0002, 0, -0.0008, 0, 0.0016, 0, -0.0016, 0, 0.0008, 0, -0.0002
     * A = 1.0000, -4.9460, 13.5565, -24.7007, 32.9948, -33.1806, 25.5461, -14.8020, 6.2854, -1.7729, 0.2778
     * 
     * My results: 
     * Butterworth: ============================================
     *    Filter Order: 5
     *    Lower Cutoff: 0.250000
     *    Upper Cutoff: 0.375000
     *
     *(a) Denominator (11): 
     *     1.000000, -4.945983, 13.556465, -24.700656, 32.994800, -33.180641, 
     *     25.546063, -14.801975, 6.285418, -1.772928, 0.277753, 
     *     
     *(b) Numerator (11): 
     *     0.000164, 0.000000, -0.000821, 0.000000, 0.001641, 0.000000, 
     *     -0.001641, 0.000000, 0.000821, 0.000000, -0.000164, 
     *     
     *============================================
     * CHECK!!
     */
#endif


    /*
     * Pass Numerator Coefficients and Denominator Coefficients arrays 
     * into function, will return the same.
     */
    vector<double> x;
    vector<double> y;
    vector<double> z;

    // read the input data from the file. 
    string   line;
    size_t   ptr;
    ifstream ifile("WFAN.txt");
    do
    {
	ifile >> line;
	ptr = line.find(',');
	x.push_back(stod(line.substr(0,ptr)));
	y.push_back(stod(line.substr(ptr+1)));
	
    } while (!ifile.eof());

    Butterworth bw( FiltOrd, FrequencyBands[0], FrequencyBands[1]);

    cout << "Butterworth: " << bw << endl;

    z = bw.filter(y);

    ofstream ofile("output.txt");
    for (uint32_t i = 0; i<z.size(); i++)
    {
	ofile << z[i] << endl;
    }
    ofile.close();
}
/**
 ******************************************************************
 *
 * Function Name : Initialize
 *
 * Description : Initialze the process
 *               - Setup traceback utility
 *               - Connect all signals to route through the terminate 
 *                 method
 *               - Perform any user initialization
 *
 * Inputs : none
 *
 * Returns : true on success. 
 *
 * Error Conditions : depends mostly on user code
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
static bool Initialize(void)
{
    SET_DEBUG_STACK;
    char   msg[32];
    double version;

    SetSignals();
    // User initialization goes here. 
    sprintf(msg, "%d.%d",MAJOR_VERSION, MINOR_VERSION);
    version = atof( msg);
    logger = new CLogger("SP.log", "SP", version);
    logger->SetVerbose(VerboseLevel);

    return true;
}

/**
 ******************************************************************
 *
 * Function Name : main
 *
 * Description : It all starts here:
 *               - Process any command line arguments
 *               - Do any necessary initialization as a result of that
 *               - Do the operations
 *               - Terminate and cleanup
 *
 * Inputs : command line arguments
 *
 * Returns : exit code
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
int main(int argc, char **argv)
{
    ProcessCommandLineArgs(argc, argv);
    if (Initialize())
    {
	Do();
    }
    Terminate(0);
}
