#pragma once
#include "vec.hpp"
#include <vector>

class Particles { 
public: 
  Particles(const int n)
    : size(n),
      positions(n),
      prevPositions(n),
      velocities(n)
  {}

  Particles(const std::vector<Vec> &positions, 
            const std::vector<Vec> &prevPositions,
            const std::vector<Vec> &velocities)
    : size(positions.size()),
      positions(positions),
      prevPositions(prevPositions),
      velocities(velocities) {}
  int size; 
  std::vector<Vec> positions;
  std::vector<Vec> prevPositions;
  std::vector<Vec> velocities;
};
