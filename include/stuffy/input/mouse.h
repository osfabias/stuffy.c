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
  @file include/stuffy/input/mouse.h
  @brief Mouse input handling
  @author Ilya Buravov (ilburale@gmail.com)
  @details This header file provides mouse input functionality including
           mouse button definitions and mouse state management.
*/

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "stuffy/apidef.h"

/*
  @brief Mouse button enumeration.
  @details Defines all supported mouse button codes. These codes identify
           which mouse button was pressed or released.
*/
typedef enum
{
  /* Left mouse button. */
  MOUSE_BUTTON_LEFT,

  /* Right mouse button. */
  MOUSE_BUTTON_RIGHT,

  /* Middle mouse button (scroll). */
  MOUSE_BUTTON_MIDDLE,

  /* Side mouse button. */
  MOUSE_BUTTON_FORWARD,

  /* Side mouse button. */
  MOUSE_BUTTON_BACKWARD,

  /* Number of mouse buttons. */
  MOUSE_BUTTON_COUNT
} MouseButton;

/*
  @brief Mouse state structure.
  @details Contains the current state of the mouse, including cursor position,
           scroll delta, and button states. The position is in window coordinates
           with the origin at the top-left corner.
*/
typedef struct
{
  /* Cursor X position in window coordinates in pixels. */
  int32_t x;

  /* Cursor Y position in window coordinates in pixels. */
  int32_t y;

  /* Scroll. */
  float scroll;

  /* Buttons state. */
  bool buttons[ MOUSE_BUTTON_COUNT ];
} MouseState;

/*
  @brief Returns the current mouse state.
  @details Retrieves a pointer to the global mouse state structure.
           The state is updated automatically during each @ref update_app call.
  @return Pointer to the mouse state structure. The returned pointer is
          owned by the library and should not be freed by the caller. The
          state is updated after each @ref update_app call.
  @note The mouse state is global and shared across all application instances.
*/
__STUFFY_API__ const MouseState *get_mouse_state (void);

