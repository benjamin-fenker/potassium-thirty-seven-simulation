// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef Aggregator_h
#define Aggregator_h 1

#include <string>
#include "K37_Data.hh"

namespace AGG {
class Aggregator {
 public:
  Aggregator();
  ~Aggregator();
  void RegisterData(K37_ABC::K37_Data *dataPointer_);
  void RegisterIOMethod(const std::string &fileName_);
  void BeginRun();
  void EndEvent();
  void EndRun();

 private:
  // to make Aggregator non-copyable
  Aggregator(const Aggregator &);
  // to make Aggregator non-copyable
  const Aggregator & operator=(const Aggregator &);
  class ImplementAGG;
  ImplementAGG *mAGG;  // stands for my Aggregator
};
}
#endif
