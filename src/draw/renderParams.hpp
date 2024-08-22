#pragma once
#include "../fluid/vec.hpp"

struct RenderParameters { 
  bool isPaused = false;
  bool isDebugMenu = false;
  bool isAdjacentView = false;
  bool isDensityView = false;
  bool isLClick = false;
  bool isRClick = false;
  bool isReset = false;
  bool isAutoClick = false;
  int guiScale = 1;
  Vec mousePos;
};