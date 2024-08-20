import * as esbuild from 'esbuild'

await esbuild.build({
  entryPoints: ['pkg/index.js'],
  bundle: true,
  outfile: 'dist/out.js',
  minify: true,
  globalName: "Fluid"
})
