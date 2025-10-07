#!/bin/bash
cd "$(dirname "$0")"
rm -rf ../release
mkdir -p ../release
cd ../release  
cmake ../cmake -DCMAKE_BUILD_TYPE=RELEASE
#ln -s cmake/compile_commands.json ../   > /dev/null 2>&1 
make
cd ../cmake/
JLinkExe -commanderscript flash.jlink
