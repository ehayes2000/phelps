#!/bin/bash
set -e 
cd wasm
bash build.sh
cd ..
cp wasm/dist/index.html index.html
cp wasm/dist/out.js out.js