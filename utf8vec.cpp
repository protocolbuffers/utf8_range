#include <stdint.h>
#include <string.h>


#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

#include "utf8-simd.h"
#include "utils.h"
#include "utf8.h"

// -----------------------------------------------------------------------------
// Taken from  https://github.com/zwegner/faster-utf8-validator/blob/master/z_validate.c
//
// modified
// - standard includes (stdint ...)
// - buffer overflow in the last loop iteration (up to 31 bytes are read past input buffers)
// - exact bounds about in length comparisons (offset + V_LEN <= len) 
// - loadu -> lddqu (documentation says it is faster)
// - fast ascii7 scan at the start
// 
// -----------------------------------------------------------------------------


// vector length is 64 bytes
#if defined (__AVX512F__)
#define V_LEN 64
#include "vecutf8.h"
#endif

// vector length is 32 bytes
#if defined (__AVX2__)
#define V_LEN 32
#include "vecutf8.h"
#endif

extern "C" int utf8vec_avx512(const unsigned char*, int);

extern "C" int utf8vec_avx2(const unsigned char*, int);

// Return 0 - success, -1 - error 
int utf8vec_avx2(const unsigned char *data, int len) {
    uint8_t* buf = const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(data));
    return u_utf8_d256(buf, len) ? 0 : -1;
}

// Return 0 - success, -1 - error 
int utf8vec_avx512(const unsigned char *data, int len) {
    uint8_t* buf = const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(data));
    return u_utf8_d512(buf, len) ? 0 : -1;
}