import * as esbuild from 'esbuild'

let result = await esbuild.build({
  entryPoints: ['pkg/index.js'],
  bundle: true,
  // format: "esm",
  outfile: 'dist/out.js',
  // minify: true,
  globalName: "Fluid"
})
console.log(result)