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
  @file include/stuffy/app.h
  @brief Application state management functions
  @author Ilya Buravov (ilburale@gmail.com)
*/

#pragma once

#include <stdint.h>

#include "stuffy/apidef.h"

/*
  @brief Initializes the application.
  @details Initializes the application instance. This must be called before
           using any other library functions that require an application context.
  @return 0 on success, non-zero on failure.
  @note The application must be deinitialized using @ref deinit_app when no longer
        needed.
*/
__STUFFY_API__ int32_t init_app (void);

/*
  @brief Updates the application state.
  @details Processes events, updates input states, and performs other
           per-frame operations. This function should be called once per
           frame in the main application loop.
  @note This function must be called regularly for the application to
        function correctly. Input states are updated during this call.
*/
__STUFFY_API__ void update_app (void);

/*
  @brief Deinitializes the application instance.
  @details Frees all resources associated with the application instance.
  @note After calling this function, the application should not be used for
        any further operations.
*/
__STUFFY_API__ void deinit_app (void);
