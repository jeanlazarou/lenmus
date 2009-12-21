//--------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
//    Copyright (c) 2002-2009 LenMus project
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

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "TupletBracket.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <algorithm>

#include "Score.h"

//---------------------------------------------------------
//   lmTupletBracket implementation
//---------------------------------------------------------

lmTupletBracket::lmTupletBracket(bool fShowNumber, int nNumber, bool fBracket,
								 lmEPlacement nAbove, int nActualNotes, int nNormalNotes)
	: lmMultipleRelationship<lmNoteRest>(lm_eTupletClass)
{
    Create(fShowNumber, nNumber, fBracket, nAbove, nActualNotes, nNormalNotes);
}

lmTupletBracket::~lmTupletBracket()
{
}

void lmTupletBracket::Create(bool fShowNumber, int nNumber, bool fBracket,
							 lmEPlacement nAbove, int nActualNotes, int nNormalNotes)
{
    m_fShowNumber = fShowNumber;
    m_nTupletNumber = nNumber;
    m_fBracket = fBracket;
    m_nAbove = nAbove;
    m_nActualNotes = nActualNotes;
    m_nNormalNotes = nNormalNotes;
	m_pShape = (lmShapeTuplet*)NULL;

    //TODO: Allow user to change this values
    m_sFontName = _T("Arial");
    m_nFontSize = (int)PointsToLUnits(8);
    m_fBold = false;
    m_fItalic = true;
}

lmShape* lmTupletBracket::LayoutObject(lmBox* pBox, lmPaper* pPaper, wxColour color)
{
	//AWARE: Althoug shape pointer is initialized to NULL never assume that there is
	//a shape if not NULL, as the shape is deleted in the graphical model.
	m_pShape = (lmShapeTuplet*)NULL;
    if (!m_fBracket) return m_pShape;

	wxString sNumber = wxString::Format(_T("%d"), m_nTupletNumber);

	//prepare the font
    int nWeight = (m_fBold ? wxBOLD : wxNORMAL);
    int nStyle = (m_fItalic ? wxITALIC : wxNORMAL);
    wxFont* pFont = pPaper->GetFont(m_nFontSize, m_sFontName, wxDEFAULT, nStyle,
									nWeight, false);
    if (!pFont) {
        wxMessageBox(_("Sorry, an error has occurred while allocating the font."),
            _T("lmTupletBracket::SetFont"), wxOK);
        ::wxExit();
    }

	//create the shape
	bool fAbove = (m_nAbove == ep_Above) || (m_nAbove == ep_Default && 
					!((lmNote*)m_Notes.front())->StemGoesDown() );
	m_pShape = new lmShapeTuplet(GetStartNoteRest(), GetEndNoteRest(), NumNotes(), fAbove,
								 m_fShowNumber, sNumber, pFont, color, lm_eSquared);
	pBox->AddShape(m_pShape, GetStartNoteRest()->GetLayer());

	//attach the tuplet to start and end notes
	GetStartNoteRest()->GetShape()->Attach(m_pShape, lm_eGMA_StartObj);
	GetEndNoteRest()->GetShape()->Attach(m_pShape, lm_eGMA_EndObj);

	return m_pShape;
}

wxString lmTupletBracket::SourceLDP()
{
    wxString sSource = _T("");
    if (m_fShowNumber && m_fBracket)
        sSource += wxString::Format(_T(" t%d/%d"), GetActualNotes(), GetNormalNotes() );
    else
    {
        sSource += wxString::Format(_T(" (t + %d %d"), GetActualNotes(), GetNormalNotes() );
        if (!m_fShowNumber)
            sSource += _T(" numNone");
        if (!m_fBracket)
            sSource += _T(" noBracket");
        sSource += _T(")");
    }
    return sSource;
}
