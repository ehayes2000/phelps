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
  while (offset_i < Grid::ADJACENT_CELLS){
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

