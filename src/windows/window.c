#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <windows.h>
#include <windowsx.h>
#include <winuser.h>

#include "stuffy/window.h"

#include "src/windows/names.h"
#include "src/windows/platform_state.h"
#include "src/windows/window.h"
#include "src/windows/window_procedure.h"

inline static bool  is_window_fullscreen (HWND window);
inline static void  set_window_fullscreen (Window *window, bool fullscreen);
inline static HWND  open_windows_window (const WindowConf *conf);
inline static DWORD translate_style_mask (WindowStyleMask style_mask);

Window *open_window (const WindowConf *conf)
{
  Window *window = malloc (sizeof (Window));
  if (window == NULL)
  {
    return NULL;
  }

  window->windows_window = open_windows_window (conf);
  if (window->windows_window == NULL)
  {
    goto FAILED_WINDOWS_WINDOW;
  }

  if (SetProp (window->windows_window, WINDOW_PROPERTY_NAME, window) == FALSE)
  {
    goto FAILED_SET_PROP;
  }

  if (!(conf->style_mask & WINDOW_STYLE_CLOSABLE_BIT))
  {
    EnableMenuItem (GetSystemMenu (window->windows_window, FALSE), SC_CLOSE,
      MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
  }

  ShowWindow (window->windows_window, SW_SHOW);

  window->window_init_style = translate_style_mask (conf->style_mask);
  window->should_close      = false;

  return window;

FAILED_SET_PROP:
  DestroyWindow (window->windows_window);
FAILED_WINDOWS_WINDOW:
  free (window);
  return NULL;
}

void close_window (Window *window)
{
  DestroyWindow (window->windows_window);

  free (window);
}

bool should_window_close (Window *window) { return window->should_close; }

void set_window_title (Window *window, const char *title)
{
  SetWindowText (window->windows_window, title);
}

const char *get_window_title (Window *window)
{
  static char buf[ 128 ];
  GetWindowText (window->windows_window, buf, sizeof (buf) / sizeof (buf[ 0 ]));
  return buf;
}

void set_window_rect (Window *window, WindowRect rect)
{
  if (get_window_state (window) != WINDOW_STATE_NORMAL) { return; }

  MoveWindow (window->windows_window, rect.x, rect.y, rect.width, rect.height, TRUE);
}

WindowRect get_window_rect (Window *window)
{
  RECT rect;
  GetWindowRect (window->windows_window, &rect);

  WindowRect window_rect = {
    .x      = (int32_t)rect.left,
    .y      = (int32_t)rect.top,
    .width  = (uint32_t)(rect.right - rect.left),
    .height = (uint32_t)(rect.bottom - rect.top),
  };

  return window_rect;
}

void set_window_state (Window *window, WindowState state)
{
  set_window_fullscreen (window, state == WINDOW_STATE_FULLSCREEN);
  if (state == WINDOW_STATE_FULLSCREEN) { return; }

  switch (state)
  {
  case WINDOW_STATE_ICONIFIED :
    ShowWindow (window->windows_window, SW_HIDE);
    break;
  case WINDOW_STATE_NORMAL :
    ShowWindow (window->windows_window, SW_SHOW);
    break;
  default :
    return;
  }
}

WindowState get_window_state (Window *window)
{
  if (IsIconic (window->windows_window))
  {
    return WINDOW_STATE_ICONIFIED;
  }
  if (is_window_fullscreen (window->windows_window))
  {
    return WINDOW_STATE_FULLSCREEN;
  }
  return WINDOW_STATE_NORMAL;
}

HWND open_windows_window (const WindowConf *conf)
{
  return CreateWindowEx (0,
    WINDOW_CLASS_NAME,
    conf->title,
    translate_style_mask (conf->style_mask),
    conf->rect.x,
    conf->rect.y,
    conf->rect.width,
    conf->rect.height,
    NULL,
    NULL,
    g_windows_state.module,
    NULL);
}

DWORD translate_style_mask (WindowStyleMask style_mask)
{
  DWORD windows_style_mask = WS_BORDER;

  if (style_mask & WINDOW_STYLE_TITLED_BIT)
  {
    windows_style_mask |= WS_CAPTION | WS_SYSMENU;
  }
  else
  {
    windows_style_mask |= WS_POPUP;
  }

  if (style_mask & WINDOW_STYLE_ICONIFIABLE_BIT)
  {
    windows_style_mask |= WS_MINIMIZEBOX;
  }

  if (style_mask & WINDOW_STYLE_RESIZABLE_BIT)
  {
    windows_style_mask |= WS_MAXIMIZEBOX | WS_SIZEBOX;
  }

  return windows_style_mask;
}

void set_window_fullscreen (Window *window, bool fullscreen)
{
  if (fullscreen)
  {
    SetWindowLong (window->windows_window, GWL_STYLE,
      window->window_init_style & ~(WS_CAPTION | WS_THICKFRAME));

    MONITORINFO monitor_info = {0};
    monitor_info.cbSize      = sizeof (MONITORINFO);
    GetMonitorInfo (MonitorFromWindow (window->windows_window, MONITOR_DEFAULTTOPRIMARY),
      &monitor_info);

    MoveWindow (window->windows_window,
      monitor_info.rcMonitor.left,
      monitor_info.rcMonitor.top,
      monitor_info.rcMonitor.right - monitor_info.rcMonitor.left,
      monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top,
      TRUE);
  }
  else
  {
    SetWindowLong (window->windows_window, GWL_STYLE, window->window_init_style);
  }
}

bool is_window_fullscreen (HWND window)
{
  MONITORINFO monitor_info = {0};
  monitor_info.cbSize      = sizeof (MONITORINFO);
  GetMonitorInfo (MonitorFromWindow (window, MONITOR_DEFAULTTOPRIMARY), &monitor_info);

  RECT rect;
  GetWindowRect (window, &rect);

  return rect.left == monitor_info.rcMonitor.left &&
         rect.right == monitor_info.rcMonitor.right &&
         rect.top == monitor_info.rcMonitor.top &&
         rect.bottom == monitor_info.rcMonitor.bottom;
}

