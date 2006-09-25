//--------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
//    Copyright (c) 2002-2006 Cecilio Salmeron
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
#ifndef __AGGDRAWER_H__        //to avoid nested includes
#define __AGGDRAWER_H__

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "Drawer.h"

#include "wx/image.h"


#include "agg_rendering_buffer.h"       //to deal with bytes buffer
#include "agg_pixfmt_rgb.h"             //to use buffer as pixels (to know about the pixel format)
#include "agg_rasterizer_scanline_aa.h" //to use rasterized scanline
#include "agg_scanline_u.h"
#include "agg_renderer_scanline.h"
#include "agg_renderer_primitives.h"    //for rendering aliassed lines
#include "agg_renderer_markers.h"       //for rendering markers
#include "agg_scanline_bin.h"
//#include "agg_font_freetype.h"


//basic types
typedef agg::rgba8 lmColor_rgba8;           //rgba, 8 bits per channel (four bytes)
typedef agg::rgba  lmColor_rgba;            //internal rgba, double value per channel


// AggDrawer is an anti-aliased drawer using the AGG library (Anti-Grain Geometry)
class lmAggDrawer : public lmDrawer
{
public:
    lmAggDrawer(wxDC* pDC, int widthPixels, int heightPixels, int stride=0);
    ~lmAggDrawer();

    //access to image
    wxImage& GetImageBuffer() { return m_buffer; }

    //draw shapes
    void DrawLine(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2);
    void DrawRectangle(wxPoint point, wxSize size);
    void DrawRectangle(wxCoord left, wxCoord top, wxCoord width, wxCoord height);
    void DrawCircle(wxCoord x, wxCoord y, wxCoord radius);
    void DrawCircle(const lmUPoint& pt, wxCoord radius);

    void DrawRectangle(lmUPoint uPoint, wxSize size);
    void RenderPolygon(int n, lmUPoint points[]);
    void RenderCircle(const lmUPoint& pt, lmLUnits radius);

    //brushes, colors, fonts, ...
    void SetBrush(wxBrush brush);
    void SetFont(wxFont& font);
    void SetPen(wxPen& pen);
    const wxPen& GetPen() const;
    void SetLogicalFunction(int function);

    //text
    void DrawText(const wxString& text, wxCoord x, wxCoord y);
    void SetTextForeground(const wxColour& colour);
    void SetTextBackground(const wxColour& colour);
    void GetTextExtent(const wxString& string, lmLUnits* w, lmLUnits* h); 

    // units conversion
    lmLUnits DeviceToLogicalX(lmPixels x);
    lmLUnits DeviceToLogicalY(lmPixels y);
    lmPixels LogicalToDeviceX(lmLUnits x);
    lmPixels LogicalToDeviceY(lmLUnits y);

private:
    void Initialize();
    inline double WorldToDeviceX(lmLUnits x) const { return m_xDevicePixelsPerLU * (double)x; }
    inline double WorldToDeviceY(lmLUnits y) const { return m_yDevicePixelsPerLU * (double)y; }
    lmColor_rgba8 lmToRGBA8(wxColour color);



        // member variables

    //the rendering buffer as bytes
    wxImage                     m_buffer;               //the image to serve as buffer
    unsigned char*              m_pdata;                //ptr to the real bytes buffer
    agg::rendering_buffer       m_oRenderingBuffer;     //the agg rendering buffer
    int                         m_nBufWidth, m_nBufHeight;      //size of the bitmap
    int                         m_nStride;               //the row step

    //the rendering buffer as pixels
    #define pix_format agg::pix_format_rgb24
    typedef agg::pixfmt_rgb24 lmPixelsBuffer;
    lmPixelsBuffer*     m_pPixelsBuffer;

    //the base renderer
    typedef agg::renderer_base<lmPixelsBuffer> base_ren_type;
    base_ren_type*      m_pRenBase;

    //anti-aliased rendered
    typedef agg::renderer_scanline_aa_solid<base_ren_type> lmRendererSolidType;
    lmRendererSolidType*     m_pRenSolid;


    agg::scanline_u8                m_sl;
    agg::rasterizer_scanline_aa<>   m_ras;

    //the renderer_marker, for aliased drawing of shapes.
    typedef agg::renderer_markers<base_ren_type> lmRendererMarkerType;
    lmRendererMarkerType*       m_pRenMarker;

    // scaling factors between device pixels and logical units
    double        m_xDevicePixelsPerLU;
    double        m_yDevicePixelsPerLU;


    //current settings
    lmColor_rgba8   m_colorF;   //foreground color;
    lmColor_rgba8   m_colorB;   //background color;
    lmColor_rgba8   m_penColor;
    int             m_penWidth;


    wxBitmap* m_pDummyBitmap;

};


#endif  // __AGGDRAWER_H__
