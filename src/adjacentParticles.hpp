#pragma once
#include "adjacentParticles.hpp"
#include "gridView.hpp"
#include "particle.hpp"
#include "vec.hpp"

static const short ADJACENT_CELLS = 9;
static const Vec ADJACENT_OFFSETS[ADJACENT_CELLS] = {
  Vec(-1, -1), Vec(0, -1), Vec(1, -1),
  Vec(-1, 0),  Vec(0, 0), Vec(1, 0),
  Vec(-1, 1),  Vec(0, 1), Vec(1, 1)
};


class AdjacentParticles
{
private:
  const Vec &point;
  const GridView &grid;
public:
  class iterator
  {
  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = int;
    using value_type = Particle;
    using pointer = Particle *;
    using reference = Particle &;
    using ParticlesView = GridView::ParticlesView;

  public:
    explicit iterator(
      AdjacentParticles &adjacent, 
      unsigned short int offset_i,
      unsigned int particle_i
    ): 
      adjacent(adjacent),
      offset_i(offset_i),
      particle_i(particle_i)
    {
      init();
    }

    iterator& operator++() { next(); return *this; }
    iterator operator++(int) { iterator retval = *this; ++(*this); return retval; }
    reference operator*() const {
      const ParticlesView* particles = getCollection();
      return *(*particles)[particle_i];
    }
    reference operator->() const { 
      return *(*this);
    }

    bool operator==(const iterator &it) const { 
      return adjacent.point == it.adjacent.point && offset_i == it.offset_i && particle_i == it.particle_i; 
    }
    bool operator!=(const iterator &it) const { return !(*this==it); }

  private:
    AdjacentParticles &adjacent;
    short unsigned int offset_i;
    unsigned int particle_i;
    void next();
    void init();
    const ParticlesView* getCollection() const;
  };

public:
  AdjacentParticles(const Vec &point, const GridView &grid): 
    point(point), 
    grid(grid)
  {}
    
  iterator begin() { 
    return iterator(*this, 0, 0);
  }
  iterator end() { 
    return iterator(*this, ADJACENT_CELLS, 0);
  }
};