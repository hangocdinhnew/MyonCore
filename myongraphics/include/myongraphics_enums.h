#ifndef MYON_GRAPHICS_ENUMS_H
#define MYON_GRAPHICS_ENUMS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define ENUM_FORCE32(name) name##_FORCE32 = INT32_MAX

typedef enum {
  MG_BACKEND_NONE = 0,
  MG_BACKEND_VULKAN,
  ENUM_FORCE32(BACKEND)
} myonBackend;

typedef enum {
  MG_RESULT_SUCCESS = 0,
  MG_RESULT_OUT_OF_MEMORY = -1,
  MG_RESULT_BACKEND_ERROR = -2,
  MG_RESULT_UNKNOWN_BACKEND = -3
} myonResult;

#ifdef __cplusplus
}
#endif

#endif
