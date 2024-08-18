#pragma once
#include "../fluid/vec.hpp"

struct RenderParameters { 
  bool isPaused = true;
  bool isDebugMenu = false;
  bool isAdjacentView = false;
  bool isDensityView = false;
  bool isLClick = false;
  bool isRClick = false;
  int guiScale = 1;
  Vec mousePos;
};