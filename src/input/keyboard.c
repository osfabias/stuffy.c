#include "stuffy/input/keyboard.h"

#include "src/input/keyboard_state.h"

KeyboardState g_keyboard_state = {0};

const KeyboardState *get_keyboard_state (void) { return &g_keyboard_state; }

