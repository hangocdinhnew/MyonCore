#ifndef MYON_GRAPHICS_FUNC_H
#define MYON_GRAPHICS_FUNC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "myongraphics_enums.h"
#include "myongraphics_structs.h"

typedef void (*myonGLogCallback)(myonGLogLevel level, const char *message);

myonGResult myonGCreateInstance(myonGBackend backend, myonGInstance *instance);
void myonGDestroyInstance(myonGInstance instance);

myonGResult myonGEnumeratePhysicalDevices(myonGInstance instance,
                                        myonGPhysicalDevice *physicalDevice);
void myonGDestroyPhysicalDevice(myonGPhysicalDevice device);

void myonGSetLogCallback(myonGLogCallback callback);
void myonGSetLogLevel(myonGLogLevel level);

#ifdef __cplusplus
}
#endif

#endif
