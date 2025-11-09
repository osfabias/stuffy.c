#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <X11/Xlib.h>

typedef struct
{
  Display *display;
  Window   root_window;
  int32_t  screen_index;
  bool     initialized;
} LinuxPlatformState;

extern LinuxPlatformState g_linux_state;

