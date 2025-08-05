#ifndef MYON_GRAPHICS_INTERNAL_H
#define MYON_GRAPHICS_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "myongraphics_enums.h"
#include "myongraphics_impl.h"

struct myonInstance_T {
  myonBackend backend;
  union {
    VulkanInstance vulkan;
  };
};

void myonLog(myonLogLevel level, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
