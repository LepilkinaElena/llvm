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

class Features {
protected:
  std::string PassName;
public:
  Features(const std::string & PassName) :
    PassName(PassName) {}
  virtual ~Features();
  std::string BooleanToString (bool Feature) const;
  virtual std::string ToJSON() const;
};

}
#endif 