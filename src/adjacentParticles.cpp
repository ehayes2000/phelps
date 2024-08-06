#include "adjacentParticles.hpp"

const AdjacentParticles::iterator::ParticlesView*
AdjacentParticles::iterator::getCollection() const { 
  Vec offsetPoint = adjacent.point + 
    ADJACENT_OFFSETS[offset_i] * adjacent.grid.cellSize;
  int cellId = adjacent.grid.hashCell(offsetPoint);
  if (adjacent.grid.cells.find(cellId) == adjacent.grid.cells.end())
    return nullptr;
  return &adjacent.grid.cells.at(cellId);
}

void AdjacentParticles::iterator::init() { 
  particle_i = 0;
  while (offset_i < ADJACENT_CELLS) {
    const ParticlesView* collection = getCollection();
    if (collection && !collection->empty()){
      return;
    }
    offset_i++;
  }
}

void AdjacentParticles::iterator::next() { 
  while (offset_i < ADJACENT_CELLS){
    const ParticlesView* collection = getCollection();
    // collection exists and is not iterated through
    if (collection && particle_i + 1 < collection->size()){
      particle_i ++;
      return;
    }
    else { 
      offset_i ++;
      particle_i = 0;
    }
  }
}

