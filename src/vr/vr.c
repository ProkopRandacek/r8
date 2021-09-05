#include <stdio.h>

#include "vr.h"
#include "log.h"

static struct VR_IVRSystem_FnTable *IVRSystem;

void *get_openvr_func_table(const char *interfacename) {
	EVRInitError e;
	char fnTableName[128];
	snprintf(fnTableName, 128, "FnTable:%s", interfacename);
	void *ret = (void *)VR_GetGenericInterface(fnTableName, &e);
	msg("Getting System FnTable: %s = %p (%d)\n", fnTableName, ret, e);
	if (!ret) die("Could not get openvr func table");
	return ret;
}

void init_vr() {
	EVRInitError e;
	uint32_t vrtoken;
	vrtoken = VR_InitInternal(&e, EVRApplicationType_VRApplication_Scene);
	if (!vrtoken) {
		msg("Error calling VR_InitInternal: %d (%s)\n", e, VR_GetVRInitErrorAsEnglishDescription(e));
		die("Failed to start OpenVR");
	}

	IVRSystem = (struct VR_IVRSystem_FnTable *)get_openvr_func_table(IVRSystem_Version);
}

