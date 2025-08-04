#ifndef MYON_GRAPHICS_INTERNAL_H
#define MYON_GRAPHICS_INTERNAL_H

#include "myongraphics_enums.h"
#include "myongraphics_impl.h"

struct myonInstance_T {
  myonBackend backend;
  union {
    VulkanInstance vulkan;
  };
};

void myonLog(myonLogLevel level, const char* fmt, ...);

#endif
