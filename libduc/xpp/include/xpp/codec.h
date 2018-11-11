#ifndef DUC_XPP_CODEC_H
#define DUC_XPP_CODEC_H

#include <xpp/xpp.h>

/**
 * Rectangle represented by 4 edges
 *
 * width = right - left;
 * height = bottom - top;
 */

struct _DUC_EDGE_RECT
{
	int16_t left;
	int16_t top;
	int16_t right;
	int16_t bottom;
};
typedef struct _DUC_EDGE_RECT DUC_EDGE_RECT;

#ifdef __cplusplus
extern "C" {
#endif

DUC_EXPORT int Xpp_Copy(uint8_t* pDstData, int nDstStep, int nXDst, int nYDst, int nWidth, int nHeight,
			      uint8_t* pSrcData, int nSrcStep, int nXSrc, int nYSrc);

DUC_EXPORT int Xpp_CopyFromRetina(uint8_t* pDstData, int nDstStep, int nXDst, int nYDst, int nWidth, int nHeight,
					uint8_t* pSrcData, int nSrcStep, int nXSrc, int nYSrc);

DUC_EXPORT int Xpp_Compare(uint8_t* pData1, int nStep1, int nWidth, int nHeight, uint8_t* pData2, int nStep2,
				 DUC_EDGE_RECT* rect);

DUC_EXPORT int Xpp_Compare32(uint8_t* pData1, int step1, uint8_t* pData2, int step2, int width, int height,
				   DUC_EDGE_RECT* rect);

DUC_EXPORT int Xpp_Compare8(uint8_t* pData1, int step1, uint8_t* pData2, int step2, int width, int height,
				  DUC_EDGE_RECT* rect);

DUC_EXPORT int Xpp_Move(uint8_t* pData, int nStep, int nXDst, int nYDst, int nWidth, int nHeight, int nXSrc,
			      int nYSrc);

DUC_EXPORT int Xpp_Copy_c(uint8_t* pDstData, int nDstStep, int nXDst, int nYDst, int nWidth, int nHeight,
				uint8_t* pSrcData, int nSrcStep, int nXSrc, int nYSrc);

DUC_EXPORT int Xpp_CopyFromRetina_c(uint8_t* pDstData, int nDstStep, int nXDst, int nYDst, int nWidth,
					  int nHeight, uint8_t* pSrcData, int nSrcStep, int nXSrc, int nYSrc);

DUC_EXPORT int Xpp_Compare_c(uint8_t* pData1, int nStep1, int nWidth, int nHeight, uint8_t* pData2, int nStep2,
				   DUC_EDGE_RECT* rect);

DUC_EXPORT int Xpp_Compare32_c(uint8_t* pData1, int step1, uint8_t* pData2, int step2, int width, int height,
				     DUC_EDGE_RECT* rect);

DUC_EXPORT int Xpp_Compare8_c(uint8_t* pData1, int step1, uint8_t* pData2, int step2, int width, int height,
				    DUC_EDGE_RECT* rect);

DUC_EXPORT int Xpp_Move_c(uint8_t* pData, int nStep, int nXDst, int nYDst, int nWidth, int nHeight, int nXSrc,
				int nYSrc);

DUC_EXPORT int Xpp_Copy_halide(uint8_t* pDstData, int nDstStep, int nXDst, int nYDst, int nWidth, int nHeight,
				     uint8_t* pSrcData, int nSrcStep, int nXSrc, int nYSrc);

DUC_EXPORT int Xpp_Compare8_halide(uint8_t* pData1, int step1, uint8_t* pData2, int step2, int width, int height,
					 DUC_EDGE_RECT* rect);

DUC_EXPORT int Xpp_Compare32_halide(uint8_t* pData1, int step1, uint8_t* pData2, int step2, int width, int height,
					  DUC_EDGE_RECT* rect);

#ifdef __cplusplus
}
#endif

#endif /* DUC_XPP_CODEC_H */
