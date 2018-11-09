#ifndef WAYK_CODEC_SIMD_H
#define WAYK_CODEC_SIMD_H

#include <Wayk/Now.h>
#include <Wayk/NowMath.h>
#include <Wayk/NowProto.h>

#define SIMD_NONE	0
#define SIMD_SSE2	1
#define SIMD_NEON	2

#if defined(__aarch64__) || defined(__x86_64__)
#define BITS	64
#else
#define BITS	32
#endif

unsigned int init_simd(void);
unsigned int get_simd(void);
unsigned int auto_simd(void);
unsigned int override_simd(unsigned int simd);

#ifndef WAYK_SIMD_INTERNAL

#if (defined(__arm__) || defined(__aarch64__)) && defined(__ANDROID__)

/* Using memcpy() to implement the Copy algorithm always seems to be faster on
   Android, but our algorithm is faster on iOS. */

#define NowCodec_Copy_simd NowCodec_Copy_c

#else

int NowCodec_Copy_simd(BYTE* pDstData, int nDstStep, int nXDst, int nYDst,
	int nWidth, int nHeight, BYTE* pSrcData, int nSrcStep, int nXSrc, int nYSrc);

#endif

int NowCodec_Move_simd(BYTE* pData, int nStep, int nXDst, int nYDst,
	int nWidth, int nHeight, int nXSrc, int nYSrc);

int NowCodec_CopyFromRetina_simd(BYTE* pDstData, int nDstStep, int nXDst,
	int nYDst, int nWidth, int nHeight, BYTE* pSrcData, int nSrcStep, int nXSrc,
	int nYSrc);

#if !defined(LINEAR_COMPARE32) && (defined(__arm__) || defined(__aarch64__)) && defined(__ANDROID__)

#define NowCodec_Compare32_simd NowCodec_Compare32_c

#else

int NowCodec_Compare32_simd(BYTE* pData1, int step1, BYTE* pData2, int step2,
	int width, int height, NOW_EDGE_RECT* rect);

#endif

#if defined(__aarch64__) && defined(__APPLE__)

/* Our optimized UINT32 C Compare8 algorithm is faster than NEON when using
   64-bit code on iOS devices. */

#define NowCodec_Compare8_simd NowCodec_Compare8_c

#else

int NowCodec_Compare8_simd(BYTE* pData1, int step1, BYTE* pData2, int step2,
	int width, int height, NOW_EDGE_RECT* rect);

#endif

void NowColor_YCoCgR420ToRGB_8u_P3AC4R_simd(const BYTE* pSrc[3],
	int srcStep[3], BYTE* pDst, int dstStep, int width, int height);

void NowColor_RGBToYCoCgR420_8u_P3AC4R_simd(const BYTE* pSrc, INT32 srcStep,
	BYTE* pDst[3], INT32 dstStep[3], int width, int height);

#endif

#endif /* WAYK_CODEC_SIMD_H */
