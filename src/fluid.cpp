#include <iostream>
#include <math.h>
#include "fluid.hpp"

void Fluid::addDensity(int x, int y, float amount)
{
    this->density[idx2d(x, y)] += amount;
}

void Fluid::addVelocity(int x, int y, float amountX, float amountY)
{
    int i = idx2d(x, y);
    this->Vx[i] += amountX;
    this->Vy[i] += amountY;
}

void Fluid::step()
{
    this->diffuse(
        1,
        this->Vx0,
        this->Vx,
        this->visc,
        this->dt,
        4);

    this->diffuse(
        2,
        this->Vy0,
        this->Vy,
        this->visc,
        this->dt,
        4);

    this->project(
        this->Vx0,
        this->Vy0,
        this->Vx,
        this->Vy,
        4);

    this->advect(
        1,
        Vx,
        Vx0,
        Vx0,
        Vy0,
        dt);

    this->advect(
        2,
        Vy,
        Vy0,
        Vx0,
        Vy0,
        dt);
    project(
        Vx,
        Vy,
        Vx0,
        Vy0,
        4);

    diffuse(
        0,
        s,
        density,
        diff,
        dt,
        4);
    advect(
        0,
        density,
        s,
        Vx,
        Vy,
        dt);
}

void Fluid::diffuse(int b, float *x, float *x0, float diff, float dt, int iter)
{
    float a = dt * diff * (this->size - 2) * (this->size - 2);
    this->linSolve(b, x, x0, a, 1 + 6 * a, iter);
}

void Fluid::setBnd(int b, float *x)
{
    for (int i = 1; i < this->size - 1; i++)
    {
        x[this->idx2d(i, 0)] = b == 2 ? -x[this->idx2d(i, 1)] : x[this->idx2d(i, 1)];
        x[this->idx2d(i, this->size - 1)] = b == 2 ? -x[this->idx2d(i, this->size - 2)] : x[this->idx2d(i, this->size - 2)];
    }
    for (int j = 1; j < this->size - 1; j++)
    {
        x[this->idx2d(0, j)] = b == 1 ? -x[this->idx2d(1, j)] : x[this->idx2d(1, j)];
        x[this->idx2d(this->size - 1, j)] = b == 1 ? -x[this->idx2d(this->size - 2, j)] : x[this->idx2d(this->size - 2, j)];
    }
    x[this->idx2d(0, 0)] = 0.5f * (x[this->idx2d(1, 0)] + x[this->idx2d(0, 1)]);
    x[this->idx2d(0, this->size - 1)] = 0.5f * (x[this->idx2d(1, this->size - 1)] + x[this->idx2d(this->size - 2, 0)]);
    x[this->idx2d(this->size - 1, 0)] = 0.5f * (x[this->idx2d(this->size - 2, 0)] + x[this->idx2d(this->size - 1, 0)]);
    x[this->idx2d(this->size, this->size - 1)] = 0.5f * (x[this->idx2d(this->size - 2, this->size - 1)] + x[this->idx2d(this->size - 1, this->size - 2)]);
}

void Fluid::linSolve(int b, float *x, float *x0, float a, float c, int iter)
{
    float cRecip = 1.0 / c;
    for (int m = 0; m < iter; m++)
    {
        for (int j = 1; j < this->size - 1; j++)
        {
            for (int i = 1; i < this->size - 1; i++)
            {
                x[this->idx2d(i, j)] =
                    (x0[this->idx2d(i, j)] +
                     a *
                         (x[this->idx2d(i + 1, j)] +
                          x[this->idx2d(i - 1, j)] +
                          x[this->idx2d(i, j + 1)] +
                          x[this->idx2d(i, j - 1)])) *
                    cRecip;
            }
        }
        this->setBnd(b, x);
    }
}

void Fluid::project(float *velocX, float *velocY, float *p, float *div, int iter)
{
    for (int j = 1; j < this->size - 1; j++)
    {
        for (int i = 1; i < this->size - 1; i++)
        {
            div[this->idx2d(i, j)] = -0.5f * (velocX[this->idx2d(i + 1, j)] - velocX[this->idx2d(i - 1, j)] + velocY[this->idx2d(i, j + 1)] - velocY[this->idx2d(i, j - 1)]) /
                                     this->size;
            p[this->idx2d(i, j)] = 0;
        }
    }
    this->setBnd(0, div);
    this->setBnd(0, p);
    this->linSolve(0, p, div, 1, 6, iter);

    for (int j = 1; j < this->size - 1; j++)
    {
        for (int i = 1; i < this->size - 1; i++)
        {
            velocX[this->idx2d(i, j)] -= 0.5f * (p[this->idx2d(i + 1, j)] - p[this->idx2d(i - 1, j)]) * this->size;
            velocY[this->idx2d(i, j)] -= 0.5f * (p[this->idx2d(i, j + 1)] - p[this->idx2d(i, j - 1)]) * this->size;
        }
    }
    this->setBnd(1, velocX);
    this->setBnd(2, velocY);
}

void Fluid::advect(int b, float *d, float *d0, float *velocX, float *velocY, float dt)
{
    float i0, i1, j0, j1;

    float dtx = dt * (this->size - 2);
    float dty = dt * (this->size - 2);

    float s0, s1, t0, t1, u0, u1;
    float tmp1, tmp2, tmp3, x, y;

    float Nfloat = this->size;
    float ifloat, jfloat, kfloat;
    int i, j, k;

    for (j = 1, jfloat = 1; j < this->size - 1; j++, jfloat++)
    {
        for (i = 1, ifloat = 1; i < this->size - 1; i++, ifloat++)
        {
            tmp1 = dtx * velocX[this->idx2d(i, j)];
            tmp2 = dty * velocY[this->idx2d(i, j)];
            x = ifloat - tmp1;
            y = jfloat - tmp2;

            if (x < 0.5f)
                x = 0.5f;
            if (x > Nfloat + 0.5f)
                x = Nfloat + 0.5f;
            i0 = floorf(x);
            i1 = i0 + 1.0f;
            if (y < 0.5f)
                y = 0.5f;
            if (y > Nfloat + 0.5f)
                y = Nfloat + 0.5f;
            j0 = floorf(y);
            j1 = j0 + 1.0f;

            s1 = x - i0;
            s0 = 1.0f - s1;
            t1 = y - j0;
            t0 = 1.0f - t1;

            int i0i = i0;
            int i1i = i1;
            int j0i = j0;
            int j1i = j1;

            d[this->idx2d(i, j)] =
                s0 * (t0 * d0[this->idx2d(i0i, j0i)] + t1 * d0[this->idx2d(i0i, j1i)]) +
                s1 * (t0 * d0[this->idx2d(i1i, j0i)] + t1 * d0[this->idx2d(i1i, j1i)]);
        }
    }
    this->setBnd(b, d);
}
