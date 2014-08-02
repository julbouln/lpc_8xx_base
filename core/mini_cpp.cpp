#include <stdlib.h>                   // for prototypes of malloc() and free()
#include <stdint.h>
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

// Approximate some math func for size optimization
// https://code.google.com/p/fastapprox/
extern "C" float
fastpow2 (float p)
{
  float offset = (p < 0) ? 1.0f : 0.0f;
  float clipp = (p < -126) ? -126.0f : p;
  int w = clipp;
  float z = clipp - w + offset;
  union { uint32_t i; float f; } v = { (uint32_t) ( (1 << 23) * (clipp + 121.2740575f + 27.7280233f / (4.84252568f - z) - 1.49012907f * z) ) };

  return v.f;
}

extern "C" float 
fastlog2 (float x)
{
  union { float f; uint32_t i; } vx = { x };
  union { uint32_t i; float f; } mx = { (vx.i & 0x007FFFFF) | 0x3f000000 };
  float y = vx.i;
  y *= 1.1920928955078125e-7f;

  return y - 124.22551499f
           - 1.498030302f * mx.f 
           - 1.72587999f / (0.3520887068f + mx.f);
}

extern "C" float
fastlog (float x)
{
  return 0.69314718f * fastlog2 (x);
}

extern "C" float
fastpow (float x,
         float p)
{
  return fastpow2 (p * fastlog2 (x));
}

extern "C" float
fastexp (float p)
{
  return fastpow2 (1.442695040f * p);
}


// redefinitions


extern "C" double log(double x)
{
	return (double) fastlog((float) x);
}

extern "C" double exp(double x)
{
	return (double) fastexp((float) x);
}


extern "C" double pow(double x, double y)
{
	return (double) fastpow((float) x,(float) y);
}