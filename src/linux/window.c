#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xmd.h>
#include <X11/Xutil.h>

#include "stuffy/window.h"

#include "src/input/mouse_state.h"
#include "src/linux/platform_state.h"
#include "src/linux/window.h"

inline static void stuffy__send_x11_window_fullscreen_switch_event (XID x11_window, int32_t state);

StuffyWindow *stuffy_window_open (const StuffyWindowConfig *config)
{
  StuffyWindow *window = malloc (sizeof (StuffyWindow));
  if (window == NULL)
  {
    return NULL;
  }

  window->should_close = false;
  window->resize_callback = NULL;
  window->move_callback = NULL;

  XSetWindowAttributes attrs = {
    .background_pixmap = ParentRelative,
    .background_pixel  = 0xff101010,
    .border_pixmap     = CopyFromParent,
    .border_pixel      = 0xff1c1f25,
    .bit_gravity       = 0,
    .win_gravity       = NorthWestGravity,
    .event_mask =
      KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonRelease | PointerMotionMask |
      StructureNotifyMask,
    .save_under            = 0,
    .backing_pixel         = 0,
    .backing_store         = 0,
    .backing_planes        = 0,
    .override_redirect     = True,
    .do_not_propagate_mask = 0,
    .colormap = DefaultColormap (g_linux_state.display, g_linux_state.screen_index),
    .cursor   = None,
  };

  window->x11_window = XCreateWindow (g_linux_state.display,
    g_linux_state.root_window,
    config->rect.x,
    config->rect.y,
    config->rect.width,
    config->rect.height,
    1,
    DefaultDepth (g_linux_state.display, g_linux_state.screen_index),
    InputOutput,
    DefaultVisual (g_linux_state.display, g_linux_state.screen_index),
    CWBackPixel | CWBorderPixel | CWWinGravity | CWEventMask | CWColormap,
    &attrs);

  if (!window->x11_window)
  {
    free (window);
    return NULL;
  }

  XStoreName (g_linux_state.display, window->x11_window, config->title);

  XChangeProperty (g_linux_state.display,
    window->x11_window,
    XInternAtom (g_linux_state.display, WINDOW_BACKLINK_PROPERTY_NAME, False),
    XA_INTEGER,
    8,
    PropModeReplace,
    (unsigned char *)&window,
    sizeof (StuffyWindow *));

  XMapWindow (g_linux_state.display, window->x11_window);

  XMoveWindow (g_linux_state.display,
    window->x11_window,
    config->rect.x - (int32_t)config->rect.width / 2,
    config->rect.y - (int32_t)config->rect.height / 2);

  return window;
}

void stuffy_window_close (StuffyWindow *window)
{
  XUnmapWindow (g_linux_state.display, window->x11_window);

  XDestroyWindow (g_linux_state.display, window->x11_window);

  free (window);
}

bool stuffy_window_should_close (StuffyWindow *window) { return window->should_close; }

void stuffy_window_set_title (StuffyWindow *window, const char *title)
{
  XStoreName (g_linux_state.display, window->x11_window, title);
}

const char *stuffy_window_get_title (StuffyWindow *window)
{
  static char *window_title = NULL;
  if (window_title != NULL)
  {
    XFree (window_title);
    window_title = NULL;
  }

  XFetchName (g_linux_state.display, window->x11_window, &window_title);

  return window_title;
}

void stuffy_window_set_rect (StuffyWindow *window, StuffyWindowRect rect)
{
  if (stuffy_window_get_state (window) != STUFFY_WINDOW_STATE_NORMAL) { return; }

  XMoveWindow (g_linux_state.display, window->x11_window, rect.x, rect.y);
  XResizeWindow (g_linux_state.display, window->x11_window, rect.width, rect.height);
}

StuffyWindowRect stuffy_window_get_rect (StuffyWindow *window)
{
  XID          root_win;
  uint32_t     width, height;
  unsigned int border, depth;
  int32_t      x, y;

  XGetGeometry (g_linux_state.display,
    window->x11_window,
    &root_win,
    &x,
    &y,
    &width,
    &height,
    &border,
    &depth);

  StuffyWindowRect rect = {
    .x      = x,
    .y      = y,
    .width  = width,
    .height = height,
  };

  return rect;
}

void stuffy_window_set_state (StuffyWindow *window, StuffyWindowState state)
{
  switch (state)
  {
  case STUFFY_WINDOW_STATE_FULLSCREEN :
    stuffy__send_x11_window_fullscreen_switch_event (window->x11_window, 1);
    break;
  case STUFFY_WINDOW_STATE_ICONIFIED :
    XIconifyWindow (g_linux_state.display, window->x11_window, g_linux_state.screen_index);
    break;
  case STUFFY_WINDOW_STATE_NORMAL :
    stuffy__send_x11_window_fullscreen_switch_event (window->x11_window, 0);
    break;
  default :
    return;
  }
}

StuffyWindowState stuffy_window_get_state (StuffyWindow *window)
{
  Atom atom_state      = XInternAtom (g_linux_state.display, "_NET_WM_STATE", True);
  Atom atom_fullscreen = XInternAtom (g_linux_state.display, "_NET_WM_STATE_FULLSCREEN", True);
  Atom atom_iconified  = XInternAtom (g_linux_state.display, "_NET_WM_STATE_HIDDEN", True);

  Atom          actual_type;
  int           actual_format;
  unsigned long num_items, bytes_after;
  Atom         *property = NULL;

  XGetWindowProperty (g_linux_state.display,
    window->x11_window,
    atom_state,
    0,
    1024,
    False,
    XA_ATOM,
    &actual_type,
    &actual_format,
    &num_items,
    &bytes_after,
    (unsigned char **)&property);

  for (unsigned long i = 0; i < num_items; i++)
  {
    if (property[ i ] == atom_fullscreen)
    {
      XFree (property);
      return STUFFY_WINDOW_STATE_FULLSCREEN;
    }
    else if (property[ i ] == atom_iconified)
    {
      XFree (property);
      return STUFFY_WINDOW_STATE_ICONIFIED;
    }
  }

  XFree (property);
  return STUFFY_WINDOW_STATE_NORMAL;
}

void stuffy__send_x11_window_fullscreen_switch_event (XID x11_window, int32_t state)
{
  Atom net_wm_state_atom = XInternAtom (g_linux_state.display, "_NET_WM_STATE", False);
  Atom fullscreen        = XInternAtom (g_linux_state.display, "_NET_WM_STATE_FULLSCREEN", False);

  XEvent event = {
    .xclient = (XClientMessageEvent) {
      .type         = ClientMessage,
      .send_event   = True,
      .window       = x11_window,
      .message_type = net_wm_state_atom,
      .format       = 32,
      .data.l[ 0 ]  = state,
      .data.l[ 1 ]  = fullscreen,
    },
  };

  const Bool propagate  = false;
  const long event_mask = SubstructureNotifyMask | SubstructureRedirectMask;

  XSendEvent (
    g_linux_state.display, g_linux_state.root_window, propagate, event_mask, &event);
}

void stuffy_window_set_resize_callback (
  StuffyWindow *window, StuffyWindowResizeCallback callback)
{
  if (window == NULL) { return; }
  window->resize_callback = callback;
}

void stuffy_window_set_move_callback (
  StuffyWindow *window, StuffyWindowMoveCallback callback)
{
  if (window == NULL) { return; }
  window->move_callback = callback;
}

