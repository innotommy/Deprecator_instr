#!/bin/bash

# This is just an example using "third_party/WebKit/Source/core/frame" - set it up to work with all relevant directories

pushd third_party/WebKit/Source/core/frame
git checkout .
popd

INSTRUMENT="third_party/WebKit/Source/core/frame"

time tools/clang/scripts/update.py --bootstrap --force-local-build --without-android --tools deprecator_logging || exit 1
tools/clang/scripts/run_tool.py deprecator_logging out/Default $INSTRUMENT

time ninja -C out/Default chrome || exit 1

