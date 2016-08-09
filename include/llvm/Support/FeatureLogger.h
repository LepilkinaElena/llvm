//===----------------------------------------------------------------------===//
//
//  This file defines the FeatureLogger class, which log features for machine 
// learning framework.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_SUPPORT_FEATURELOGGER_H
#define LLVM_SUPPORT_FEATURELOGGER_H

#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Features.h"
#include "llvm/Support/Signals.h"
#include "llvm/ADT/STLExtras.h"

namespace llvm {

class FeatureLogger {

  const std::string LogFileName;
  std::error_code EC;
  std::unique_ptr<raw_fd_ostream> Output;
public:
  FeatureLogger():LogFileName("features.output"), 
                  Output(std::move(make_unique<raw_fd_ostream>(LogFileName, EC,
                         sys::fs::OpenFlags::F_Text))) {
    if (EC) {
      errs() << "error: Error during opening file.\n";
      sys::RunInterruptHandlers();
      std::exit(1);
    }

  }

  void Log(const Features &LoggingFeatures);
  ~FeatureLogger();
};
}
#endif 