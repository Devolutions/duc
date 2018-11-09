
#include <winpr/platform.h>

#include <Wayk/NowMath.h>

#define WAYK_SIMD_INTERNAL
#include "simd.h"

#if defined(_M_IX86) || defined(_M_AMD64)
#include "NowCodec_sse2.c"
#include "NowColor_sse2.c"
#include "simd_x86.c"
#endif

#if defined(_M_ARM) || defined(_M_ARM64)
#include "NowCodec_neon.c"
#include "NowColor_neon.c"
#include "simd_arm.c"
#endif
