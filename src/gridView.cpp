#include "gridView.hpp"
#include "adjacentParticles.hpp"

static const unsigned int k1 = 15823;
static const unsigned int k2 = 9737333;

int GridView::hashCell(const Vec &p) const { 
  int xCord = p.x / cellSize;
  int yCord = p.y / cellSize;
  return (k1 * xCord ^ k2 * yCord) % nCells;
}

void GridView::gridify(Particles &particles) { 
  cells.clear();
  for (Particle &p: particles){
    cells[hashCell(p.position)].push_back(&p);
  }
}

AdjacentParticles GridView::adj(const Particle &p) const {
  return AdjacentParticles(p.position, *this);
}