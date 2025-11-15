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
  @file include/stuffy/vulkan.h
  @brief Vulkan integration functions
  @author Ilya Buravov (ilburale@gmail.com)
  @details This header file provides functions for creating Vulkan surfaces
           and managing Vulkan device extensions.
*/

#pragma once

#include <stdint.h>

#include <vulkan/vulkan.h>

#include "stuffy/apidef.h"
#include "stuffy/window.h"

/*
  @brief Vulkan surface creation information.
  @details Contains the parameters required to create a Vulkan surface from
           a window. This structure is used to bridge the window system with
           Vulkan rendering.
*/
typedef struct
{
  /* Window handle. */
  const StuffyWindow *window;

  /* Vulkan instance. */
  VkInstance instance;

  /* Vulkan allocation callbacks. */
  const VkAllocationCallbacks *allocator;

  /* A pointer to an out vulkan surface variable. */
} StuffyVkSurfaceCreateInfo;

/*
  @brief Creates a Vulkan surface from a window.
  @details Creates a Vulkan surface that can be used for rendering to the
           specified window. The surface is created using platform-specific
           Vulkan extensions.
  @param info Pointer to the surface creation information (must not be NULL).
  @param surface Pointer to a VkSurfaceKHR variable that will receive the
                 created surface handle (must not be NULL).
  @return VK_SUCCESS on success, or a Vulkan error code on failure.
  @note The created surface must be destroyed using vkDestroySurfaceKHR
        when no longer needed.
*/
__STUFFY_API__ VkResult
stuffy_vk_create_surface (const StuffyVkSurfaceCreateInfo *info, VkSurfaceKHR *surface);
