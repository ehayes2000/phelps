#pragma once
#include "particle.hpp"
#include "grid.hpp"
#include "adjacentParticles.hpp"
#include <iostream> 
#include <map>

class GridView { 
public:
  using Particles = std::vector<Particle>;
  using ParticlesView = std::vector<Particle*>;
public:
  GridView(Vec &boundSize, float &smoothingRadius): 
    grid(boundSize, smoothingRadius)
    {}
  void gridify(Particles &ps) { grid.gridify(ps); }

  const ParticlesView *cellCollection(const Vec &p) const {
    return grid.viewCell(p);
  }

  AdjacentParticles adj(const Particle &p) const { 
    return AdjacentParticles(p.position, grid);
  }
  AdjacentParticles adj(const Vec &p) const { 
    return AdjacentParticles(p, grid);
  }
private:
  Grid grid;
};
