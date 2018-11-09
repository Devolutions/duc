#ifndef DUC_XPP_CODEC_H
#define DUC_XPP_CODEC_H

#include <xpp/xpp.h>

/**
 * Rectangle represented by 4 edges
 *
 * width = right - left;
 * height = bottom - top;
 */

struct _NOW_EDGE_RECT
{
	int16_t left;
	int16_t top;
	int16_t right;
	int16_t bottom;
};
typedef struct _NOW_EDGE_RECT NOW_EDGE_RECT;

/**
 * Rectangle represented by position and size
 */

struct _NOW_SIZE_RECT
{
	int16_t  x;
	int16_t  y;
	uint16_t width;
	uint16_t height;
};
typedef struct _NOW_SIZE_RECT NOW_SIZE_RECT;

#ifdef __cplusplus
extern "C" {
#endif

DUC_EXPORT int NowCodec_Copy(uint8_t* pDstData, int nDstStep, int nXDst, int nYDst, int nWidth, int nHeight,
			      uint8_t* pSrcData, int nSrcStep, int nXSrc, int nYSrc);

DUC_EXPORT int NowCodec_CopyFromRetina(uint8_t* pDstData, int nDstStep, int nXDst, int nYDst, int nWidth, int nHeight,
					uint8_t* pSrcData, int nSrcStep, int nXSrc, int nYSrc);

DUC_EXPORT int NowCodec_Compare(uint8_t* pData1, int nStep1, int nWidth, int nHeight, uint8_t* pData2, int nStep2,
				 NOW_EDGE_RECT* rect);

DUC_EXPORT int NowCodec_Compare32(uint8_t* pData1, int step1, uint8_t* pData2, int step2, int width, int height,
				   NOW_EDGE_RECT* rect);

DUC_EXPORT int NowCodec_Compare8(uint8_t* pData1, int step1, uint8_t* pData2, int step2, int width, int height,
				  NOW_EDGE_RECT* rect);

DUC_EXPORT int NowCodec_Move(uint8_t* pData, int nStep, int nXDst, int nYDst, int nWidth, int nHeight, int nXSrc,
			      int nYSrc);

DUC_EXPORT int NowCodec_Copy_c(uint8_t* pDstData, int nDstStep, int nXDst, int nYDst, int nWidth, int nHeight,
				uint8_t* pSrcData, int nSrcStep, int nXSrc, int nYSrc);

DUC_EXPORT int NowCodec_CopyFromRetina_c(uint8_t* pDstData, int nDstStep, int nXDst, int nYDst, int nWidth,
					  int nHeight, uint8_t* pSrcData, int nSrcStep, int nXSrc, int nYSrc);

DUC_EXPORT int NowCodec_Compare_c(uint8_t* pData1, int nStep1, int nWidth, int nHeight, uint8_t* pData2, int nStep2,
				   NOW_EDGE_RECT* rect);

DUC_EXPORT int NowCodec_Compare32_c(uint8_t* pData1, int step1, uint8_t* pData2, int step2, int width, int height,
				     NOW_EDGE_RECT* rect);

DUC_EXPORT int NowCodec_Compare8_c(uint8_t* pData1, int step1, uint8_t* pData2, int step2, int width, int height,
				    NOW_EDGE_RECT* rect);

DUC_EXPORT int NowCodec_Move_c(uint8_t* pData, int nStep, int nXDst, int nYDst, int nWidth, int nHeight, int nXSrc,
				int nYSrc);

DUC_EXPORT int NowCodec_Copy_halide(uint8_t* pDstData, int nDstStep, int nXDst, int nYDst, int nWidth, int nHeight,
				     uint8_t* pSrcData, int nSrcStep, int nXSrc, int nYSrc);

DUC_EXPORT int NowCodec_Compare8_halide(uint8_t* pData1, int step1, uint8_t* pData2, int step2, int width, int height,
					 NOW_EDGE_RECT* rect);

DUC_EXPORT int NowCodec_Compare32_halide(uint8_t* pData1, int step1, uint8_t* pData2, int step2, int width, int height,
					  NOW_EDGE_RECT* rect);

#ifdef __cplusplus
}
#endif

#endif /* DUC_XPP_CODEC_H */
