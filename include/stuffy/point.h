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
  @file include/stuffy/point.h
  @brief Point structure declaration
  @author Osfabias
  @details This header file provides a simple 2D point structure for representing
           coordinates in screen or window space.
*/

#pragma once

#include <stdint.h>

#include "stuffy/apidef.h"

/*
  @brief Point in screen coordinates in pixels.
  @details Represents a 2D point with integer coordinates. Used for representing
           positions in screen or window coordinate space.
*/
typedef struct
{
  int32_t x, y;
} Point;

