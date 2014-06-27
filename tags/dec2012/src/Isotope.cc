#include "Isotope.hh"
#include "SplitString.hh"
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <cassert>
#include <fstream>
#include <cmath>

using std::cerr; using std::endl; using std::cout;
using std::left; using std::setw;

namespace ISO
{
   class values
   {
      public:
      values(const double &value_, const double &uncertainty_, 
      const std::string &message_)
      :value(value_), uncertainty(uncertainty_), message(message_)
      {};
      double GetValue()const{return value;};
      double GetUncertainty()const{return uncertainty;};
      void SetUncertainty(const double &uncertainty_)
      {uncertainty = uncertainty_;};
      void SetValue(const double &value_)
      {value = value_;};
      void Print()
      {
         std::cout<<std::setw(13)<<value
         <<std::setw(12)<<uncertainty
         <<message<<std::endl;
      };
      private:
      double value;
      double uncertainty;
      std::string message;
   };

   class Isotope::ImplementI
   {
      public:
         ImplementI(const std::string &filename_);
         ~ImplementI();
         void Print();
         double FindValue(const std::string &key_) const;
         void SetValue(const std::string &key_, const double &value_);
         double FindUncertainty(const std::string &key_) const;
         double PartialHalfLife()const;
         double U_PartialHalfLife()const;
         void AddPartialHalflife();
         double ComparativeHalflife()const;
         double U_ComparativeHalflife()const;
         void AddComparativeHalflife();
         double CorrectedComparativeHalflife()const;
         double U_CorrectedComparativeHalflife()const;
         void AddCorrectedComparativeHalflife();
         double U_AxialOverVector()const;
         void Add_U_AxialOverVector();
         double Rho()const;
         double U_Rho()const;
         void AddRho();
         void AddIsospin();
         double WeakMagnetism()const;
         double U_WeakMagnetism()const;
         void AddWeakMagnetism();
         void FillMap();
         void FillStaticValues();
         void AddInitalMass();
         void AddFinalMass();
         void AddDeltaM();
         void AddAverageMass();
         void AddTotalEndpointEnergy();
         void AddHalfEndpoint();
         void DivideBy_hbarC2();
         void AddRecoilCorrection();
         void Add_a1();
         void Add_Delta_a1();
         void Add_Delta_c1();
         void Add_a2();
         void Add_b();
         void Add_c1();
         void Add_c2();
         void Add_dI();
         void Add_dII();
         void Add_eI();
         void Add_eII();
         void Add_f();
         void Add_g();
         void Add_h();
         void Add_j2();
         void Add_j3();
         void AddHolstein_g();
         double R_nucleus(const double &A_nucleus)const;
         void Add_X_coulomb();
         void Add_Y_coulomb();
         void AddTotalNucleons();
         //void Add_a2();
         double sq(const double &value_)const; //square a value
         //double h1(const double &T_)const;
         //double h2(const double &T_)const;
         //double h4PlusOneThirdh7(const double &T_)const;
         //double h6PlusOneThirdh5(const double &T_)const;
         double f_1(const double &T_)const;
         double df_1_coulomb(const double &T_)const;
         double f_2(const double &T_) const;
         double df_2_coulomb(const double &T_) const;
         double f_4(const double &T_) const;
         double df_4_coulomb(const double &T_) const;
         double f_5(const double &T_) const;
         double f_6(const double &T_) const;
         double df_6_coulomb(const double &T_) const;
         double f_7(const double &T_) const;
         double df_7_coulomb(const double &T_) const;
         double f_10(const double &T_) const;
         double f_12(const double &T_) const;
         double df_12_coulomb(const double &T_) const;
         double c_align(const double &T_) const;
         double gamma_uv () const;
         double lambda_uv (const double &u)const;
         double theta_uv ()const;
         double kappa_uv(const double &u)const;
         double epsilon_uv (const double &u)const;
         std::string filename;
         std::string isotopeName;
         std::map<std::string, values * > theInputs;
         double delta;
         double alpha;
         double a1;
         double D_a1;
         double a1_tot;
         double a2;
         double b;
         double e;
         double eI;
         double eII;
         double f;
         double c1;
         double D_c1;
         double c1_tot;
         double c1_smll;
         double c2;
         double d;
         double dI;
         double dII;
         double g;
         double h;
         double j2;
         double j3;
         double X;
         double Y;
         double Z_d;
         double sign;
         double A;     //number of nucleons
         double E0;    //Q_electron capture
         double M;     //average mass parent and daughter
         double me;    //mass of the electron
         double me2;   //mass of the electron squared
         double J;     //spin of the parent
         bool debug;   //turn on debuging print out 
         bool radcorr; //turn on the columb corrections 
   };

   Isotope::ImplementI::ImplementI(const std::string &filename_)
      :filename(filename_),
      delta(0),
      alpha(0),
      a1(0),
      D_a1(0),
      a1_tot(0),
      a2(0),
      b(0),
      e(0),
      eI(0),
      eII(0),
      f(0),
      c1(0),
      D_c1(0),
      c1_tot(0),
      c1_smll(0),
      c2(0),
      d(0),
      dI(0),
      dII(0),
      g(0),
      h(0),
      j2(0),
      j3(0),
      X(0),
      Y(0),
      Z_d(0),
      sign(0),
      A(0),
      E0(0),
      M(0),
      me(0),
      me2(0),
      J(0),
      debug(false),
      radcorr(true)
   {
      this->FillMap();
      this->AddTotalNucleons();
      this->AddPartialHalflife();
      this->AddComparativeHalflife();
      this->AddCorrectedComparativeHalflife();
      this->Add_U_AxialOverVector();
      this->AddRho();
      this->AddIsospin();
      this->AddWeakMagnetism();
      this->AddInitalMass();
      this->AddFinalMass();
      this->AddDeltaM();
      this->AddAverageMass();
      this->AddTotalEndpointEnergy();
      this->AddHalfEndpoint();
      this->DivideBy_hbarC2();
      this->AddRecoilCorrection();
      this->Add_a1();
      this->Add_Delta_a1();
      this->Add_Delta_c1();
      this->Add_a2();
      this->Add_b();
      this->Add_c1();
      this->Add_c2();
      this->Add_dI();
      this->Add_dII();
      this->Add_eI();
      this->Add_eII();
      this->Add_f();
      this->Add_g();
      this->Add_h();
      this->Add_j2();
      this->Add_j3();
      this->AddHolstein_g();
      this->Add_X_coulomb();
      this->Add_Y_coulomb();
      this->FillStaticValues();

      //std::ios::fmtflags f( std::cout.flags() );
      //std::cout<<"Isotope Name: "<<isotopeName<<std::endl;
      //for(auto i : theInputs)
      //{
         //std::cout<<std::left<<setw(32)<<i.first;
         //i.second->Print();
      //}
      //std::cout<<"=========================================="<<std::endl;
      //std::cout.flags( f );
   }

