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
  @file include/stuffy/window.h
  @brief Window management functions
  @author Ilya Buravov (ilburale@gmail.com)
  @details This header file provides functions for creating, managing, and
           controlling application windows.
*/

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "stuffy/apidef.h"

typedef enum
{
  /* The window have a title bar at the top. */
  STUFFY_WINDOW_STYLE_TITLED_BIT = 1 << 0,

  /* The window have an active "close" button. */
  STUFFY_WINDOW_STYLE_CLOSABLE_BIT = 1 << 1,

  /* The window have an active "iconify" button. */
  STUFFY_WINDOW_STYLE_ICONIFIABLE_BIT = 1 << 2,

  /* The window can be resized by dragging it's edges. */
  STUFFY_WINDOW_STYLE_RESIZABLE_BIT = 1 << 3,
} StuffyWindowStyleMask;

typedef enum
{
  /* The window is hidden and it's position and size are nullified. */
  STUFFY_WINDOW_STATE_ICONIFIED,

  /* The window is shown. */
  STUFFY_WINDOW_STATE_NORMAL,

  /* The window covers the entire screen, hiding OS UI elements. */
  STUFFY_WINDOW_STATE_FULLSCREEN,

  /* The number of window states in total. */
  STUFFY_WINDOW_STATE_COUNT
} StuffyWindowState;

/*
  @brief Window rectangle in screen coordinates.
  @details Defines the position and size of a window in screen coordinates.
           The x and y values represent the top-left corner position, while
           width and height define the window dimensions in pixels.
*/
typedef struct
{
  int32_t x, y;              // Position in screen coordinates.
  uint32_t width, height; // Size in screen coordinates.
} StuffyWindowRect;

/*
  @brief Window configuration structure.
  @details Contains the initial configuration parameters for creating a new window.
*/
typedef struct
{
  const char                *title;
  StuffyWindowRect           rect;      // Window rectangle in screen coordinates.
  StuffyWindowStyleMask      style_mask;
} StuffyWindowConfig;

/*
  @brief Window handle.
  @details Opaque handle representing a window instance. All window operations
           require a valid StuffyWindow pointer.
*/
typedef struct StuffyWindow StuffyWindow;

/*
  @brief Opens a window.
  @details Creates and displays a new window based on the provided configuration.
           The window is created with the specified title, size, position, and
           style attributes.
  @param config Pointer to the window configuration (must not be NULL).
  @return Pointer to the created StuffyWindow instance on success, NULL on failure.
  @note The returned StuffyWindow pointer must be closed using @ref stuffy_window_close
        when no longer needed.
*/
__STUFFY_API__ StuffyWindow *stuffy_window_open (const StuffyWindowConfig *config);

/*
  @brief Closes a window.
  @details Destroys the window and frees all associated resources. After calling
           this function, the StuffyWindow pointer becomes invalid.
  @param window Pointer to the StuffyWindow instance to close (must not be NULL).
  @note After calling this function, the StuffyWindow pointer should not be used
        for any further operations.
*/
__STUFFY_API__ void stuffy_window_close (StuffyWindow *window);

/*
  @brief Checks if the window should be closed.
  @details Returns whether the platform has requested the window to be closed,
           typically in response to user actions like clicking the close button.
  @param window Pointer to the StuffyWindow instance to check (must not be NULL).
  @return true if the window should be closed, false otherwise.
  @note This function should be checked regularly in the main loop to handle
        window close requests.
*/
__STUFFY_API__ bool stuffy_window_should_close (StuffyWindow *window);

/*
  @brief Sets the window title.
  @details Updates the text displayed in the window's title bar.
  @param window Pointer to the StuffyWindow instance (must not be NULL).
  @param title Null-terminated string containing the new title (must not be NULL).
*/
__STUFFY_API__ void stuffy_window_set_title (StuffyWindow *window, const char *title);

/*
  @brief Returns the current window title.
  @details Retrieves a pointer to the string containing the window's current title.
  @param window Pointer to the StuffyWindow instance (must not be NULL).
  @return Pointer to the window title string. The returned pointer is owned by
          the window and should not be freed by the caller.
*/
__STUFFY_API__ const char *stuffy_window_get_title (StuffyWindow *window);

/*
  @brief Sets the window rectangle.
  @details Updates the window's position and size in screen coordinates.
  @param window Pointer to the StuffyWindow instance (must not be NULL).
  @param rect Window rectangle containing the new position and size.
*/
__STUFFY_API__ void stuffy_window_set_rect (StuffyWindow *window, StuffyWindowRect rect);

/*
  @brief Returns the current window rectangle.
  @details Retrieves the window's current position and size in screen coordinates.
  @param window Pointer to the StuffyWindow instance (must not be NULL).
  @return Window rectangle containing the current position and size.
*/
__STUFFY_API__ StuffyWindowRect stuffy_window_get_rect (StuffyWindow *window);

/*
  @brief Sets the window state.
  @details Changes the window's state (normal, iconified, or fullscreen).
  @param window Pointer to the StuffyWindow instance (must not be NULL).
  @param state New window state to apply.
*/
__STUFFY_API__ void stuffy_window_set_state (StuffyWindow *window, StuffyWindowState state);

/*
  @brief Returns the current window state.
  @details Retrieves the window's current state.
  @param window Pointer to the StuffyWindow instance (must not be NULL).
  @return Current window state.
*/
__STUFFY_API__ StuffyWindowState stuffy_window_get_state (StuffyWindow *window);

