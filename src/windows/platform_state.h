#pragma once

#include <windows.h>

typedef struct
{
  HMODULE module;
} WindowsPlatformState;

extern WindowsPlatformState g_windows_state;

