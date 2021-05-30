// vim: filetype=c
#include "common.h"

char* createVertSource(void);

char* createVersionSource(void);
char* createSettingsSource(Scene* s);
char* createHeaderSource(void);
char* createSDFsSource(short shapesMask);
char* createMapWorldSource(Scene* s);
char* createEndSource(void);
char* createFragSource(Scene* s);

char* genSDF(Scene* s);
char* genSDFD(Scene* s);
void makeSDF(Scene* s, int pos);
void makeSDFD(Scene* s, int pos);
void makeShape(Scene* s, int i);
char* strReplace(const char* org, char pre, char* post);
