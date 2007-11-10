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

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "GMObject.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "../app/global.h"
#include "GMObject.h"
#include "../app/Paper.h"
#include "../score/StaffObj.h"

//access to colors
#include "../globals/Colors.h"
extern lmColors* g_pColors;

//========================================================================================
//Implementation of class lmGMObject: the root object for the graphical model
//========================================================================================

static int m_IdCounter = 0;        //to assign unique IDs to GMObjects



lmGMObject::lmGMObject(lmEGMOType nType)
{
    m_nId = m_IdCounter++;      // give it an ID
    m_nType = nType;            // save its type

	//initializations
	m_uBoundsBottom = lmUPoint(0.0, 0.0);
    m_uBoundsTop = lmUPoint(0.0, 0.0);
}

lmGMObject::~lmGMObject()
{
}

bool lmGMObject::ContainsPoint(lmUPoint& pointL)
{
    //returns true if point received is within the limits of this Box
    return GetBounds().Contains(pointL);

}

void lmGMObject::DrawBounds(lmPaper* pPaper, wxColour color)
{
    //draw a border around instrSlice region
    pPaper->SketchRectangle(m_uBoundsTop,
                            lmUSize(m_uBoundsBottom.x - m_uBoundsTop.x, m_uBoundsBottom.y - m_uBoundsTop.y),
                            color);

}

wxString lmGMObject::DumpBounds()
{
    return wxString::Format(_T("Bounds=(%.2f, %.2f, %.2f, %.2f)"),
        	m_uBoundsTop.x, m_uBoundsTop.y,
            m_uBoundsBottom.x - m_uBoundsTop.x, m_uBoundsBottom.y - m_uBoundsTop.y);
}   

void lmGMObject::NormaliceBoundsRectangle()
{
	// Ensure that TopLeft point is at left-top of BottomRight point

	if (m_uBoundsBottom.x < m_uBoundsTop.x)
	{
		lmLUnits uxMin = m_uBoundsBottom.x;
		m_uBoundsBottom.x = m_uBoundsTop.x;
		m_uBoundsTop.x = uxMin;
	}
	if (m_uBoundsBottom.y < m_uBoundsTop.y)
	{
		lmLUnits uyMin = m_uBoundsBottom.y;
		m_uBoundsBottom.y = m_uBoundsTop.y;
		m_uBoundsTop.y = uyMin;
	}
}


//========================================================================================
//Implementation of class lmBox: the container objects root
//========================================================================================


lmBox::lmBox(lmEGMOType nType) : lmGMObject(nType)
{
}

lmBox::~lmBox()
{
    //delete shapes collection
    for (int i=0; i < (int)m_Shapes.size(); i++)
    {
        delete m_Shapes[i];
    }
    m_Shapes.clear();
}

void lmBox::AddShape(lmShape* pShape)
{
    m_Shapes.push_back(pShape);
}

lmShape* lmBox::FindShapeAtPosition(lmUPoint& pointL)
{
    //loop to look up in the shapes collection
	for(int i=0; i < (int)m_Shapes.size(); i++)
    {
        if (m_Shapes[i]->ContainsPoint(pointL))
			return m_Shapes[i];    //found
    }

    // no shape found.
    return (lmShape*)NULL;      
}

//========================================================================================
// Implementation of class lmShape: any renderizable object, such as a line,
// a glyph, a note head, an arch, etc.
//========================================================================================


lmShape::lmShape(lmEGMOType nType, lmObject* pOwner, wxString sName)
	: lmGMObject(nType)
{
	m_pOwner = pOwner;
    m_sShapeName = sName;
}

lmShape::~lmShape()
{
	//delete attachment data
	for(int i=0; i < (int)m_cAttachments.size(); i++)
    {
		delete m_cAttachments[i];
    }

}


void lmShape::SetSelRectangle(lmLUnits x, lmLUnits y, lmLUnits uWidth, lmLUnits uHeight)
{
    m_uSelRect.x = x;
    m_uSelRect.y = y;
    m_uSelRect.width = uWidth;
    m_uSelRect.height = uHeight;
}

void lmShape::DrawSelRectangle(lmPaper* pPaper, lmUPoint uPos, wxColour colorC)
{
    lmUPoint uPoint = m_uSelRect.GetPosition();
    pPaper->SketchRectangle(uPoint + uPos, m_uSelRect.GetSize(), colorC);
}

bool lmShape::Collision(lmShape* pShape)
{
    lmURect rect1 = GetBounds();
    return rect1.Intersects( pShape->GetBounds() );
}

void lmShape::RenderCommon(lmPaper* pPaper)
{
	RenderCommon(pPaper, g_pColors->ScoreSelected());
}

void lmShape::RenderCommon(lmPaper* pPaper, wxColour colorC)
{
    // Code common to all shapes renderization. Must be invoked after specific code at
    // each shape renderization method

    // draw selection rectangle
    //TODO: remove lmUPoint parameter when old shapes, relative positioned, removed
    if (g_fDrawSelRect)      // || m_pOwner->IsSelected() )
        DrawSelRectangle(pPaper, lmUPoint(0.0, 0.0), colorC);

    if (g_fDrawBounds)
        DrawBounds(pPaper, colorC);
}
            
wxString lmShape::DumpSelRect()
{
    return wxString::Format(_T("SelRect=(%.2f, %.2f, %.2f, %.2f)"),
        	m_uSelRect.x, m_uSelRect.y, m_uSelRect.width, m_uSelRect.height);     

}   

