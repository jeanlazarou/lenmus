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

#ifndef __LM_THEOHARMONYTOOLSET_H__        //to avoid nested includes
#define __LM_THEOHARMONYTOOLSET_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "Harmony.cpp"
#endif

// aware: if included <list> before <map>: syntax error !  (MS bug?)
#include <map>  
#include <list>

#include "../auxmusic/ChordManager.h"

//
// Chord harmony types and classes
//


// To store all the possible errors resulting from chord analysis
// Error <--> broken rule
// Basically it is a compressed list of rule identifiers. Each rule is a bit
typedef struct lmChordErrorStruct
{
    unsigned int nErrList;
    bool IncludesError(int nBrokenRule);  // arg: lmChordValidationRules
    void SetError(int nBrokenRule, bool fVal); // arg1: lmChordValidationRules
} lmChordError;



typedef struct lmChordDescriptorStruct {
    lmChordManager*  pChord;
    lmNote* pChordNotes[lmNOTES_IN_CHORD-1];
    int nNumChordNotes;
    lmChordError  tChordErrors;

    // methods
    lmChordDescriptorStruct()
    {
        nNumChordNotes = 0;
        pChord = NULL;
        for (int i = 0; i<lmNOTES_IN_CHORD-1; i++)
        {
            pChordNotes[i] = NULL;
        }
        tChordErrors.nErrList = 0;
    }
    wxString ToString();
} lmChordDescriptor;
#define lmMAX_NUM_CHORDS 50






//--------------------------------------------------------------------------
// A list of notes 
//   with individual absolute end time
//   with global absolute current time
//--------------------------------------------------------------------------
typedef struct lmActiveNoteInfoStruct {
    lmNote*  pNote;
    float    rEndTime;
    lmActiveNoteInfoStruct(lmNote* pNoteS, float rEndTimeS)
    {
        pNote = pNoteS;
        rEndTime = rEndTimeS;
    }
} lmActiveNoteInfo;

class lmActiveNotes 
{
public:
    lmActiveNotes();
    ~lmActiveNotes();
    
    void SetTime(float r_new_current_time);
    float GetTime() { return r_current_time; };
    void GetChordDescriptor(lmChordDescriptor* ptChordDescriptor);
    void AddNote(lmNote* pNote, float rEndTime);
    void RecalculateActiveNotes();
    int  GetNumActiveNotes();
    // For debugging
    wxString ToString();

protected:
    void ResetNotes();

    float r_current_time;
    std::list<lmActiveNoteInfo*> m_ActiveNotesInfo; 
};


enum  lmChordValidationRules 
{
    lmCVR_ChordHasAllSteps,  // 1. The chord is complete (has all note steps)
    lmCVR_FirstChordValidationRule = lmCVR_ChordHasAllSteps,
    lmCVR_ChordHasAllNotes,  // *2. lm_eFifthMissing - Acorde completo. Contiene todas las notas (en todo caso, elidir la 5�)
    lmCVR_NoParallelMotion, // 3. No parallel motion of perfect octaves, perfect fifths, and unisons
    lmCVR_NoResultingFifthOctaves,
    // *4. lm_eResultantFifthOctves - No hacer 5�s ni 8�s resultantes, excepto:
                                            //> a) la soprano se ha movido por segundas
                                            //> b) (para 5�s) uno de los sonidos ya estaba
    lmCVR_NoFifthDoubled, // 5. The fifth is not doubled
	lmCVR_NoLeadingToneDoubled, // 6. The leading tone is never doubled
    lmCVR_LeadingToneResolveToTonic, // 7. Scale degree seven (the leading tone) should resolve to tonic.
    lmCVR_SeventhResolution, // *8.lm_eSeventhResolution - the seventh of a chord should always resolve down by second.
    lmCVR_NoIntervalHigherThanOctave, // 9. voices interval not greater than one octave (except bass-tenor)
    lmCVR_NoVoicesCrossing, // 10. Do not allow voices crossing. No duplicates
    lmCVR_NoVoicesOverlap, // 11. Voice overlap: when a voice moves above or below a pitch previously sounded by another voice.
    lmCVR_ChromaticAlterationsDirection, // 12. Resolve chromatic alterations by step in the same direction than the alteration.
    lmCVR_NoIntervalHigherThanSixth, // *13.lm_eGreaterThanSixth - No es conveniente exceder el intervalo de sexta, exceptuando la octava justa
    lmCVR_BassMovementByStep, // 14. If bass moves by step all other voices moves in opposite direction to bass
    lmCVR_ThirdDoubledInBrokenCadence, // *15.lm_eNotDoubledThird - Cuando el bajo enlaza el V grado con el VI (cadencia rota), en el acorde de VI grado se duplica la tercera.
    lmCVR_LastChordValidationRule = lmCVR_ThirdDoubledInBrokenCadence,

};

