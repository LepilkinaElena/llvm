//===----------------------------------------------------------------------===//
//
//  This file defines the LoopFeatures class, which store loop features for 
//  machine learning framework.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_SUPPORT_LOOPFEATURES_H
#define LLVM_SUPPORT_LOOPFEATURES_H

#include "llvm/IR/Features.h"
#include "llvm/Analysis/CodeMetrics.h"


namespace llvm {
class LoopFeaturesParametersBuilder;

class LoopFeaturesParameters {
  unsigned NumIVUsers;
  bool IsLoopSimplifyForm;
  bool IsEmpty;
  unsigned NumIntToFloatCast;
  bool HasLoopPreheader;
  unsigned NumTermBrBlocks;
  unsigned LatchBlockTermOpcode;
  CodeMetrics Metrics;
  unsigned TripCount;
  unsigned TripMultiply;
  bool TermByCondBr;
  bool HeaderAddressTaken;
  bool PHINodesInExitBlocks;

public:
  std::string ToJSON() const;
  friend LoopFeaturesParametersBuilder;
};

class LoopFeaturesParametersBuilder {
protected:
  LoopFeaturesParameters FeaturesParameters;
  unsigned CountTermBrBlocks(const Loop& L);
public:
  LoopFeaturesParametersBuilder();
  LoopFeaturesParametersBuilder WithNumIVUsers(unsigned NumIVUsers);
  LoopFeaturesParametersBuilder WithNumIntToFloatCast(unsigned NumIntToFloatCast);
  LoopFeaturesParametersBuilder WithCodeMetrics(const CodeMetrics& Metrics);
  LoopFeaturesParametersBuilder WithTripTuple(const std::tuple<unsigned, unsigned>& TripTuple);
  LoopFeaturesParametersBuilder WithTermByCondBr(bool TermByCondBr);
  LoopFeaturesParametersBuilder WithHeaderAddressTaken(bool HeaderAddressTaken);
  LoopFeaturesParametersBuilder WithPHINodesInExitBlocks(bool PHINodesInExitBlocks);
  LoopFeaturesParametersBuilder WithIsLoopSimplifyForm(bool IsLoopSimplifyForm);
  LoopFeaturesParametersBuilder WithIsEmpty(bool IsEmpty);
  LoopFeaturesParametersBuilder WithHasLoopPreheader(bool HasLoopPreheader);
  LoopFeaturesParametersBuilder WithNumTermBrBlocks(unsigned NumTermBrBlocks);
  LoopFeaturesParametersBuilder WithLatchBlockTermOpcode(unsigned LatchBlockTermOpcode);

  LoopFeaturesParameters Build();
};

class LoopFeatures : public Features {
  std::string LoopId;
  LoopFeaturesParameters FeaturesParameters;
  
public:
  LoopFeatures(const std::string &CurPassName, std::string LoopId, 
               LoopFeaturesParameters& FeaturesParameters);
  std::string ToJSON() const;
};

}
#endif 
