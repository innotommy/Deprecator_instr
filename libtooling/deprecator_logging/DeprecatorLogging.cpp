// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// 
// It should be run using the tools/clang/scripts/run_tool.py helper.
//
// Deprecator rewriter, Michael Weissbacher '17
//

// See file deprecator_target.txt.sample and function get_targets for configuration

#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Type.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/PCHContainerOperations.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Refactoring.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/PPCallbacks.h"
#include "llvm/Support/CommandLine.h"
#include <stdio.h>
#include <regex>
#include <algorithm>

using namespace clang::ast_matchers;

using llvm::Error;
using clang::ast_matchers::MatchFinder;
using clang::ast_matchers::argumentCountIs;
using clang::ast_matchers::expr;
using clang::ast_matchers::forEach;
using clang::ast_matchers::has;
using clang::ast_matchers::hasArgument;
using clang::ast_matchers::hasDeclaration;
using clang::ast_matchers::hasName;
using clang::ast_matchers::id;
using clang::ast_matchers::ofClass;
using clang::ast_matchers::stringLiteral;
using clang::ast_matchers::varDecl;
using clang::tooling::CommonOptionsParser;
using clang::tooling::Replacement;
using clang::tooling::Replacements;
using clang::tooling::FrontendActionFactory;
using clang::CXXMethodDecl;
using clang::CXXMethodDecl;

using namespace clang;
using namespace std;


bool DEBUG = true;
std::vector<std::string> target_vect (0);
std::vector<std::string> edit_seen(0);

namespace {

class ConstructorCallback : public MatchFinder::MatchCallback {
 public:
  ConstructorCallback(Replacements* replacements)
      : replacements_(replacements) {}

  virtual void run(const MatchFinder::MatchResult& result) override;

 private:
  Replacements* const replacements_;
};


class FunDeclHandler {
  public:
    explicit FunDeclHandler(Replacements* replacements)
        : constructor_callback_(replacements) {}
  void SetupMatchers(MatchFinder* match_finder);

