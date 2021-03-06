//===----------------------------------------------------------------------===//
//
//  This file defines the Features class, which is base class for other features 
//  classes for machine learning framework.
//
//===----------------------------------------------------------------------===//
#ifndef LLVM_SUPPORT_FEATURES_H
#define LLVM_SUPPORT_FEATURES_H

#include <string>

namespace llvm {
class raw_ostream;
class Features {
protected:
  std::string PassName;
public:
  Features(const std::string & PassName);
  virtual ~Features();
  static std::string BooleanToString (bool Feature);
  virtual std::string ToJSON() const;
  static raw_ostream &getFeaturesOutput(std::string FileName);
};

}
#endif 