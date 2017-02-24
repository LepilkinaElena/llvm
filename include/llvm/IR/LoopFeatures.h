//===----------------------------------------------------------------------===//
//
//  This file defines the LoopFeatures class, which store loop features for 
//  machine learning framework.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_SUPPORT_LOOPFEATURES_H
#define LLVM_SUPPORT_LOOPFEATURES_H

#include "llvm/IR/Features.h"


namespace llvm {

class LoopFeatures : public Features {
  std::string LoopId;

  unsigned NumIVUsers;
  bool IsLoopSimplifyForm;
  bool IsEmpty;
  unsigned NumIntToFloatCast;
  bool HasLoopPreheader;
  unsigned NumTermBrBlocks;
  unsigned LatchBlockTermOpcode;
  
public:
  LoopFeatures(const std::string &CurPassName, std::string LoopId, unsigned NumIVUsers,
                           bool IsLoopSimplifyForm, bool isEmptyLoop, unsigned NumIntToFloatCast,
                           bool HasLoopPreheader, unsigned NumTermBrBlocks, 
                           unsigned LatchBlockTermOpcode);
  std::string ToJSON() const;
};

}
#endif 
