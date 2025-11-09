#include "stuffy/input/mouse.h"

#include "src/input/mouse_state.h"

MouseState g_mouse_state = {0};

const MouseState *get_mouse_state (void) { return &g_mouse_state; }

