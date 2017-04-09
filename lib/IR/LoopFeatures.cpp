#include "llvm/IR/LoopFeatures.h"
#include "llvm/Pass.h"

using namespace llvm;

std::string LoopFeaturesParameters::ToJSON() const {
  std::string Json = "  \"features\": {\n";
  Json += "    \"numIVUsers\": " + std::to_string(NumIVUsers) + ",\n";
  Json += "    \"isLoopSimplifyForm\": " + Features::BooleanToString(IsLoopSimplifyForm) + ",\n";
  Json += "    \"isEmpty\": " + Features::BooleanToString(IsEmpty) + ",\n";
  Json += "    \"numIntToFloatCast\": " + std::to_string(NumIntToFloatCast) + ",\n";
  Json += "    \"hasLoopPreheader\": " + Features::BooleanToString(HasLoopPreheader) + ",\n";
  Json += "    \"numTermBrBlocks\": " + std::to_string(NumTermBrBlocks) + ",\n";
  Json += "    \"latchBlockTermOpcode\": " + std::to_string(LatchBlockTermOpcode) + ",\n";
  Json += "    \"NumCalls\": " + std::to_string(Metrics.NumInlineCandidates) + ",\n";
  Json += "    \"NotDuplicatable\": " + Features::BooleanToString(Metrics.notDuplicatable) + ",\n";
  Json += "    \"Convergent\": " + Features::BooleanToString(Metrics.convergent) + ",\n";
  Json += "    \"LoopSize\": " + std::to_string(Metrics.NumInsts) + ",\n";
  Json += "    \"TripCount\": " + std::to_string(TripCount) + ",\n";
  Json += "    \"TripMultiply\": " + std::to_string(TripMultiply) + ",\n";
  Json += "    \"TermByCondBr\": " + Features::BooleanToString(TermByCondBr) + ",\n";
  Json += "    \"HeaderAddressTaken\": " + Features::BooleanToString(HeaderAddressTaken) + ",\n";
  Json += "    \"PHINodesInExitBlocks\": " + Features::BooleanToString(PHINodesInExitBlocks) + "\n";
  Json += "  }";
  return Json;
}

LoopFeaturesParametersBuilder::LoopFeaturesParametersBuilder() {
  FeaturesParameters = LoopFeaturesParameters();
}

LoopFeaturesParametersBuilder 
  LoopFeaturesParametersBuilder::WithNumIVUsers(unsigned NumIVUsers) {
    FeaturesParameters.NumIVUsers = NumIVUsers;
    return *this;
}

LoopFeaturesParametersBuilder 
  LoopFeaturesParametersBuilder::WithNumIntToFloatCast(unsigned NumIntToFloatCast) {
    FeaturesParameters.NumIntToFloatCast = NumIntToFloatCast;
    return *this;
}

LoopFeaturesParametersBuilder 
  LoopFeaturesParametersBuilder::WithCodeMetrics(const CodeMetrics& Metrics) {
    FeaturesParameters.Metrics = Metrics;
    return *this;
}

LoopFeaturesParametersBuilder 
  LoopFeaturesParametersBuilder::WithTripTuple(const std::tuple<unsigned, unsigned>& TripTuple) {
    FeaturesParameters.TripCount = std::get<0>(TripTuple);
    FeaturesParameters.TripMultiply = std::get<1>(TripTuple);
    return *this;
}

LoopFeaturesParametersBuilder 
  LoopFeaturesParametersBuilder::WithTermByCondBr(bool TermByCondBr) {
    FeaturesParameters.TermByCondBr = TermByCondBr;
    return *this;
}

LoopFeaturesParametersBuilder 
  LoopFeaturesParametersBuilder::WithHeaderAddressTaken(bool HeaderAddressTaken) {
    FeaturesParameters.HeaderAddressTaken = HeaderAddressTaken;
    return *this;
}

LoopFeaturesParametersBuilder 
  LoopFeaturesParametersBuilder::WithPHINodesInExitBlocks(bool PHINodesInExitBlocks) {
    FeaturesParameters.PHINodesInExitBlocks = PHINodesInExitBlocks;
    return *this;
}

LoopFeaturesParametersBuilder 
  LoopFeaturesParametersBuilder::WithIsLoopSimplifyForm(bool IsLoopSimplifyForm) {
    FeaturesParameters.IsLoopSimplifyForm = IsLoopSimplifyForm;
    return *this;
}

LoopFeaturesParametersBuilder 
  LoopFeaturesParametersBuilder::WithIsEmpty(bool IsEmpty) {
    FeaturesParameters.IsEmpty = IsEmpty;
    return *this;
}

LoopFeaturesParametersBuilder 
  LoopFeaturesParametersBuilder::WithHasLoopPreheader(bool HasLoopPreheader) {
    FeaturesParameters.HasLoopPreheader = HasLoopPreheader;
    return *this;
}

LoopFeaturesParametersBuilder 
  LoopFeaturesParametersBuilder::WithNumTermBrBlocks(unsigned NumTermBrBlocks) {
    FeaturesParameters.NumTermBrBlocks = NumTermBrBlocks;
    return *this;
}

LoopFeaturesParametersBuilder 
  LoopFeaturesParametersBuilder::WithLatchBlockTermOpcode(unsigned LatchBlockTermOpcode) {
    FeaturesParameters.LatchBlockTermOpcode = LatchBlockTermOpcode;
    return *this;
}

LoopFeaturesParameters LoopFeaturesParametersBuilder::Build() {
  return FeaturesParameters;
}

LoopFeatures::LoopFeatures(const std::string &CurPassName, std::string LoopId, 
                           LoopFeaturesParameters &FeaturesParameters) : 
    Features(CurPassName), LoopId(LoopId), FeaturesParameters(FeaturesParameters) {}

std::string LoopFeatures::ToJSON() const {
  std::string Json = "{\n  \"type\": \"loop\",\n";
  Json += "  \"id\": \"" + LoopId + "\",\n";
  Json += "  \"pass_place\": \"" + PassName + "\",\n";
  Json += FeaturesParameters.ToJSON();
  Json += "\n}\n";
  return Json;
}
