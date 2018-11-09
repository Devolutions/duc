
#include <xpp/prim.h>
#include <xpp/codec.h>
#include <xpp/color.h>

#ifdef WITH_CODEC_SIMD
#include "simd/simd.h"
#endif

static NowPrimitives g_Primitives = { 0 };
static bool primitivesInitialized = false;

bool NowPrimitives_Init(NowPrimitives* primitives, uint32_t flags)
{
	bool result = false;

	if (!primitives)
	{
		return false;
	}

	memset(primitives, 0, sizeof(NowPrimitives));

	if (flags & NOW_PRIMITIVES_GENERIC)
	{
		primitives->Compare32 = NowCodec_Compare32_c;
		primitives->Compare8 = NowCodec_Compare8_c;
		primitives->Copy = NowCodec_Copy_c;
		primitives->CopyFromRetina = NowCodec_CopyFromRetina_c;
		primitives->Move = NowCodec_Move_c;
		primitives->RGBToYCoCgR420_8u_P3AC4R = NowColor_RGBToYCoCgR420_8u_P3AC4R_c;
		primitives->YCoCgR420ToRGB_8u_P3AC4R = NowColor_YCoCgR420ToRGB_8u_P3AC4R_c;

		result = true;
	}

#ifdef WITH_CODEC_SIMD
	if (flags & NOW_PRIMITIVES_SIMD)
	{
		primitives->Compare32 = NowCodec_Compare32_simd;
		primitives->Compare8 = NowCodec_Compare8_simd;
		primitives->Copy = NowCodec_Copy_simd;
		primitives->CopyFromRetina = NowCodec_CopyFromRetina_simd;
		primitives->Move = NowCodec_Move_simd;
		primitives->RGBToYCoCgR420_8u_P3AC4R = NowColor_RGBToYCoCgR420_8u_P3AC4R_simd;
		primitives->YCoCgR420ToRGB_8u_P3AC4R = NowColor_YCoCgR420ToRGB_8u_P3AC4R_simd;
		result = true;
	}
#endif

#ifdef WITH_HALIDE
	if (flags & NOW_PRIMITIVES_HALIDE)
	{
		primitives->Copy = NowCodec_Copy_halide;
		primitives->Compare32 = NowCodec_Compare32_halide;
		primitives->RGBToYCoCgR420_8u_P3AC4R = NowColor_Halide_RGBToYCoCgR420_8u_P3AC4R;
		primitives->YCoCgR420ToRGB_8u_P3AC4R = NowColor_Halide_YCoCgR420ToRGB_8u_P3AC4R;
		result = true;
	}
#endif

	return result;
}

NowPrimitives* NowPrimitives_Get()
{
	if (!primitivesInitialized)
		NowPrimitives_Init(&g_Primitives, NOW_PRIMITIVES_ALL);

#ifdef WITH_HALIDE
#ifdef WITH_CODEC_SIMD
	g_Primitives.Compare32 = NowCodec_Compare32_simd;
	g_Primitives.Compare8 = NowCodec_Compare8_simd;
#else
	g_Primitives.Compare32 = NowCodec_Compare32_c;
	g_Primitives.Compare8 = NowCodec_Compare8_c;
#endif
#endif

	return &g_Primitives;
}