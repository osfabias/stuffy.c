#pragma once

#include <windows.h>
#include <windowsx.h>
#include <winuser.h>

#include "stuffy/window.h"

#include "src/input/keyboard_state.h"
#include "src/input/mouse_state.h"
#include "src/windows/names.h"
#include "src/windows/window.h"

static LRESULT CALLBACK stuffy__window_procedure (
  HWND window, UINT message_code, WPARAM wparam, LPARAM lparam)
{
  switch (message_code)
  {
  case WM_ERASEBKGND :
    return 1;
  case WM_CLOSE :
  {
    StuffyWindow *stuffy_window = GetProp (window, WINDOW_PROPERTY_NAME);

    stuffy_window->should_close = true;
    break;
  }
  case WM_KEYDOWN :
  case WM_SYSKEYDOWN :
    g_keyboard_state.keys[ wparam ] = 1;
    break;
  case WM_KEYUP :
  case WM_SYSKEYUP :
    g_keyboard_state.keys[ wparam ] = 0;
    break;
  case WM_MOUSEMOVE :
    g_mouse_state.x = (int32_t)GET_X_LPARAM (lparam);
    g_mouse_state.y = (int32_t)GET_Y_LPARAM (lparam);
    break;
  case WM_MOUSEWHEEL :
    g_mouse_state.scroll = (float)GET_WHEEL_DELTA_WPARAM (wparam);
    break;
  case WM_LBUTTONDOWN :
    g_mouse_state.buttons[ STUFFY_MOUSE_BUTTON_LEFT ] = 1;
    break;
  case WM_LBUTTONUP :
    g_mouse_state.buttons[ STUFFY_MOUSE_BUTTON_LEFT ] = 0;
    break;
  case WM_RBUTTONDOWN :
    g_mouse_state.buttons[ STUFFY_MOUSE_BUTTON_RIGHT ] = 1;
    break;
  case WM_RBUTTONUP :
    g_mouse_state.buttons[ STUFFY_MOUSE_BUTTON_RIGHT ] = 0;
    break;
  case WM_MBUTTONDOWN :
    g_mouse_state.buttons[ STUFFY_MOUSE_BUTTON_MIDDLE ] = 1;
    break;
  case WM_MBUTTONUP :
    g_mouse_state.buttons[ STUFFY_MOUSE_BUTTON_MIDDLE ] = 0;
    break;
  case WM_MOVE :
  {
    StuffyWindow *stuffy_window = GetProp (window, WINDOW_PROPERTY_NAME);
    if (stuffy_window && stuffy_window->move_callback)
    {
      RECT rect;
      GetWindowRect (window, &rect);
      const StuffyWindowRect window_rect = {
        .x      = (int32_t)rect.left,
        .y      = (int32_t)rect.top,
        .width  = (uint32_t)(rect.right - rect.left),
        .height = (uint32_t)(rect.bottom - rect.top),
      };
      stuffy_window->move_callback (stuffy_window, window_rect);
    }
    break;
  }
  case WM_SIZE :
  {
    StuffyWindow *stuffy_window = GetProp (window, WINDOW_PROPERTY_NAME);
    if (stuffy_window && stuffy_window->resize_callback)
    {
      RECT rect;
      GetWindowRect (window, &rect);
      const StuffyWindowRect window_rect = {
        .x      = (int32_t)rect.left,
        .y      = (int32_t)rect.top,
        .width  = (uint32_t)(rect.right - rect.left),
        .height = (uint32_t)(rect.bottom - rect.top),
      };
      stuffy_window->resize_callback (stuffy_window, window_rect);
    }
    break;
  }
  case WM_DESTROY :
    PostQuitMessage (0);
    break;
  default :
    return DefWindowProc (window, message_code, wparam, lparam);
  }

  return 0;
}

