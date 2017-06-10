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
#include <map>
#include <stdio.h>
#include <string.h>
#include <cstring>


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
std::map<std::string, std::string> Map;
std::vector<std::string> integer{"long long","long","short","double","unsigned long", "unsigned long long","unsigned short","float","GLint","DOMHighResTimeStamp","GLenum","SourceBuffer","ClientRect"};
std::vector<std::string> str{"String","ByteString","DOMString","USVString","ByteString"};
std::vector<std::string> null{"Attr","HTMLCollection","Element","nodelist o Element","HTMLElement","Node","IDBRequest","SVGTransform","ItemProperty""NodeList","MimeType","IDBKeyRang","Window","Response","Range","applicationCache","XPathResult","WebGLActiveInfo","Text"
                              ,"Plugin","Navigator","MediaQueryList","ImageData","IDBOpenDBRequest","IDBObjectStore","IDBIndex","History","DOMWindow","CSSStyleDeclaration","XMLDocument","WebGLUniformLocation","WebGLTexture","WebGLShaderPrecisionFormat","WebGLShader"
                              ,"WebGLRenderbuffer","WebGLProgram","WebGLFramebuffer","WebGLBuffer","TextTrackCue","StyleSheet","Request","RTCDataChannel","ArrayBufferView","Document","MediaStream","location","TextTrack","Selection","Nullable<Vector<String>>","Nullable<HeapVector<Member<WebGLShader>>>"
                              ,"HTMLHeadElement","File","CanvasPattern","SVGRect","HTMLTableSectionElement","screen","PerformanceNavigation","PerformanceTiming","Float32Array","XPathNSResolver","WaveShaperNode","StereoPannerNode","TreeWalker","ScriptProcessorNode","PannerNode"
                              ,"OscillatorNode","NodeIterator","MimeTypeArray","MediaStreamAudioSourceNode","MediaStreamAudioSourceNode","MediaStreamAudioDestinationNode","MediaElementAudioSourceNode","Geolocation","DynamicsCompressorNode","DelayNode"
                              ,"ConvolverNode","ChannelSplitterNode","ChannelMergerNode","BiquadFilterNode","AnalyserNode","DocumentFragment","SVGPoint","SVGMatrix","AudioParam","SVGTransform","Remote_BarProp","ReadOnly_DOMMatrix","DocumentType","HTMLTableCaptionElement"
                              ,"Nullable<ExceptionCode>","HTMLDocument","MediaDevices"};
