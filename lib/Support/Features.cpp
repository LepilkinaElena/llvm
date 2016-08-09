#include "llvm/Support/Features.h"

using namespace llvm;

std::string Features::BooleanToString (bool Feature) const {
	return Feature ? "true" : "false";
}