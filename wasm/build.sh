rm -rf dist build 
mkdir dist build
cd build 
emcmake cmake ../..
emmake make
cp phelps.mjs ../pkg/phelps/phelpss.js
cd ..
node bundle.js
cp index.html dist/index.html