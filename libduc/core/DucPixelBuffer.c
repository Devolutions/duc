
#include <duc/core.h>

/**
 * Intel IPP naming convention:
 *
 * Function Naming:
 * https://software.intel.com/en-us/node/503696
 *
 * Name:
 * https://software.intel.com/en-us/node/503700
 *
 * Descriptors:
 * https://software.intel.com/en-us/node/503923
 *
 * Data Types:
 * https://software.intel.com/en-us/node/503922
 *
 * Parameters:
 * https://software.intel.com/en-us/node/503925
 */

size_t Duc_GetRegionOffset(int16_t x, int16_t y, uint16_t step, uint8_t bytesPerPixel)
{
	return (x * bytesPerPixel) + (step * y);
}

uint8_t* Duc_GetRegionPointer(uint8_t* pImage, int16_t x, int16_t y, uint16_t step, uint8_t bytesPerPixel)
{
	return &pImage[(x * bytesPerPixel) + (step * y)];
}

uint8_t* Duc_GetImagePointer(uint8_t* pRegion, int16_t x, int16_t y, uint16_t step, uint8_t bytesPerPixel)
{
	return &pRegion[-1 * ((x * bytesPerPixel) + (step * y))];
}

size_t Duc_GetPlaneSize(uint16_t step, uint16_t vstep)
{
	return step * vstep;
}

size_t Duc_GetPlaneSizes(uint16_t step[4], uint16_t vstep[4], size_t planeSize[4])
{
	size_t totalSize[5];

	if (!planeSize)
		planeSize = totalSize;

	planeSize[0] = step[0] * vstep[0];
	planeSize[1] = step[1] * vstep[1];
	planeSize[2] = step[2] * vstep[2];
	planeSize[3] = step[3] * vstep[3];
	totalSize[4] = planeSize[0] + planeSize[1] + planeSize[2] + planeSize[3];

	return totalSize[4];
}

int Duc_Copy_8u_CXR(uint8_t* pSrc, int srcStep, uint8_t* pDst, int dstStep, int width, int height, int c)
{
	int y;
	uint8_t* src = pSrc;
	uint8_t* dst = pDst;
	int roiStep = width * c;

	for (y = 0; y < height; y++)
	{
		duc_memcpy(dst, src, roiStep);
		src += srcStep;
		dst += dstStep;
	}

	return 1;
}

int Duc_Copy_8u_C1R(uint8_t* pSrc, int srcStep, uint8_t* pDst, int dstStep, int width, int height)
{
	return Duc_Copy_8u_CXR(pSrc, srcStep, pDst, dstStep, width, height, 1);
}

int Duc_Copy_8u_C2R(uint8_t* pSrc, int srcStep, uint8_t* pDst, int dstStep, int width, int height)
{
	return Duc_Copy_8u_CXR(pSrc, srcStep, pDst, dstStep, width, height, 2);
}

int Duc_Copy_8u_C3R(uint8_t* pSrc, int srcStep, uint8_t* pDst, int dstStep, int width, int height)
{
	return Duc_Copy_8u_CXR(pSrc, srcStep, pDst, dstStep, width, height, 3);
}

int Duc_Copy_8u_C4R(uint8_t* pSrc, int srcStep, uint8_t* pDst, int dstStep, int width, int height)
{
	return Duc_Copy_8u_CXR(pSrc, srcStep, pDst, dstStep, width, height, 4);
}

int Duc_XRGB32ToRGB(uint8_t* pSrc, int srcStep, uint8_t* pDst[3], int dstStep[3], int width, int height)
{
	int x, y;
	int srcPad;
	int dstPad[3];
	uint8_t* src = pSrc;
	uint8_t* dst[3];

	dst[0] = pDst[0];
	dst[1] = pDst[1];
	dst[2] = pDst[2];

	srcPad = (srcStep - width * 4);

	dstPad[0] = (dstStep[0] - width);
	dstPad[1] = (dstStep[1] - width);
	dstPad[2] = (dstStep[2] - width);

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			*dst[0] = src[0];
			*dst[1] = src[1];
			*dst[2] = src[2];

			dst[0]++;
			dst[1]++;
			dst[2]++;
			src += 4;
		}

		dst[0] += dstPad[0];
		dst[1] += dstPad[1];
		dst[2] += dstPad[2];
		src += srcPad;
	}

	return 1;
}

int Duc_RGBToXRGB32(uint8_t* pSrc[3], int srcStep[3], uint8_t* pDst, int dstStep, int width, int height)
{
	int x, y;
	int dstPad;
	int srcPad[3];
	uint8_t* dst = pDst;
	uint8_t* src[3];

	src[0] = pSrc[0];
	src[1] = pSrc[1];
	src[2] = pSrc[2];

	dstPad = (dstStep - width * 4);

	srcPad[0] = (srcStep[0] - width);
	srcPad[1] = (srcStep[1] - width);
	srcPad[2] = (srcStep[2] - width);

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			dst[0] = *src[0];
			dst[1] = *src[1];
			dst[2] = *src[2];
			dst[3] = 0xFF;

			src[0]++;
			src[1]++;
			src[2]++;
			dst += 4;
		}

		src[0] += srcPad[0];
		src[1] += srcPad[1];
		src[2] += srcPad[2];
		dst += dstPad;
	}

	return 1;
}
