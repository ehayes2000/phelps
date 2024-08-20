import Phelps from "./phelps"

export async function showAnimation(canvasId) { 
  const canvas = document.getElementById(canvasId);
  if (!canvas){
    console.error("canvas not found!");
    return;
  }
  document.addEventListener('contextmenu', event => event.preventDefault());
  let modArgs;
  modArgs = await Phelps({ canvas: canvas });
}
