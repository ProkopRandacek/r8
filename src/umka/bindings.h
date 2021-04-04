#include "../../submodules/umka/src/umka_api.h"

void umkaBind(void* umka);

void umSetShapeClr(UmkaStackSlot* p, UmkaStackSlot* r);
void umSetShapePos(UmkaStackSlot* p, UmkaStackSlot* r);
void umSetShapeRv (UmkaStackSlot* p, UmkaStackSlot* r);

void umCreateSphere  (UmkaStackSlot* P, UmkaStackSlot* R);
void umCreateCube    (UmkaStackSlot* P, UmkaStackSlot* R);
void umCreateTorus   (UmkaStackSlot* P, UmkaStackSlot* R);
void umCreateCapsule (UmkaStackSlot* P, UmkaStackSlot* R);
void umCreateCylinder(UmkaStackSlot* P, UmkaStackSlot* R);
void umCreateCCone   (UmkaStackSlot* P, UmkaStackSlot* R);
void umCreateGroup   (UmkaStackSlot* P, UmkaStackSlot* R);

void umGetCamPos  (UmkaStackSlot* p, UmkaStackSlot* r);
void umGetCamDir  (UmkaStackSlot* p, UmkaStackSlot* r);
void umSetLightPos(UmkaStackSlot* p, UmkaStackSlot* r);