void lmShape::ShiftBoundsAndSelRec(lmLUnits xIncr, lmLUnits yIncr)
{
	// Auxiliary method to be used by derived classes to perform common actions when the
	// shape is shifted    
	
	m_uSelRect.x += xIncr;		//AWARE: As it is a rectangle, changing its origin does not change
    m_uSelRect.y += yIncr;		//       its width/height. So no need to adjust bottom right point

	m_uBoundsTop.x += xIncr;
	m_uBoundsBottom.x += xIncr;
	m_uBoundsTop.y += yIncr;
	m_uBoundsBottom.y += yIncr;
}

int lmShape::Attach(lmShape* pShape, lmEAttachType nTag)
{
	lmAtachPoint* pData = new lmAtachPoint;
	pData->nType = nTag;
	pData->pShape = pShape;

    m_cAttachments.push_back(pData);

	//return index to attached shape
	return (int)m_cAttachments.size() - 1;

}

void lmShape::InformAttachedShapes()
{
	for(int i=0; i < (int)m_cAttachments.size(); i++)
    {
		lmAtachPoint* pData = m_cAttachments[i];
        pData->pShape->OnAttachmentPointMoved(this, pData->nType);
    }
}

//========================================================================================
// Implementation of class lmSimpleShape
//========================================================================================

lmSimpleShape::lmSimpleShape(lmEGMOType nType, lmObject* pOwner, wxString sName)
	: lmShape(nType, pOwner, sName)
{
}

lmSimpleShape::~lmSimpleShape()
{
}

void lmSimpleShape::Shift(lmLUnits xIncr, lmLUnits yIncr)
{
	//Default behaviour is to shift bounding and selection rectangles

    m_uSelRect.x += xIncr;		//AWARE: As it is a rectangle, changing its origin does not
    m_uSelRect.y += yIncr;		//       change its width/height

	m_uBoundsTop.x += xIncr;
	m_uBoundsBottom.x += xIncr;
	m_uBoundsTop.y += yIncr;
	m_uBoundsBottom.y += yIncr;

}



//========================================================================================
// Implementation of class lmCompositeShape
//========================================================================================


lmCompositeShape::lmCompositeShape(lmObject* pOwner, wxString sName, lmEGMOType nType)
	: lmShape(nType, pOwner, sName)
{
    m_fGrouped = true;	//by default all constituent shapes are grouped
}

lmCompositeShape::~lmCompositeShape()
{
    //delete Components collection
    for (int i=0; i < (int)m_Components.size(); i++)
    {
        delete m_Components[i];
    }
    m_Components.clear();
}

int lmCompositeShape::Add(lmShape* pShape)
{
    m_Components.push_back(pShape);

	if (m_Components.size() == 1)
	{
		//compute new selection rectangle
		m_uSelRect = pShape->GetSelRectangle();

		// compute outer rectangle for bounds
		m_uBoundsTop.x = pShape->GetXLeft();
		m_uBoundsTop.y = pShape->GetYTop();
		m_uBoundsBottom.x = pShape->GetXRight();
		m_uBoundsBottom.y = pShape->GetYBottom();
	}
	else
	{
		//compute new selection rectangle by union of individual selection rectangles
		m_uSelRect.Union(pShape->GetSelRectangle());

		// compute outer rectangle for bounds
		m_uBoundsTop.x = wxMin(m_uBoundsTop.x, pShape->GetXLeft());
		m_uBoundsTop.y = wxMin(m_uBoundsTop.y, pShape->GetYTop());
		m_uBoundsBottom.x = wxMax(m_uBoundsBottom.x, pShape->GetXRight());
		m_uBoundsBottom.y = wxMax(m_uBoundsBottom.y, pShape->GetYBottom());
	}

	//return index to added shape
	return (int)m_Components.size() - 1;

}

void lmCompositeShape::Shift(lmLUnits xIncr, lmLUnits yIncr)
{
	//Default behaviour is to shift all components
    for (int i=0; i < (int)m_Components.size(); i++)
    {
        m_Components[i]->Shift(xIncr, yIncr);
    }

	ShiftBoundsAndSelRec(xIncr, yIncr);
}

wxString lmCompositeShape::Dump(int nIndent)
{
	//TODO
	wxString sDump = _T("");
	sDump.append(nIndent * lmINDENT_STEP, _T(' '));
	sDump += wxString::Format(_T("%04d %s: grouped=%s, "), m_nId, m_sShapeName, 
        (m_fGrouped ? _T("yes") : _T("no")) );
    sDump += DumpBounds();
    sDump += _T("\n");

    //dump all its components
    nIndent++;
    for (int i=0; i < (int)m_Components.size(); i++)
    {
        sDump += m_Components[i]->Dump(nIndent);
    }
	return sDump;
}

void lmCompositeShape::Render(lmPaper* pPaper, lmUPoint uPos, wxColour color)
{
	RenderCommon(pPaper, *wxGREEN);

	//Default behaviour: render all components
    for (int i=0; i < (int)m_Components.size(); i++)
    {
        m_Components[i]->Render(pPaper, uPos, color);
    }
}

bool lmCompositeShape::ContainsPoint(lmUPoint& pointL)
{
    for (int i=0; i < (int)m_Components.size(); i++)
    {
        if (m_Components[i]->ContainsPoint(pointL))
			return true;
    }
	return false;

}

bool lmCompositeShape::Collision(lmShape* pShape)
{
    for (int i=0; i < (int)m_Components.size(); i++)
    {
        if (m_Components[i]->Collision(pShape))
			return true;
    }
	return false;
}

lmShape* lmCompositeShape::GetShape(int nShape)
{
	wxASSERT(nShape < (int)m_Components.size());
	return m_Components[nShape];
}
