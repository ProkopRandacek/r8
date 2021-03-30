// vim: filetype=c
unsigned int shd(void);
void shdSetInt(unsigned int s, const char* name, int value);
void shdSetFloat(unsigned int s, const char* name, float value);
void shdSetIVec2(unsigned int s, const char* name, int x, int y);
void shdSetVec3Array (unsigned int s, const char* name, int count, float* values);
void shdSetVec4Array (unsigned int s, const char* name, int count, float* values);
void shdSetFloatArray(unsigned int s, const char* name, int count, float* values);
void shdSetIntArray  (unsigned int s, const char* name, int count, int*   values);
