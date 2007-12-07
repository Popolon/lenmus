//--------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
//    Copyright (c) 2002-2007 Cecilio Salmeron
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

#ifndef __PITCH_H__        //to avoid nested includes
#define __PITCH_H__

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "Pitch.cpp"
#endif

//pitch
typedef int lmDPitch;       // Diatonic pitch
typedef int lmMPitch;       // Midi pitch
typedef int lmFPitch;       // Base-40 pitch: Absolute pitch, interval-invariant
typedef int lmFIntval;      // Intervals, in FPitch mode.




//------------------------------------------------------------------------------------------
// Note steps
// 'step' refers to the diatonic note name in the octave. It is
//  encoded as a number in the range 0..6:
//      0=C, 1=D, 2=E, 3=F, 4=G, 5=A, 6=B

#define lmSTEP_C    0
#define lmSTEP_D    1
#define lmSTEP_E    2
#define lmSTEP_F    3
#define lmSTEP_G    4
#define lmSTEP_A    5
#define lmSTEP_B    6

//-----------------------------------------------------------------------------------------
// Octaves
// The octave is represented by a number in the range 0..9. 
// Same meaning as in MIDI but it is not possible to represent the lowest MIDI octave (-1)

#define lmOCTAVE_0      0
#define lmOCTAVE_1      1
#define lmOCTAVE_2      2
#define lmOCTAVE_3      3
#define lmOCTAVE_4      4
#define lmOCTAVE_5      5
#define lmOCTAVE_6      6
#define lmOCTAVE_7      7
#define lmOCTAVE_8      8
#define lmOCTAVE_9      9

//----------------------------------------------------------------------------------------
// Accidentals
// No microtonal accidentals. Only traditional ones.
#define lmFLAT_FLAT         -2  
#define lmFLAT              -1  
#define lmNO_ACCIDENTAL     0   
#define lmSHARP             1   
#define lmSHARP_SHARP       2   

#define lmNO_NOTE           -1  // DPitch = -1


//====================================================================================
// lmAPitch
//  Absoulte pitch is defined by the diatonic pitch (1..68) plus the number of 
//  accidentals (-2..+2)
//====================================================================================

class lmAPitch
{
public:

    lmAPitch() : 
        m_nDPitch(-1), m_nAcc(0) {}
    lmAPitch(lmDPitch nDPitch, int nAcc) :
        m_nDPitch(nDPitch), m_nAcc(nAcc) {}
    lmAPitch(int nStep, int nOctave, int nAcc) {
        m_nAcc = nAcc;
        m_nDPitch = nOctave * 7 + nStep + 1;
    }
    lmAPitch(const wxString& sNote);
    
    ~lmAPitch() {}
    
    inline lmDPitch ToDPitch() { return m_nDPitch; }
    inline int Accidentals() { return m_nAcc; }
    
    // comparison operators
    bool operator ==(lmAPitch& ap) { return m_nDPitch == ap.ToDPitch() && m_nAcc == ap.Accidentals(); }
    bool operator !=(lmAPitch& ap) { return m_nDPitch != ap.ToDPitch() || m_nAcc != ap.Accidentals(); }
    bool operator < (lmAPitch& ap) { return m_nDPitch < ap.ToDPitch() ||
        (m_nDPitch == ap.ToDPitch() && m_nAcc < ap.Accidentals()); }
    bool operator > (lmAPitch& ap) { return m_nDPitch > ap.ToDPitch() ||
        (m_nDPitch == ap.ToDPitch() && m_nAcc > ap.Accidentals()); }
    bool operator <= (lmAPitch& ap) { return m_nDPitch < ap.ToDPitch() ||
        (m_nDPitch == ap.ToDPitch() && m_nAcc <= ap.Accidentals()); }
    bool operator >= (lmAPitch& ap) { return m_nDPitch > ap.ToDPitch() ||
        (m_nDPitch == ap.ToDPitch() && m_nAcc >= ap.Accidentals()); }

    //operations
    inline lmDPitch IncrStep() { return ++m_nDPitch; } 
    inline lmDPitch DecrStep() { return --m_nDPitch; } 

    void Set(int nStep, int nOctave, int nAcc) {
        m_nAcc = nAcc;
        m_nDPitch = nOctave * 7 + nStep + 1;
    }
    void Set(lmAPitch& ap) {
        m_nDPitch = ap.ToDPitch();
        m_nAcc = ap.Accidentals();
    }
    void Set(lmDPitch nDPitch, int nAcc) {
        m_nDPitch = nDPitch;
        m_nAcc = nAcc;
    }

