#include "adjacentParticles.hpp"

const AdjacentParticles::iterator::ParticlesView*
AdjacentParticles::iterator::getCollection() const { 
  Vec offsetPoint = adjacent.point + adjacent.grid.adjacentOffsets[offset_i];
  return adjacent.grid.viewCell(offsetPoint);
}

void AdjacentParticles::iterator::init() { 
  particle_i = 0;
  while (offset_i < Grid::ADJACENT_CELLS) {
    const ParticlesView* collection = getCollection();
    if (collection && !collection->empty()){
      return;
    }
    offset_i++;
  }
}

void AdjacentParticles::iterator::next() { 
  if (offset_i >= Grid::ADJACENT_CELLS)
    return;
  const ParticlesView* collection = getCollection();
  if (collection && ++particle_i < collection->size())
    return;
  particle_i = 0;
  do {
    offset_i ++;
    collection = getCollection();
  }
  while (offset_i < Grid::ADJACENT_CELLS && (!collection  || collection->empty()));
}

