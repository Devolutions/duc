#ifndef DUC_XPP_COLOR_H
#define DUC_XPP_COLOR_H

#include <xpp/xpp.h>

#define NOW_COLOR_FORMAT_XRGB		0
#define NOW_COLOR_FORMAT_YUV420		1
#define NOW_COLOR_FORMAT_YCOCGR		2
#define NOW_COLOR_FORMAT_YCOCGR420	3

#ifdef __cplusplus
extern "C" {
#endif

DUC_EXPORT void NowColor_YUV420ToRGB_8u_P3AC4R(const uint8_t* pSrc[3], int srcStep[3], uint8_t* pDst, int dstStep,
						int width, int height);

DUC_EXPORT void NowColor_RGBToYUV420_8u_P3AC4R(const uint8_t* pSrc, int32_t srcStep, uint8_t* pDst[3],
						int32_t dstStep[3], int width, int height);

DUC_EXPORT void NowColor_YCoCgR420ToRGB_8u_P3AC4R(const uint8_t* pSrc[3], int srcStep[3], uint8_t* pDst, int dstStep,
						   int width, int height);

DUC_EXPORT void NowColor_RGBToYCoCgR420_8u_P3AC4R(const uint8_t* pSrc, int32_t srcStep, uint8_t* pDst[3],
						   int32_t dstStep[3], int width, int height);

DUC_EXPORT void NowColor_RGBToYCoCgR420_8u_P3AC4R_ds2x(const uint8_t* pSrc, int32_t srcStep, uint8_t* pDst[3],
							int32_t dstStep[3], int width, int height);

DUC_EXPORT void NowColor_YCoCgRToRGB_16s_P3AC4R(const int16_t* pSrc[3], int srcStep[3], uint8_t* pDst, int dstStep,
						 int width, int height);

DUC_EXPORT void NowColor_RGBToYCoCgR_16s_P3AC4R(const uint8_t* pSrc, int32_t srcStep, int16_t* pDst[3],
						 int32_t dstStep[3], int width, int height);

DUC_EXPORT void NowColor_MultiplyAlpha(const uint8_t* pSrc, int32_t srcStep, uint8_t* pDst, int32_t dstStep, int width,
					int height);

DUC_EXPORT void NowColor_UnmultiplyAlpha(const uint8_t* pSrc, int32_t srcStep, uint8_t* pDst, int32_t dstStep,
					  int width, int height);

DUC_EXPORT void NowColor_YCoCgR420ToRGB_8u_P3AC4R_c(const uint8_t* pSrc[3], int srcStep[3], uint8_t* pDst, int dstStep,
						     int width, int height);

DUC_EXPORT void NowColor_RGBToYCoCgR420_8u_P3AC4R_c(const uint8_t* pSrc, int32_t srcStep, uint8_t* pDst[3],
						     int32_t dstStep[3], int width, int height);

DUC_EXPORT void NowColor_Halide_RGBToYCoCgR420_8u_P3AC4R(const uint8_t* pSrc, int32_t srcStep, uint8_t* pDst[3],
							  int32_t dstStep[3], int width, int height);

DUC_EXPORT void NowColor_Halide_YCoCgR420ToRGB_8u_P3AC4R(const uint8_t* pSrc[3], int srcStep[3], uint8_t* pDst,
							  int dstStep, int width, int height);

#ifdef __cplusplus
}
#endif

#endif /* DUC_XPP_COLOR_H */
