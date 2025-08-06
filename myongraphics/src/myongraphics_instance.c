// clang-format off
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "myongraphics_internal.h"
#include "myongraphics.h"
// clang-format on

#define MAX_EXTENSIONS 32
#define MAX_LAYERS 4

static myonLogCallback g_logCallback = NULL;
static myonLogLevel g_logLevel = MG_LOG_LEVEL_WARN;

void myonSetLogCallback(myonLogCallback callback) { g_logCallback = callback; }

void myonSetLogLevel(myonLogLevel level) { g_logLevel = level; }

void myonLog(myonLogLevel level, const char *fmt, ...) {
  if (level > g_logLevel || level == MG_LOG_LEVEL_OFF) {
    return;
  }

  char buffer[1024];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  if (g_logCallback) {
    g_logCallback(level, buffer);
  } else {
    switch (level) {
    case MG_LOG_LEVEL_ERROR:
      fprintf(stderr, "[myongraphics] Error: %s\n", buffer);
      break;
    case MG_LOG_LEVEL_WARN:
      printf("[myongraphics] Warning: %s\n", buffer);
      break;
    case MG_LOG_LEVEL_INFO:
      printf("[myongraphics] Info: %s\n", buffer);
      break;
    case MG_LOG_LEVEL_DEBUG:
      printf("[myongraphics] Debug: %s\n", buffer);
      break;
    case MG_LOG_LEVEL_TRACE:
      printf("[myongraphics] Trace: %s\n", buffer);
      break;
    default:
      fprintf(stderr, "[myongraphics] Unknown loglevel: %s\n", buffer);
      break;
    }
  }
}

VKAPI_ATTR VkBool32 VKAPI_CALL vk_DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT *callbackData, void *userData) {
  myonLog(MG_LOG_LEVEL_TRACE, "vk_DebugCallback");

  myonLogLevel level = MG_LOG_LEVEL_INFO;

  if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    level = MG_LOG_LEVEL_ERROR;
  else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    level = MG_LOG_LEVEL_WARN;
  else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
    level = MG_LOG_LEVEL_TRACE;
  else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
    level = MG_LOG_LEVEL_DEBUG;

  myonLog(level, "Vulkan - %s", callbackData->pMessage);

  return VK_FALSE;
}

