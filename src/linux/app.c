#include <stdbool.h>
#include <stdint.h>

#include <X11/Xlib.h>

#include "stuffy/app.h"
#include "stuffy/window.h"

#include "src/input/keyboard_state.h"
#include "src/input/mouse_state.h"
#include "src/linux/platform_state.h"
#include "src/linux/window.h"

inline static void process_client_message_event (XClientMessageEvent *event);

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
      process_client_message_event (&event.xclient);
      break;
    case ConfigureNotify :
      process_configure_notify_event (&event.xconfigure);
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

void process_client_message_event (XClientMessageEvent *p_event)
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

inline static void process_configure_notify_event (XConfigureEvent *event);

inline static void process_configure_notify_event (XConfigureEvent *event)
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
      if (window && window->resize_callback)
      {
        window->resize_callback (
          window, (uint32_t)event->width, (uint32_t)event->height
        );
      }
      XFree (prop_data);
    }
  }
}

