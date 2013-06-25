// Authors: Spencer Behling and Benjamin Fenerk 2013

#ifndef K37_Data_h
#define K37_Data_h 1

#include <stdint.h>

#include <string>
#include <vector>

namespace K37_ABC {

class K37_Data {
 public:
  virtual ~K37_Data() {}
  // Event By Event as data is upacked from the midas file it will than be
  // stored into the class with the InsertData function. In the case of a
  // waveform each element of the waveform requires is own call to InsertData
  virtual void InsertData(const double &) = 0;

  // Event By Event as data is upacked from the midas file it will than be
  // stored into the class with the InsertData function. I realized later that
  // overloading this function is the best way to deal with multiple types
  virtual void InsertData(const uint32_t &) = 0;
  virtual void InsertDataL(const uint64_t &data_) = 0;
  // All of the coulumns that are returning double can be given a default
  // value to signal if they are not filed the only exception is the
  // TTTL column that will return the time because this coulum returns a
  // unsigned integer
  virtual void SetDefaultValue(const double &) = 0;

  // This function is used to clear out all of the old data if something goes
  // wrong or at the end of every event. They are all put back to there default
  // values set by SetDefaultValue()
  virtual void Clear() = 0;

  // This is the string that TTree needs to know in order to save the data
  // it will also be the switch for what function to call in case of type
  // differences
  virtual std::string GetType() const = 0;

  // This function actually prints the content of the object as a string. This
  // is so that each derived class can handle it own formatting and the
  // results can be directly piped into cout without further manipulation.
  virtual std::string Print() const = 0;

  // All of the derived classes will have a unique name that is associated.
  // with the particular peice of the detector hardware that they handle.
  virtual std::string GetName() const = 0;

  // All of the derived classes will have a unique number that is associated
  // with that detector element. In most cases this would be the channel on
  // the module that the signal plugs into. In the case of the strip detector
  // this will be the physical channel number.
  virtual int  GetNumber() const = 0;

  // This function could be used to print the object but is really intended to
  // pass the data around within the program.
  virtual double GetValue() const = 0;

  // This function will be used to fill the TTree.
  virtual uint64_t* GetReferenceI() = 0;

  // This function will be used to fill the TTree.
  virtual std::vector<double>* GetReferenceV() = 0;

  // This function will be used to fill the TTree. Any is overloaded to handle
  // different types.
  virtual double* GetReferenceD() = 0;

  // This function returns true if by all indications the data is OK. It is
  // set to false by a call to clear. If the derived class expects more than one
  // call to InsertData it will return true only when the expected number has
  // been reached.
  virtual bool IsTheDataGood() const = 0;
};
}
#endif
