//--------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
//    Copyright (c) 2002-2008 Cecilio Salmeron
//
//    This program is free software; you can redistribute it and/or modify it under the
//    terms of the GNU General Public License as published by the Free Software Foundation;
//    either version 2 of the License, or (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful, but WITHOUT ANY
//    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
//    PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along with this
//    program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street,
//    Fifth Floor, Boston, MA  02110-1301, USA.
//
//    For any comment, suggestion or feature request, please contact the manager of
//    the project at cecilios@users.sourceforge.net
//
//-------------------------------------------------------------------------------------

#ifndef __LM_TIMEPOSTABLE_H__        //to avoid nested includes
#define __LM_TIMEPOSTABLE_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "TimeposTable.cpp"
#endif

#include "../score/defs.h"
class lmStaffObj;

#include <wx/dynarray.h>

//entry types
enum eTimeposEntryType
{
    eAlfa = 1,              //start of thread
    ePrologue,              //prologue of staff
    eStaffobj,              //lmStaffObj inside bar
    eOmega,                 //end of thread
};


// Definition of an entry of the timepos table
class lmTimeposEntry
{
public:
    // constructor and destructor
    lmTimeposEntry(eTimeposEntryType nType, int nThread, lmStaffObj* pSO, float rTimePos) {
        m_nThread = nThread;
        m_nType = nType;
        m_pSO = pSO;
        m_rTimePos = rTimePos;
        m_uSize = 0;
        m_xLeft = 0;
        m_xInitialLeft = 0;
        m_uxAnchor = 0;
        m_xRight = 0;
        m_xFinal = 0;
    }
    ~lmTimeposEntry() {}

    //member variables (one entry of the table)
    //----------------------------------------------------------------------------
    int             m_nThread;      //num. of the thread to which this element belongs
    eTimeposEntryType m_nType;      //type of entry
    lmStaffObj*     m_pSO;          //ptr to the lmStaffObj
    float           m_rTimePos;     //timepos for this pSO or -1 if not anchored in time
    lmLUnits        m_uSize;        //size of this lmStaffObj
    lmLUnits        m_xLeft;        //current position of the left border of the object
    lmLUnits        m_xInitialLeft; //initial position of the left border of the object
    lmLUnits        m_uxAnchor;      //position of the anchor line
    lmLUnits        m_xRight;       //position of the right border
    lmLUnits        m_xFinal;       //next position (right border position + trailing space)

};

// this defines the type ArrayOfTimepos as an array of lmTimeposEntry pointers
WX_DEFINE_ARRAY(lmTimeposEntry*, ArrayOfTimepos);


// Definition of an entry of the auxiliary table for ordering by time
class lmTimeauxEntry
{
public:
    // constructor and destructor
    lmTimeauxEntry(int item, int thread, float timePos) {
        nThread = thread;
        nItem = item;
        rTimePos = timePos;
        uShift = 0;
    }
    ~lmTimeauxEntry() {}

    //member variables (one entry of the table)
    int         nThread;        //num. of the thread to which this element belongs
    int         nItem;          //index to TimePos table
    float       rTimePos;       //timepos for this entry
    lmLUnits    uShift;         //x position shift to apply to this entry

};

// this defines the type ArrayOfTimeaux as an array of lmTimeauxEntry objects
WX_DEFINE_ARRAY(lmTimeauxEntry*, ArrayOfTimeaux);

//Finally let's define the class that implements the tables and the algoritms
class lmTimeposTable
{
public:
    lmTimeposTable();
    ~lmTimeposTable();

    void    NewThread();
    void    CloseThread(lmLUnits uCurX);
    void    AddEntry(float rTimePos, lmStaffObj* pSO);
    void    AddBarline(lmStaffObj* pSO);
    void    CleanTable();

    // methods for accesing/updating entries
    void        SetCurXLeft(lmLUnits uValue);
    lmLUnits    GetCurXLeft();
    void        SetCurXFinal(lmLUnits uValue);
    void        SetCurXAnchor(lmLUnits uValue);
    void        SetxIni(float rTimePos, lmLUnits uxPos);
    lmLUnits    GetXFinal(float rTimePos);
    void        SetXFinal(float rTimePos, lmLUnits uxRight);
    void        UpdateEntry(float rTimePos, lmLUnits uxLeft, lmLUnits uxRight);
    lmLUnits    LastFinalX();

    //methods to compute results
    lmLUnits    GetStartOfBarPosition();
    lmLUnits    GetGrossBarSize();
    lmLUnits    ArrangeStaffobjsByTime(bool fTrace = false);
    lmLUnits    RedistributeSpace(lmLUnits uNewBarSize, lmLUnits uNewStart);

    //methods for debugging
    wxString DumpTimeposTable();
    wxString DumpTimeauxTable();


private:
    void    NewEntry(eTimeposEntryType nType, float rTimePos, lmStaffObj* pSO);
    int     FindItem(float rTimePos);
    void    AddTimeAuxEntry(int nItem);


private:
    ArrayOfTimepos  m_aTimePos;     //The main table
    ArrayOfTimeaux  m_aTimeAux;     //auxiliary table for ordering by time
    int             m_nCurThread;   //num of current thread

};

#endif    // __LM_TIMEPOSTABLE_H__

