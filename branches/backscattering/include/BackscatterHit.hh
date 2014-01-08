// Authors: Spencer Behling and Benjamin Fenker 2014

#ifndef BackscatterHit_h
#define BackscatterHit_h 1

#include "G4VHit.hh"

// This class in (multiply) inherited from numerous K37*Hit Classes to
// allow easy incorporation of backscattering events without
// redefining it every time. Note that multiple inheritance can easily
// lead to super complicated problems, but in this circumstance I
// believe it will be okay as BackscatterHit and G4VHit (its usual
// partner) share no base classes.  If this becomes a headache, it
// should be easy to switch things to use composition instead.

class BackscatterHit {
 public:
  BackscatterHit();
  ~BackscatterHit();

  void SetBoundaryStatus(G4int bs) {boundary_status_ = bs;}
  void SetParentID(G4int pi) {parent_id_ = pi;}
  void SetTheta(G4double theta) {theta_ = theta;}

  G4int GetBoundaryStatus() {return boundary_status_;}
  G4int GetParentID() {return parent_id_;}
  G4double GetTheta() {return theta_;}
 private:
  G4int boundary_status_;
  G4int parent_id_;
  G4double theta_;
};
#endif
