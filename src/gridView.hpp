#pragma once
#include "particle.hpp"
#include <iostream> 
#include <map>

class AdjacentParticles;
class GridView { 
public:
  friend AdjacentParticles;
  using Particles = std::vector<Particle>;
  using ParticlesView = std::vector<Particle*>;
public:
  GridView(Vec boundSize, float smoothingRadius): 
    cellSize(smoothingRadius),
    nCells((boundSize.x / smoothingRadius) * (boundSize.y / smoothingRadius))
    {}
  void gridify(Particles &);
  AdjacentParticles adj(const Particle &) const;
private:
  float cellSize;
  int nCells;
  std::unordered_map<int, ParticlesView> cells;
  int hashCell(const Vec &) const;
};