static const char *vk_desired_extensions[] = {
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

static const char *vk_desired_layers[] = {"VK_LAYER_KHRONOS_validation", NULL};

static uint32_t vk_desired_extension_count = 0;
static const char *vk_filtered_extensions[MAX_EXTENSIONS];
static uint32_t vk_filtered_extension_count = 0;

static const char *vk_filtered_layers[MAX_LAYERS];
static uint32_t vk_filtered_layers_count = 0;

static void vk_init_filtered_extensions() {
  myonLog(MG_LOG_LEVEL_TRACE, "Vulkan - vk_init_filtered_extensions");

  uint32_t available_count = 0;
  vkEnumerateInstanceExtensionProperties(NULL, &available_count, NULL);

  VkExtensionProperties *props =
      malloc(sizeof(VkExtensionProperties) * available_count);
  vkEnumerateInstanceExtensionProperties(NULL, &available_count, props);

  for (vk_desired_extension_count = 0;
       vk_desired_extensions[vk_desired_extension_count] != NULL;
       ++vk_desired_extension_count)
    ;
  vk_filtered_extension_count = 0;

  for (uint32_t i = 0; i < vk_desired_extension_count; ++i) {
    const char *desired = vk_desired_extensions[i];
    int found = 0;

    for (uint32_t j = 0; j < available_count; ++j) {
      if (strcmp(desired, props[j].extensionName) == 0) {
        found = 1;
        break;
      }
    }

    if (found) {
      vk_filtered_extensions[vk_filtered_extension_count++] = desired;
    } else {
      myonLog(MG_LOG_LEVEL_WARN, "Vulkan - Extension not available: %s",
              desired);
    }
  }

  free(props);

  myonLog(MG_LOG_LEVEL_DEBUG, "Vulkan - Extensions: [");
  for (size_t i = 0; i < vk_filtered_extension_count; i++) {
    myonLog(MG_LOG_LEVEL_DEBUG, "Vulkan - \t%s,", vk_filtered_extensions[i]);
  }
  myonLog(MG_LOG_LEVEL_DEBUG, "Vulkan - ]");
  myonLog(MG_LOG_LEVEL_DEBUG, "Vulkan - Extensions count: %d",
          vk_filtered_extension_count);

  myonLog(MG_LOG_LEVEL_DEBUG,
          "Vulkan - vk_init_filtered_extensions succeeded!");
}

static void vk_init_filtered_layers() {
  myonLog(MG_LOG_LEVEL_TRACE, "Vulkan - vk_init_filtered_layers");

  uint32_t count = 0;
  vkEnumerateInstanceLayerProperties(&count, NULL);

  VkLayerProperties *props = malloc(sizeof(VkLayerProperties) * count);
  vkEnumerateInstanceLayerProperties(&count, props);

  vk_filtered_layers_count = 0;
  for (uint32_t i = 0; vk_desired_layers[i] != NULL; ++i) {
    for (uint32_t j = 0; j < count; ++j) {
      if (strcmp(vk_desired_layers[i], props[j].layerName) == 0) {
        vk_filtered_layers[vk_filtered_layers_count++] = vk_desired_layers[i];
        break;
      }
    }
  }

  free(props);

  myonLog(MG_LOG_LEVEL_DEBUG, "Vulkan - Layers: [");
  for (size_t i = 0; i < vk_filtered_layers_count; i++) {
    myonLog(MG_LOG_LEVEL_DEBUG, "Vulkan - \t%s,", vk_filtered_layers[i]);
  }
  myonLog(MG_LOG_LEVEL_DEBUG, "Vulkan - ]");
  myonLog(MG_LOG_LEVEL_DEBUG, "Vulkan - Layers count: %d",
          vk_filtered_layers_count);

  myonLog(MG_LOG_LEVEL_DEBUG, "Vulkan - vk_init_filtered_layers succeeded!");
}

myonResult myonCreateInstance(myonBackend backend, myonInstance *instance) {
  myonLog(MG_LOG_LEVEL_TRACE, "myonCreateInstance");

  myonInstance internal_Instance = (myonInstance)calloc(1, sizeof(myonInstance_T));
  if (!internal_Instance) {
    myonLog(MG_LOG_LEVEL_ERROR, "Out of memory!");
    return MG_RESULT_OUT_OF_MEMORY;
  }

  internal_Instance->backend = backend;

  switch (backend) {
  case MG_BACKEND_VULKAN: {
    vk_init_filtered_extensions();
    vk_init_filtered_layers();

    VkApplicationInfo appInfo = {.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                 .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                                 .engineVersion = VK_MAKE_VERSION(1, 0, 0),
                                 .apiVersion = VK_API_VERSION_1_4};
    internal_Instance->vulkan.appInfo = appInfo;

    VkInstanceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &internal_Instance->vulkan.appInfo,
        .enabledExtensionCount = vk_filtered_extension_count,
        .ppEnabledExtensionNames = vk_filtered_extensions,
        .enabledLayerCount = vk_filtered_layers_count,
        .ppEnabledLayerNames = vk_filtered_layers};

    for (uint32_t i = 0; i < vk_filtered_extension_count; ++i) {
      if (strcmp(vk_filtered_extensions[i], "VK_KHR_portability_enumeration") ==
          0) {
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
        break;
      }
    }

    internal_Instance->vulkan.createInfo = createInfo;

    VkInstance vk_Instance;
    VkResult result = vkCreateInstance(&internal_Instance->vulkan.createInfo,
                                       NULL, &vk_Instance);

    if (result != VK_SUCCESS) {
      myonLog(MG_LOG_LEVEL_ERROR, "Failed to create Vulkan instance!");
      return MG_RESULT_BACKEND_ERROR;
    }
    internal_Instance->vulkan.instance = vk_Instance;

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = vk_DebugCallback,
        .pUserData = NULL};

    PFN_vkCreateDebugUtilsMessengerEXT func =
        (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            internal_Instance->vulkan.instance,
            "vkCreateDebugUtilsMessengerEXT");

    if (func != NULL) {
      VkResult result =
          func(internal_Instance->vulkan.instance, &debugCreateInfo, NULL,
               &internal_Instance->vulkan.debugMessenger);
      if (result != VK_SUCCESS) {
        myonLog(MG_LOG_LEVEL_WARN, "Failed to create Vulkan debug messenger.");
      }
    } else {
      myonLog(MG_LOG_LEVEL_WARN, "vkCreateDebugUtilsMessengerEXT not found.");
    }

    break;
  }

  case MG_BACKEND_METAL: {
#ifndef MYONG_METAL
    myonLog(MG_LOG_LEVEL_ERROR, "Metal - Metal is not enabled!");
    return MG_RESULT_UNKNOWN_BACKEND;
#else
    myonLog(MG_LOG_LEVEL_DEBUG, "Metal - Instance creation is no-op.");
    break;
#endif
  }

  default:
    myonLog(MG_LOG_LEVEL_ERROR, "Unknown backend!");
    return MG_RESULT_UNKNOWN_BACKEND;
  }

  *instance = internal_Instance;

  myonLog(MG_LOG_LEVEL_DEBUG, "Instance created successfully!");
  return MG_RESULT_SUCCESS;
}

void myonDestroyInstance(myonInstance instance) {
  myonLog(MG_LOG_LEVEL_TRACE, "myonDestroyInstance");

  switch (instance->backend) {
  case MG_BACKEND_VULKAN: {
    PFN_vkDestroyDebugUtilsMessengerEXT destroyFunc =
        (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            instance->vulkan.instance, "vkDestroyDebugUtilsMessengerEXT");

    destroyFunc(instance->vulkan.instance, instance->vulkan.debugMessenger,
                NULL);

    vkDestroyInstance(instance->vulkan.instance, NULL);
    break;
  }

  case MG_BACKEND_METAL: {
#ifndef MYONG_METAL
    myonLog(MG_LOG_LEVEL_ERROR, "Metal - Metal Is not enabled!");
#else
    myonLog(MG_LOG_LEVEL_DEBUG, "Metal - Instance destruction is no-op.");
#endif
    break;
  }

  default:
    break;
  }

  free(instance);
}
