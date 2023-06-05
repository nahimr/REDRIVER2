#!/usr/bin/env bash
set -ex

cd "$APPVEYOR_BUILD_FOLDER/.build/build"

for config in Release Debug RelWithDebInfo
do
    make package
done
