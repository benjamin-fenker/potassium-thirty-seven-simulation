#ifndef Isotope_h
#define Isotope_h 1

#include <string>

class IsotopeMessenger;

namespace ISO
{
   class Isotope
   {
      public:
         Isotope(const std::string &filename_);
         ~Isotope();
         //Get the beta assymetry as a function of kinetic energy of e+
         double A_beta   (const double &T_);
         //Get the neutrino assymetry as a function of kinetic energy of e+ 
         double B_nu     (const double &T_);
         //Get the beta neutrino correlation as a function of kinetic energy of e+ 
         double a_beta_nu(const double &T_);
         //Get the alignment as a function of kinetic energy of e+ 
         double c_align  (const double &T_);
         //Get zhi as a function of kinetic energy of e+ 
         double zhi      (const double &T_);

         void ChangeRho (const double &NewRhoValue_);

         //This function can retrieve the value of a const stored in the 
         //isotope database. Really this should not be used in simulations
         //but is useful for debugging.
         double FindValue(const std::string &key_) const;

         //To make Isotope non-copyable
         Isotope(const Isotope &) = delete;
         const Isotope & operator=(const Isotope &) = delete;

         //stands for my Implement Isotope
         class ImplementI;
         //my Isotope
         ImplementI *mI;

         IsotopeMessenger* isotopeMessenger;   // messenger of this class


   };
}
#endif
