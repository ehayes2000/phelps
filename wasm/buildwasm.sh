rm -rf dist
mkdir dist 
cd dist
emcmake cmake ../..
emmake make
cp phelps.mjs ../pkg/phelps/phelpss.js