   Isotope::ImplementI::~ImplementI()
   {
      for(auto i : theInputs)
      {
         delete i.second;
      }
      theInputs.clear();
   }


   double Isotope::ImplementI::f_1(const double &T_) const
   {
      // this is equal to \epsilon from JTW in limit of no corrections
      double E = T_ + me;
      if (debug)
      {
         printf("a1_tot = %9.7f, a2 = %9.7f, me2 = %9.7f, E = %9.7f, E0 = %9.7f, M = %9.7f\n",a1_tot,a2,me2,E,E0,M);

      }
      double tmp1 = sq(a1_tot) + 2.*a1_tot*a2*(me2+4.*E*E0+2.*me2*E0/E-4.*E*E)/3./M/M;
      double tmp2 = sq(c1_tot) + 2.*c1_tot*c2*(11.*me2+20.*E*(E0-E)
            -2.*me2*E0/E)/9./M/M - 2.*E0*c1_tot*(c1_smll+d+sign*b)/3./M
            + 2.*E*(3.*sq(a1_tot) + c1_tot*(5.*c1_smll+sign*2.*b))/3./M;
      double tmp3 = me2*( 2.*sq(c1_tot)+c1_tot*(d+sign*2.*b)
            -3.*a1_tot*e - c1_tot*h*(E0-E)/2./M)/3./M/E;
      double tmp = tmp1 + tmp2 - tmp3;
      if (debug)
      {
         printf("tmp1 = %12.10f, tmp2 = %12.10f, tmp3 = %12.10f\n",tmp1,tmp2,tmp3);
      }

      if (radcorr)
      {
         if (debug)
         {
            printf("   a1_tot = %9.5f, A_nucleus = %9.5f\n",a1_tot, A);
            //printf("   df_1   = %9.5f\n",df_1_coulomb(i, E0, E));
         }
         tmp = tmp + df_1_coulomb(T_);
      }
      return tmp;
   }

   double Isotope::ImplementI::df_1_coulomb(const double &T_) const
   {
      double E = T_ + me;

      return sign*(-8.)*alpha*Z_d/3./M_PI*
         ( sq(a1_tot)*(4.*E*(X+Y) + E0*X + me2*(X+2.*Y)/E ) +
           sq(c1_tot)*(4.*E*(4.*X/3.+Y) - E0*X/3. + me2*(X+2.*Y)/E ) 
         );
   }

   double Isotope::ImplementI::f_2(const double &T_) const
   {
      // this is the beta-neutrino correlation parameter
      double E = T_ + me;

      double tmp = a1_tot*(a1_tot + 2.*a2*me2/M/M) - c1_tot*c1_tot/3.
         -2.*c1_tot*c2*(me2 + 8.*E*(E0-E))/3./sq(M)
         + 2.*E0*c1_tot*(c1_smll+d+sign*b)/3./M
         - 4.*E*c1_tot*(3.*c1_smll+sign*b)/3./M;

      if (radcorr)
      {
         tmp = tmp + df_2_coulomb(T_);
      }
      return tmp;
   }

   double Isotope::ImplementI::df_2_coulomb(const double &T_) const
   {
      double E = T_ + me;
      return sign*(-8.)*alpha*Z_d/3./M_PI
         *( sq(a1_tot)*(4.*E*(X+Y) + E0*X )
         - sq(c1_tot)*(4.*E*(2.*X+Y)/3. - E0*X ));
   }

   double Isotope::ImplementI::f_4(const double &T_) const
   {
      // this is the dominant part of the beta asymmetry parameter
      double E = T_ + me;
      double tmp  =0;
      double tmp1 =0;
      double tmp2 =0;
      double tmp3 =0;
      tmp1 = sqrt(J/(J+1.))*(a1_tot*(2.*c1_tot-E0*(c1_tot+d+sign*b)/M
               + E*(7.*c1_tot+d+sign*b)/M + me2*h/2./M/M)
               + 2.*(a1_tot*c2+c1_tot*a2)*(me2+2.*E*(E0-E))/M/M);

      tmp2 = gamma_uv()*c1_tot*( c1_tot + 2.*c2*(me2+2.*E*E0-2.*E*E)/M/M
            - E0*(c1_tot+d-b)/2./M + E*(5.*c1_tot-d-3.*b)/2./M)/(J+1.);

      tmp3 = lambda_uv(J)*c1_tot*(sqrt(3./2.)*g*(E*(2.*E-5.*E0)+E0*E0+2.*me2)/2./M/M
            - f*(E0+9.*E)/2./M - 3.*j2*(4.*E*E-E*E0-E0*E0-2.*me2)/4./M/M)/(J+1.);

      if (debug > 0)
      {
         printf("tmp1 = %12.10f, tmp2 = %12.10f, tmp3 = %12.10f\n",tmp1,tmp2,tmp3);
      }
      if (debug > 0) 
      {
         printf("   \033[93m gamma-term from f_4 = %+g\033[m\n", tmp2);
         printf("   \033[93mlambda-term from f_4 = %+g\033[m\n", tmp3);
      }
      tmp = tmp1 - sign*tmp2 + tmp3;
      if (radcorr)
      {
         tmp = tmp + df_4_coulomb(T_);
      }
      return tmp;
   }
   
   double Isotope::ImplementI::df_4_coulomb(const double &T_) const
   {
      double E = T_ + me;
      double tmp1 =0;
      double tmp2 =0;
      double tmp3 =0;
      tmp1 = sign*(-8.)*alpha*Z_d*E*(5.*X+4.*Y)/3./M_PI;
      tmp2 = sqrt(J/(J+1.))*2.*a1_tot*c1_tot;
      tmp3 = sign*gamma_uv()*sq(c1_tot)/(J+1.);
      if (debug > 0)
      {
         printf("   \033[93m Coulomb correction to f_4 = %+g\033[m\n",
               tmp1*(tmp2-tmp3));
      }
      return tmp1*(tmp2-tmp3);
   }

   double Isotope::ImplementI::f_5(const double &T_) const
   {
      double E = T_ + me;
      double tmp  =0;
      double tmp1 =0;
      double tmp2 =0;
      double tmp3 =0;
      tmp1 = -sqrt(J/(J+1.))*E*(a1_tot*(5.*c1_tot-d-b)
            + 4.*(a1_tot*c2+c1_tot*a2)*(E0-E)/M)/M;
      tmp2 = -gamma_uv()*E*c1_tot*(5.*c1_tot+8.*c2*(E0-E)/M-d-b)/2./M/(J+1.);
      tmp3 = lambda_uv(J)*E*( 3.*f - sqrt(3./2.)*g*(E0+2.*E)/M
            - 3.*j2*(E0-4.*E)/2./M)/2./M/(J+1.);
      tmp = tmp1 + tmp2 + tmp3*c1_tot;
      if (debug>0)
      {
         printf("      \033[93mlambda-term/3 from f_5 = %+g\033[m\n", tmp3/3.);
      }
      return tmp;
   }

