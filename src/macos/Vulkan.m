#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>
#include <vulkan/vulkan_core.h>

#import <vulkan/vulkan.h>
#import <vulkan/vulkan_metal.h>

#import "stuffy/vulkan.h"

#import "src/macos/Window.h"

static const char *g_extension_names[] = {
  "VK_EXT_metal_surface",
  "VK_KHR_surface",
  VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
#ifdef DEBUG
  VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
};

const VkDeviceExtensions VK_DEVICE_EXTENSIONS = {
  .count = sizeof (g_extension_names) / sizeof (g_extension_names[ 0 ]),
  .names = g_extension_names,
};

VkResult create_vk_surface (
  const VkSurfaceCreateInfo *info, VkSurfaceKHR *surface)
{
  const VkMetalSurfaceCreateInfoEXT surface_create_info = {
    .sType  = VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT,
    .pNext  = NULL,
    .flags  = 0,
    .pLayer = info->window->metal_layer,
  };

  return vkCreateMetalSurfaceEXT (
    info->instance, &surface_create_info, info->allocator, surface);
}

