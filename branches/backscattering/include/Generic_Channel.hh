// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef Generic_Channel_h
#define Generic_Channel_h 1

// C header
#include <stdint.h>

// C++ standard library
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>

// Abstract Base Class
#include "K37_Data.hh"

class Generic_Channel:public K37_ABC::K37_Data {
 public:
  Generic_Channel(const std::string &uniqueName_ , const int &detectorNumber_,
                   const std::string &dataType_)
      :dataIsOK(false),
       detectorNumber(detectorNumber_),
       printSize(15),
       printPrecision(6),
       intData(0),
       longData(0),
       data(0),
       defaultValue(0),
       uniqueName(uniqueName_),
       dataType(dataType_) {}
    ~Generic_Channel() {}
    void InsertData(const double &data_) {
        data = data_;
        dataIsOK = true;
    }

    void InsertData(const uint32_t &data_) {intData = data_;}
    void InsertDataL(const uint64_t &data_) {longData = data_;}
    uint64_t* GetReferenceI() {return &longData;}
    double* GetReferenceD() {return &data;}
    std::vector<double>* GetReferenceV() {return &vectData;}
    void SetDefaultValue(const double &defaultValue_) {
      defaultValue = defaultValue_;
    }
    std::string GetType() const {return dataType;}
    void Clear() {
        data = defaultValue;
        dataIsOK = false;
    }
    std::string Print() const {
        std::ostringstream ss;
        ss.precision(printPrecision);
        ss <<std::left << std::scientific << std::setw(printSize) << data;
        return ss.str();
    }
    std::string GetName() const { return uniqueName;}
    int GetNumber() const {return detectorNumber;}
    double GetValue() const {return data;}
    bool IsTheDataGood() const {return dataIsOK;}

 private:
  bool dataIsOK;
  int detectorNumber;
  int printSize;
  int printPrecision;
  uint32_t intData;
  uint64_t longData;
  double data;
  double defaultValue;
  std::vector<double> vectData;
  std::string uniqueName;
  std::string dataType;
};

#endif
