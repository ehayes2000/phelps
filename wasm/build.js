import * as esbuild from 'esbuild'

let result = await esbuild.build({
  entryPoints: ['pkg/index.js'],
  bundle: true,
  outfile: 'dist/out.js',
  minify: true,
  globalName: "Fluid"
})