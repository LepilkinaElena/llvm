#include "llvm/Support/FeatureLogger.h"

using namespace llvm;

FeatureLogger::~FeatureLogger() {
  Output->close();
}

void FeatureLogger::Log(const Features &LoggingFeatures) {
  *(Output) << LoggingFeatures.ToJSON();
}