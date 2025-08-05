#ifndef MYON_GRAPHICS_FUNC_H
#define MYON_GRAPHICS_FUNC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "myongraphics_enums.h"
#include "myongraphics_structs.h"

typedef void (*myonLogCallback)(myonLogLevel level, const char *message);

myonResult myonCreateInstance(myonBackend backend, myonInstance *instance);
void myonDestroyInstance(myonInstance instance);

void myonSetLogCallback(myonLogCallback callback);
void myonSetLogLevel(myonLogLevel level);

#ifdef __cplusplus
}
#endif

#endif
