/**
 ******************************************************************
 *
 * Module Name : cell.hh
 *
 * Author/Date : C.B. Lirakis / 04-Dec-07
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
#ifndef __CELL_HH__
# define __CELL_HH__

class TASImage;
class Cell 
{
private:

    void   CreateCells(UInt_t NCol, UInt_t NRow);
    void   PutDataPutData( UInt_t X, UInt_t Y, Bool_t OnK);
    Bool_t GetData( UInt_t X, UInt_t Y);
    Int_t  SourceRule( UInt_t X, UInt_t Y);

    TASImage *cells;

public:
    Cell( UInt_t NCol=25, UInt_t NRow=25);
    ~Cell();
    void UpdateCells();
    void DumpRow(UInt_t Y);

};
#endif
