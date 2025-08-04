#ifndef MYON_GRAPHICS_FUNC_H
#define MYON_GRAPHICS_FUNC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "myongraphics_structs.h"

myonResult myonCreateInstance(myonBackend backend, myonInstance* instance);
void myonDestroyInstance(myonInstance instance);

#ifdef __cplusplus
}
#endif

#endif
