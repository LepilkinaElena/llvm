#include "llvm/Analysis/LoopFeatures.h"
#include "llvm/Pass.h"

using namespace llvm;

LoopFeatures::LoopFeatures(const Loop* L, const std::string &CurPassName, const IVUsers &IU) :
    Features(CurPassName) {
  LoopId = L->LoopId;
  PassName = CurPassName;
  NumIVUsers = 0;
  IsLoopSimplifyForm = L->isLoopSimplifyForm();
  IsEmpty = L->empty();
  NumIntToFloatCast = 0;
  HasLoopPreheader = L->getLoopPreheader();
  NumTermBrBlocks = 0;
  CountTermBrBlocks(L);
  LatchBlockTermOpcode = L->getLoopLatch()->getTerminator()->getOpcode();
  CountIntToFloatCast(IU);
  CollectAccessTypes(IU);
}

void LoopFeatures::CountIntToFloatCast(const IVUsers &IU) {
  for (IVUsers::const_iterator UI = IU.begin(), E = IU.end();
       UI != E; ) {
    IVUsers::const_iterator CandidateUI = UI;
    ++UI;
    Type *DestTy = nullptr;
    if (UIToFPInst *UCast = dyn_cast<UIToFPInst>(CandidateUI->getUser())) {
      DestTy = UCast->getDestTy();
    }
    else if (SIToFPInst *SCast = dyn_cast<SIToFPInst>(CandidateUI->getUser())) {
      DestTy = SCast->getDestTy();
    }
    if (DestTy) {
      NumIntToFloatCast++;
    }
    NumIVUsers ++;
  }
}

void LoopFeatures::CountTermBrBlocks(const Loop* L) {
  SmallVector<BasicBlock*, 8> ExitingBlocks;
  L->getExitingBlocks(ExitingBlocks);

  for (BasicBlock *ExitingBlock : ExitingBlocks) {

    BranchInst *TermBr = dyn_cast<BranchInst>(ExitingBlock->getTerminator());
    if (TermBr)
      if (!TermBr->isUnconditional() && isa<ICmpInst>(TermBr->getCondition()))
        NumTermBrBlocks++;
  }
}

void LoopFeatures::CollectAccessTypes(const IVUsers &IU) {
  for (IVUsers::const_iterator UI = IU.begin(), E = IU.end(); UI != E; ++UI)
    IVMemAccessTypes.push_back(getAccessType(UI->getUser()));
}

MemAccess LoopFeatures::getAccessType(const Instruction *Inst) {
  MemAccess AccessTy(Inst->getType(), MemAccess::UnknownAddressSpace);
  if (const StoreInst *SI = dyn_cast<StoreInst>(Inst)) {
    AccessTy.MemTy = SI->getOperand(0)->getType();
    AccessTy.AddrSpace = SI->getPointerAddressSpace();
  } else if (const LoadInst *LI = dyn_cast<LoadInst>(Inst)) {
    AccessTy.AddrSpace = LI->getPointerAddressSpace();
  }

  // All pointers have the same requirements, so canonicalize them to an
  // arbitrary pointer type to minimize variation.
  if (PointerType *PTy = dyn_cast<PointerType>(AccessTy.MemTy))
    AccessTy.MemTy = PointerType::get(IntegerType::get(PTy->getContext(), 1),
                                      PTy->getAddressSpace());

  return AccessTy;
}

std::string LoopFeatures::ToJSON() const {
  std::string Json = "{\n  \"type\": \"loop\",\n  \"features\": {\n";
  Json += "    \"id\": " + std::to_string(LoopId) + ",\n";
  Json += "    \"pass\": \"" + PassName + "\",\n";
  Json += "    \"numIVUsers\": " + std::to_string(NumIVUsers) + ",\n";
  Json += "    \"isLoopSimplifyForm\": " + BooleanToString(IsLoopSimplifyForm) + ",\n";
  Json += "    \"isEmpty\": " + BooleanToString(IsEmpty) + ",\n";
  Json += "    \"numIntToFloatCast\": " + std::to_string(NumIntToFloatCast) + ",\n";
  Json += "    \"hasLoopPreheader\": " + BooleanToString(HasLoopPreheader) + ",\n";
  Json += "    \"numTermBrBlocks\": " + std::to_string(NumTermBrBlocks) + ",\n";
  Json += "    \"latchBlockTermOpcode\": " + std::to_string(LatchBlockTermOpcode) + ",\n";
  Json += "    \"IVMemAccessTypes\": [\n";
  unsigned Num = 0;
  for (const MemAccess &AccessTy : IVMemAccessTypes) {
    Json += AccessTy.ToJSON();
    if (Num < IVMemAccessTypes.size() - 1)
      Json += ",\n ";
    Num++;
  }
  Json += "]\n}\n}\n";
  return Json;
}