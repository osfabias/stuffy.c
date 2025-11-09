#pragma once

#include <stdbool.h>

#include <X11/Xlib.h>

#define WINDOW_BACKLINK_PROPERTY_NAME "STUFFY_WINDOW_BACKLINK"

struct Window
{
  XID  x11_window;
  bool should_close;
};

