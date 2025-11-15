#include "stuffy/input/mouse.h"

#include "src/input/mouse_state.h"

StuffyMouseState g_mouse_state = {0};

const StuffyMouseState *stuffy_mouse_get_state (void) { return &g_mouse_state; }