//
// Base virtual class of rules
// 
class lmRule 
{
public:
  ///  DECLARE_ABSTRACT_CLASS(lmRule) //@@ TODO: aclarar �necesario?

    lmRule(int nRuleID, wxString sDescription);
    virtual ~lmRule(){};
    virtual int Evaluate(wxString& sResultDetails, int pNumFailuresInChord[] )=0;
//@@TODO QUITAR    virtual bool Evaluate(wxString& sResultDetails, lmChordDescriptor* pFailingChord)=0;
    void SetChordDescriptor(lmChordDescriptor* pChD, int nNumChords)
    {
        m_pChordDescriptor = pChD; 
        m_nNumChords = nNumChords;
    };
    bool IsEnabled(){ return m_fEnabled; };
    void Enable( bool fVal ){ m_fEnabled = fVal; };
    wxString GetDescription() { return m_sDescription;};
    int GetRuleId() { return m_nRuleId;};
protected:
    int m_nNumChords;
    lmChordDescriptor* m_pChordDescriptor; // array of chords
    bool m_fEnabled;
    wxString m_sDescription;
    wxString m_sDetails;
    int m_nRuleId;
};

//
// The list of rules
// 
// possibly useful: typedef std::map<int, lmRule*> lmRuleMapType;
class lmRuleList 
{
public:
    lmRuleList(lmChordDescriptor* pChD, int nNumChords);
    ~lmRuleList();
    
    bool AddRule(lmRule* pNewRule);  // return: ok
    bool DeleteRule(int nRuleId);  // arg: lmChordValidationRules; return: ok
    lmRule* GetRule(int nRuleId);  // arg: lmChordValidationRules;
    void SetChordDescriptor(lmChordDescriptor* pChD, int nNumChords);

protected:
    void CreateRules();
    std::map<int, lmRule*> m_Rules; 
};


//
// Derived classes of rules
// 
// TODO: improve this with a template...
#define LM_CREATE_CHORD_RULE(classname, id, text) \
class classname : public lmRule  \
{ \
public: \
    classname() :lmRule(id, _T(text)) {}; \
    int Evaluate(wxString& sResultDetails, int pNumFailuresInChord[]); \
};


//
// GLOBAL AUX FUNCTIONS
//

/*----TODO QUITAR!
extern void  DisplayChordInfo(lmScore* pScore, lmChordDescriptor*  pChordDsct
                        , wxColour colour, wxString &sText, bool reset);

--*/
extern void  HDisplayChordInfo(lmScore* pScore, lmChordDescriptor*  pChordDsct
                                           , wxColour colour, wxString &sText, bool reset);

enum lmHarmonicMovementType {
    lm_eDirectMovement ,    // 2 voices with the same delta sign (cero included)
    lm_eObliqueMovement ,   // one delta sign is 0, the other not
    lm_eContraryMovement    // 2 voices with contrary delta sign (cero not included)
};
extern int GetHarmonicMovementType(  lmNote* pVoice10, lmNote* pVoice11, lmNote* pVoice20, lmNote* pVoice21);

#endif