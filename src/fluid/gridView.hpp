#pragma once
#include "particles.hpp"
#include "grid.hpp"
#include "adjacentParticles.hpp"
#include <iostream> 
#include <map>

class GridView { 
public:
  using ParticlesView = Grid::ParticlesView;
public:
  GridView(Vec &boundSize, float &smoothingRadius): 
    grid(boundSize, smoothingRadius)
    {}
  void gridify(Particles &ps) { grid.gridify(ps); }

  const ParticlesView *cellCollection(const Vec &p) const {
    return grid.viewCell(p);
  }

  AdjacentParticles adj(const Vec &p) const { 
    return AdjacentParticles(p, grid);
  }
private:
  Grid grid;
};
