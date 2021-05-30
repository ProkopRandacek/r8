// vim: filetype=c
#include "common.h"

unsigned int shd(const char* vertShdSource, const char* fragShdSource);

void shdSetInt       (const char* name, int value);
void shdSetFloat     (const char* name, float value);
void shdSetIVec2     (const char* name, int x, int y);
void shdSetVec3Array (const char* name, int count, float* values);
void shdSetVec4Array (const char* name, int count, float* values);
void shdSetFloatArray(const char* name, int count, float* values);
void shdSetIntArray  (const char* name, int count, int*   values);

void activateShader(unsigned int s);
unsigned int createShader(Scene* s);
