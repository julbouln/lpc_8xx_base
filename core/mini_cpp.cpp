#include <stdlib.h>                   // for prototypes of malloc() and free()

//............................................................................
void *operator new(size_t size) throw() {
    return malloc(size);
}
//............................................................................
void operator delete(void *p) throw() {
    free(p);
}
//............................................................................
extern "C" int __aeabi_atexit(void *object,
                              void (*destructor)(void *),
                              void *dso_handle)
{
    // avoid "unused" warnings (mthomas):
    object = object; destructor=destructor; dso_handle=dso_handle;

    return 0;
}
  
  
extern "C" void * _sbrk (int n)
{
	return (void *)-1;
}