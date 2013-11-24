//---------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
//    Copyright (c) 2002-2013 LenMus project
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
//---------------------------------------------------------------------------------------

//lenmus
#include "lenmus_dictation_constrains.h"
#include "lenmus_scores_constrains.h"

#include "lenmus_constrains.h"
#include "lenmus_generators.h"

//lomse
#include <lomse_logger.h>
using namespace lomse;

//wxWidgets
#include <wx/wxprec.h>


namespace lenmus
{

//---------------------------------------------------------------------------------------
DictationConstrains::DictationConstrains(wxString sSection, ApplicationScope& appScope)
    : ExerciseOptions(sSection, appScope)
    , m_pScoreConstrains(LENMUS_NEW ScoreConstrains(appScope))
{
//    load_settings();
}

////---------------------------------------------------------------------------------------
//bool DictationConstrains::IsValidGroup(EScaleGroup nGroup)
//{
//    if (nGroup == k_scale_group_major)
//    {
//        return (IsScaleValid( est_MajorNatural ) ||
//                IsScaleValid( est_MajorTypeII ) ||
//                IsScaleValid( est_MajorTypeIII ) ||
//                IsScaleValid( est_MajorTypeIV ) );
//    }
//    else if(nGroup == k_scale_group_minor)
//    {
//        return (IsScaleValid( est_MinorNatural ) ||
//                IsScaleValid( est_MinorDorian ) ||
//                IsScaleValid( est_MinorHarmonic ) ||
//                IsScaleValid( est_MinorMelodic ) );
//    }
//    else if(nGroup == k_scale_group_gregorian)
//    {
//        return (IsScaleValid( est_GreekIonian ) ||
//                IsScaleValid( est_GreekDorian ) ||
//                IsScaleValid( est_GreekPhrygian ) ||
//                IsScaleValid( est_GreekLydian ) ||
//                IsScaleValid( est_GreekMixolydian ) ||
//                IsScaleValid( est_GreekAeolian ) ||
//                IsScaleValid( est_GreekLocrian ) );
//    }
//    else if(nGroup == k_scale_group_other)
//    {
//        return (IsScaleValid( est_PentatonicMinor ) ||
//                IsScaleValid( est_PentatonicMajor ) ||
//                IsScaleValid( est_Blues ) ||
//                IsScaleValid( est_WholeTones ) ||
//                IsScaleValid( est_Chromatic ) );
//    }
//    else {
//        wxASSERT(false);    //impossible
//        return false;
//    }
//}

//---------------------------------------------------------------------------------------
void DictationConstrains::save_settings()
{
//    wxConfigBase* pPrefs = m_appScope.get_preferences();
//
//    // allowed scales
//    int i;
//    wxString sKey;
//    for (i=0; i < est_Max; i++)
//    {
//        sKey = wxString::Format(_T("/Constrains/RhythmicDictation/%s/Scale%dAllowed"),
//            m_sSection.c_str(), i );
//        pPrefs->Write(sKey, m_fValidScales[i]);
//    }
//
//    // key signatures
//    bool fValid;
//    for (i=k_min_key; i <= k_max_key; i++)
//    {
//        sKey = wxString::Format(_T("/Constrains/RhythmicDictation/%s/KeySignature%d"),
//            m_sSection.c_str(), i );
//        fValid = m_oValidKeys.IsValid((EKeySignature)i);
//        pPrefs->Write(sKey, fValid);
//    }
//
//    // other settings
//    sKey = wxString::Format(_T("/Constrains/RhythmicDictation/%s/DisplayKey"), m_sSection.c_str());
//    pPrefs->Write(sKey, m_fDisplayKey);
//    sKey = wxString::Format(_T("/Constrains/RhythmicDictation/%s/PlayMode"), m_sSection.c_str());
//    pPrefs->Write(sKey, m_nPlayMode);
    m_pScoreConstrains->save_settings();
}

//---------------------------------------------------------------------------------------
void DictationConstrains::load_settings()
{
//    wxConfigBase* pPrefs = m_appScope.get_preferences();
//
//    // allowed scales. Default: major natural, minor natural, harmonic and melodic
//    int i;
//    wxString sKey;
//    for (i=0; i < est_Max; i++)
//    {
//        sKey = wxString::Format(_T("/Constrains/RhythmicDictation/%s/Scale%dAllowed"),
//            m_sSection.c_str(), i );
//        EScaleType nType = (EScaleType)i;
//        bool fDefault = (nType == est_MajorNatural || nType == est_MinorNatural
//                         || nType == est_MinorHarmonic || nType == est_MinorMelodic);
//
//        pPrefs->Read(sKey, &m_fValidScales[i], fDefault);
//    }
//
//    // key signatures. Default all
//    bool fValid;
//    for (i=k_min_key; i <= k_max_key; i++)
//    {
//        sKey = wxString::Format(_T("/Constrains/RhythmicDictation/%s/KeySignature%d"),
//            m_sSection.c_str(), i );
//        pPrefs->Read(sKey, &fValid, true);
//        m_oValidKeys.SetValid((EKeySignature)i, fValid);
//    }
//
//    // other settings:
//    //      Display key - default: no
//    sKey = wxString::Format(_T("/Constrains/RhythmicDictation/%s/DisplayKey"), m_sSection.c_str());
//    pPrefs->Read(sKey, &m_fDisplayKey, false);
//    // play modes. Default: ascending
//    sKey = wxString::Format(_T("/Constrains/RhythmicDictation/%s/PlayMode"), m_sSection.c_str());
//    pPrefs->Read(sKey, &m_nPlayMode, 0);   //0-ascending
    m_pScoreConstrains->load_settings();
}

////---------------------------------------------------------------------------------------
//EScaleType DictationConstrains::GetRandomScaleType()
//{
//    int nWatchDog = 0;
//    int nType = RandomGenerator::random_number(0, est_Max-1);
//    while (!IsScaleValid((EScaleType)nType)) {
//        nType = RandomGenerator::random_number(0, est_Max-1);
//        if (nWatchDog++ == 1000)
//        {
//            LOMSE_LOG_ERROR("Program error: Loop detected");
//            return (EScaleType)0;
//        }
//    }
//    return (EScaleType)nType;
//}
//
////---------------------------------------------------------------------------------------
//bool DictationConstrains::GetRandomPlayMode()
//{
//    //return 'true' for ascending and 'false' for descending
//
//    if (m_nPlayMode == 0) //ascending
//        return true;
//    else if (m_nPlayMode == 1) //descending
//        return false;
//    else {  // both modes allowed. Choose one at random
//        RandomGenerator oGenerator;
//        return oGenerator.flip_coin();
//   }
//}


}   // namespace lenmus
