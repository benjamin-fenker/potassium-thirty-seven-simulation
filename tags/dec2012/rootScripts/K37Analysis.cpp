{
  Double_t A_beta_in = -0.573938;
  TFile *f = new TFile("K37.root");
  histo -> cd();
  TH1D *plusZScintillator = (TH1D *)f.Get("histo/ADA Detector Scintillator");
  TH1D *minsZScintillator = (TH1D *)f.Get("histo/ODA Detector Scintillator");
  TH1D *v_over_c_hist = (TH1D*) f->Get("histo/v_over_c");

  Double_t plusZHits = plusZScintillator -> GetEntries();
  Double_t d_plusZHits = sqrt(plusZHits);

  Double_t minsZHits = minsZScintillator -> GetEntries();
  Double_t d_minsZHits = sqrt(minsZHits);

  Double_t asym = (plusZHits - minsZHits) / (plusZHits + minsZHits);
  Double_t d_asym_p = 2*minsZHits*d_plusZHits / pow(plusZHits + minsZHits, 2);
  Double_t d_asym_m = 2*plusZHits*d_minsZHits / pow(minsZHits + plusZHits, 2);
  Double_t d_asym = sqrt(pow(d_asym_p, 2) + pow(d_asym_m, 2));

  Double_t mean_v_over_c = v_over_c_hist -> GetMean();
  asym /= mean_v_over_c;
  d_asym /= mean_v_over_c;

  Double_t max_A_beta = asym + d_asym;
  Double_t min_A_beta = asym - d_asym;
  printf("The beta-asymmetry is: %8.6G +/- %8.6G\n", asym, d_asym);
  if (max_A_beta >= A_beta_in && min_A_beta <= A_beta_in) {
    cout << "IN MARGIN OF ERROR!" << endl;
  } else {
    cout << "NOT IN MARGIN OF ERROR" << endl;
  }

  cout << "*******USING THE NTUPLE NOW************" << endl;
  TTree *ntuple = (TTree*)f->Get("ntuple/K37");
  Int_t nbytes = 0;
  Int_t num_events = ntuple->GetEntries();
  Double_t num = 0.0;
  Double_t dnum = 0.0;
  Double_t denom = 0.0;
  Double_t ddenom = 0.0;

  // Detector data
  Double_t stripWidth = 1.0;    // mm
  Int_t numStrips = 40;
  Double_t stripNumCenter = (static_cast<double>(numStrips)/2.0) - 0.5;
  Double_t distanceToTrap = 98.5; // mm

  Int_t detector_hit;
  Double_t v_over_c;
  ntuple->SetBranchAddress("detector_hit", &detector_hit);
  ntuple->SetBranchAddress("v_over_c", &v_over_c);
  Double_t  SD_plusZ_X[40], SD_plusZ_Y[40], SD_minsZ_X[40], SD_minsZ_Y[40];
  char branch[10];
  for (int i = 1; i <= 40; i++) {
    sprintf(branch, "SDplusZX%02d", i);
    ntuple->SetBranchAddress(branch, &SD_plusZ_X[i-1]);
  }
  for (int i = 1; i <= 40; i++) {
    sprintf(branch, "SDplusZY%02d", i);
    ntuple->SetBranchAddress(branch, &SD_plusZ_Y[i-1]);
  }
  for (int i = 1; i <= 40; i++) {
    sprintf(branch, "SDminsZX%02d", i);
    ntuple->SetBranchAddress(branch, &SD_minsZ_X[i-1]);
  }
  for (int i = 1; i <= 40; i++) {
    sprintf(branch, "SDminsZY%02d", i);
    ntuple->SetBranchAddress(branch, &SD_minsZ_Y[i-1]);
  }
  cout << "sign-Z\t v/c \ plusZ-x \ plusZ-y \ minsZ-x / minsZ-y" << endl;
  for (Int_t ev = 0; ev < 20; ev++) {
    nbytes += ntuple->GetEvent(ev);

    if (false) {
      cout << detector_hit << "\t " << v_over_c << endl;
      for (Int_t i = 0; i < 40; i++) cout << SD_plusZ_X[i] << "  ";
      cout << "\n";
      for (Int_t i = 0; i < 40; i++) cout << SD_plusZ_Y[i] << "  ";
      cout << "\n";
      for (Int_t i = 0; i < 40; i++) cout << SD_minsZ_X[i] << "  ";
      cout << "\n";
      for (Int_t i = 0; i < 40; i++) cout << SD_minsZ_Y[i] << "  ";
      cout << "\n";
    }
    Double_t x_coord = 0.0;
    Double_t y_coord = 0.0;
    Double_t x_max_E = 0.0;
    Double_t x_max_E_strip = -1;
    Double_t y_max_E = 0.0;
    Double_t y_max_E_strip = -1;
    if (detector_hit > 0) {     // Scan through the plus Z detectors
      for (Int_t i = 0; i < 40; i++) {
        if (SD_plusZ_X[i] > x_max_E) {
          x_max_E = SD_plusZ_X[i];
          x_max_E_strip = i;
        }
        if (SD_plusZ_Y[i] > y_max_E) {
          y_max_E = SD_plusZ_Y[i];
          y_max_E_strip = i;
        }
      }
    }
    if (detector_hit < 0) {
      for (Int_t i = 0; i < 40; i++) {
        if (SD_minsZ_X[i] > x_max_E) {
          x_max_E = SD_minsZ_X[i];
          x_max_E_strip = i;
        }
        if (SD_minsZ_Y[i] > y_max_E) {
          y_max_E = SD_minsZ_Y[i];
          y_max_E_strip = i;
        }
      }
    }
    x_coord = stripWidth/2.0;   // Moves to the center of each pixel
    x_coord += (floor(fabs(x_max_E_strip - stripNumCenter)))*stripWidth; 
    y_coord = stripWidth/2.0;   // Moves to the center of each pixel
    y_coord += (floor(fabs(y_max_E_strip - stripNumCenter)))*stripWidth; 
    if (false) {
      cout << "X-strip: " << x_max_E_strip << "\t y-coord: ";
      cout << y_max_E_strip << endl;
      cout << "X-coord: " << x_coord << "\t y-coord: " << y_coord;
    }
    Double_t z, cos_theta, dcos_theta;
    // Total distance from center of detector
    z = sqrt(pow(x_coord, 2.0) + pow(y_coord, 2.0));

    cos_theta = distanceToTrap;
    cos_theta /= sqrt(pow(distanceToTrap, 2.0) + pow(z, 2.0));

    dcos_theta = distanceToTrap * z * stripWidth / 2.0;
    dcos_theta /= pow(pow(distanceToTrap, 2.0) + pow(z, 2.0), 1.5);
    // The 0.97 comes from the cos(theta) = 0.97 +/- 0.03 in the actual geometry

    num += detector_hit*v_over_c*cos_theta;
    dnum += dcos_theta*dcos_theta;
    denom += v_over_c;
    //denom += 1.0;
    //    printf("detector_hit,v_over_c = %d, %f\n", detector_hit, v_over_c);
  }
  Double_t dnum = sqrt(dnum);
  Double_t A_beta = num/denom;
  cout << "A_beta =? " << A_beta << " +/- " << dnum << endl;
}
