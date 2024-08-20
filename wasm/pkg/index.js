import Phelps from "./phelps"

let first = true;
export async function showAnimation(canvasId) { 
  const canvas = document.getElementById(canvasId);
  let modArgs;
  if (first){
    console.log("FIRST")
    modArgs = await Phelps({ canvas: canvas, wasmMemory: memory1});
    console.log(modArgs.wasmMemory);
  }
  else{
    console.log("LAST")
    modArgs = await Phelps({ canvas: canvas, wasmMemory: memory2});
    console.log(modArgs.wasmMemory);
  }
  first = false
  console.log("crashed ?????");
  // await module(modArgs);
  console.log(" Hehe xd");
  // await module({ canvas });
}
