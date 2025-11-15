#pragma once

#include <stdbool.h>

#include <X11/Xlib.h>

#define WINDOW_BACKLINK_PROPERTY_NAME "STUFFY_WINDOW_BACKLINK"

struct StuffyWindow
{
  XID  x11_window;
  bool should_close;
  void (*resize_callback) (struct StuffyWindow *, uint32_t, uint32_t);
};

