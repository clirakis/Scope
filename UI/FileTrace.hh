/**
 ******************************************************************
 *
 * Module Name : FileTrace.hh
 *
 * Author/Date : C.B. Lirakis / 01-Jan-23
 *
 * Description : Save the waveform data along with the 
 * strings defining the configuration to a root file. 
 *
 * Restrictions/Limitations :
 *
 * Change Descriptions :
 *
 * Classification : Unclassified
 *
 * References :
 *   DSA602 Programming reference manual
 *   cern root documentation. 
 *
 *******************************************************************
 */
#ifndef __FILETRACE_hh_
#define __FILETRACE_hh_

/// FileTrace documentation here. 
class FileTrace {
public:
    /// Default Constructor
    FileTrace(const char *Filename);

    /// Default destructor
    ~FileTrace();


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
    bool LoadRoot(const char *Name);

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
    bool SaveRoot(const char *Name);
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
    bool LoadCSV(const char *Name);

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
    bool SaveCSV(const char *Name);

private:

};
#endif
