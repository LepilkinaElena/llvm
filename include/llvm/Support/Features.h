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
public:
  std::string BooleanToString (bool Feature) const;
  virtual std::string ToJSON() const = 0;
};

}
#endif 