   double Isotope::ImplementI::f_6(const double &T_) const
   {
      double E = T_ + me;
      double tmp  =0;
      double tmp1 =0;
      double tmp2 =0;
      double tmp3 =0;
      //if (debug>0) printf("\033[95mc1=%g=-%g, f=%g, g=%g, j2=%g, M=%g, E=%g, E0=%g\n\033[m", c1,i.h.c1,f,g,j2,M,E,E0);
      tmp1 = sqrt(J/(J+1.))*(a1_tot*(2.*c1_tot+E*(5.*c1_tot-d-b)/M
            -me2*(c1_tot-b+e-h*(E0-E)/2./M)/M/E)
            +2.*(a1_tot*c2+c1_tot*a2)*(me2+2.*E*E0-2.*E*E)/M/M);
      tmp2 = gamma_uv()*c1_tot*(c1_tot+2.*c2*(me2+2.*E*E0-2.*E*E)/M/M
            -E0*(c1_tot+d+sign*b)/M
            +E*(7.*c1_tot+d+sign*3.*b)/M
            -me2*(c1_tot+d+sign*b)/2./M/E)/(J+1.);
      tmp3 = lambda_uv(J)*c1_tot*(f*(9.*E-10.*E0+me2/E)/2./M
            +sqrt(3./2.)*g*(2.*E*E+E*E0-2.*E0*E0-4.*me2
            +3.*E0*me2/E)/2./M/M
            -3.*j2*(E0*(7.*E-2.*E0)-4.*E*E+2.*me2
            -3.*E0*me2/E)/4./M/M)/(J+1.);

      if (debug>0)
      {
         printf("tmp1 = %12.10f, tmp2 = %12.10f, tmp3 = %12.10f\n",tmp1,tmp2,tmp3);
      }
      tmp = tmp1 + sign*tmp2 + tmp3;
      if (debug>0)
      {
         printf("       \033[93m gamma-term  from f_6 = %+g\033[m\n", tmp2);
         printf("       \033[93mlambda-term  from f_6 = %+g\033[m\n", tmp3);
      }
      if (radcorr)
      {
         tmp = tmp + df_6_coulomb(T_);
      }
      return tmp;
   }

   double Isotope::ImplementI::df_6_coulomb(const double &T_) const
   {
      double E = T_ + me;
      double tmp1 =0;
      double tmp2 =0;
      double tmp3 =0;
      tmp1 = sign*(-8.)*alpha*Z_d/3./M_PI;
      tmp2 = sqrt(J/(J+1.))*2.*a1_tot*c1_tot*( 4.*E*(X+Y) + E0*X
            + me2*(X+2.*Y)/E );
      tmp3 = sign*gamma_uv()*sq(c1_tot)*( E*(6.*X+4.*Y) - E0*X
            + me2*(X+2.*Y)/E )/(J+1.);
      if (debug > 0)
      {
         printf("   \033[93m Coulomb correction to f_6 = %+g\033[m\n",
               tmp1*(tmp2+tmp3));
      }
      return tmp1*(tmp2+tmp3);
   }

   double Isotope::ImplementI::f_7(const double &T_) const
   {
      double E = T_ + me;
      double tmp  =0;
      double tmp1 =0;
      double tmp2 =0;
      double tmp3 =0;
      tmp1 = sqrt(J/(J+1.))*(  a1_tot*( E0*(c1_tot+d-b) + E*(-7.*c1_tot-d+b) )/M
            - 4.*(a1_tot*c2+c1_tot*a2)*E*(E0-E)/M/M);
      tmp2 = (-1.)*gamma_uv()*( + E0*(c1_tot+d-b)/2./M
            - E*(7.*c1_tot+d-b)/2./M
            - 4.*c2*E*(E0-E)/M/M)/(J+1.);
      tmp3 = lambda_uv(J)*( 3.*f*(E0-E)/2./M
            + sqrt(3./2.)*g*(2.*E0*E-E0*E0-E*E)/M/M
            - 3.*j2*(E0*(3.*E0-7.*E)/4.+E*E)/M/M)/(J+1.);
      tmp = tmp1 + tmp2*c1_tot + tmp3*c1_tot;
      if (debug > 0)
      {
         printf("   \033[93m gamma-term/3 from f_7 = %+g\033[m\n", tmp2/3.);
         printf("   \033[93mlambda-term/3 from f_7 = %+g\033[m\n", tmp3/3.);
      }
      if (radcorr)
      {
         tmp = tmp + df_7_coulomb(T_);
      }
      return tmp;
   }

   double Isotope::ImplementI::df_7_coulomb(const double &T_) const
   {
      double E = T_ + me;
      double tmp1 =0;
      double tmp2 =0;
      double tmp3 =0;
      tmp1 = sign*(-8.)*alpha*Z_d/3./M_PI*(E0-E)*X;
      tmp2 = sqrt(J/(J+1.))*2.*a1_tot*c1_tot;
      tmp3 = sign*gamma_uv()*sq(c1_tot)/(J+1.);
      //printf("radcorr = %s\n",radcorr);
      if (debug > 0)
      {
         printf("   \033[93m Coulomb correction to f_7 = %+g\033[m\n", 
               tmp1*(tmp2+tmp3));
      }
      return tmp1*(tmp2+tmp3);
   }

   double Isotope::ImplementI::f_10(const double &T_) const
   {
      double E = T_ + me;
      double tmp  =0;
      double tmp1 =0;
      double tmp2 =0;
      double tmp3 =0;
      double tmp4 =0;
      tmp1 = -theta_uv()*E*c1_tot*(c1_tot+d-b)/2./M;
      tmp2 = -sqrt(J*(J+1.)/((2.*J-1.)*(2.*J+3.)))*E
         *a1_tot*(sqrt(3./2.)*f+3.*g*E/4./M+sqrt(3./2.)*j2*(E0-E)/2./M)/M;
      tmp3 = kappa_uv(J)*E*c1_tot*((-3.)*f-sqrt(3./2.)*g*(E0-E)/M
            +3.*j2*(E0-2.*E)/2./M)/2./M;
      tmp4 = epsilon_uv(J)*3.*c1_tot*j3*(2.*E*E0+5.*E*E)/8./M/M;
      tmp = tmp1 + tmp2 + tmp3 + tmp4;
      return tmp;
   }

