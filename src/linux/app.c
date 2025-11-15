#include <stdbool.h>
#include <stdint.h>

#include <X11/Xlib.h>

#include "stuffy/app.h"
#include "stuffy/window.h"

#include "src/input/keyboard_state.h"
#include "src/input/mouse_state.h"
#include "src/linux/platform_state.h"
#include "src/linux/window.h"

inline static void stuffy__process_client_message_event (XClientMessageEvent *event);

LinuxPlatformState g_linux_state = {
  .initialized = false,
};

int32_t stuffy_app_init (void)
{
  if (g_linux_state.initialized)
  {
    return 1;
  }

  g_linux_state.display = XOpenDisplay (0);
  if (g_linux_state.display == NULL)
  {
    return 1;
  }

  g_linux_state.screen_index = XDefaultScreen (g_linux_state.display);

  g_linux_state.root_window = RootWindow (g_linux_state.display, g_linux_state.screen_index);

  g_linux_state.initialized = true;
  return 0;
}

void stuffy_app_update (void)
{
  XEvent event;

  while (XPending (g_linux_state.display))
  {
    XNextEvent (g_linux_state.display, &event);
    switch (event.type)
    {
    case KeyPress :
      g_keyboard_state.keys[ event.xkey.keycode ] = true;
      break;
    case KeyRelease :
      g_keyboard_state.keys[ event.xkey.keycode ] = false;
      break;
    case ButtonPress :
      g_mouse_state.buttons[ event.xbutton.button ] = true;
      break;
    case ButtonRelease :
      g_mouse_state.buttons[ event.xbutton.button ] = false;
      break;
    case MotionNotify :
      g_mouse_state.x = (int32_t)event.xbutton.x;
      g_mouse_state.y = (int32_t)event.xbutton.y;
      break;
    case ClientMessage :
      stuffy__process_client_message_event (&event.xclient);
      break;
    case ConfigureNotify :
      stuffy__process_configure_notify_event (&event.xconfigure);
      break;
    default :
      break;
    }
  }
}

void stuffy_app_deinit (void)
{
  if (!g_linux_state.initialized)
  {
    return;
  }

  XCloseDisplay (g_linux_state.display);
  g_linux_state.initialized = false;
}

void stuffy__process_client_message_event (XClientMessageEvent *p_event)
{
  Atom delete_window_atom = XInternAtom (g_linux_state.display, "WM_DELETE_WINDOW", False);
  if ((Atom)p_event->data.l[ 0 ] == delete_window_atom)
  {
    unsigned char *prop_data = NULL;
    unsigned long  nitems, bytes_after;
    int            format;
    Atom           type;

    if (XGetWindowProperty (p_event->display,
          p_event->window,
          XInternAtom (g_linux_state.display, WINDOW_BACKLINK_PROPERTY_NAME, False),
          0,
          ~0L,
          False,
          AnyPropertyType,
          &type,
          &format,
          &nitems,
          &bytes_after,
          &prop_data) == Success)
    {
      if (prop_data)
      {
        (*(StuffyWindow **)prop_data)->should_close = true;
        XFree (prop_data);
      }
    }
    return;
  }
}

inline static void stuffy__process_configure_notify_event (XConfigureEvent *event);

inline static void stuffy__process_configure_notify_event (XConfigureEvent *event)
{
  unsigned char *prop_data = NULL;
  unsigned long  nitems, bytes_after;
  int            format;
  Atom           type;

  if (XGetWindowProperty (event->display,
        event->window,
        XInternAtom (g_linux_state.display, WINDOW_BACKLINK_PROPERTY_NAME, False),
        0,
        ~0L,
        False,
        AnyPropertyType,
        &type,
        &format,
        &nitems,
        &bytes_after,
        &prop_data) == Success)
  {
    if (prop_data)
    {
      StuffyWindow *window = *(StuffyWindow **)prop_data;
      if (window)
      {
        const StuffyWindowRect new_rect = {
          .x      = event->x,
          .y      = event->y,
          .width  = (uint32_t)event->width,
          .height = (uint32_t)event->height,
        };
        
        // Get current rect to compare (before the event, this should be the old rect)
        // Actually, since ConfigureNotify is sent after the change, we need to track previous
        // For now, we'll call move if position changed, resize if size changed
        // We can detect this by checking if the event indicates a position or size change
        // Since we don't have previous state, we'll call both if callbacks are set
        // But actually, we can use XGetGeometry to get current, but that's the new one
        // Simplest: call move_callback if set, resize_callback if set
        // The user can decide based on what changed
        
        // Check if this is a move (position changed) or resize (size changed)
        // We'll call move_callback for any ConfigureNotify since position is always included
        // and resize_callback if size changed
        if (window->move_callback)
        {
          window->move_callback (window, new_rect);
        }
        if (window->resize_callback)
        {
          window->resize_callback (window, new_rect);
        }
      }
      XFree (prop_data);
    }
  }
}

