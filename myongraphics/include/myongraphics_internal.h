#ifndef MYON_GRAPHICS_INTERNAL_H
#define MYON_GRAPHICS_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "myongraphics_enums.h"
#include "myongraphics_impl.h"

struct myonGInstance_T {
  myonGBackend backend;
  union {
    VulkanInstance vulkan;
  };
};

struct myonGPhysicalDevice_T {
  myonGBackend backend;

  union {
    VulkanPhysicalDevice vulkan;
  };
};

void myonGLog(myonGLogLevel level, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