    void SetAccidentals(int nAcc) { m_nAcc = nAcc; }
    void SetDPitch(lmDPitch dnPitch) { m_nDPitch = dnPitch; }
    
    //conversions
    inline int Step() const { return ((int)m_nDPitch - 1) % 7; }
    inline int Octave()const { return ((int)m_nDPitch - 1) / 7; }
    wxString LDPName() const;
    const lmMPitch GetMPitch() const;




private:
    lmDPitch    m_nDPitch;
    int         m_nAcc;
    
};


//-------------------------------------------------------------------------
// lmDPitch
// Diatonic pitch: Represents the note in a diatonic scale.
// Only Step and Octave information.
// - Accidentals not represented. Incomplete information.
// - Usable for sweeps along the diatonic notes range
//-------------------------------------------------------------------------


#define lmC4_DPITCH   29
#define lmNO_DPITCH	  -1


// constructors
#define DPitch(nStep, nOctave)   (nOctave * 7 + nStep + 1)
//extern lmDPitch DPitch(wxString& sLDPNote);

// conversion
extern lmMPitch DPitch_ToMPitch(lmDPitch dp);
//extern lmAPitch DPitch_ToAPitch(lmDPitch dp);
extern wxString DPitch_GetEnglishNoteName(lmDPitch dp);
extern wxString DPitch_ToLDPName(lmDPitch dp);

//components extraction
#define DPitch_Step(dp)   (((int)dp - 1) % 7)
#define DPitch_Octave(dp) (((int)dp - 1) / 7)

//====================================================================================
// lmMPitch
//  MIDI pitch
//====================================================================================

#define lmC4_MPITCH   60


extern wxString MPitch_ToLDPName(lmMPitch nMidiPitch);
extern bool MPitch_IsNaturalNote(lmMPitch ntMidi, lmEKeySignatures nKey);


//-------------------------------------------------------------------------
// lmFPitch
//  Base-40 absolute pitch representation. Interval-invariant. Only 2 accidentals
//-------------------------------------------------------------------------

// FPitch

#define lmC4_FPITCH   163

//constructors
extern lmFPitch FPitch(lmAPitch ap);
extern lmFPitch FPitch(lmDPitch dp, int nAcc);
extern lmFPitch FPitch(int nStep, int nOctave, int nAcc);
extern lmFPitch FPitch(wxString& sLDPNote);

//components extraction
#define FPitch_Step(fp) ((((fp - 1) % 40) + 1) / 6)
#define FPitch_Octave(fp) ((fp - 1) / 40)
extern int FPitch_Accidentals(lmFPitch fp);

//conversion
extern wxString FPitch_ToAbsLDPName(lmFPitch fp);
extern wxString FPitch_ToRelLDPName(lmFPitch fp, lmEKeySignatures nKey);
extern lmMPitch FPitch_ToMPitch(lmFPitch fp);
extern lmDPitch FPitch_ToDPitch(lmFPitch fp);
extern lmAPitch FPitch_ToAPitch(lmFPitch fp);

//operations
extern lmFPitch FPitch_AddSemitone(lmFPitch fpNote, lmEKeySignatures nKey);
extern lmFPitch FPitch_AddSemitone(lmFPitch fpNote, bool fUseSharps);




// Intervals in FPitch

// Intervals are just the difference between the two FPitches.
// Intervals can be added/substracted. For example: p5-3M = 3m (23-12=11)
// Intervals greater than one octave are computed by adding 40 (p8) for each octave.
// For example, a Major 10th is M3+p8 = 12+40 = 52


    //unison
#define lm_p1   0
#define lm_a1   1
    //second
#define lm_d2   4 
#define lm_m2   5 
#define lm_M2   6 
#define lm_a2   7
    //third
#define lm_d3   10 
#define lm_m3   11 
#define lm_M3   12 
#define lm_a3   13
    //fourth
#define lm_d4   16 
#define lm_p4   17 
#define lm_a4   18
    //fifth
#define lm_d5   22 
#define lm_p5   23 
#define lm_a5   24
    //sixth
#define lm_d6   27 
#define lm_m6   28 
#define lm_M6   29 
#define lm_a6   30
    //seventh
#define lm_d7   33 
#define lm_m7   34 
#define lm_M7   35 
#define lm_a7   36
    //octave
#define lm_d8   39 
#define lm_p8   40

// Lets define a 'NULL interval'. It is usefull to signal special situations such as
// 'no interval defined', 'end of list', etc.
#define lmNULL_FIntval  -1

//interval constructors
extern lmFIntval FIntval(wxString& sName);


#endif    // __PITCH_H__