std::vector<std::string> promise{"Promise","Promise<USVString>","Promise<JSON>","Promise<Blob>","Promise<ArrayBuffer>","promise","ScriptPromise"};

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
                //checking whitch returning value have to be added at that function
                ofstream mappinglog;
                mappinglog.open ("/tmp/mapping.out.txt", std::ios_base::app);
                std::string temp2 = std::string(srcLocation) + ":" + std::string(fun->getNameAsString());
                mappinglog << "search: (" << temp2 << ")\n";
                std::map<std::string, std::string>::iterator it;
                it = Map.find(temp2); 
                if(it != Map.end())
                {
                  if(Map[temp2] == "void"){
                    mappinglog << "found void return";
                    trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn;\b}\b");
                    mappinglog.close();
                  }
                  if(std::find(integer.begin(), integer.end(), Map[temp2]) != integer.end()){
                    mappinglog << "found integer return";
                    trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn 0;\b}\b");
                    mappinglog.close();
                  }
                  if(std::find(str.begin(), str.end(), Map[temp2]) != str.end()){
                    mappinglog << "found string return";
                    trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn \"\";\b}\b");
                    mappinglog.close();
                  }
                  if(Map[temp2] == "DOMImplementation"){
                    mappinglog << "found DOMImplementation return";
                    trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{Document* sample=this;sample->initialize();DOMImplementation* prova=DOMImplementation::create(*sample);\breturn *prova;\b}\b");
                    mappinglog.close();
                  }
                  if(Map[temp2] == "FloatRect"){
                    mappinglog << "found FloatRect return";
                    trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn FloatRect();\b}\b");
                    mappinglog.close();
                  }
                  if(Map[temp2] == "boolean"){
                    mappinglog << "found boolean return";
                    trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn true;\b}\b");
                    mappinglog.close();
                  }
                  if(Map[temp2] == "Uint8Array"){
                    mappinglog << "found boolean return";
                    trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{unsigned char* sample\breturn DOMUint8Array::create(sample, 0);\b}\b");
                    mappinglog.close();
                  }
                  if(Map[temp2] == "TextTrack"){
                    mappinglog << "found TextTrack return";
                    trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn TextTrack::create("", "", "");\b}\b");
                    mappinglog.close();
                  }
                  if(Map[temp2] == "AtomicString&"){
                    mappinglog << "found AtomicString& return";
                    trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn nullAtom;\b}\b");
                    mappinglog.close();
                  }
                  if(Map[temp2] =="GLboolean"){
                    mappinglog << "found GLboolean return";
                    trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn 0;\b}\b");
                    mappinglog.close();
                  }
                  if(Map[temp2] == "Comment"){
                    mappinglog << "found Comment return";
                    trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn Comment::create(*this, \"\");\b}\b");
                    mappinglog.close();
                  }
                  if(std::find(null.begin(), null.end(), Map[temp2]) != null.end()){
                    mappinglog << "found nullptr return";
                    trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn nullptr;\b}\b");
                    mappinglog.close();
                  }
                  if(Map[temp2] == "BarProp"){
                    mappinglog << "found BarProp return";
                    trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn BarProp::create(nullptr, BarProp::Menubar);\b}\b");
                    mappinglog.close();
                  }
                  if(Map[temp2] == "DispatchEventResult"){
                    mappinglog << "found DispatchEventResult return";
                    trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn DispatchEventResult::CanceledByDefaultEventHandler;\b}\b");
                    mappinglog.close();
                  }
                  if(Map[temp2] == "DOMMatrix"){
                    mappinglog << "found DOMMatrix return";
                    trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn this;\b}\b");
                    mappinglog.close();
                  }
                  if(std::find(promise.begin(), promise.end(), Map[temp2]) != promise.end()){
                    mappinglog << "found Promise return";
                    trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn ScriptPromise();\b}\b");
                    mappinglog.close();
                  }
                  if(Map[temp2] == "StaticNodeList"){
                    mappinglog << "found StaticNodeList return";
                    trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn StaticNodeList::createEmpty();\b}\b");
                    mappinglog.close();
                  }
                  if(Map[temp2] == "sequence<MediaStreamTrack>"){
                    mappinglog << "found sequence<MediaStreamTrack> return";
                    trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{MediaStreamTrackVector sample;\breturn sample;\b}\b");
                    mappinglog.close();
                  }
                  if(Map[temp2] == "PerformanceEntryList"){
                    mappinglog << "found PerformanceEntryList return";
                    trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{PerformanceEntryVector sample;\breturn sample;\b}\b");
                    mappinglog.close();
                  }
                  if(Map[temp2] == "TagCollection"){
                    mappinglog << "found TagCollection return";
                    trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn ensureCachedCollection<TagCollection>(TagCollectionType,nullAtom,localName);\b}\b");
                    mappinglog.close();
                  }
                  if(Map[temp2] == "MediaStreamVector"){
                    mappinglog << "found MediaStreamVector return";
                    trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{MediaStreamVector sample;\breturn sample;\b}\b");
                    mappinglog.close();
                  }
                  if(Map[temp2] == "ClientRectList"){
                    mappinglog << "found ClientRectList return";
                    trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn ClientRectList::create();\b}\b");
                    mappinglog.close();
                  }
                  if(Map[temp2] == "CanvasGradient"){
                    mappinglog << "found CanvasGradient return";
                    trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn CanvasGradient::create(FloatPoint(0,0), FloatPoint(0,0));\b}\b");
                    mappinglog.close();
                  }
                  if(Map[temp2] == "vector"){
                    mappinglog << "found vector return";
                    trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn std::vector<rtc::scoped_refptr<RtpSenderInterface>> sample;\b}\b");
                    mappinglog.close();
                  }
                  if(Map[temp2] == "std::vector<rtc::scoped_refptr<RtpReceiverInterface>>"){
                    mappinglog << "found std::vector<rtc::scoped_refptr<RtpReceiverInterface>> return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn std::vector<rtc::scoped_refptr<RtpReceiverInterface>> sample;\b}\b");	
                    mappinglog.close();
                  }
                  if(Map[temp2] == "sequence<unrestricted double>"){
                    mappinglog << "found sequence<unrestricted double> return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn CanvasRenderingContext2DState::create()->lineDash();;\b}\b");	
                    mappinglog.close();
                  }
                  if(Map[temp2] == "sequence<USVString>"){
                    mappinglog << "found sequence<USVString> return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{Vector<String> sample;\breturn sample;\b}\b");	
                    mappinglog.close();
                  }
                  if(Map[temp2] == "sequence<MutationRecord>"){
                    mappinglog << "found sequence<MutationRecord> return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{MutationRecordVector sample;\breturn sample;\b}\b");	
                    mappinglog.close();
                  }
                  if(Map[temp2] == "sequence<FormDataEntryValue>"){
                    mappinglog << "found sequence<FormDataEntryValue> return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{HeapVector<FormDataEntryValue> sample;\breturn sample;\b}\b");	
                    mappinglog.close();
                  }
                  if(Map[temp2] == "sequence<ByteString>"){
                    mappinglog << "found sequence<ByteString> return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn Vector<String>();\b}\b");	
                    mappinglog.close();
                  }
                  if(Map[temp2] == "XPathExpression"){
                    mappinglog << "found XPathExpression return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn XPathExpression::create();\b}\b");	
                    mappinglog.close();
                  }
                  if(Map[temp2] == "VideoPlaybackQuality"){
                    mappinglog << "found VideoPlaybackQuality return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn VideoPlaybackQuality::create(videoElement.document(),0,0,0);\b}\b");	
                    mappinglog.close();
                  }
                  if(Map[temp2] == "USVString"){
                    mappinglog << "found USVString return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn String();\b}\b");	
                    mappinglog.close();
                  }
                  if(Map[temp2] == "TextMetrics"){
                    mappinglog << "found TextMetrics return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn TextMetrics::create();\b}\b");	
                    mappinglog.close();
                  }
                  if(Map[temp2] == "ScriptValue"){
                    mappinglog << "found ScriptValue return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn ScriptValue::createNull(scriptState);\b}\b");	
                    mappinglog.close();
                  }
                  if(Map[temp2] == "SVGNumber"){
                    mappinglog << "found SVGNumber return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn SVGNumberTearOff::create(SVGNumber::create(0.0f), 0,PropertyIsNotAnimVal);\b}\b");	
                    mappinglog.close();
                  }
                  if(Map[temp2] == "SVGLength"){
                    mappinglog << "found SVGLength return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn SVGLengthTearOff::create(SVGLength::create(), 0, PropertyIsNotAnimVal);\b}\b");	
                    mappinglog.close();
                  }
                  if(Map[temp2] == "SVGAngle"){
                    mappinglog << "found SVGAngle return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn SVGAngleTearOff::create(SVGAngle::create(), 0, PropertyIsNotAnimVal);\b}\b");	
                    mappinglog.close();
                  }
                  if(Map[temp2] == "PeriodicWave"){
                    mappinglog << "found PeriodicWave return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{ DOMFloat32Array* sample=nullptr;DOMFloat32Array* sample2=nullptr;\breturn PeriodicWave::create(*this,sample,sample2 , false, exceptionState);\b}\b");	
                    mappinglog.close();
                  }
                  if(Map[temp2] == "ItemPropertyType"){
                    mappinglog << "found ItemPropertyType return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn ItemProperty* sample;\b}\b");	
                    mappinglog.close();
                  }
                  if(Map[temp2] == "GamepadList"){
                    mappinglog << "found GamepadList return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn GamepadList::create();\b}\b");	
                    mappinglog.close();
                  }
                  if(Map[temp2] == "Float64Array"){
                    mappinglog << "found Float64Array return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{double array[]={};\breturn DOMFloat64Array::create(array, 16);\b}\b");	
                    mappinglog.close();
                  }
                  if(Map[temp2] == "DOMString[]"){
                    mappinglog << "found DOMString[] return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{Vector<String> sample\breturn sample;\b}\b");	
                    mappinglog.close();
                  }
                  if(Map[temp2] == "CanPlayTypeResult"){
                    mappinglog << "found CanPlayTypeResult return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn emptyString();\b}\b");	
                    mappinglog.close();
                  }
                  if(Map[temp2] == "CSSValue&"){
                    mappinglog << "found CSSValue& return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn HeapVector<CSSValue, 4> sample;\b}\b");	
                    mappinglog.close();
                  }
                  if(Map[temp2] == "AudioBufferSourceNode"){
                    mappinglog << "found AudioBufferSourceNode return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{AudioBufferSourceNode* sample=nullptr;\breturn sample;\b}\b");	
                    mappinglog.close();
                  }
                  if(Map[temp2] == "AudioBuffer"){
                    mappinglog << "found ArrayBuffer return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{AudioBuffer* sample=nullptr;\breturn samples;\b}\b");	
                    mappinglog.close();
                  }
                  if(Map[temp2] == "SVGMatrixTearOff"){
                    mappinglog << "found SVGMatrixTearOff return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{\breturn SVGMatrixTearOff::create(AffineTransform());\b}\b");	
                    mappinglog.close();
                  }
                  if(Map[temp2] == "AffineTransform"){
                    mappinglog << "found AffineTransform return";
                  	trace_full = std::string("\bif(DeprecatorClass::GetInstance()->Check(\"") + std::string(fun_id) + std::string("\"))\b{AffineTransform sample\breturn sample;\b}\b");	
                    mappinglog.close();
                  }
                }   
                else //not found in a map!!!
                {
                    mappinglog << "Not found associated returning in Map";
                    mappinglog.close();
                }
                 
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

