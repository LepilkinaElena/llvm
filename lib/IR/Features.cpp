#include "llvm/IR/Features.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/ADT/STLExtras.h"
using namespace llvm;

std::string Features::BooleanToString (bool Feature) const {
  return Feature ? "true" : "false";
}

Features::~Features() {}

std::string Features::ToJSON() const {
  return "";
}

Features::Features(const std::string & PassName) {
  this->PassName = PassName;
}

raw_ostream &Features::getFeaturesOutput(std::string FileName) {
  std::error_code EC;
  static raw_fd_ostream FeaturesOutput(FileName, EC, sys::fs::F_Append | sys::fs::F_Text);
  if (!EC)
    return FeaturesOutput;
  errs() << "Error opening features-output-file '"
    << FileName << " for appending!\n";
  return *llvm::make_unique<raw_fd_ostream>(2, false); // stderr.
}