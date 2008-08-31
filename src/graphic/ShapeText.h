//--------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
//    Copyright (c) 2002-2008 Cecilio Salmeron
//
//    This program is free software; you can redistribute it and/or modify it under the
//    terms of the GNU General Public License as published by the Free Software Foundation,
//    either version 3 of the License, or (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful, but WITHOUT ANY
//    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
//    PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along with this
//    program. If not, see <http://www.gnu.org/licenses/>.
//
//    For any comment, suggestion or feature request, please contact the manager of
//    the project at cecilios@users.sourceforge.net
//
//-------------------------------------------------------------------------------------

#ifndef __LM_SHAPETEXT_H__        //to avoid nested includes
#define __LM_SHAPETEXT_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "ShapeText.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "../score/defs.h"      // lmLUnits
#include "GMObject.h"
#include "Shapes.h"             // lmShapeRectangle

class lmScoreObj;
class lmStaff;
class lmStaffObj;
class lmPaper;



//------------------------------------------------------------------------------------

class lmShapeText : public lmSimpleShape
{
public:
    lmShapeText(lmScoreObj* pOwner, wxString sText, wxFont* pFont, lmPaper* pPaper,
                lmUPoint offset, wxString sName=_T("ShapeText"),
				bool fDraggable = false, wxColour color = *wxBLACK);
    ~lmShapeText() {}

    //implementation of virtual methods from base class
    void Render(lmPaper* pPaper, wxColour color = *wxBLACK);
    wxString Dump(int nIndent);
    void Shift(lmLUnits xIncr, lmLUnits yIncr);
	virtual wxBitmap* OnBeginDrag(double rScale, wxDC* pDC);

    //specific methods
    void SetFont(wxFont *pFont);
    wxString* GetText() { return &m_sText; }


private:
    wxString    m_sText;
    wxFont*     m_pFont;
    lmUPoint    m_uPos;        // text position (absolute)

};

//------------------------------------------------------------------------------------

class lmShapeTextBlock : public lmShapeRectangle
{
public:
    lmShapeTextBlock(lmScoreObj* pOwner, const wxString& sText, wxFont* pFont,
                     lmPaper* pPaper, lmEBlockAlign nBlockAlign,
                     lmEHAlign nHAlign, lmEVAlign nVAlign,
                     lmLUnits xLeft, lmLUnits yTop,
                     lmLUnits xRight, lmLUnits yBottom,
                     wxColour nColor = *wxBLACK, wxString sName=_T("ShapeTextBlock"),
					 bool fDraggable = true);

    ~lmShapeTextBlock() {}

    //implementation of virtual methods from base class
    void Render(lmPaper* pPaper, wxColour color = *wxBLACK);
    wxString Dump(int nIndent);
    void Shift(lmLUnits xIncr, lmLUnits yIncr);

    //specific methods
    void SetFont(wxFont *pFont);
    wxString* GetText() { return &m_sText; }

    //call backs
    wxBitmap* OnBeginDrag(double rScale, wxDC* pDC);
	lmUPoint OnDrag(lmPaper* pPaper, const lmUPoint& uPos);
	void OnEndDrag(lmController* pCanvas, const lmUPoint& uPos);



private:
    void Create(const wxString& sText, wxFont* pFont, lmPaper* pPaper,
                lmEBlockAlign nBlockAlign, lmEHAlign nHAlign, lmEVAlign nVAlign,
                lmLUnits xLeft, lmLUnits yTop, lmLUnits xRight, lmLUnits yBottom);

    void ComputeTextPosition();
    void ComputeBlockBounds(lmLUnits xLeft, lmLUnits yTop, lmLUnits xRight, lmLUnits yBottom);

    lmEBlockAlign   m_nBlockAlign;
    lmEHAlign       m_nHAlign;
    lmEVAlign       m_nVAlign;
    wxString        m_sText;
    wxFont*         m_pFont;
    lmUPoint        m_uTextPos;     // text position (relative to top-left of rectangle)
    lmLUnits        m_uTextWidth;
    lmLUnits        m_uTextHeight;

};

#endif    // __LM_SHAPETEXT_H__

