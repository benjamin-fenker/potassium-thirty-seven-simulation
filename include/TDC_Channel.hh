// Authors: Spencer Behling and Benjamin Fenerk 2013

#ifndef TDC_Channel_h
#define TDC_Channel_h 1

// C header
#include <stdint.h>

// C++ standard library
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

// Abstract Base Class
#include "Generic_Channel.hh"
#include "K37_Data.hh"

class TDC_Channel:public K37_ABC::K37_Data {
 public:
  TDC_Channel(const std::string &uniqueName_ , const int &detectorNumber_,
              const std::string &dataType_, const bool &registerCount_)
      :dataIsOK(false),
       registerCount(registerCount_),
       detectorNumber(detectorNumber_),
       printSize(15),
       printPrecision(6),
       theCount(new Generic_Channel((uniqueName_ +"_Count"), 0, "/D")),
       intData(0),
       longData(0),
       count(0),
       data(0),
       defaultValue(0),
       uniqueName(uniqueName_),
       dataType(dataType_) {}

  ~TDC_Channel() {
    delete theCount;
    theCount = 0;
  }

  void InsertData(const double &data_ ) {
    data = data_;
    vectData.push_back(data_);
    count += 1;
    dataIsOK = true;
    theCount->InsertData(count);
  };

  void InsertData(const uint32_t &data_ ) {
    intData = data_;
  };

  void InsertDataL(const uint64_t &data_) {intData = data_;}
  uint64_t* GetReferenceI() {
    return &longData;
  };
  double* GetReferenceD() {
    return &data;
  };
  std::vector<double>* GetReferenceV() {
    return &vectData;
  };

  void SetDefaultValue(const double &defaultValue_) {
    defaultValue = defaultValue_;
  };

  std::string GetType() const {
    return dataType;
  };

  void Clear() {
    data = defaultValue;
    vectData.clear(), count = 0;
    dataIsOK = false;
  };

  std::string Print() const {
    std::ostringstream ss;
    ss.precision(printPrecision);
    ss <<std::left << std::scientific << std::setw(printSize) << data;
    return ss.str();
  };
  std::string GetName() const {
    return uniqueName;
  };
  int GetNumber() const {
    return detectorNumber;
  };
  double GetValue() const {
    return data;
  };
  bool IsTheDataGood() const {
    return dataIsOK;
  };
  K37_ABC::K37_Data* GetCount() {
    return theCount;
  };
  bool ShouldCountBeRegistered() {
    return registerCount;
  };

 private:
  bool dataIsOK;
  bool registerCount;
  int detectorNumber;
  int printSize;
  int printPrecision;
  Generic_Channel *theCount;
  uint32_t intData;
  uint64_t longData;
  double count;
  double data;
  double defaultValue;
  std::vector<double> vectData;
  std::string uniqueName;
  std::string dataType;
};

#endif
