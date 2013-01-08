// Since the Geant4 histogram managers insist on filling the histograms and
// ntuples based on an integer identifier, I'll map these with variable names
// to (hopefully) avoid confusion.

// It is VERY IMPORTANT to not re-order any of the relevant lines where this
// is set up in K37RunAction::BeginOfRunAction(...)

#ifndef INCLUDE_K37ANALYSISNUMBERING_H_
#define INCLUDE_K37ANALYSISNUMBERING_H_

// Numbering of histograms
#define hist_ADA_Scintillator 0
#define hist_ADA_StripDetector 1
#define hist_ODA_Scintillator 2
#define hist_ODA_StripDetector 3
#define hist_e_angle_to_pol_generated 4
#define hist_e_energy_generated 5
#define hist_v_over_c_generated 6

// Numbering of ntuples is independent of histogram numbering
#define ntup_sign_z_hit 0
#define ntup_v_over_c 1
#define ntup_v_over_c_generated 2
#define ntup_theta_e_generated 3
#define ntup_SD_plusZ_X_start 4
#define ntup_SD_plusZ_Y_start 44
#define ntup_SD_minsZ_X_start 84
#define ntup_SD_minsZ_Y_start 124
#define ntup_accepted 164
#endif  // INCLUDE_K37ANALYSISNUMBERING_H_