   double Isotope::ImplementI::f_12(const double &T_) const
   {
      double E = T_ + me;
      double tmp  =0;
      double tmp1 =0;
      double tmp2 =0;
      double tmp3 =0;
      double tmp4 =0;
      if (J>=1.5)
      {
         tmp1 = -theta_uv()*c1_tot*(c1_tot+2.*c2*(me2+2.*E*E0-2.*E*E)/M/M
               - E0*(c1_tot+d-b)/2./M+E*(3.*c1_tot-b)/M	
               - me2*h*(1.+delta/2./M)/2./M/M);
         tmp2 = -sqrt(J*(J+1.)/((2.*J-1.)*(2.*J+3.)))*a1_tot
                *(sqrt(3./2.)*f*E0/M+3.*g*(E*E0-E*E)/2./M/M
                      -sqrt(3./2.)*j2*(E0*E0-2.*E*E0+me2)/2./M/M);
         tmp3 = kappa_uv(J)*c1_tot*((-3.)*f*(E0-2.*E)/2./M
               - sqrt(3./2.)*g*(E0*E0-2.*E*E0+me2)/2./M/M
               - j2*(8.*E*E0-E0*E0-8.*E*E+me2)/4./M/M);
         tmp4 = epsilon_uv(J)*c1_tot*j3*(E0*E0+9.*E*E0-me2-9.*E*E)/4./M/M;
         tmp = tmp1 + tmp2 + tmp3 + tmp4;
         if (debug>0) 
         {
            printf("tmp1 = %12.10f, tmp2 = %12.10f, tmp3 = %12.10f, tmp4 = %12.10f\n",tmp1,tmp2,tmp3,tmp4);
         }
      }
      else
      {
         tmp = 0.;
      }
      if (radcorr)
      {
         tmp = tmp + df_12_coulomb(T_);
      }
      return tmp;
   }

   double Isotope::ImplementI::c_align(const double &T_) const
   {
      return (-f_12(T_)*(2.0*J-1.0))/(f_1(T_)*(J+1.0));
   }

   double Isotope::ImplementI::df_12_coulomb(const double &T_) const
   {
      double E = T_ + me;
      double tmp1 =0;
      double tmp2 =0;
      tmp1 = sign*(-8.)*alpha*Z_d/3./M_PI;
      tmp2 = theta_uv()*sq(c1_tot)*E*(5.*X+4.*Y);
      if (debug > 0)
      {
         printf("   \033[93m Coulomb correction to f_12 = %+g\033[m\n", tmp1*tmp2);
      }
      return tmp1*tmp2;
   }

   // first, \gamma_{u,v} is correct; simple for case u = v:
   double Isotope::ImplementI::gamma_uv () const
   { return 1.; }

   // but the expression for lambda_{u,v} is wrong; needs to be divided by 
   // -5\sqrt{6} (see ~/latex/holstein/spin-fxns.tex
   double Isotope::ImplementI::lambda_uv (const double &u)const // valid for u=v
   { return (-1.)*sqrt((2.*u-1.)*(2.*u+3.)/6.)/5.; }

   // expression for \theta is correct
   double Isotope::ImplementI::theta_uv ()const
   { return 1.; }

   // expression for \kappa is correct
   double Isotope::ImplementI::kappa_uv(const double &u)const
   { return sqrt(3./(2.*(2.*u-1.)*(2.*u+3.))); }

   // also, the expression for epsilon_{u,v} is wrong
   double Isotope::ImplementI::epsilon_uv (const double &u)const // also for case u=v
   { return (-2.)*sqrt(3.*(2.*u-2.)*(2.*u+4.)/(70.*2.*(2.*u-1.)*(2.*u+3.))); }


   double Isotope::ImplementI::R_nucleus(const double &A_nucleus)const
   { 
      double hbarc = FindValue("HBARC");
      double R = 0;
      if (A_nucleus == 37.)
      {
         R = 4.637/hbarc; // units are MeV^-1
      }
      else
      {
         R = 1.2*std::pow(A_nucleus,(1.0/3.0))/hbarc;  //nuclear radius
      }
      return R;
   }

   void Isotope::ImplementI::Add_X_coulomb()
   { 
      double A_nucleus = FindValue("TOTAL_NUCLEONS");
      double temp = 9.0*M_PI*R_nucleus(A_nucleus)/140.0;
      theInputs["X_COULOMB"] = new values(temp, 0, "!calculated!");
   }

   void Isotope::ImplementI::Add_Y_coulomb()
   { 
      double A_nucleus = FindValue("TOTAL_NUCLEONS");
      double temp = 9.0*M_PI*R_nucleus(A_nucleus)/140.0;
      theInputs["Y_COULOMB"] = new values(temp, 0, "!calculated!");
   }


   double Isotope::ImplementI::sq(const double &value_) const
   {
      return std::pow(value_,2);
   }

   //double Isotope::ImplementI::h1(const double &T_)const
   //{
      //double E_ = T_ + me;
      //double a2 = a*a;
      //double c2 = c*c;
      //double cb = c*b;
      //return a2 + c2 - (2.0*E0/(3.0*M))*(c2 + cb)
         //+(2.0*E_/(2.0*M)*(3.0*a2+5.0*c2+2.0*cb))
         //-(2.0*me*me/(3.0*E_*M))*(c2+cb);
   //}

   //double Isotope::ImplementI::h2(const double &T_)const
   //{
      //double E_ = T_ + me;
      //double a2 = a*a;
      //double c2 = c*c;
      //double cb = c*b;
      //return a2*c2/3.0 - (2.0*E0/(3.0*M))*(c2 + cb)
         //- (4.0*E_/(3.0*M))*(3.0*c2 + cb);
   //}

   //double Isotope::ImplementI::h4PlusOneThirdh7(const double &T_)const
   //{
      //double E_ = T_ + me;
      //double lam = sqrt(J/(J+1.0));
      //double la =(1.0/(J+1.0));
      //double ac = a*c;
      //double ab = a*b;
      //double c2 = c*c;
      //double cb = c*b;
      //return -lam*(2.0*ac - (2.0*E0/(3.0*M))*(ac+cb)
            //+(2.0*E_/(3.0*M))*(7.0*ac+ab))+la*(c2-(2.0*E0/(3.0*M))
            //*(c2+cb)+(E_/(3.0*M))*(11.0*c2+5.0*cb));
   //}

   //double Isotope::ImplementI::h6PlusOneThirdh5(const double &T_)const
   //{
      //double E_ = T_ + me;
      //double lam = sqrt(J/(J+1.0));
      //double la =(1.0/(J+1.0));
      //double ac = a*c;
      //double ab = a*b;
      //double c2 = c*c;
      //double cb = c*b;
      //return -lam*(2.0*ac+(E_/M)*(5.0*ac+ab)
            //+(me*me*E_/(3.0*E_*M*M))*(5.0*ac+ab)) - la*(c2 - (E0/M)*(c2+cb)
            //+(E_/(2.0*M))*(7.0*c2+3.0*cb)) -la*((E_/6.0*M)*(5.0*c2+cb));
   //}

