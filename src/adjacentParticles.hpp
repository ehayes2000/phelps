#pragma once
#include "gridView.hpp"
#include "vec.hpp"

static const short ADJACENT_CELLS = 9;
static const Vec ADJACENT_OFFSETS[ADJACENT_CELLS] = {
  Vec(-1, -1), Vec(0, -1), Vec(1, -1),

  Vec(-1, 0),  Vec(0, 0),  Vec(1, 0),

  Vec(-1, 1),  Vec(0, 1),  Vec(1, 1)
};


class AdjacentParticles
{
public:
  using iterator_category = std::forward_iterator_tag;
  using difference_type   = int;
  using value_type        = Particle;
  using pointer           = Particle *;
  using reference         = Particle &;
  using ParticlesView     = GridView::ParticlesView;
public:
  AdjacentParticles(const Vec& point, GridView &grid): 
    point(point),
    grid(grid),
    currentCollection(nullptr),
    offset_i(0),
    isEnd(false)
    {}

  AdjacentParticles& operator++() { 
    getNextValidIter();
    return *this;
  }

  AdjacentParticles operator++(int){
    AdjacentParticles retval = *this;
    ++(*this);
    return retval;
  }

  reference operator*() const { 
    return **iter;
  }
   
  bool operator==(AdjacentParticles &p) const {
    if (isEnd && p.isEnd){
      return true;
    }
    else if (isEnd && !p.isEnd || !isEnd && p.isEnd) {
      return false;
    }
    return iter == p.iter;
  }

  bool operator!=(AdjacentParticles &p) const {
    return !(*this == p);
  }

private:
  const Vec& point;
  GridView &grid;
  ParticlesView * currentCollection;
  ParticlesView::iterator iter;
  short unsigned int offset_i;
  bool isEnd; 

  // get the collection at offset_i
  // no range checking :)
  ParticlesView *getNextCollection(){
    Vec offset = ADJACENT_OFFSETS[offset_i] * grid.cellSize;
    Vec offsetPoint = point + offset;
    int cellHash = grid.hashCell(offsetPoint);
    if (grid.cells.find(cellHash) != grid.cells.end()){
      offset_i ++;
      return &grid.cells[cellHash];
    } else { 
      return nullptr;
    }
  }

  void getNextValidIter() {
    if (currentCollection && iter != currentCollection->end()){
      iter++;
      return;
    }
    // current collection is none or done
    while (offset_i < ADJACENT_CELLS) { 
      ParticlesView *ps = getNextCollection();
      if (ps != nullptr){ 
        currentCollection = ps;
        iter = currentCollection->begin();
        return;
      }
    }
    isEnd = true;
  }
};