#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>
#include <vulkan/vulkan_core.h>

#import <vulkan/vulkan.h>
#import <vulkan/vulkan_metal.h>

#import "stuffy/vulkan.h"

#import "src/macos/Window.h"

VkDeviceExtensions get_required_vk_device_extensions() {
  static const char *extension_names[] = {
      "VK_KHR_surface",
      "VK_EXT_metal_surface",
  };

  return (VkDeviceExtensions){
      .count = sizeof(extension_names) / sizeof(extension_names[0]),
      .names = extension_names,
  };
}

VkResult create_vk_surface(const VkSurfaceCreateInfo *info,
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
