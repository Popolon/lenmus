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

#ifndef __LM_LDPPARSER_H        //to avoid nested includes
#define __LM_LDPPARSER_H

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "LDPParser.cpp"
#endif


#include "wx/txtstrm.h"
#include "wx/wfstream.h"

#include "LDPNode.h"
#include "LDPToken.h"
#include "LDPTags.h"

#include "../score/Score.h"


enum lmETagLDP
{
	lm_eTag_Visible = 0,
	lm_eTag_Location_x,
	lm_eTag_Location_y,
	lm_eTag_StaffNum,
	//
	lm_eTag_Max				//to know the number of tags defined
};



//The parser
class lmLDPParser
{
public:
    lmLDPParser();
    lmLDPParser(wxString sLanguage, wxString sCharset);
    ~lmLDPParser();

    // "Parse" methods: work on source text
    lmScore*    ParseFile(const wxString& filename);
    lmLDPNode*  ParseText(const wxString& sSource);

    // "Analyze" methods: work on a tree of LMNodes generated by ParseText().
    // User is responsible for calling the appropiate Analyze method, depending on
    // the type of text parsed
    bool        AnalyzeBarline(lmLDPNode* pNode, lmVStaff* pVStaff);
    void        AnalyzeChord(lmLDPNode* pNode, lmVStaff* pVStaff);
    bool        AnalyzeClef(lmVStaff* pVStaff, lmLDPNode* pNode);
    lmEPlacement AnalyzeFermata(lmLDPNode* pNode, lmVStaff* pVStaff, lmLocation* pPos);
    void        AnalyzeFont(lmLDPNode* pNode, lmFontInfo* pFont);
    void        AnalyzeGraphicObj(lmLDPNode* pNode, lmVStaff* pVStaff);
    void        AnalyzeInstrument(lmLDPNode* pNode, lmScore* pScore, int nInstr);
    void        AnalyzeInstrument105(lmLDPNode* pNode, lmScore* pScore, int nInstr);
    bool        AnalyzeKeySignature(lmLDPNode* pNode, lmVStaff* pVStaff);
    void        AnalyzeLocation(lmLDPNode* pNode, float* pValue, lmEUnits* pUnit);
    void        AnalyzeLocation(lmLDPNode* pNode, lmLocation* pPos);
    void        AnalyzeMeasure(lmLDPNode* pNode, lmVStaff* pVStaff);
    bool        AnalyzeMetronome(lmLDPNode* pNode, lmVStaff* pVStaff);
    bool        AnalyzeNewSystem(lmLDPNode* pNode, lmVStaff* pVStaff);
    lmNote*     AnalyzeNote(lmLDPNode* pNode, lmVStaff* pVStaff, bool fChord=false);
    lmNoteRest* AnalyzeNoteRest(lmLDPNode* pNode, lmVStaff* pVStaff, bool fChord=false);
    void        AnalyzeOption(lmLDPNode* pNode, lmScoreObj* pObject);
    lmRest*     AnalyzeRest(lmLDPNode* pNode, lmVStaff* pVStaff);
    lmScore*    AnalyzeScore(lmLDPNode* pNode);
    void        AnalyzeSpacer(lmLDPNode* pNode, lmVStaff* pVStaff);
    void        AnalyzeSplit(lmLDPNode* pNode, lmVStaff* pVStaff);
    lmEStemType   AnalyzeStem(lmLDPNode* pNode, lmVStaff* pVStaff);
    bool        AnalyzeText(lmLDPNode* pNode, lmVStaff* pVStaff);
    bool        AnalyzeTitle(lmLDPNode* pNode, lmScore* pScore);
    bool        AnalyzeTimeSignature(lmVStaff* pVStaff, lmLDPNode* pNode);
    void        AnalyzeMusicData(lmLDPNode* pNode, lmVStaff* pVStaff);
    void        AnalyzeVStaff(lmLDPNode* pNode, lmVStaff* pVStaff);

	//analyze options
    int         AnalyzeNumStaff(const wxString& sNotation, long nNumStaves);

    // for lmLDPToken
    const wxString& GetNewBuffer();

    // error and logging
    void AnalysisError(const wxChar* szFormat, ...);
    void ParseMsje(wxString sMsg);


    // auxiliary methods
    static float GetDefaultDuration(lmENoteType nNoteType, bool fDotted, bool fDoubleDotted,
									int nActualNotes, int nNormalNotes);
    int GetBeamingLevel(lmENoteType nNoteType);
    bool ParenthesisMatch(const wxString& sSource);


private:
    enum EParsingStates
    {
        A0_WaitingForStartOfElement = 0,
        A1_WaitingForName,
        A2_WaitingForParameter,
        A3_ProcessingParameter,
        A4_Exit,
        A5_ExitError
    };

