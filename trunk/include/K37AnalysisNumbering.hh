// Authors: Spencer Behling and Benjamin Fenker 2013

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
// Event generator ntuples
#define ntup_electron_kinetic_energy_gen 0
#define ntup_v_over_c_generated 1
#define ntup_theta_e_generated 2
#define ntup_omega 3

// Scintillator ntuples
#define ntup_sign_z_hit 4
#define ntup_v_over_c 5

// Strip detector ntuples
#define ntup_SD_plusZ_X_start 6
#define ntup_SD_plusZ_Y_start 46
#define ntup_SD_minsZ_X_start 86
#define ntup_SD_minsZ_Y_start 126

// Other ntuples
#define ntup_accepted 166
#define ntup_tdc_scint_top 167
#define ntup_tdc_scint_bottom 168
#define ntup_qdc_scint_top 169
#define ntup_qdc_scint_bottom 170
#define ntup_rmcp_x_pos 171
#define ntup_rmcp_z_pos 172
#define ntup_rmcp_time 173
#define ntup_emcp_time 174
#endif  // INCLUDE_K37ANALYSISNUMBERING_H_
