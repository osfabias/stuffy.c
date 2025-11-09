#include <stdbool.h>
#include <stdint.h>

#include <X11/Xlib.h>

#include "stuffy/app.h"

#include "src/input/keyboard_state.h"
#include "src/input/mouse_state.h"
#include "src/linux/platform_state.h"

inline static void process_client_message_event (XClientMessageEvent *event);

LinuxPlatformState g_linux_state = {
  .initialized = false,
};

int32_t init_app (void)
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

void update_app (void)
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
    default :
      break;
    }
  }
}

void deinit_app (void)
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
        (*(Window **)prop_data)->should_close = true;
        XFree (prop_data);
      }
    }
    return;
  }
}

