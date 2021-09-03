// vim: ft=c
#ifndef OPENVR_H
#define OPENVR_H

// this includes the openvr api but ignores the warnings that the api generates
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wstrict-prototypes"
#undef EXTERN_C
#include <openvr_capi.h>
#pragma GCC diagnostic pop

#include <stdlib.h>

#include "common.h"

// OpenVR doesn't define these for some reason so we define the functions here. They are copy-pasted from the bottom of openvr_capi.h
intptr_t VR_InitInternal(EVRInitError *peError, EVRApplicationType eType);
void VR_ShutdownInternal(void);
bool VR_IsHmdPresent(void);
intptr_t VR_GetGenericInterface(const char *pchInterfaceVersion, EVRInitError *peError);
bool VR_IsRuntimeInstalled(void);
const char *VR_GetVRInitErrorAsSymbol(EVRInitError error);
const char *VR_GetVRInitErrorAsEnglishDescription(EVRInitError error);

/** @brief Internal */
void *get_openvr_func_table(const char *interfacename);

void init_vr(void);

#endif
