#pragma once
#include "grid.hpp"
#include "particles.hpp"
#include "vec.hpp"

class AdjacentParticles
{
private:
  const Vec &point;
  const Grid &grid;
public:
  class iterator
  {
  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = unsigned int;
    using value_type = unsigned int;
    using pointer = unsigned int*;
    using reference = unsigned int;
    using ParticlesView = Grid::ParticlesView;

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
      const ParticlesView* view = getCollection();
      return view->operator[](particle_i);
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
  AdjacentParticles(const Vec &point, const Grid &grid): 
    point(point), 
    grid(grid)
  {
  }
    
  iterator begin() { 
    return iterator(*this, 0, 0);
  }
  iterator end() { 
    return iterator(*this, Grid::ADJACENT_CELLS, 0);
  }
};