#pragma once

class Fluid{
public:
  int steps;
  int size;
  float dt;
  float diff;
  float visc;
    
  float *s;
  float *density;
    
  float *Vx;
  float *Vy;

  float *Vx0;
  float *Vy0;

  Fluid(int size, float diffusion, float viscosity, float dt):
    size(size),
    dt(dt),
    diff(diffusion),
    visc(viscosity),
    s(new float[size * size]),
    density(new float[size * size]), 
    Vx(new float[size * size]),
    Vy(new float[size * size]),
    Vx0(new float[size * size]),
    Vy0(new float[size * size]),
    steps(0)
  {}
  ~Fluid() {
    delete[] s;
    delete[] density;
    delete[] Vx;
    delete[] Vy;
    delete[] Vx0;
    delete[] Vy0;
  }

  void addDensity(int x, int y, float amount);
  void addVelocity(int x, int y, float amountX, float amountY);
  void step();

private:
  inline int idx2d(int x, int y){
    return (y * size) + x;
  }
  void diffuse(int b, float *x, float *x0, float diff, float dt, int iter);
  void project(float *velocX, float *velocY, float *p, float *div, int iter);
  void advect(int b, float *d, float *d0,  float *velocX, float *velocY, float dt);
  void setBnd(int b, float *x);
  void linSolve(int b, float *x, float *x0, float a, float c, int iter); 
};
