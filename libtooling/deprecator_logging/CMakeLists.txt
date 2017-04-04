set(LLVM_LINK_COMPONENTS
  BitReader
  MCParser
  Option
  )

add_llvm_executable(deprecator_logging
  DeprecatorLogging.cpp
  )

target_link_libraries(deprecator_logging
  clangAST
  clangASTMatchers
  clangAnalysis
  clangBasic
  clangDriver
  clangEdit
  clangFrontend
  clangLex
  clangParse
  clangSema
  clangSerialization
  clangTooling
  )

cr_install(TARGETS deprecator_logging RUNTIME DESTINATION bin)
