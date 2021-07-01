// vim: filetype=c
#ifdef DEBUG
void* dmalloc(size_t size);
void dfree(void*);
#else
#define dmalloc malloc
#define dfree free
#endif

