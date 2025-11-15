#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>
#include <vulkan/vulkan_core.h>

#import <vulkan/vulkan.h>
#import <vulkan/vulkan_metal.h>

#import "stuffy/vulkan.h"

#import "src/macos/Window.h"

StuffyVkDeviceExtensions stuffy_vk_get_required_device_extensions() {
  static const char *extension_names[] = {
      "VK_KHR_surface",
      "VK_EXT_metal_surface",
  };

  return (StuffyVkDeviceExtensions){
      .count = sizeof(extension_names) / sizeof(extension_names[0]),
      .names = extension_names,
  };
}

VkResult stuffy_vk_create_surface(const StuffyVkSurfaceCreateInfo *info,
                           VkSurfaceKHR *surface) {
  const VkMetalSurfaceCreateInfoEXT surface_create_info = {
      .sType = VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT,
      .pNext = NULL,
      .flags = 0,
      .pLayer = info->window->metal_layer,
  };

  return vkCreateMetalSurfaceEXT(info->instance, &surface_create_info,
                                 info->allocator, surface);
}
