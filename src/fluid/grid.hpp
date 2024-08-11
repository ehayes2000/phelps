#pragma once
#include "vec.hpp"
#include "particle.hpp"
#include <vector>
#include <map>
#include <iostream>

class Grid { 
public:
  using Particles = std::vector<Particle>;
  using ParticlesView = std::vector<Particle*>;
public:
  Grid(Vec &boundSize, float &cellSize): 
    cellSize(cellSize),
    nCells((boundSize.x / cellSize) * (boundSize.y / cellSize)),
    adjacentOffsets{
      Vec(-1, -1) * cellSize, Vec(0, -1) * cellSize, Vec(1, -1) * cellSize,
      Vec(-1, 0) * cellSize,  Vec(0, 0) * cellSize,  Vec(1, 0) * cellSize,
      Vec(-1, 1) * cellSize,  Vec(0, 1) * cellSize,  Vec(1, 1) * cellSize
      }
  {
  }
  const ParticlesView* viewCell(const Vec &) const;
  void gridify(Particles &);
  static const unsigned short ADJACENT_CELLS = 9;
  const Vec adjacentOffsets[ADJACENT_CELLS];
  const float cellSize;
  const int nCells;
  int hashCell(const Vec &) const;
private: 
  std::unordered_map<int, ParticlesView> cells;
};