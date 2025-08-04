#ifndef MYON_GRAPHICS_IMPL_H
#define MYON_GRAPHICS_IMPL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <vulkan/vulkan.h>

typedef struct {
  VkInstance instance;
  VkApplicationInfo appInfo;
  VkInstanceCreateInfo createInfo;
} VulkanInstance;

#ifdef __cplusplus
}
#endif

#endif
