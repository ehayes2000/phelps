#pragma once
#include "particle.hpp"
#include <iostream> 
#include <list>

class AdjacentParticles;

class GridView { 
public:
  friend AdjacentParticles;
  using Particles = std::vector<Particle>;
  using ParticlesView = std::list<Particle*>;
public:
  GridView(Vec boundSize, float smoothingRadius): 
    cellSize(smoothingRadius),
    xCells(boundSize.x / smoothingRadius),
    yCells(boundSize.y / smoothingRadius),
    nCells((boundSize.x / smoothingRadius) * (boundSize.y / smoothingRadius))
    {}
  void gridify(Particles &);
  AdjacentParticles adj(const Particle &) const;
private:
  float cellSize;
  int nCells;
  int xCells;
  int yCells;
  std::unordered_map<int, ParticlesView> cells;
  int hashCell(const Vec &) const;
};