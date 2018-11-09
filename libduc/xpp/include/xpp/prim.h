#ifndef NOW_PRIMITIVES_H
#define NOW_PRIMITIVES_H

#include <xpp/xpp.h>

#include <xpp/codec.h>

#define NOW_PRIMITIVES_GENERIC  0x1
#define NOW_PRIMITIVES_SIMD     0x2
#define NOW_PRIMITIVES_HALIDE   0x4
#define NOW_PRIMITIVES_ALL      0x7

typedef int(*fnNowPrimitivesCompare32)(uint8_t* pData1, int step1, uint8_t* pData2, int step2, int width, int height,
				       NOW_EDGE_RECT* rect);
typedef int(*fnNowPrimitivesCompare8)(uint8_t* pData1, int step1, uint8_t* pData2, int step2, int width, int height,
				      NOW_EDGE_RECT* rect);
typedef int(*fnNowPrimitivesCopy)(uint8_t* pDstData, int nDstStep, int nXDst, int nYDst, int nWidth, int nHeight,
				  uint8_t* pSrcData, int nSrcStep, int nXSrc, int nYSrc);
typedef int(*fnNowPrimitivesMove)(uint8_t* pData, int nStep, int nXDst, int nYDst, int nWidth, int nHeight, int nXSrc,
				  int nYSrc);
typedef int(*fnNowPrimitivesCopyFromRetina)(uint8_t* pDstData, int nDstStep, int nXDst, int nYDst, int nWidth,
					    int nHeight, uint8_t* pSrcData, int nSrcStep, int nXSrc, int nYSrc);
typedef void(*fnNowPrimitivesYCoCgR420ToRGB_8u_P3AC4R)(const uint8_t* pSrc[3], int srcStep[3], uint8_t* pDst,
						       int dstStep, int width, int height);
typedef void(*fnNowPrimitivesRGBToYCoCgR420_8u_P3AC4R)(const uint8_t* pSrc, int32_t srcStep, uint8_t* pDst[3],
						       int32_t dstStep[3], int width, int height);

typedef struct now_primitives
{
	fnNowPrimitivesCompare32 Compare32;
	fnNowPrimitivesCompare8 Compare8;
	fnNowPrimitivesYCoCgR420ToRGB_8u_P3AC4R YCoCgR420ToRGB_8u_P3AC4R;
	fnNowPrimitivesRGBToYCoCgR420_8u_P3AC4R RGBToYCoCgR420_8u_P3AC4R;
	fnNowPrimitivesCopy Copy;
	fnNowPrimitivesMove Move;
	fnNowPrimitivesCopyFromRetina CopyFromRetina;
} NowPrimitives;

bool NowPrimitives_Init(NowPrimitives* primitives, uint32_t flags);
NowPrimitives* NowPrimitives_Get();

#endif /* NOW_PRIMITIVES_H */
