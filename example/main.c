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
  @file example/main.c
  @brief Example program demonstrating library usage
  @author Osfabias
  @details This file provides a simple example of how to use the library.
          It demonstrates basic application lifecycle, window creation, and
          input handling. This serves as a template for users to understand
          library usage.
*/

#include "stuffy/extent.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <stuffy/app.h>
#include <stuffy/input/keyboard.h>
#include <stuffy/input/mouse.h>
#include <stuffy/window.h>

static void print_window_resolution (StuffyWindow *window)
{
  const StuffyExtent2D resolution = stuffy_window_get_framebuffer_size (window);
  printf ("Window resolution: %ux%u\n", resolution.width, resolution.height);
}

/*
  @brief Window resize callback function.
  @details This callback is invoked whenever the window is resized.
  @param window Pointer to the window that was resized.
  @param rect New window rectangle containing position and size in screen coordinates.
*/
static void on_window_resize (StuffyWindow *window, StuffyWindowRect rect)
{
  (void)window;  // Unused parameter
  printf ("Window resized: size %ux%u\n", rect.width, rect.height);
  print_window_resolution (window);
}

/*
  @brief Window move callback function.
  @details This callback is invoked whenever the window is moved.
  @param window Pointer to the window that was moved.
  @param rect New window rectangle containing position and size in screen coordinates.
*/
static void on_window_move (StuffyWindow *window, StuffyWindowRect rect)
{
  (void)window;  // Unused parameter
  printf ("Window moved: position size %ux%u\n", rect.width, rect.height);
}

int main (void)
{
  // Initialize application
  if (stuffy_app_init ( ) != 0)
  {
    fprintf (stderr, "Failed to initialize application\n");
    return EXIT_FAILURE;
  }

  // Configure window
  StuffyWindowConfig config = {
    .title      = "Stuffy.c Example",
    .rect       = {.x = 100, .y = 100, .width = 800, .height = 600},
    .style_mask = STUFFY_WINDOW_STYLE_TITLED_BIT | STUFFY_WINDOW_STYLE_CLOSABLE_BIT |
                  STUFFY_WINDOW_STYLE_ICONIFIABLE_BIT | STUFFY_WINDOW_STYLE_RESIZABLE_BIT,
  };

  // Open window
  StuffyWindow *window = stuffy_window_open (&config);
  if (window == NULL)
  {
    fprintf (stderr, "Failed to open window\n");
    stuffy_app_deinit ( );
    return EXIT_FAILURE;
  }

  printf ("Window opened successfully!\n");
  printf ("Press ESC to close the window\n");
  printf ("Try resizing the window to see the resize callback in action\n");
  printf ("Try moving the window to see the move callback in action\n");

  // Set window resize callback
  stuffy_window_set_resize_callback (window, on_window_resize);

  // Set window move callback
  stuffy_window_set_move_callback (window, on_window_move);

  // Main loop
  while (!stuffy_window_should_close (window))
  {
    // Update application (processes events and updates input state)
    stuffy_app_update ( );

    // Get input states
    const StuffyKeyboardState *keyboard = stuffy_keyboard_get_state ( );

    // Get window resolution

    // Check for ESC key to close window
    if (keyboard->keys[ STUFFY_KEY_ESCAPE ]) { break; }
  }

  // Cleanup
  stuffy_window_close (window);
  stuffy_app_deinit ( );

  printf ("Application closed successfully\n");

  return EXIT_SUCCESS;
}
