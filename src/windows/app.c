#include <stdint.h>

#include <windows.h>
#include <winuser.h>

#include "stuffy/app.h"

#include "src/windows/names.h"
#include "src/windows/platform_state.h"
#include "src/windows/window_procedure.h"

inline static HMODULE get_current_module_handle (void);

inline static void register_window_class (void);

WindowsPlatformState g_windows_state = {0};

int32_t stuffy_app_init (void)
{
  g_windows_state.module = get_current_module_handle ( );

  register_window_class ( );

  return 0;
}

void stuffy_app_update (void)
{
  for (MSG message; PeekMessage (&message, NULL, 0, 0, PM_REMOVE);)
  {
    TranslateMessage (&message);
    DispatchMessage (&message);
  }
}

void stuffy_app_deinit (void)
{
  UnregisterClass (WINDOW_CLASS_NAME, g_windows_state.module);
}

HMODULE get_current_module_handle (void)
{
  extern IMAGE_DOS_HEADER __ImageBase;

  HMODULE module = ((HMODULE)&__ImageBase);

  return module;
}

void register_window_class (void)
{
  WNDCLASS wc = {
    .lpfnWndProc   = window_procedure,
    .hInstance     = g_windows_state.module,
    .lpszClassName = WINDOW_CLASS_NAME,
  };
  RegisterClass (&wc);
}

