#!/bin/bash

# This is just an example using "third_party/WebKit/Source/core/frame" - set it up to work with all relevant directories

pushd third_party/WebKit/
git checkout .
popd
pushd v8/src/inspector/
git checkout .
popd
pushd third_party/webrtc/
git checkout .
popd
pushd out/Default/gen/blink/core/frame/
git checkout .
popd


INSTRUMENT="third_party/WebKit/ v8/src/inspector/ third_party/webrtc/ out/Default/gen/blink/core/frame/"

time tools/clang/scripts/update.py --bootstrap --force-local-build --without-android --tools deprecator_logging || exit 1
tools/clang/scripts/run_tool.py deprecator_logging out/Default $INSTRUMENT || exit 1

time ninja -C out/Default chrome || exit 1

