//--------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
//    Copyright (c) 2002-2008 Cecilio Salmeron
//
//    This program is free software; you can redistribute it and/or modify it under the 
//    terms of the GNU General Public License as published by the Free Software Foundation;
//    either version 3 of the License, or (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful, but WITHOUT ANY 
//    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
//    PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along with this 
//    program. If not, see <http://www.gnu.org/licenses/>. 
//
//
//    For any comment, suggestion or feature request, please contact the manager of 
//    the project at cecilios@users.sourceforge.net
//
//-------------------------------------------------------------------------------------
#ifndef __LM_DLGCOMPILEBOOK_H__        //to avoid nested includes
#define __LM_DLGCOMPILEBOOK_H__

// GCC interface
#if defined(__GNUG__) && !defined(__APPLE__)
    #pragma interface "DlgCompileBook.h"
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#else
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/checkbox.h>
#include <wx/statbox.h>
#include <wx/dialog.h>
#endif

// headers
#include "wx/dialog.h"

// supported languages table
typedef struct lmLangDataStruct {
    wxString sLang;
    wxString sLangName;
    wxString sCharCode;
} lmLangData;

#define lmNUM_LANGUAGES 7
extern const lmLangData g_tLanguages[lmNUM_LANGUAGES];

//table must be ordered by language name (in English) to ensure
//correspondence with table in DlgCompileBook.cpp
enum ELanguages
{   
    eLangBasque = 0,
    eLangDutch,
    eLangEnglish,
    eLangFrench,
    eLangItalian,
    eLangSpanish,
    eLangTurkish,
    eLangLast
};

typedef struct lmCompileBookOptionsStruct {
    wxString sSrcPath;
    wxString sDestPath;
    bool fLanguage[eLangLast];
    //debug options
    bool fLogTree;
    bool fDump;
} lmCompileBookOptions;


// class definition
class lmDlgCompileBook : public wxDialog {

public:
    lmDlgCompileBook(wxWindow* parent, lmCompileBookOptions* pOptions);
    virtual ~lmDlgCompileBook();

    // event handlers
    void OnAcceptClicked(wxCommandEvent& WXUNUSED(event));
    void OnCancelClicked(wxCommandEvent& WXUNUSED(event)) { EndDialog(wxID_CANCEL); }
    void OnDataChanged(wxCommandEvent& WXUNUSED(event));
    void OnBrowseSrc(wxCommandEvent& WXUNUSED(event));



private:
    void Create();
    bool VerifyData();

    //controls
	wxButton*       pBtBrowseSrc;
	wxButton*       pBtBrowseDest;
	wxButton*       btOK;
	wxButton*       btCancel;
	wxStaticText*   pLblSrcPath;
    wxStaticText*   pLblDestPath;

    wxCheckBox*     m_pChkLang[eLangLast+1];        //the last one is for option 'All languages'
    wxTextCtrl*     m_pTxtSrcPath;
    wxTextCtrl*     m_pTxtDestPath;

    wxCheckBox*     m_pChkDumpTree;
    wxCheckBox*     m_pChkLogTree;


    //communication with parent
    lmCompileBookOptions*   m_pOptions;


    DECLARE_EVENT_TABLE()
};

#endif    // __LM_DLGCOMPILEBOOK_H__
