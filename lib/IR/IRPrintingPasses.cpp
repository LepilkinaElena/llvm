//===--- IRPrintingPasses.cpp - Module and Function printing passes -------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// PrintModulePass and PrintFunctionPass implementations.
//
//===----------------------------------------------------------------------===//

#include "llvm/IR/Features.h"
#include "llvm/IR/IRPrintingPasses.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

PrintModulePass::PrintModulePass() : OS(dbgs()) {}
PrintModulePass::PrintModulePass(raw_ostream &OS, const std::string &Banner,
                                 bool ShouldPreserveUseListOrder)
    : OS(OS), Banner(Banner),
      ShouldPreserveUseListOrder(ShouldPreserveUseListOrder) {}

PreservedAnalyses PrintModulePass::run(Module &M, ModuleAnalysisManager &) {
  OS << Banner;
  if (llvm::isFunctionInPrintList("*"))
    M.print(OS, nullptr, ShouldPreserveUseListOrder);
  else {
    for(const auto &F : M.functions())
      if (llvm::isFunctionInPrintList(F.getName()))
        F.print(OS);
  }
  return PreservedAnalyses::all();
}

PrintFunctionPass::PrintFunctionPass() : OS(dbgs()) {}
PrintFunctionPass::PrintFunctionPass(raw_ostream &OS, const std::string &Banner)
    : OS(OS), Banner(Banner) {}

PreservedAnalyses PrintFunctionPass::run(Function &F,
                                         FunctionAnalysisManager &) {
  if (isFunctionInPrintList(F.getName()))
    OS << Banner << static_cast<Value &>(F);
  return PreservedAnalyses::all();
}

namespace {

class PrintModulePassWrapper : public ModulePass {
  PrintModulePass P;

public:
  static char ID;
  PrintModulePassWrapper() : ModulePass(ID) {}
  PrintModulePassWrapper(raw_ostream &OS, const std::string &Banner,
                         bool ShouldPreserveUseListOrder)
      : ModulePass(ID), P(OS, Banner, ShouldPreserveUseListOrder) {}

  bool runOnModule(Module &M) override {
    ModuleAnalysisManager DummyMAM;
    P.run(M, DummyMAM);
    return false;
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }
};

class PrintFunctionPassWrapper : public FunctionPass {
  PrintFunctionPass P;

public:
  static char ID;
  PrintFunctionPassWrapper() : FunctionPass(ID) {}
  PrintFunctionPassWrapper(raw_ostream &OS, const std::string &Banner)
      : FunctionPass(ID), P(OS, Banner) {}

  // This pass just prints a banner followed by the function as it's processed.
  bool runOnFunction(Function &F) override {
    FunctionAnalysisManager DummyFAM;
    P.run(F, DummyFAM);
    return false;
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }
};

class PrintBasicBlockPass : public BasicBlockPass {
  raw_ostream &Out;
  std::string Banner;

public:
  static char ID;
  PrintBasicBlockPass() : BasicBlockPass(ID), Out(dbgs()) {}
  PrintBasicBlockPass(raw_ostream &Out, const std::string &Banner)
      : BasicBlockPass(ID), Out(Out), Banner(Banner) {}

  bool runOnBasicBlock(BasicBlock &BB) override {
    Out << Banner << BB;
    return false;
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }
};

class PrintFeaturesModulePass : public ModulePass,
                                public PrintFeaturesPass {
public:
  static char ID;
  PrintFeaturesModulePass() : ModulePass(ID), PrintFeaturesPass("", "") {}
  PrintFeaturesModulePass(const std::string &FileName, const std::string &PassName)
      : ModulePass(ID), PrintFeaturesPass(FileName, PassName) {}

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

  bool runOnModule(Module &M) override {
    Features Features(PassName);
    run(Features);
    return false;
  }
};

class PrintFeaturesFunctionPass : public FunctionPass,
                                  public PrintFeaturesPass {
public:
  static char ID;
  PrintFeaturesFunctionPass() : FunctionPass(ID), PrintFeaturesPass("", "") {}
  PrintFeaturesFunctionPass(const std::string &FileName, const std::string &PassName)
      : FunctionPass(ID), PrintFeaturesPass(FileName, PassName) {}

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

  bool runOnFunction(Function &F) override {
    Features Features(PassName);
    run(Features);
    return false;
  }
};

class PrintFeaturesBasicBlockPass : public BasicBlockPass,
                                    public PrintFeaturesPass {
public:
  static char ID;
  PrintFeaturesBasicBlockPass() : BasicBlockPass(ID), PrintFeaturesPass("", "") {}
  PrintFeaturesBasicBlockPass(const std::string &FileName, const std::string &PassName)
      : BasicBlockPass(ID), PrintFeaturesPass(FileName, PassName) {}

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

  bool runOnBasicBlock(BasicBlock &BB) override {
    Features Features(PassName);
    run(Features);
    return false;
  }
};


}

char PrintModulePassWrapper::ID = 0;
INITIALIZE_PASS(PrintModulePassWrapper, "print-module",
                "Print module to stderr", false, false)
char PrintFunctionPassWrapper::ID = 0;
INITIALIZE_PASS(PrintFunctionPassWrapper, "print-function",
                "Print function to stderr", false, false)
char PrintBasicBlockPass::ID = 0;
INITIALIZE_PASS(PrintBasicBlockPass, "print-bb", "Print BB to stderr", false,
                false)
char PrintFeaturesModulePass::ID = 0;
/*INITIALIZE_PASS(PrintFeaturesModulePass, "print-features-module",
                "Print module to stderr", false, false)*/
char PrintFeaturesFunctionPass::ID = 0;
/*INITIALIZE_PASS(PrintFeaturesFunctionPass, "print-features-function",
                "Print function to stderr", false, false)*/
char PrintFeaturesBasicBlockPass::ID = 0;
/*INITIALIZE_PASS(PrintFeaturesBasicBlockPass, "print-features-bb", "Print BB to stderr", false,
                false)*/

ModulePass *llvm::createPrintModulePass(llvm::raw_ostream &OS,
                                        const std::string &Banner,
                                        bool ShouldPreserveUseListOrder) {
  return new PrintModulePassWrapper(OS, Banner, ShouldPreserveUseListOrder);
}

FunctionPass *llvm::createPrintFunctionPass(llvm::raw_ostream &OS,
                                            const std::string &Banner) {
  return new PrintFunctionPassWrapper(OS, Banner);
}

BasicBlockPass *llvm::createPrintBasicBlockPass(llvm::raw_ostream &OS,
                                                const std::string &Banner) {
  return new PrintBasicBlockPass(OS, Banner);
}

ModulePass *llvm::createPrintFeaturesModulePass(const std::string &FileName,
                                        const std::string &PassName) {
  return new PrintFeaturesModulePass(FileName, PassName);
}

FunctionPass *llvm::createPrintFeaturesFunctionPass(const std::string &FileName,
                                                    const std::string &PassName) {
  return new PrintFeaturesFunctionPass(FileName, PassName);
}

BasicBlockPass *llvm::createPrintFeaturesBasicBlockPass(const std::string &FileName,
                                                        const std::string &PassName) {
  return new PrintFeaturesBasicBlockPass(FileName, PassName);
}