   void Isotope::ImplementI::FillStaticValues()
   {
      //a = 1;
      //if(theInputs.find("WEAK_MAGNETISM_FROM_MOMENTS") != theInputs.end())
      //{
         //b = FindValue("WEAK_MAGNETISM_FROM_MOMENTS");
      //}
      //else
      //{
         //b = FindValue("WEAK_MAGNETISM");
      //}
      //c  = FindValue("RHO");

      delta   = FindValue("DELTA_MASS");
      alpha   = FindValue("ALPHA");
      a1      = FindValue("A1");
      D_a1    = FindValue("DELTA_A1");
      a2      = FindValue("A2");
      a1_tot  = a1+D_a1;
      b       = FindValue("WEAK_MAGNETISM_FROM_MOMENTS");
      eI      = FindValue("EI");
      eII     = FindValue("EII");
      e       = eI + eII;
      f       = FindValue("F");
      c1      = FindValue("C1");
      D_c1    = FindValue("DELTA_C1");
      c1_tot  = c1+D_c1;
      c1_smll = c1_tot;
      c2      = FindValue("C2");
      dI      = FindValue("DI");
      dII     = FindValue("DII");
      d       = dI + dII;
      g       = FindValue("HOLSTEIN_G");
      h       = FindValue("H");
      j2      = FindValue("J2");
      j3      = FindValue("J3");
      //a1_tot  = FindValue("TOTAL_ENDPOINT_ENERGY");
      //c1_tot  = FindValue("TOTAL_ENDPOINT_ENERGY");
      //c1_smll = FindValue("TOTAL_ENDPOINT_ENERGY");
      X       = FindValue("X_COULOMB");
      Y       = FindValue("Y_COULOMB");
      Z_d     = FindValue("NUMBER_OF_PROTONS_DAUGHTER");
      sign    = -FindValue("BETA_PLUS_MINUS");
      A       = FindValue("TOTAL_NUCLEONS");
      E0      = FindValue("TOTAL_ENDPOINT_ENERGY");
      M       = FindValue("AVERAGE_MASS");
      me      = FindValue("MASS_OF_ELECTRON");
      me2     = me*me;
      J       = FindValue("SPIN");
   }

   void Isotope::ImplementI::AddInitalMass()
   {
      double M = FindValue("MASS_OF_PARENT");
      double amu_to_mev = FindValue("AMU_TO_MEV");
      theInputs["INITIAL_MASS_MEV"] = new values(M*amu_to_mev, 0, "!calculated!");
   }

   void Isotope::ImplementI::AddFinalMass()
   {
      double M = FindValue("MASS_OF_DAUGHTER");
      double amu_to_mev = FindValue("AMU_TO_MEV");
      double me = FindValue("MASS_OF_ELECTRON");
      theInputs["FINAL_MASS_MEV"] = new values(M*amu_to_mev+me, 0, "!calculated!");
   }

   void Isotope::ImplementI::AddDeltaM()
   {
      double Mi   = FindValue("INITIAL_MASS_MEV");
      double Mf   = FindValue("FINAL_MASS_MEV");
      theInputs["DELTA_MASS"] = new values(Mi-Mf, 0, "!calculated!");
   }

   void Isotope::ImplementI::AddAverageMass()
   {
      double Mi   = FindValue("INITIAL_MASS_MEV");
      double Mf   = FindValue("FINAL_MASS_MEV");
      double amu_to_mev   = FindValue("AMU_TO_MEV");
      theInputs["AVERAGE_MASS"] = new values((Mi+Mf)/2.0, 0.00000012*amu_to_mev,
            "!calculated!");
   }

   void Isotope::ImplementI::DivideBy_hbarC2()
   {
      double hbarc  = FindValue("HBARC");
      double hbarc2 = hbarc*hbarc;
      double temp   = FindValue("M_R2");
      this->SetValue("M_R2", temp/hbarc2);
      temp   = FindValue("M_SR2");
      this->SetValue("M_SR2", temp/hbarc2);
      temp   = FindValue("M_Q");
      this->SetValue("M_Q", temp/hbarc2);
      temp   = FindValue("M_RDOTP");
      this->SetValue("M_RDOTP", temp/hbarc2);
      temp   = FindValue("M_1Y");
      this->SetValue("M_1Y", temp/hbarc2);
      temp   = FindValue("M_2Y");
      this->SetValue("M_2Y", temp/hbarc2);
      temp   = FindValue("M_3Y");
      this->SetValue("M_3Y", temp/hbarc2);
      temp   = FindValue("M_SRP");
      this->SetValue("M_SRP", temp/hbarc);
   }

   void Isotope::ImplementI::AddTotalEndpointEnergy()
   {
      if(theInputs.find("DELTA_MASS") != theInputs.end()
         && theInputs.find("AVERAGE_MASS") != theInputs.end())
      {
         double deltaM        = FindValue("DELTA_MASS");
         double aveM          = FindValue("AVERAGE_MASS");
         double massElectron2 = FindValue("MASS_OF_ELECTRON")
            *FindValue("MASS_OF_ELECTRON");

         double endpoint =
            deltaM*(1.0+massElectron2/(2.0*aveM*deltaM))
            /(1.0+deltaM/(2.0*aveM));

         if(theInputs.find("TOTAL_ENDPOINT_ENERGY")!= theInputs.end())
         {
            delete theInputs.find("TOTAL_ENDPOINT_ENERGY")->second;
            theInputs.find("TOTAL_ENDPOINT_ENERGY")->second = 
               new values(endpoint, 0, "!calculated!");
         }
         else
         {
            theInputs["TOTAL_ENDPOINT_ENERGY"] =
               new values(endpoint,0, "!calculated!");
         }
      }
   }

   void Isotope::ImplementI::AddRecoilCorrection()
   {
      double deltaM = FindValue("DELTA_MASS");
      double aveM   = FindValue("AVERAGE_MASS");
      double temp = (1.0+deltaM/(2.0*aveM));
      theInputs["RECOIL_CORRECTION"] =
         new values(temp,0, "!calculated!");
   }

   void Isotope::ImplementI::Add_a2()
   {
      double gV      = FindValue("GV");
      double rc      = FindValue("RECOIL_CORRECTION");
      double aveM    = FindValue("AVERAGE_MASS");
      double M_r2    = FindValue("M_R2");
      double temp    = gV*(M_r2/6.0)*sq(aveM)/rc;
      
      theInputs["A2"] = new values(temp,0, "!calculated!");
   }

   void Isotope::ImplementI::Add_c1()
   {
      double gA      = FindValue("GA");
      double rc      = FindValue("RECOIL_CORRECTION");
      double M_GT    = FindValue("M_GT");
      double temp = gA*M_GT/rc;
      
      theInputs["C1"] = new values(temp,0, "!calculated!");
   }

