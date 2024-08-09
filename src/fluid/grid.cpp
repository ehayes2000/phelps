#include "grid.hpp"
#include <iostream>
#include <cassert>

static const unsigned int k1 = 15823;
static const unsigned int k2 = 9737333;

const Grid::ParticlesView* Grid::viewCell(const Vec &point) const {
  int cellId = hashCell(point);
  if (cells.count(cellId) == 0)
    return nullptr;
  return &cells.at(cellId);
}

int Grid::hashCell(const Vec &p) const { 
  int xCord = p.x / cellSize;
  int yCord = p.y / cellSize;
  return ((k1 * xCord) ^ (k2 * yCord)); //% nCells;
}

void Grid::gridify(Particles &particles) { 
  cells.clear();
  for (Particle &p: particles){
    int hash = hashCell(p.position);
    cells[hash].push_back(&p);
  }
}
