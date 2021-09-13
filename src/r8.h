#ifndef R8_H
#define R8_H

#if defined(R8_EDITOR) && defined(R8_RUNTIME)
#error "You can't compile editor and runtime together"
#endif

void r8_init(void);

#ifdef R8_EDITOR
void r8_run_editor(void);
void r8_exit(void);
#endif

#endif
