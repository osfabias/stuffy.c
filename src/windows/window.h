#pragma once

#include <stdbool.h>

#include <windows.h>

struct StuffyWindow
{
  HWND windows_window;
  LONG window_init_style;
  bool should_close;
};

