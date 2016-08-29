#include "llvm/Analysis/Features.h"

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