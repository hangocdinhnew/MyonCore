// clang-format off
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "myongraphics_internal.h"
#include "myongraphics.h"
#include "vulkan/vulkan_core.h"
// clang-format on

#define MAX_EXTENSIONS 32

static const char *desired_extensions[] = {
    "VK_KHR_surface",

#if defined(_WIN32)
    "VK_KHR_win32_surface",
#elif defined(__APPLE__)
    "VK_EXT_metal_surface",
    "VK_KHR_portability_enumeration",
#elif defined(__linux__)
    "VK_KHR_xlib_surface",
    "VK_KHR_xcb_surface",
    "VK_KHR_wayland_surface",
    "VK_EXT_acquire_drm_display",
    "VK_EXT_direct_mode_display",
    "VK_KHR_display",
    "VK_EXT_physical_device_drm",
    "VK_KHR_get_display_properties2",
#endif

    "VK_EXT_debug_utils",
    "VK_EXT_swapchain_colorspace",
    "VK_KHR_get_physical_device_properties2",
    NULL};

static uint32_t desired_extension_count = 0;
static const char *filtered_extensions[MAX_EXTENSIONS];
static uint32_t filtered_extension_count = 0;

static void init_filtered_extensions(void) {
  uint32_t available_count = 0;
  vkEnumerateInstanceExtensionProperties(NULL, &available_count, NULL);

  VkExtensionProperties *props =
      malloc(sizeof(VkExtensionProperties) * available_count);
  vkEnumerateInstanceExtensionProperties(NULL, &available_count, props);

  for (desired_extension_count = 0;
       desired_extensions[desired_extension_count] != NULL;
       ++desired_extension_count)
    ;
  filtered_extension_count = 0;

  for (uint32_t i = 0; i < desired_extension_count; ++i) {
    const char *desired = desired_extensions[i];
    int found = 0;

    for (uint32_t j = 0; j < available_count; ++j) {
      if (strcmp(desired, props[j].extensionName) == 0) {
        found = 1;
        break;
      }
    }

    if (found) {
      filtered_extensions[filtered_extension_count++] = desired;
    } else {
      fprintf(stderr, "[myongraphics] Extension not available: %s\n", desired);
    }
  }

  free(props);
}

myonResult myonCreateInstance(myonBackend backend, myonInstance* instance) {
  myonInstance internal_Instance = (myonInstance)malloc(sizeof(myonInstance_T));
  if (!internal_Instance) {
    return MG_RESULT_OUT_OF_MEMORY;
  }
  memset(internal_Instance, 0, sizeof(myonInstance_T));

  internal_Instance->backend = backend;

  switch (backend) {
  case MG_BACKEND_VULKAN: {
    init_filtered_extensions();

    VkApplicationInfo appInfo = {.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                 .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                                 .pEngineName = "Myon Graphics Engine",
                                 .engineVersion = VK_MAKE_VERSION(1, 0, 0),
                                 .apiVersion = VK_API_VERSION_1_4};
    internal_Instance->vulkan.appInfo = appInfo;

    VkInstanceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &internal_Instance->vulkan.appInfo,
        .enabledExtensionCount = filtered_extension_count,
        .ppEnabledExtensionNames = filtered_extensions};

    for (uint32_t i = 0; i < filtered_extension_count; ++i) {
      if (strcmp(filtered_extensions[i], "VK_KHR_portability_enumeration") ==
          0) {
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
        break;
      }
    }

    internal_Instance->vulkan.createInfo = createInfo;

    VkInstance vk_Instance;
    VkResult result =
        vkCreateInstance(&internal_Instance->vulkan.createInfo, NULL, &vk_Instance);

    if (result != VK_SUCCESS) {
      fprintf(stderr, "[myongraphics] Failed to create Vulkan instance: %d\n",
              result);
      abort();
    }
    internal_Instance->vulkan.instance = vk_Instance;

    break;
  }
  default:
    return MG_RESULT_UNKNOWN_BACKEND;
  }

  *instance = internal_Instance;

  return MG_RESULT_SUCCESS;
}

void myonDestroyInstance(myonInstance instance) {
  switch (instance->backend) {
  case MG_BACKEND_VULKAN: {
    vkDestroyInstance(instance->vulkan.instance, NULL);
    break;
  }
  default:
    break;
  }

  free(instance);
}
