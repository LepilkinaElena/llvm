#include "llvm/IR/LoopFeatures.h"
#include "llvm/Pass.h"

using namespace llvm;

LoopFeatures::LoopFeatures(const std::string &CurPassName, uint64_t LoopId, unsigned NumIVUsers,
                           bool IsLoopSimplifyForm, bool isEmptyLoop, unsigned NumIntToFloatCast,
                           bool HasLoopPreheader, unsigned NumTermBrBlocks, 
                           unsigned LatchBlockTermOpcode) : 
    Features(CurPassName), LoopId(LoopId), NumIVUsers(NumIVUsers), 
    IsLoopSimplifyForm(IsLoopSimplifyForm), IsEmpty(isEmptyLoop), 
    NumIntToFloatCast(NumIntToFloatCast), HasLoopPreheader(HasLoopPreheader),
    NumTermBrBlocks(NumTermBrBlocks), LatchBlockTermOpcode(LatchBlockTermOpcode) {}

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
  Json += "    \"latchBlockTermOpcode\": " + std::to_string(LatchBlockTermOpcode) + "\n";
  Json += "}\n}\n";
  return Json;
}