   void Isotope::ImplementI::Add_b()
   {
      double gV      = FindValue("GV");
      double gM      = FindValue("GM");
      double M_GT    = FindValue("M_GT");
      double M_L     = FindValue("M_L");
      double A       = FindValue("TOTAL_NUCLEONS");
      double temp = A*(gM*M_GT + gV*M_L);
      
      theInputs["B"] = new values(temp,0, "!calculated!");
   }

   void Isotope::ImplementI::AddTotalNucleons()
   {
      double Z = FindValue("NUMBER_OF_PROTONS");
      double N = FindValue("NUMBER_OF_NEUTRONS");
      theInputs["TOTAL_NUCLEONS"] = new values(Z+N,0, "!calculated!");
   }

   void Isotope::ImplementI::Add_dII()
   {
      double gII     = FindValue("GII");
      double A       = FindValue("TOTAL_NUCLEONS");
      double M_GT    = FindValue("M_GT");
      double temp = -gII*A*M_GT;
      
      theInputs["DII"] = new values(temp,0, "!calculated!");
   }

   void Isotope::ImplementI::Add_eII()
   {
      double gS      = FindValue("GS");
      double A       = FindValue("TOTAL_NUCLEONS");
      double M_F     = FindValue("M_F");
      double temp    = -gS*A*M_F;
      
      theInputs["EII"] = new values(temp,0, "!calculated!");
   }

   void Isotope::ImplementI::Add_eI()
   {
      double gV      = FindValue("GV");
      double M_F     = FindValue("M_F");
      double deltaM  = FindValue("DELTA_MASS");
      double M_r2    = FindValue("M_R2");
      double aveM    = FindValue("AVERAGE_MASS");
      double M_rdotp = FindValue("M_RDOTP");
      double rc      = FindValue("RECOIL_CORRECTION");
      double temp = gV*(M_F - (sq(deltaM)*M_r2/6.0) 
		      - (2.0*aveM*M_rdotp/3.0))/rc;
      
      theInputs["EI"] = new values(temp,0, "!calculated!");
   }

   void Isotope::ImplementI::AddHolstein_g()
   {
      double I     = FindValue("SPIN");
      double aveM  = FindValue("AVERAGE_MASS");
      double Q_p   = FindValue("PARENT_QUADRUPOLE_MOMENT");
      double Q_d   = FindValue("DAUGHTER_QUADRUPOLE_MOMENT");
      double hbarc = FindValue("HBARC");
      double dQ_p  = FindUncertainty("PARENT_QUADRUPOLE_MOMENT");
      double dQ_d  = FindUncertainty("DAUGHTER_QUADRUPOLE_MOMENT");

      if (I<=0.5)
      {
         // since I = I' = 0.5 [Eq.(12) of Holstein RMP 74]
         theInputs["HOLSTEIN_G"] = new values(0,0, "!calculated!");
      }
      else
      {
         double tempV  = -sqrt((I+1.0)*(2.0*I+3.0)/(I*(2.0*I-1.0)))*2.0
            *sq(aveM)*(Q_p-Q_d)/(hbarc*hbarc*3.0);
         double tempU = std::abs(tempV)*sqrt(sq(dQ_p/Q_p) + sq(dQ_d/Q_d));

         theInputs["HOLSTEIN_G"] = new values(tempV, tempU, "!calculated!");
      }
   }

   void Isotope::ImplementI::Add_j3()
   {
      double gA      = FindValue("GA");
      double aveM    = FindValue("AVERAGE_MASS");
      double M_3y    = FindValue("M_3Y");
      double temp = -gA*2.0*sq(aveM)*M_3y/3.0;
      
      theInputs["J3"] = new values(temp,0, "!calculated!");
   }

   void Isotope::ImplementI::Add_j2()
   {
      double gA      = FindValue("GA");
      double aveM    = FindValue("AVERAGE_MASS");
      double M_2y    = FindValue("M_2Y");
      double temp = -gA*2.0*sq(aveM)*M_2y/3.0;
      
      theInputs["J2"] = new values(temp,0, "!calculated!");
   }

   void Isotope::ImplementI::Add_h()
   {
      double gA      = FindValue("GA");
      double aveM    = FindValue("AVERAGE_MASS");
      double M_1y    = FindValue("M_1Y");
      double gP      = FindValue("GP");
      double M_GT    = FindValue("M_GT");
      double rc      = FindValue("RECOIL_CORRECTION");
      double temp = -(gA*2.0*sq(aveM)*M_1y/sqrt(10.) + gP*sq(A)*M_GT)/rc;
      
      theInputs["H"] = new values(temp,0, "!calculated!");
   }

   void Isotope::ImplementI::Add_g()
   {
      double gV      = FindValue("GV");
      double aveM    = FindValue("AVERAGE_MASS");
      double M_Q     = FindValue("M_Q");
      double temp = -gV*4.0*sq(aveM)*M_Q/3.0;
      
      theInputs["G"] = new values(temp,0, "!calculated!");
   }

   void Isotope::ImplementI::Add_f()
   {
      double gV      = FindValue("GV");
      double aveM    = FindValue("AVERAGE_MASS");
      double M_rp    = FindValue("M_RP");
      double temp = gV*2.0*aveM*M_rp;
      
      theInputs["F"] = new values(temp,0, "!calculated!");
   }

   void Isotope::ImplementI::Add_dI()
   {
      double gA      = FindValue("GA");
      double M_GT    = FindValue("M_GT");
      double deltaM  = FindValue("DELTA_MASS");
      double M_sr2   = FindValue("M_SR2");
      double M_1y    = FindValue("M_1Y");
      double aveM    = FindValue("AVERAGE_MASS");
      double A       = FindValue("TOTAL_NUCLEONS");
      double M_sL    = FindValue("M_SL");
      double M_srp   = FindValue("M_SRP");
      double rc      = FindValue("RECOIL_CORRECTION");
      double temp = gA*( -M_GT + sq(deltaM)*(M_sr2/6.0)
		   + M_1y*deltaM*(aveM+deltaM/6.0)/sqrt(10.)
		   + (A*M_sL) + (aveM*M_srp))/rc;
      
      theInputs["DI"] = new values(temp,0, "!calculated!");
   }

   void Isotope::ImplementI::Add_c2()
   {
      double gA      = FindValue("GA");
      double M_sr2   = FindValue("M_SR2");
      double M_1y    = FindValue("M_1Y");
      double aveM    = FindValue("AVERAGE_MASS");
      double rc      = FindValue("RECOIL_CORRECTION");
      double temp = gA*((M_sr2/6.0) + (M_1y/(6.0*sqrt(10.))))*(sq(aveM)/rc);
      
      theInputs["C2"] = new values(temp,0, "!calculated!");
   }

