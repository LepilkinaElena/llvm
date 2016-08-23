//===----------------------------------------------------------------------===//
//
//  This file defines the LoopFeatures class, which store loop features for 
//  machine learning framework.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_SUPPORT_LOOPFEATURES_H
#define LLVM_SUPPORT_LOOPFEATURES_H

#include "llvm/Analysis/Features.h"
#include "llvm/Analysis/IVUsers.h"


namespace llvm {

struct MemAccess {
  static const unsigned UnknownAddressSpace = ~0u;

  Type *MemTy;
  unsigned AddrSpace;

  MemAccess(Type *Ty, unsigned AS) :
    MemTy(Ty), AddrSpace(AS) {}

  std::string ToJSON() const {
    std::string Json = "{ \"typeId\" :" + std::to_string(MemTy->getTypeID()) + ",";
    Json += " \"addrSpace\" :" + std::to_string(AddrSpace) + "}";
    return Json;
  }
};

class LoopFeatures : public Features {
  uint64_t LoopId;

  unsigned NumIVUsers;
  bool IsLoopSimplifyForm;
  bool IsEmpty;
  unsigned NumIntToFloatCast;
  bool HasLoopPreheader;
  unsigned NumTermBrBlocks;
  unsigned LatchBlockTermOpcode;
  std::vector<MemAccess> IVMemAccessTypes;

  void CountIntToFloatCast(const IVUsers &IU);
  void CountTermBrBlocks(const Loop* L);
  MemAccess getAccessType(const Instruction *Inst);
  void CollectAccessTypes(const IVUsers &IU);
  
public:
  LoopFeatures(const Loop* L, const std::string &CurPassName, const IVUsers &IU);
  std::string ToJSON() const;
};

}
#endif 