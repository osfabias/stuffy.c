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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <stuffy/app.h>
#include <stuffy/input/keyboard.h>
#include <stuffy/input/mouse.h>
#include <stuffy/window.h>

int main (void)
{
  // Initialize application
  if (init_app ( ) != 0)
  {
    fprintf (stderr, "Failed to initialize application\n");
    return EXIT_FAILURE;
  }

  // Configure window
  WindowConf conf = {
    .title      = "Stuffy.c Example",
    .rect       = {.x = 100, .y = 100, .width = 800, .height = 600},
    .style_mask = WINDOW_STYLE_TITLED_BIT | WINDOW_STYLE_CLOSABLE_BIT |
                  WINDOW_STYLE_ICONIFIABLE_BIT | WINDOW_STYLE_RESIZABLE_BIT,
  };

  // Open window
  Window *window = open_window (&conf);
  if (window == NULL)
  {
    fprintf (stderr, "Failed to open window\n");
    deinit_app ( );
    return EXIT_FAILURE;
  }

  printf ("Window opened successfully!\n");
  printf ("Press ESC to close the window\n");

  // Main loop
  while (!should_window_close (window))
  {
    // Update application (processes events and updates input state)
    update_app ( );

    // Get input states
    const KeyboardState *keyboard = get_keyboard_state ( );
    const MouseState    *mouse    = get_mouse_state ( );

    // Check for ESC key to close window
    if (keyboard->keys[ KEY_ESCAPE ]) { break; }

    // Example: Print mouse position when mouse moves
    static int32_t last_x = -1, last_y = -1;
    if (mouse->x != last_x || mouse->y != last_y)
    {
      last_x = mouse->x;
      last_y = mouse->y;
      printf ("Mouse position: (%d, %d)\n", mouse->x, mouse->y);
    }

    // Example: Print when mouse buttons are pressed
    if (mouse->buttons[ MOUSE_BUTTON_LEFT ]) { puts ("Left mouse button pressed"); }
  }

  // Cleanup
  close_window (window);
  deinit_app ( );

  printf ("Application closed successfully\n");

  return EXIT_SUCCESS;
}
