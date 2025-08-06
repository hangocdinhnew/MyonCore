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
  MG_BACKEND_METAL,
  ENUM_FORCE32(MG_BACKEND)
} myonGBackend;

typedef enum {
  MG_RESULT_SUBOPTIMAL = 1,
  MG_RESULT_SUCCESS = 0,
  MG_RESULT_OUT_OF_MEMORY = -1,
  MG_RESULT_BACKEND_ERROR = -2,
  MG_RESULT_UNKNOWN_BACKEND = -3,
  MG_RESULT_NIL_POINTER = -4,
  ENUM_FORCE32(MG_RESULT)
} myonGResult;

typedef enum {
  MG_LOG_LEVEL_OFF = 0,
  MG_LOG_LEVEL_ERROR,
  MG_LOG_LEVEL_WARN,
  MG_LOG_LEVEL_INFO,
  MG_LOG_LEVEL_DEBUG,
  MG_LOG_LEVEL_TRACE,
  ENUM_FORCE32(MG_LOG_LEVEL)
} myonGLogLevel;

#ifdef __cplusplus
}
#endif

#endif
