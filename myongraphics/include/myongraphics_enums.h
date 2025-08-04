#ifndef MYON_GRAPHICS_ENUMS_H
#define MYON_GRAPHICS_ENUMS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define ENUM_FORCE32(name) name##_FORCE32 = INT32_MAX

typedef enum {
  BACKEND_NONE = 0,
  BACKEND_VULKAN,
  ENUM_FORCE32(BACKEND)
} Backend;

#ifdef __cplusplus
}
#endif

#endif