   void Isotope::ImplementI::Add_Delta_c1()
   {
      double gA      = FindValue("GA");
      double deltaM  = FindValue("DELTA_MASS");
      double M_sr2   = FindValue("M_SR2");
      double M_1y    = FindValue("M_1Y");
      double A       = FindValue("TOTAL_NUCLEONS");
      double M_sL    = FindValue("M_SL");
      double aveM    = FindValue("AVERAGE_MASS");
      double M_srp   = FindValue("M_SRP");
      double rc      = FindValue("RECOIL_CORRECTION");
      double temp = gA*( (-sq(deltaM)*M_sr2/6.0) + (2.0*sq(deltaM)*M_1y/(6.0*sqrt(10.)))
			 + (A*deltaM*M_sL/(2.0*aveM))
			 + (deltaM*M_srp/2.0))/rc;
      
      theInputs["DELTA_C1"] = new values(temp,0, "!calculated!");
   }

   void Isotope::ImplementI::Add_Delta_a1()
   {
      double gV      = FindValue("GV");
      double rc      = FindValue("RECOIL_CORRECTION");
      double deltaM  = FindValue("DELTA_MASS");
      double M_r2    = FindValue("M_R2");
      double M_rdotp = FindValue("M_RDOTP");
      double temp = gV*((-sq(deltaM)*M_r2/6.0) + (deltaM*M_rdotp/3.0))/rc;
      
      theInputs["DELTA_A1"] = new values(temp,0, "!calculated!");
   }

   void Isotope::ImplementI::Add_a1()
   {
      double gV   = FindValue("GV");
      double M_F  = FindValue("M_F");
      double rc   = FindValue("RECOIL_CORRECTION");
      double temp = gV*M_F/rc;
      
      theInputs["A1"] = new values(temp,0, "!calculated!");
   }

   void Isotope::ImplementI::AddHalfEndpoint()
   {
         double ep = FindValue("TOTAL_ENDPOINT_ENERGY");
            theInputs["HALF_TOTAL_ENDPOINT_ENERGY"] =
               new values(ep/2.0,0, "!calculated!");
   }

   void Isotope::ImplementI::AddIsospin()
   {
      double N = FindValue("NUMBER_OF_NEUTRONS");
      double Z = FindValue("NUMBER_OF_PROTONS");
      double beta_pm = FindValue("BETA_PLUS_MINUS");
      theInputs["ISOSPIN_T3"] = new values(std::abs((N-Z)/2.0), 0, "!calculated!");
      theInputs["DAUGHTER_ISOSPIN_T3"] = new values(
            std::abs((N-Z)/2.0) - beta_pm, 0, "!calculated!");
   }


   void Isotope::ImplementI::AddWeakMagnetism()
   {
      if(theInputs.find("PARENT_MAGNETIC_MOMENT") != theInputs.end()
         && theInputs.find("DAUGHTER_MAGNETIC_MOMENT") != theInputs.end())
      {
         theInputs["WEAK_MAGNETISM_FROM_MOMENTS"] =
            new values(WeakMagnetism(), U_WeakMagnetism(), "!calculated!");
      }
   }

   void Isotope::ImplementI::AddRho()
   {
      theInputs["RHO"] = new values(Rho(), U_Rho(), "!calculated!");
   }

   double Isotope::ImplementI::WeakMagnetism()const
   {
      double j    = FindValue("SPIN");
      double N    = FindValue("NUMBER_OF_NEUTRONS");
      double Z    = FindValue("NUMBER_OF_PROTONS");
      double T3_p = FindValue("ISOSPIN_T3");
      double T3_d = FindValue("DAUGHTER_ISOSPIN_T3");
      double mu_p = FindValue("PARENT_MAGNETIC_MOMENT");
      double mu_d = FindValue("DAUGHTER_MAGNETIC_MOMENT");
      double A    = N+Z;
      return A*sqrt((j+1.0)/j)*(mu_p-mu_d)/(T3_p-T3_d);
   }

   double Isotope::ImplementI::U_WeakMagnetism()const
   {
      double wm     = WeakMagnetism();
      double mu_p   = FindValue("PARENT_MAGNETIC_MOMENT");
      double mu_d   = FindValue("DAUGHTER_MAGNETIC_MOMENT");
      double u_mu_p = FindUncertainty("PARENT_MAGNETIC_MOMENT");
      double u_mu_d = FindUncertainty("DAUGHTER_MAGNETIC_MOMENT");
      return std::abs(wm)*sqrt((u_mu_p/mu_p)*(u_mu_p/mu_p)
            + (u_mu_d/mu_d)*(u_mu_d/mu_d));
   }

   double Isotope::ImplementI::Rho()const
   {
      double fafv    = FindValue("FA/FV");
      double FT      = FindValue("CORRECTED_COMPARATIVE_HALFLIFE");
      double FT_0T0  = FindValue("FT_0T0");
      double sign    = FindValue("SIGN_RHO");
      return sign*sqrt(((2.0*FT_0T0/FT) - 1.0)/fafv);
   }

   double Isotope::ImplementI::U_Rho()const
   {

      double fafv    = FindValue("FA/FV");
      double FT      = FindValue("CORRECTED_COMPARATIVE_HALFLIFE");
      double FT_0T0  = FindValue("FT_0T0");
      double ufafv   = FindUncertainty("FA/FV");
      double uFT     = FindUncertainty("CORRECTED_COMPARATIVE_HALFLIFE");
      double uFT_0T0 = FindUncertainty("FT_0T0");

      double temp = (0.25)*(1.0/((2.0*FT_0T0/(FT*fafv))-(1.0/fafv)));
      return sqrt(temp*((2.0/(FT*fafv))*(2.0/(FT*fafv))
               *(uFT_0T0*uFT_0T0)+(-2.0*FT_0T0/(FT*FT*fafv))
               *(-2.0*FT_0T0/(FT*FT*fafv))*(uFT*uFT)
               +((-2.0*FT_0T0/(FT*fafv*fafv))+(1.0/(fafv*fafv)))
               *((-2.0*FT_0T0/(FT*fafv*fafv))+(1.0/(fafv*fafv)))
               *(ufafv)*(ufafv)));
   }

   void Isotope::ImplementI::Add_U_AxialOverVector()
   {
      auto pointer = theInputs.find("FA/FV");
      if(pointer != theInputs.end())
      {
         pointer->second->SetUncertainty(U_AxialOverVector());
      }
   }

   double Isotope::ImplementI::U_AxialOverVector()const
   {
      //assign an error bar of 20% of its deviation from unity 
      //because we don't have any other measure of its uncertainty.

      double fafv = FindValue("FA/FV");
      return  std::abs((1.0 - fafv)*0.2);
   }

   void Isotope::ImplementI::AddCorrectedComparativeHalflife()
   {
      theInputs["CORRECTED_COMPARATIVE_HALFLIFE"] =
         new values(CorrectedComparativeHalflife(),
               U_CorrectedComparativeHalflife(), "!calculated!");
   }

