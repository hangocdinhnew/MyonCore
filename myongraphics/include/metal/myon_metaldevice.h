#ifndef MYON_METAL_DEVICE_H
#define MYON_METAL_DEVICE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void* MTLDevice;

extern MTLDevice *MTLCreateSystemDefaultDevice();

#ifdef __cplusplus
}
#endif

#endif