    void        Create(const wxString& sLanguage, const wxString& sCharset);
    lmLDPNode*  LexicalAnalysis();
    bool        AnalyzeNoteType(wxString& sNoteType, lmENoteType* pnNoteType, 
                                bool* pfDotted, bool* pfDoubleDotted);
    lmScore*    AnalyzeScoreV102(lmLDPNode* pNode);
    lmScore*    AnalyzeScoreV105(lmLDPNode* pNode);
    bool        AnalyzeTextString(lmLDPNode* pNode, wxString* pText, 
                                  lmEAlignment* pAlign, lmLocation* pPos,
                                  lmFontInfo* pFont, bool* pHasWidth);
    bool        AnalyzeTimeExpression(const wxString& sData, float* pValue);
    void        AnalyzeTimeShift(lmLDPNode* pNode, lmVStaff* pStaff);
    bool        AnalyzeTuplet(lmLDPNode* pNode, const wxString& sParent, bool fOpenAllowed,
                              bool fCloseAllowed,
                              lmTupletBracket** pTuplet, int* pActual, int* pNormal);
    void        AnalyzeVStaff_V103(lmLDPNode* pNode, lmVStaff* pVStaff);


    void Clear();
    void FileParsingError(const wxString& sMsg);
    void PushNode(EParsingStates nPopState);
    bool PopNode();
    void Do_WaitingForStartOfElement();
    void Do_WaitingForName();
    void Do_WaitingForParameter();
    void Do_ProcessingParameter();

    void ParseError(EParsingStates nState, lmLDPToken* pTk);



    //LDP tags table
    lmLdpTagsTable*     m_pTags;

    // variables to store temporary values as the file is being analized.
    int     m_nCurStaff;        //default staff num. for the lmNoteRest being processed
    int     m_nCurVoice;        //default voice num. for the lmNoteRest being processed
    long    m_nNumStaves;       //number of staffs for the lmVStaff being processed
    lmTupletBracket* m_pTupletBracket;      //tuplet being analyzed



    lmLDPTokenBuilder*  m_pTokenizer;       //ptr to token builder object
    lmLDPToken*         m_pTk;              //current token
    EParsingStates      m_nState;           //estado actual del aut�mata
    int                 m_nLevel;           //numero de nodos
    wxArrayInt          m_stackStates;      //Nodo de retorno tras un PopNodo
    lmArrayNodePtrs		m_stackNodes;       //Pila de nodos
    lmLDPNode*          m_pCurNode;         //nodo en proceso
    wxString            m_sVersion;         //versi�n del lenguaje en que est� la partitura en proceso
    int                 m_nVersion;         //versi�n convertida a numerico 100*num+rev. Ej. 1.2 = 102

    bool                m_fDebugMode;

    // parsing control and error variables
    bool            m_fFromString;        // true: parsing a string. false: parsing a file
    bool            m_fStartingTextAnalysis;    //to signal the start of a new analysis
    wxString        m_sLastBuffer;        // to keep line under analysis
    long            m_nErrors;            // numebr of serious errors during parsing
    long            m_nWarnings;          // number of warnings during parsing
    wxFileInputStream*    m_pFile;        // file being parsed
    wxTextInputStream*    m_pTextFile;    // file being parsed

    //
    //variables to keep settings that propagate
    //

    // octave and duration for notes/rests
    wxString        m_sLastOctave;
    wxString        m_sLastDuration;

    // font and aligment for <title> elements
    lmEAlignment    m_nTitleAlignment;
    wxString        m_sTitleFontName;
    int             m_nTitleFontSize; 
    lmETextStyle    m_nTitleStyle;

    // font for <text> elements
    wxString        m_sTextFontName;
    int             m_nTextFontSize; 
    lmETextStyle    m_nTextStyle;

    // tuplet options
    bool            m_fShowTupletBracket;
    bool            m_fShowNumber;
    bool            m_fTupletAbove;



};


// Helper class to analyze optional elements
class lmLDPOptionalTags
{
public:
	lmLDPOptionalTags(lmLDPParser* pParser, lmLdpTagsTable* pTags);
	~lmLDPOptionalTags();

	void SetValid(lmETagLDP nTag, ...);
	void AnalyzeCommonOptions(lmLDPNode* pNode, int iP, lmVStaff* pVStaff,
							  bool* pfVisible, int* pStaffNum, lmLocation* pLocation);

private:
	bool VerifyAllowed(lmETagLDP nTag, wxString sName);

	lmLDPParser*		m_pParser;					//owner parser
    lmLdpTagsTable*     m_pTags;
	std::vector<bool>	m_ValidTags;


};



#endif    // __LM_LDPPARSER_H
