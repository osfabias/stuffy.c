#pragma once

#include <stdbool.h>

#include <windows.h>

struct Window
{
  HWND windows_window;
  LONG window_init_style;
  bool should_close;
};

