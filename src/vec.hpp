#pragma once
#include <cmath>
#include <ostream>

class Vec { 
public:
  float x;
  float y;
  Vec(): x(0), y(0){}
  Vec(float x, float y):x(x), y(y){}
  
  Vec& operator=(const Vec& v) { 
    x = v.x;
    y = v.y;
    return *this;
  }
  Vec operator/(const float& s) const { 
    return Vec( 
      x / s,
      y / s
    );
  }
  Vec operator-(const Vec& rhs) const {
    return Vec(
      x - rhs.x,
      y - rhs.y
    );
  }
  Vec operator+(const Vec& rhs) const { 
    return Vec (
      x + rhs.x,
      y + rhs.y
    );
  }
  Vec operator*(const float &s) const {
    return Vec(
      x * s,
      y * s
    );
  }
  friend Vec operator*(const float &s, const Vec& v) { 
    return Vec(
      v.x *s,
      v.y *s
    );
  }

  Vec& operator+=(const Vec &v){ 
    x += v.x;
    y += v.y;
    return *this;
  }

  Vec& operator-=(const Vec &v) { 
    x -= v.x;
    y -= v.y;
    return *this;
  }

  Vec& operator/=(const float& v){ 
    x /= v;
    y /= v;
    return *this;
  }
  friend std::ostream& operator<<(std::ostream& os, const Vec& v){
    os << "<" << v.x << ", " << v.y << ">";
    return os;
  }
  float mag() const {
    return std::sqrt(
      std::pow(x, 2) + std::pow(y, 2)
    );
  }
  float dot(const Vec& b) const { 
    return x * b.x + y * b.y;
  }
};