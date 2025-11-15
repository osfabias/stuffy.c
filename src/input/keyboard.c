#include "stuffy/input/keyboard.h"

#include "src/input/keyboard_state.h"

StuffyKeyboardState g_keyboard_state = {0};

const StuffyKeyboardState *stuffy_keyboard_get_state (void) { return &g_keyboard_state; }

