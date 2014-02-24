// ROOT Macro
{
  TFile *f = new TFile("K37.root");
  TH1D *h = (TH1D*) f->Get("histo/Electron_angle_to_pol_generated");
  TF1 *myfit = new TF1("myfit", "[0]*(1+[1]*x)", -0.8, 0.8);
  h->Fit("myfit");
  double A_beta = myfit -> GetParameter(1);
  double dA_beta = myfit -> GetParError(1);
  TH1D *k = (TH1D*) f->Get("histo/v_over_c");
  double mean_v_over_c = k->GetMean();
  A_beta /= mean_v_over_c;
  dA_beta /= mean_v_over_c;
  cout << "Mean v/c = " << mean_v_over_c << endl;
  cout << "A_beta-out = " << A_beta << " +/- " << dA_beta << endl;

}
