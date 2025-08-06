// clang-format off
#include <string.h>
#include <stdlib.h>

#include "myongraphics_internal.h"
#include "myongraphics.h"
// clang-format on

static const char *requiredDeviceExtensions[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_SPIRV_1_4_EXTENSION_NAME,
    VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
    VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME};

myonResult myonEnumeratePhysicalDevices(myonInstance instance,
                                        myonPhysicalDevice *physicalDevice) {
  myonLog(MG_LOG_LEVEL_TRACE, "myonEnumeratePhysicalDevices");

  if (!instance) {
    myonLog(MG_LOG_LEVEL_ERROR, "Instance or outDeviceCount is NULL!");
    return MG_RESULT_NIL_POINTER;
  }

  myonPhysicalDevice internal_pDevice = calloc(1, sizeof(myonPhysicalDevice_T));
  if (!internal_pDevice) {
    myonLog(MG_LOG_LEVEL_ERROR, "Out of memory for physical device wrapper!");
    return MG_RESULT_OUT_OF_MEMORY;
  }

  switch (instance->backend) {
  case MG_BACKEND_VULKAN: {
    uint32_t vk_count = 0;
    VkResult vk_res =
        vkEnumeratePhysicalDevices(instance->vulkan.instance, &vk_count, NULL);

    if (vk_res != VK_SUCCESS || vk_count == 0) {
      myonLog(MG_LOG_LEVEL_ERROR, "Vulkan - No physical devices found!");
      return MG_RESULT_BACKEND_ERROR;
    }

    VkPhysicalDevice *vkDevices = malloc(sizeof(VkPhysicalDevice) * vk_count);
    if (!vkDevices) {
      myonLog(MG_LOG_LEVEL_ERROR, "Out of memory for vkDevices array!");
      return MG_RESULT_OUT_OF_MEMORY;
    }

    vkEnumeratePhysicalDevices(instance->vulkan.instance, &vk_count, vkDevices);

    myonResult result = MG_RESULT_BACKEND_ERROR;

    for (uint32_t i = 0; i < vk_count; ++i) {
      VkPhysicalDevice device = vkDevices[i];

      VkPhysicalDeviceProperties props;
      vkGetPhysicalDeviceProperties(device, &props);
      if (props.apiVersion < VK_API_VERSION_1_3) {
        myonLog(MG_LOG_LEVEL_WARN,
                "Skipping device %u: Vulkan API version < 1.3", i);
        continue;
      }

      uint32_t queueCount = 0;
      vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, NULL);
      if (queueCount == 0) {
        myonLog(MG_LOG_LEVEL_WARN,
                "Skipping device %u: No queue families found", i);
        continue;
      }

      VkQueueFamilyProperties *queueProps =
          malloc(sizeof(VkQueueFamilyProperties) * queueCount);
      if (!queueProps) {
        myonLog(MG_LOG_LEVEL_ERROR, "Out of memory for queue properties!");
        result = MG_RESULT_OUT_OF_MEMORY;
        break;
      }

      vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, queueProps);
      bool hasGraphicsQueue = false;
      for (uint32_t q = 0; q < queueCount; ++q) {
        if (queueProps[q].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
          hasGraphicsQueue = true;
          break;
        }
      }
      free(queueProps);

      if (!hasGraphicsQueue) {
        myonLog(MG_LOG_LEVEL_WARN, "Skipping device %u: No graphics queue", i);
        continue;
      }

      uint32_t extCount = 0;
      vkEnumerateDeviceExtensionProperties(device, NULL, &extCount, NULL);
      VkExtensionProperties *extensions =
          malloc(sizeof(VkExtensionProperties) * extCount);
      if (!extensions) {
        myonLog(MG_LOG_LEVEL_ERROR, "Out of memory for extension enumeration!");
        result = MG_RESULT_OUT_OF_MEMORY;
        break;
      }

      vkEnumerateDeviceExtensionProperties(device, NULL, &extCount, extensions);

      bool allExtensionsFound = true;
      size_t requiredExtCount = sizeof(requiredDeviceExtensions) /
                                sizeof(requiredDeviceExtensions[0]);
      for (size_t e = 0; e < requiredExtCount; ++e) {
        bool found = false;
        for (uint32_t k = 0; k < extCount; ++k) {
          if (strcmp(requiredDeviceExtensions[e],
                     extensions[k].extensionName) == 0) {
            found = true;
            break;
          }
        }
        if (!found) {
          allExtensionsFound = false;
          break;
        }
      }
      free(extensions);

      if (!allExtensionsFound) {
        myonLog(MG_LOG_LEVEL_WARN,
                "Skipping device %u: Missing required Vulkan extensions", i);
        continue;
      }

      VkPhysicalDeviceVulkan13Features features13 = {
          .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES};
      VkPhysicalDeviceExtendedDynamicStateFeaturesEXT extDynState = {
          .sType =
              VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT};

      features13.pNext = &extDynState;

      VkPhysicalDeviceFeatures2 features2 = {
          .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
          .pNext = &features13};

      vkGetPhysicalDeviceFeatures2(device, &features2);

      if (!features13.dynamicRendering || !extDynState.extendedDynamicState) {
        myonLog(MG_LOG_LEVEL_WARN,
                "Skipping device %u: Required features not supported", i);
        continue;
      }

      internal_pDevice->backend = MG_BACKEND_VULKAN;
      internal_pDevice->vulkan.device = device;

      myonLog(MG_LOG_LEVEL_DEBUG, "Vulkan - Current Physical Device:");
      myonLog(MG_LOG_LEVEL_DEBUG, "Vulkan - \tName: %s", props.deviceName);
      myonLog(MG_LOG_LEVEL_DEBUG, "Vulkan - \tAPI Version: %d.%d.%d",
              VK_VERSION_MAJOR(props.apiVersion),
              VK_VERSION_MINOR(props.apiVersion),
              VK_VERSION_PATCH(props.apiVersion));
      myonLog(MG_LOG_LEVEL_DEBUG, "Vulkan - \tDriver Version: %u",
              props.driverVersion);
      myonLog(MG_LOG_LEVEL_DEBUG, "Vulkan - \tVendor ID: 0x%04X",
              props.vendorID);
      myonLog(MG_LOG_LEVEL_DEBUG, "Vulkan - \tDevice ID: 0x%04X",
              props.deviceID);
      myonLog(MG_LOG_LEVEL_DEBUG, "Vulkan - \tDevice Type: %s",
              props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
                  ? "Discrete GPU"
              : props.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU
                  ? "Integrated GPU"
              : props.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU ? "CPU"
                                                                : "Other");

      result = MG_RESULT_SUCCESS;
      break;
    }

    free(vkDevices);
    if (result >= MG_RESULT_SUCCESS)
      break;

    return result;
  }

  case MG_BACKEND_METAL: {
#ifndef MYONG_METAL
    myonLog(MG_LOG_LEVEL_ERROR, "Metal is not enabled");
    return MG_RESULT_UNKNOWN_BACKEND;
#else
    myonLog(MG_LOG_LEVEL_DEBUG, "Metal physical device enumeration is no-op.");
    break;
#endif
  }

  default:
    myonLog(MG_LOG_LEVEL_ERROR, "Unknown backend!");
    return MG_RESULT_UNKNOWN_BACKEND;
  }

  *physicalDevice = internal_pDevice;

  myonLog(MG_LOG_LEVEL_DEBUG, "Enumeration of physical device succeeded!");
  return MG_RESULT_SUCCESS;
}

void myonDestroyPhysicalDevice(myonPhysicalDevice device) {
  myonLog(MG_LOG_LEVEL_TRACE, "myonDestroyPhysicalDevice");

  if (!device)
    return;

  free(device);
}
