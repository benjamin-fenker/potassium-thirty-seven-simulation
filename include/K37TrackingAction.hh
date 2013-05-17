// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37TrackingAction_h
#define K37TrackingAction_h 1

#include "G4UserTrackingAction.hh"


class K37TrackingAction : public G4UserTrackingAction {
 public:
  K37TrackingAction() {}
  virtual ~K37TrackingAction() {}

  void PreUserTrackingAction(const G4Track*);
};

#endif
