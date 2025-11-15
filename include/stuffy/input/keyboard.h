/*
  Copyright 2025 Osfabias

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

/*
  @file include/stuffy/input/keyboard.h
  @brief Keyboard input handling
  @author Ilya Buravov (ilburale@gmail.com)
  @details This header file provides keyboard input functionality including
           key code definitions and keyboard state management.
*/

#pragma once

#include <stdbool.h>

#include "stuffy/apidef.h"

/*
  @brief Key code enumeration.
  @details Defines all supported keyboard key codes. These codes correspond
           to physical keys on the keyboard and are platform-independent.
*/
typedef enum
{
  STUFFY_KEY_BACKSPACE = 0x08,
  STUFFY_KEY_ENTER     = 0x0D,
  STUFFY_KEY_TAB       = 0x09,
  STUFFY_KEY_SHIFT     = 0x10,
  STUFFY_KEY_CONTROL   = 0x11,

  STUFFY_KEY_PAUSE    = 0x13,
  STUFFY_KEY_CAPSLOCK = 0x14,

  STUFFY_KEY_ESCAPE = 0x1B,

  STUFFY_KEY_CONVERT    = 0x1C,
  STUFFY_KEY_NONCONVERT = 0x1D,
  STUFFY_KEY_ACCEPT     = 0x1E,
  STUFFY_KEY_MODECHANGE = 0x1F,

  STUFFY_KEY_SPACE = 0x20,

  STUFFY_KEY_PAGEUP   = 0x21,
  STUFFY_KEY_PAGEDOWN = 0x22,
  STUFFY_KEY_END      = 0x23,
  STUFFY_KEY_HOME     = 0x24,

  STUFFY_KEY_LEFT  = 0x25,
  STUFFY_KEY_UP    = 0x26,
  STUFFY_KEY_RIGHT = 0x27,
  STUFFY_KEY_DOWN  = 0x28,

  STUFFY_KEY_SELECT      = 0x29,
  STUFFY_KEY_PRINT       = 0x2A,
  STUFFY_KEY_EXECUTE     = 0x2B,
  STUFFY_KEY_PRINTSCREEN = 0x2C,
  STUFFY_KEY_INSERT      = 0x2D,
  STUFFY_KEY_DELETE      = 0x2E,
  STUFFY_KEY_HELP        = 0x2F,

  STUFFY_KEY_0 = 0x30,
  STUFFY_KEY_1 = 0x31,
  STUFFY_KEY_2 = 0x32,
  STUFFY_KEY_3 = 0x33,
  STUFFY_KEY_4 = 0x34,
  STUFFY_KEY_5 = 0x35,
  STUFFY_KEY_6 = 0x36,
  STUFFY_KEY_7 = 0x37,
  STUFFY_KEY_8 = 0x38,
  STUFFY_KEY_9 = 0x39,

  STUFFY_KEY_A = 0x41,
  STUFFY_KEY_B = 0x42,
  STUFFY_KEY_C = 0x43,
  STUFFY_KEY_D = 0x44,
  STUFFY_KEY_E = 0x45,
  STUFFY_KEY_F = 0x46,
  STUFFY_KEY_G = 0x47,
  STUFFY_KEY_H = 0x48,
  STUFFY_KEY_I = 0x49,
  STUFFY_KEY_J = 0x4A,
  STUFFY_KEY_K = 0x4B,
  STUFFY_KEY_L = 0x4C,
  STUFFY_KEY_M = 0x4D,
  STUFFY_KEY_N = 0x4E,
  STUFFY_KEY_O = 0x4F,
  STUFFY_KEY_P = 0x50,
  STUFFY_KEY_Q = 0x51,
  STUFFY_KEY_R = 0x52,
  STUFFY_KEY_S = 0x53,
  STUFFY_KEY_T = 0x54,
  STUFFY_KEY_U = 0x55,
  STUFFY_KEY_V = 0x56,
  STUFFY_KEY_W = 0x57,
  STUFFY_KEY_X = 0x58,
  STUFFY_KEY_Y = 0x59,
  STUFFY_KEY_Z = 0x5A,

  STUFFY_KEY_LSUPER = 0x5B,
  STUFFY_KEY_RSUPER = 0x5C,

  STUFFY_KEY_APPS  = 0x5D,
  STUFFY_KEY_SLEEP = 0x5F,

  STUFFY_KEY_NUMPAD0   = 0x60,
  STUFFY_KEY_NUMPAD1   = 0x61,
  STUFFY_KEY_NUMPAD2   = 0x62,
  STUFFY_KEY_NUMPAD3   = 0x63,
  STUFFY_KEY_NUMPAD4   = 0x64,
  STUFFY_KEY_NUMPAD5   = 0x65,
  STUFFY_KEY_NUMPAD6   = 0x66,
  STUFFY_KEY_NUMPAD7   = 0x67,
  STUFFY_KEY_NUMPAD8   = 0x68,
  STUFFY_KEY_NUMPAD9   = 0x69,
  STUFFY_KEY_MULTIPLY  = 0x6A,
  STUFFY_KEY_ADD       = 0x6B,
  STUFFY_KEY_SEPARATOR = 0x6C,
  STUFFY_KEY_SUBTRACT  = 0x6D,
  STUFFY_KEY_DECIMAL   = 0x6E,
  STUFFY_KEY_DIVIDE    = 0x6F,

  STUFFY_KEY_F1  = 0x70,
  STUFFY_KEY_F2  = 0x71,
  STUFFY_KEY_F3  = 0x72,
  STUFFY_KEY_F4  = 0x73,
  STUFFY_KEY_F5  = 0x74,
  STUFFY_KEY_F6  = 0x75,
  STUFFY_KEY_F7  = 0x76,
  STUFFY_KEY_F8  = 0x77,
  STUFFY_KEY_F9  = 0x78,
  STUFFY_KEY_F10 = 0x79,
  STUFFY_KEY_F11 = 0x7A,
  STUFFY_KEY_F12 = 0x7B,
  STUFFY_KEY_F13 = 0x7C,
  STUFFY_KEY_F14 = 0x7D,
  STUFFY_KEY_F15 = 0x7E,
  STUFFY_KEY_F16 = 0x7F,
  STUFFY_KEY_F17 = 0x80,
  STUFFY_KEY_F18 = 0x81,
  STUFFY_KEY_F19 = 0x82,
  STUFFY_KEY_F20 = 0x83,
  STUFFY_KEY_F21 = 0x84,
  STUFFY_KEY_F22 = 0x85,
  STUFFY_KEY_F23 = 0x86,
  STUFFY_KEY_F24 = 0x87,

  STUFFY_KEY_NUMLOCK      = 0x90,
  STUFFY_KEY_SCROLL       = 0x91,
  STUFFY_KEY_NUMPAD_EQUAL = 0x92,

  STUFFY_KEY_LSHIFT   = 0xA0,
  STUFFY_KEY_RSHIFT   = 0xA1,
  STUFFY_KEY_LCONTROL = 0xA2,
  STUFFY_KEY_RCONTROL = 0xA3,
  STUFFY_KEY_LALT     = 0xA4,
  STUFFY_KEY_RALT     = 0xA5,

  STUFFY_KEY_SEMICOLON = 0x3B,

  STUFFY_KEY_APOSTROPHE = 0xDE,
  STUFFY_KEY_EQUAL      = 0xBB,
  STUFFY_KEY_COMMA      = 0xBC,
  STUFFY_KEY_MINUS      = 0xBD,
  STUFFY_KEY_PERIOD     = 0xBE,
  STUFFY_KEY_SLASH      = 0xBF,

  STUFFY_KEY_GRAVE = 0xC0,

  STUFFY_KEY_LBRACKET  = 0xDB,
  STUFFY_KEY_BACKSLASH = 0xDC,
  STUFFY_KEY_RBRACKET  = 0xDD,

  STUFFY_KEY_COUNT = 0xFF
} StuffyKey;

/*
  @brief Keyboard state structure.
  @details Contains the current state of all keyboard keys. Each element in
           the keys array corresponds to a key code and indicates whether
           that key is currently pressed.
*/
typedef struct
{
  /* Keys state. */
  bool keys[ STUFFY_KEY_COUNT ];
} StuffyKeyboardState;

/*
  @brief Returns the current keyboard state.
  @details Retrieves a pointer to the global keyboard state structure.
           The state is updated automatically during each @ref stuffy_app_update call.
  @return Pointer to the keyboard state structure. The returned pointer is
          owned by the library and should not be freed by the caller. The
          state is updated after each @ref stuffy_app_update call.
  @note The keyboard state is global and shared across all application instances.
*/
__STUFFY_API__ const StuffyKeyboardState *stuffy_keyboard_get_state (void);