void Returning( std::map<std::string, std::string> *Map ){
    ifstream returntarget;
    returntarget.open("/tmp/return_target.txt");
    std::string line;
    while(std::getline(returntarget,line)){
       if (line.length() < 2) {
            continue;
        }
        char array[250];
        strncpy(array,line.c_str(),line.length());
        bool first=true;
        int temp=0;
        char comp;
        std::string key = "";
        for(int i=0;i<line.length();i++){
          comp=array[i];
          if((comp == ':') & !first){
            for (int j=0; j<i; j++)
            {
                key=key+array[j];
            }
            temp=i;
            break;
          }
          if(comp == ':'){
            first=false;
          }
        } 
        string value = "";
        // add +1 to skip the : after the name of the function 
        for(temp=temp+1;temp<line.length();temp++){
            value=value+array[temp];
        }

      Map->insert(std::pair<std::string, std::string>(key,value));
    }
  ofstream printmap;
  printmap.open("/tmp/printmap.txt", std::ios_base::app);
  for(std::map<std::string, std::string>::iterator itera = Map->begin(); itera != Map->end(); ++itera)
  {
    printmap << "element:" << "\n";
    printmap << itera->first << "\n";
    printmap << itera->second <<"\n";
  }
  printmap.close();
}


int main(int argc, const char* argv[]) {
    llvm::cl::OptionCategory category("Deprecator logging tool");

    get_targets(&target_vect);
    Returning(&Map);


    CommonOptionsParser options(argc, argv, category);
    clang::tooling::RefactoringTool tool(options.getCompilations(), options.getSourcePathList());
    std::string include_replacement =  "#include \"base/DeprecatorClass/DeprecatorClass.h\"\b";
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