  private:
  ConstructorCallback constructor_callback_;
};
    


void FunDeclHandler::SetupMatchers(MatchFinder* match_finder) {
  match_finder->addMatcher(ast_matchers::functionDecl().bind("function_definition"), &constructor_callback_);
}

bool endswith(const std::string &str, const std::string &suffix) {
    return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool is_target_function( std::string fname, std::string function ) {

    std::string target = std::string(fname) + ":" + std::string(function);
    if( std::find(target_vect.begin(), target_vect.end(), target) != target_vect.end()) {
        return true;
    }
    return false;

}

void ConstructorCallback::run(const MatchFinder::MatchResult& Result) {

    SourceManager &srcMgr = Result.Context->getSourceManager();
    if (const FunctionDecl *fun = Result.Nodes.getNodeAs<clang::FunctionDecl>("function_definition")) {
 
         std::string srcLocation = srcMgr.getFilename(fun->getLocation()).str();
 
         // include file skipping
         if (!fun || !srcMgr.isInMainFile(fun->getLocation())) {
             if (DEBUG) {
                 llvm::outs() << "Skipping (!mainfile): " << srcLocation << "\n";
             }
             return;
         }
 
         // no body: skip
         if (! fun->hasBody()) {
             if (DEBUG) {
                 llvm::outs() << "Skipping (!hasBody): " << srcLocation << "\n";
             }
             return;
         }
 
         // Not handling function overloading
         if ( fun->isOverloadedOperator() ) {
             if (DEBUG) {
                 llvm::outs() << "Skipping (overload): " << srcLocation << "\n";
             }
             return;
         }
 
         llvm::outs() << "Processing function: " << srcLocation << " ## " <<  fun->getDeclName() << "\n";
 
         if (fun->hasBody()) {
             Stmt *body = fun->getBody();
 
             // Function signature
             std::string fun_signature = "";
 
 
            for (clang::Stmt::child_iterator iter = body->child_begin(), end = body->child_end(); iter!=end; ++iter) {

                // Replacing signature simply with number - we just need to be unique!
                fun_signature = std::to_string(iter->getLocStart().getRawEncoding());
                std::string fun_id = srcLocation + "##" + fun->getNameAsString() + "##" + fun_signature;
                std::string trace_full;

                if ( ! is_target_function( srcLocation, fun->getNameAsString() ) )
                    // Abort!
                    break;

                trace_full = std::string("\bDeprecatorClass *singleton=nullptr;\bsingleton = singleton->getInstance();\bsingleton->LogFunction(\"") + std::string(fun_id) + std::string("\");\b");
                 
                Replacement Rep(*(Result.SourceManager), iter->getLocStart(), 0, trace_full );
                std::string reps = Rep.toString();

                // When the src filename is empty (e.g.: macro)
                if ( reps.compare(0, 2, std::string(": ")) == 0) {
                    continue;
                }

                if ( srcLocation.length() <= 1 ) {
                    continue;
                }

                ofstream logfile;
                logfile.open ("/tmp/clang.out.txt", std::ios_base::app);
                logfile << "edit: (" << srcLocation <<")\n";
                logfile << "istarget? " << std::string(Rep.getFilePath()) << ":" << fun->getNameAsString() << "\n";
                logfile.close();

                std::string insert_once = std::to_string(Rep.getOffset()) + ":" + std::string(Rep.getFilePath());
 
                if(! ( std::find(edit_seen.begin(), edit_seen.end(), insert_once) != edit_seen.end())) {
                    edit_seen.push_back(insert_once);
                    auto addErr = replacements_->add(Rep);
                    if ( addErr ) {
                        // nothing
                    };
                }

                // we only need do this before the first statement.
                break;
             }
         }
     }
   }
}


static llvm::cl::extrahelp common_help(CommonOptionsParser::HelpMessage);

void get_targets( std::vector<std::string> *target_vect ) {
    ifstream targetfile;
    targetfile.open ("/tmp/deprecator_target.txt");
    std::string line;
    while (std::getline(targetfile, line)) {
        if (line.length() < 2) {
            continue;
        }
        target_vect->push_back(line);
    }
    targetfile.close();
}

int main(int argc, const char* argv[]) {
    llvm::cl::OptionCategory category("Deprecator logging tool");

    get_targets(&target_vect);


    CommonOptionsParser options(argc, argv, category);
    clang::tooling::RefactoringTool tool(options.getCompilations(), options.getSourcePathList());
    std::string include_replacement =  "#include \"chrome/browser/Deprecator.cpp\"\b";
    std::vector<std::string> seen_files(0);


    Replacements xreplacements;
    FunDeclHandler converter(&xreplacements);
    MatchFinder match_finder;
    converter.SetupMatchers(&match_finder);

    std::unique_ptr<clang::tooling::FrontendActionFactory> frontend_factory = clang::tooling::newFrontendActionFactory(&match_finder);
    int result = tool.run(frontend_factory.get());
    if (result != 0)
        return result;



    //llvm::outs() << "Replacements collected by the tool:\n";
    llvm::outs() << "==== BEGIN EDITS ====\n";

    for (const auto& r : xreplacements) {

        llvm::outs() << "r:::" << r.getFilePath() << ":::" << r.getOffset()
                    << ":::" << r.getLength() << ":::" << r.getReplacementText()
                    << "\n";
        
        if (! ( std::find(seen_files.begin(), seen_files.end(), r.getFilePath() )!=seen_files.end() )) {
            seen_files.push_back(r.getFilePath());
        }
    }

    // This emits *one* added #include directive for each edited file.
    // These must be applied *after* all other edits, as the offsets wouldn't match up anymore.
    // Also - this should go in conjunction with #pragma once in the trace header file.


    for(size_t i = 0; i < seen_files.size(); ++i) {
        llvm::outs() << "r:::" << seen_files[i]<< ":::" << 0 << ":::" << 0 << ":::" << include_replacement << "\b\n";
    }

    llvm::outs() << "==== END EDITS ====\n";

    return 0;
}

