#ifndef MYON_GRAPHICS_STRUCTS_H
#define MYON_GRAPHICS_STRUCTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "myongraphics_enums.h"
#include "myongraphics_impl.h"

typedef struct {
  Backend backend;
  union {
    VulkanInstance vulkan;
  };
} myonInstance_T;

typedef myonInstance_T* myonInstance;

#ifdef __cplusplus
}
#endif

#endif
