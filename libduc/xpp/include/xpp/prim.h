#ifndef XPP_PRIMITIVES_H
#define XPP_PRIMITIVES_H

#include <xpp/xpp.h>

#include <xpp/codec.h>

#define XPP_PRIMITIVES_GENERIC  0x1
#define XPP_PRIMITIVES_SIMD     0x2
#define XPP_PRIMITIVES_HALIDE   0x4
#define XPP_PRIMITIVES_ALL      0x7

typedef int(*fnXppPrimitivesCompare32)(uint8_t* pData1, int step1, uint8_t* pData2, int step2, int width, int height,
				       DUC_EDGE_RECT* rect);
typedef int(*fnXppPrimitivesCompare8)(uint8_t* pData1, int step1, uint8_t* pData2, int step2, int width, int height,
				      DUC_EDGE_RECT* rect);
typedef int(*fnXppPrimitivesCopy)(uint8_t* pDstData, int nDstStep, int nXDst, int nYDst, int nWidth, int nHeight,
				  uint8_t* pSrcData, int nSrcStep, int nXSrc, int nYSrc);
typedef int(*fnXppPrimitivesMove)(uint8_t* pData, int nStep, int nXDst, int nYDst, int nWidth, int nHeight, int nXSrc,
				  int nYSrc);
typedef int(*fnXppPrimitivesCopyFromRetina)(uint8_t* pDstData, int nDstStep, int nXDst, int nYDst, int nWidth,
					    int nHeight, uint8_t* pSrcData, int nSrcStep, int nXSrc, int nYSrc);
typedef void(*fnXppPrimitivesYCoCgR420ToRGB_8u_P3AC4R)(const uint8_t* pSrc[3], int srcStep[3], uint8_t* pDst,
						       int dstStep, int width, int height);
typedef void(*fnXppPrimitivesRGBToYCoCgR420_8u_P3AC4R)(const uint8_t* pSrc, int32_t srcStep, uint8_t* pDst[3],
						       int32_t dstStep[3], int width, int height);

typedef struct now_primitives
{
	fnXppPrimitivesCompare32 Compare32;
	fnXppPrimitivesCompare8 Compare8;
	fnXppPrimitivesYCoCgR420ToRGB_8u_P3AC4R YCoCgR420ToRGB_8u_P3AC4R;
	fnXppPrimitivesRGBToYCoCgR420_8u_P3AC4R RGBToYCoCgR420_8u_P3AC4R;
	fnXppPrimitivesCopy Copy;
	fnXppPrimitivesMove Move;
	fnXppPrimitivesCopyFromRetina CopyFromRetina;
} XppPrimitives;

bool XppPrimitives_Init(XppPrimitives* primitives, uint32_t flags);
XppPrimitives* XppPrimitives_Get();

#endif /* XPP_PRIMITIVES_H */
