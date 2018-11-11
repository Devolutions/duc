#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xpp/scale.h>

#ifdef WITH_LIBYUV

#include <libyuv/libyuv.h>

static FilterModeEnum XppXpp_GetLibYuvFilterMode(int mode)
{
	switch (mode)
	{
		case XppInterpolationNearest:
			return kFilterNone;
		case XppInterpolationLinear:
			return kFilterLinear;
		case XppInterpolationBilinear:
			return kFilterBilinear;
		case XppInterpolationBox:
			return kFilterBox;
	}

	return kFilterBilinear;
}

#endif

int XppXpp_Scale_8u_C4R(const uint8_t* pSrc, int srcStep, int srcWidth, int srcHeight, uint8_t* pDst, int dstStep,
			int dstWidth, int dstHeight, int mode)
{
	int status = -1;

#ifdef WITH_LIBYUV
	status = ARGBScale(pSrc, srcStep, srcWidth, srcHeight, pDst, dstStep, dstWidth, dstHeight,
			   XppXpp_GetLibYuvFilterMode(mode));
#endif

	return status;
}