   double Isotope::ImplementI::CorrectedComparativeHalflife()const
   {
      double ft      = FindValue("COMPARATIVE_HALFLIFE");
      double drp     = FindValue("DELTA_R_PRIME");
      double vcvns   = FindValue("DELTA_VC-DELTA_VNS");
      return ft*(1.0 + drp)*(1.0 - vcvns);
   }

   double Isotope::ImplementI::U_CorrectedComparativeHalflife()const
   {
      double ft      = FindValue("COMPARATIVE_HALFLIFE");
      double drp     = FindValue("DELTA_R_PRIME");
      double vcvns   = FindValue("DELTA_VC-DELTA_VNS");
      double u_ft    = FindUncertainty("COMPARATIVE_HALFLIFE");
      double u_drp   = FindUncertainty("DELTA_R_PRIME");
      double u_vcvns = FindUncertainty("DELTA_VC-DELTA_VNS");
      
      return sqrt((1.0 + drp)*(1.0 + drp) *(1.0 - vcvns)*(1.0 - vcvns)*(u_ft)
            *(u_ft)+(ft)*(ft)*(1.0 - vcvns)*(1.0 - vcvns)*(u_drp)
            *(u_drp)-(ft)*(ft)*(1.0 + drp)*(1.0 + drp)*(u_vcvns)*(u_vcvns));
   }

   void Isotope::ImplementI::AddComparativeHalflife()
   {
      theInputs["COMPARATIVE_HALFLIFE"]= new values(ComparativeHalflife(),
            U_ComparativeHalflife(), "!calculated!");
   }

   double Isotope::ImplementI::ComparativeHalflife()const
   {
      double srf   = FindValue("STATISTICAL_RATE_FUNCTION");
      double t     = FindValue("PARTIAL_HALFLIFE");
      return srf*t;
   }

   double Isotope::ImplementI::U_ComparativeHalflife()const
   {
      double srf   = FindValue("STATISTICAL_RATE_FUNCTION");
      double t     = FindValue("PARTIAL_HALFLIFE");
      double u_srf = FindUncertainty("STATISTICAL_RATE_FUNCTION");
      double u_t   = FindUncertainty("PARTIAL_HALFLIFE");
      return sqrt(t*t*u_srf*u_srf + srf*srf*u_t*u_t);
   }

   void Isotope::ImplementI::AddPartialHalflife()
   {
      theInputs["PARTIAL_HALFLIFE"]= new values(PartialHalfLife(),
            U_PartialHalfLife(), "!calculated!");
   }

   double Isotope::ImplementI::PartialHalfLife()const
   {
      double hl   = FindValue("HALFLIFE");
      double pec  = FindValue("PROBABILITY_ELECTRON_CAPTURE");
      double br   = FindValue("BRANCHING_RATIO");
      assert(br != 0);
      return hl*((1.0 + pec)/br);
   }

   double Isotope::ImplementI::U_PartialHalfLife()const
   {
      double hl   = FindValue("HALFLIFE");
      double pec  = FindValue("PROBABILITY_ELECTRON_CAPTURE");
      double br   = FindValue("BRANCHING_RATIO");
      double u_hl = FindUncertainty("HALFLIFE");
      double u_br = FindUncertainty("BRANCHING_RATIO"); 

      return sqrt(pow(((1.0 +pec)/br),2.0)*pow(u_hl,2.0)
            + pow((-1.0*hl*pow(br, -2.0) - hl*pec *pow(br, -2.0)),2.0)
            *pow(u_br,2.0));
   }

   void Isotope::ImplementI::SetValue(const std::string &key_,
         const double &value_)
   {
      auto findIt = theInputs.find(key_);
      if(findIt != theInputs.end())
      {
         return findIt->second->SetValue(value_);
      }
   }

   double Isotope::ImplementI::FindValue(const std::string &key_) const
   {
      auto findIt = theInputs.find(key_);
      if(findIt != theInputs.end())
      {
         return findIt->second->GetValue();
      }
      else
      {
         return 0;
      }
   }

   double Isotope::ImplementI::FindUncertainty(const std::string &key_) const
   {
      auto findIt = theInputs.find(key_);
      if(findIt != theInputs.end())
      {
         return findIt->second->GetUncertainty();
      }
      else
      {
         return 0;
      }
   }

   void Isotope::ImplementI::Print()
   {
      std::cout<<filename<<std::endl;
   }

   void Isotope::ImplementI::FillMap()
   {
      std::fstream inputfile(filename.c_str(), std::fstream::in);
      if(inputfile)
      {
         std::string line;
         std::vector<std::string> parsed;
         int lineNumber = 1;
         while(getline( inputfile, line ))
         {
            parsed = SS::split(line, ':');
            switch(parsed.size())
            {
               case 1:
                  {
                     cout<<"Problem with input line: "<<lineNumber<<endl;
                     break;
                  }
               case 2:
                  {
                     isotopeName = parsed[0];
                     break;
                  }
               case 3:
                  {
                     parsed[2].erase(parsed[2].begin(), parsed[2].begin() + parsed[2].find_first_of("#"));
                     theInputs[parsed[0]]= new values(std::stod(parsed[1]),0, parsed[2]);
                     break;
                  }
               case 4:
                  {
                     parsed[3].erase(parsed[3].begin(), parsed[3].begin() + parsed[3].find_first_of("#"));
                     theInputs[parsed[0]]= new values(std::stod(parsed[1]),std::stod(parsed[2]), parsed[3]);
                     break;
                  }
               default:
                  {
                     cout<<"Problem with input line: "<<lineNumber<<endl;
                     break;
                  }
            }
            parsed.clear();
            ++lineNumber;
         }
      }
      else
      {
         std::cerr<<"File: "<<filename<<" could not be opened."<<std::endl;
         assert(0);
      }
      inputfile.close();
   }


   Isotope::Isotope(const std::string &filename_)
      :mI(new ImplementI(filename_))
   {
   }

   Isotope::~Isotope()
   {
      delete mI; mI = nullptr;
   }

   double Isotope::A_beta(const double &T_)
   {
      return (mI->f_4(T_)+(mI->f_7(T_)/3.0))/mI->f_1(T_);
   }

   double Isotope::B_nu(const double &T_)
   {
      return (mI->f_6(T_)+(mI->f_5(T_)/3.0))/mI->f_1(T_);
   }

   double Isotope::a_beta_nu(const double &T_)
   {
      return mI->f_2(T_)/mI->f_1(T_);
   }

   double Isotope::c_align(const double &T_)
   {
      return mI->c_align(T_);
   }

   double Isotope::zhi(const double &T_)
   {
      return 2.0*mI->f_1(T_);
   }

   double Isotope::FindValue(const std::string &key_) const
   {
      return mI->FindValue(key_);
   }


}//end namespace
