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

#ifndef __LM_SHAPEREST_H__        //to avoid nested includes
#define __LM_SHAPEREST_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "ShapeRest.cpp"
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
#include "Shapes.h"

class lmShapeBeam;
class lmPaper;


class lmShapeRest : public lmCompositeShape
{
public:
    lmShapeRest(lmScoreObj* pOwner, bool fDraggable = false, bool fVisible = true,
                lmEGMOType nType = eGMO_ShapeRest, wxString sName = _T("Rest"));


	virtual ~lmShapeRest();

	//overrides of virtual methods in base class
    virtual void Render(lmPaper* pPaper, wxColour color = *wxBLACK);

	//info about related shapes
	inline void SetBeamShape(lmShapeBeam* pBeamShape) { m_pBeamShape = pBeamShape; }
	inline lmShapeBeam* GetBeamShape() const { return m_pBeamShape; }

protected:
	//related shapes
	lmShapeBeam*	m_pBeamShape;

};

#endif    // __LM_SHAPEREST_H__
