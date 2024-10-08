cd wasm 
CURRENT_BRANCH=$(git rev-parse --abbrev-ref HEAD)
set -e
bash build.sh
git checkout prod
git checkout "$CURRENT_BRANCH" -- wasm/dist
cp wasm/dist/index.html index.html
cp wasm/dist/out.js out.js
git add -A
MESSAGE=$(git log -1 "$CURRENT_BRANCH")
git commit -m "Publish page\n$MESSAGE"
git push 