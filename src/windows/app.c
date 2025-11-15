#include <stdint.h>

#include <windows.h>
#include <winuser.h>

#include "stuffy/app.h"

#include "src/windows/names.h"
#include "src/windows/platform_state.h"
#include "src/windows/window_procedure.h"

inline static HMODULE stuffy__get_current_module_handle (void);

inline static void stuffy__register_window_class (void);

WindowsPlatformState g_windows_state = {0};

int32_t stuffy_app_init (void)
{
  g_windows_state.module = stuffy__get_current_module_handle ( );

  stuffy__register_window_class ( );

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

HMODULE stuffy__get_current_module_handle (void)
{
  extern IMAGE_DOS_HEADER __ImageBase;

  HMODULE module = ((HMODULE)&__ImageBase);

  return module;
}

void stuffy__register_window_class (void)
{
  WNDCLASS wc = {
    .lpfnWndProc   = stuffy__window_procedure,
    .hInstance     = g_windows_state.module,
    .lpszClassName = WINDOW_CLASS_NAME,
  };
  RegisterClass (&wc);
}

