rm -rf dist build 
mkdir dist build
cd build 
emcmake cmake ../..
emmake make
mv phelps.mjs phelps.js
cd ..
node bundle.js
cp index.html dist/index.html