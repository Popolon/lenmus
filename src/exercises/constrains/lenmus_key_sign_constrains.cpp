//---------------------------------------------------------------------------------------
//    LenMus Phonascus: The teacher of music
//    Copyright (c) 2002-2021 LenMus project
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
#include "lenmus_key_sign_constrains.h"
#include "lenmus_constrains.h"

//wxWidgets
#include <wx/wxprec.h>


namespace lenmus
{

TheoKeySignConstrains::TheoKeySignConstrains(wxString sSection,
                                             ApplicationScope& appScope)
    : ExerciseOptions(sSection, appScope)
{
    //
    // default values
    //

    // all key signatures allowed
    for (int i=k_min_key; i <= k_max_key; i++)
        m_oValidKeys.SetValid((EKeySignature)i, true);

    // only G clef allowed
    for (int i = k_min_clef_in_exercises; i <= k_max_clef_in_exercises; i++)
        m_oClefs.SetValid((EClef) i, false);
    m_oClefs.SetValid(k_clef_G2, true);

    // other settings
    m_nProblemType = eBothKeySignProblems;      // both problems allowed
    m_nMaxAccidentals = 5;                      // max.: 5 accidentals
    m_nMode = k_scale_both;              // both, major and minor, allowed
}


}   // namespace lenmus
