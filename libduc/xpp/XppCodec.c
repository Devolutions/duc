#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xpp/math.h>
#include <xpp/color.h>
#include <xpp/codec.h>
#include <xpp/prim.h>

#ifdef WITH_CODEC_SIMD
#include "simd/simd.h"
#endif

#ifdef WITH_LIBYUV
#include <libyuv/libyuv.h>
#endif

#ifdef WITH_HALIDE
#include <halide/Copy.h>
#include <halide/Compare32Stage1.h>
#include <halide/Compare8Stage1.h>
#include <HalideRuntime.h>
#endif

#define TAG "NowCodec"

#ifdef WITH_HALIDE

void codec_setup_u32_buffer_t(buffer_t* buffer, uint32_t* data, int width, int height, int stride)
{
	memset(buffer, 0, sizeof(buffer_t));

	buffer->extent[0] = width;
	buffer->extent[1] = height;
	buffer->elem_size = 4;
	buffer->host = (uint8_t*)data;
	buffer->stride[0] = 1;
	buffer->stride[1] = stride;
	buffer->host_dirty = true;
}

void codec_setup_1d_u32_buffer_t(buffer_t* buffer, uint32_t* data, int width)
{
	memset(buffer, 0, sizeof(buffer_t));

	buffer->extent[0] = width;
	buffer->elem_size = 4;
	buffer->host = (uint8_t*)data;
	buffer->stride[0] = 1;
	buffer->host_dirty = true;
}

void codec_setup_u8_buffer_t(buffer_t* buffer, uint8_t* data, int width, int height, int stride)
{
	memset(buffer, 0, sizeof(buffer_t));

	buffer->extent[0] = width;
	buffer->extent[1] = height;
	buffer->elem_size = 4;
	buffer->host = data;
	buffer->stride[0] = 1;
	buffer->stride[1] = stride;
	buffer->host_dirty = true;
}

void codec_setup_1d_u8_buffer_t(buffer_t* buffer, uint8_t* data, int width)
{
	memset(buffer, 0, sizeof(buffer_t));

	buffer->extent[0] = width;
	buffer->elem_size = 1;
	buffer->host = data;
	buffer->stride[0] = 1;
	buffer->host_dirty = true;
}

int NowCodec_Copy_halide(uint8_t* pDstData, int nDstStep, int nXDst, int nYDst,
	int nWidth, int nHeight, uint8_t* pSrcData, int nSrcStep, int nXSrc, int nYSrc)
{
	buffer_t input;
	buffer_t output;

	codec_setup_u32_buffer_t(&input, (uint32_t*)&pSrcData[nXSrc * sizeof(uint32_t) + nYSrc * nSrcStep], nWidth,
				 nHeight, nSrcStep / sizeof(uint32_t));
	codec_setup_u32_buffer_t(&output, (uint32_t*)&pDstData[nXDst * sizeof(uint32_t) + nYDst * nDstStep], nWidth,
				 nHeight, nDstStep / sizeof(uint32_t));

	Copy_old_buffer_t(&input, &output);

	return 0;
}

int NowCodec_Compare32_halide(uint8_t* pData1, int step1, uint8_t* pData2, int step2, int width, int height,
			      DUC_EDGE_RECT* rect)
{
	int i;

	buffer_t frame1Buffer;
	buffer_t frame2Buffer;
	buffer_t diffXBuffer;
	buffer_t diffYBuffer;

	uint32_t* diffX = calloc(width, sizeof(uint32_t));
	uint32_t* diffY = calloc(height, sizeof(uint32_t));

	codec_setup_u32_buffer_t(&frame1Buffer, (uint32_t*)pData1, width, height, step1 / sizeof(uint32_t));
	codec_setup_u32_buffer_t(&frame2Buffer, (uint32_t*)pData2, width, height, step2 / sizeof(uint32_t));
	codec_setup_1d_u32_buffer_t(&diffXBuffer, diffX, width);
	codec_setup_1d_u32_buffer_t(&diffYBuffer, diffY, height);

	Compare32Stage1_old_buffer_t(width, height, &frame1Buffer, &frame2Buffer, &diffXBuffer, &diffYBuffer);

	rect->left = width - 1;
	rect->bottom = height - 1;

	for (i = 0; i < width; i++)
	{
		if (diffX[0] == 0) rect->left = min(rect->left, i);
		if (diffX[0] != 0) rect->right = max(rect->right, i);
	}

	for (i = 0; i < width; i++)
	{
		if (diffY[0] == 0) rect->top = min(rect->top, i);
		if (diffY[0] != 0) rect->bottom = max(rect->bottom, i);
	}

	free(diffX);
	free(diffY);

	return 0;
}

int NowCodec_Compare8_halide(uint8_t* pData1, int step1, uint8_t* pData2, int step2, int width, int height,
			     DUC_EDGE_RECT* rect)
{
	int i;

	buffer_t frame1Buffer;
	buffer_t frame2Buffer;
	buffer_t diffXBuffer;
	buffer_t diffYBuffer;

	uint8_t* diffX = calloc(width, 1);
	uint8_t* diffY = calloc(height, 1);

	codec_setup_u8_buffer_t(&frame1Buffer, pData1, width, height, step1);
	codec_setup_u8_buffer_t(&frame2Buffer, pData2, width, height, step2);
	codec_setup_1d_u8_buffer_t(&diffXBuffer, diffX, width);
	codec_setup_1d_u8_buffer_t(&diffYBuffer, diffY, height);

	Compare8Stage1_old_buffer_t(width, height, &frame1Buffer, &frame2Buffer, &diffXBuffer, &diffYBuffer);

	rect->left = width - 1;
	rect->bottom = height - 1;

	for (i = 0; i < width; i++)
	{
		if (diffX[0] == 0) rect->left = min(rect->left, i);
		if (diffX[0] != 0) rect->right = max(rect->right, i);
	}

	for (i = 0; i < width; i++)
	{
		if (diffY[0] == 0) rect->top = min(rect->top, i);
		if (diffY[0] != 0) rect->bottom = max(rect->bottom, i);
	}

	free(diffX);
	free(diffY);

	return 0;
}

#endif

int NowCodec_Copy_c(uint8_t* pDstData, int nDstStep, int nXDst, int nYDst, int nWidth, int nHeight, uint8_t* pSrcData,
		    int nSrcStep, int nXSrc, int nYSrc)
{
	int y;
	uint8_t* pSrcPixel;
	uint8_t* pDstPixel;

	if (nSrcStep < 0)
		nSrcStep = 4 * nWidth;

	if (nDstStep < 0)
		nDstStep = 4 * nWidth;

	pSrcPixel = &pSrcData[(nYSrc * nSrcStep) + (nXSrc * 4)];
	pDstPixel = &pDstData[(nYDst * nDstStep) + (nXDst * 4)];

	for (y = 0; y < nHeight; y++)
	{
		memcpy(pDstPixel, pSrcPixel, nWidth * 4);
		pSrcPixel = &pSrcPixel[nSrcStep];
		pDstPixel = &pDstPixel[nDstStep];
	}

	return 1;
}

int NowCodec_Copy(uint8_t* pDstData, int nDstStep, int nXDst, int nYDst, int nWidth, int nHeight, uint8_t* pSrcData,
		  int nSrcStep, int nXSrc, int nYSrc)
{
	NowPrimitives* primitives = NowPrimitives_Get();
	return primitives->Copy(pDstData, nDstStep, nXDst, nYDst, nWidth, nHeight, pSrcData, nSrcStep, nXSrc, nYSrc);
}

int NowCodec_Move_c(uint8_t* pData, int nStep, int nXDst, int nYDst, int nWidth, int nHeight, int nXSrc, int nYSrc)
{
	uint8_t* pSrcPixel;
	uint8_t* pDstPixel;

	if (nStep < 0)
		nStep = 4 * nWidth;

	pSrcPixel = &pData[((nYSrc + nHeight - 1) * nStep) + (nXSrc * 4)];
	pDstPixel = &pData[((nYDst + nHeight - 1) * nStep) + (nXDst * 4)];

	if (pSrcPixel > pDstPixel)
		return NowCodec_Copy_c(pData, nStep, nXDst, nYDst, nWidth, nHeight, pData,
			nStep, nXSrc, nYSrc);

	while (nHeight--)
	{
		memmove(pDstPixel, pSrcPixel, nWidth * 4);
		pSrcPixel -= nStep;
		pDstPixel -= nStep;
	}

	return 1;
}

int NowCodec_Move(uint8_t* pData, int nStep, int nXDst, int nYDst, int nWidth, int nHeight, int nXSrc, int nYSrc)
{
	NowPrimitives* primitives = NowPrimitives_Get();
	return primitives->Move(pData, nStep, nXDst, nYDst, nWidth, nHeight, nXSrc, nYSrc);
}

int NowCodec_CopyFromRetina_c(uint8_t* pDstData, int nDstStep, int nXDst, int nYDst, int nWidth, int nHeight,
			      uint8_t* pSrcData, int nSrcStep, int nXSrc, int nYSrc)
{
	int x, y;
	int nSrcPad;
	int nDstPad;
	uint32_t R, G, B;
	uint8_t* pSrcPixel;
	uint8_t* pDstPixel;

	if (nSrcStep < 0)
		nSrcStep = 8 * nWidth;

	if (nDstStep < 0)
		nDstStep = 4 * nWidth;

	nSrcPad = (nSrcStep - (nWidth * 8));
	nDstPad = (nDstStep - (nWidth * 4));

	pSrcPixel = &pSrcData[(nYSrc * nSrcStep) + (nXSrc * 8)];
	pDstPixel = &pDstData[(nYDst * nDstStep) + (nXDst * 4)];

	for (y = 0; y < nHeight; y++)
	{
		for (x = 0; x < nWidth; x++)
		{
			/* simple box filter scaling, could be improved with better algorithm */

			B = pSrcPixel[0] + pSrcPixel[4] + pSrcPixel[nSrcStep + 0] + pSrcPixel[nSrcStep + 4];
			G = pSrcPixel[1] + pSrcPixel[5] + pSrcPixel[nSrcStep + 1] + pSrcPixel[nSrcStep + 5];
			R = pSrcPixel[2] + pSrcPixel[6] + pSrcPixel[nSrcStep + 2] + pSrcPixel[nSrcStep + 6];
			pSrcPixel += 8;

			*pDstPixel++ = (uint8_t) (B >> 2);
			*pDstPixel++ = (uint8_t) (G >> 2);
			*pDstPixel++ = (uint8_t) (R >> 2);
			*pDstPixel++ = 0xFF;
		}

		pSrcPixel = &pSrcPixel[nSrcPad + nSrcStep];
		pDstPixel = &pDstPixel[nDstPad];
	}

	return 1;
}

int NowCodec_CopyFromRetina(uint8_t* pDstData, int nDstStep, int nXDst, int nYDst, int nWidth, int nHeight,
			    uint8_t* pSrcData, int nSrcStep, int nXSrc, int nYSrc)
{
	NowPrimitives* primitives = NowPrimitives_Get();
	return primitives->CopyFromRetina(pDstData, nDstStep, nXDst, nYDst, nWidth, nHeight, pSrcData, nSrcStep, nXSrc,
					  nYSrc);
}

int NowCodec_Compare32_c(uint8_t* pData1, int step1, uint8_t* pData2,
	int step2, int width, int height, DUC_EDGE_RECT* rect)
{
	bool equal;
	bool allEqual;
	int tw, th;
	int tx, ty, k;
	int nrow, ncol;
	int l, t, r, b;
	int left, top;
	int right, bottom;
	uint8_t *p1, *p2;
	bool rows[1024];

	allEqual = true;
	memset(rows, 0xFF, sizeof(rows));

	nrow = (height + 15) / 16;
	ncol = (width + 15) / 16;

	l = ncol + 1;
	r = -1;

	t = nrow + 1;
	b = -1;

	for (ty = 0; ty < nrow; ty++)
	{
		th = ((ty + 1) == nrow) ? (height % 16) : 16;

		if (!th)
			th = 16;

		for (tx = 0; tx < ncol; tx++)
		{
			equal = true;

			tw = ((tx + 1) == ncol) ? (width % 16) : 16;

			if (!tw)
				tw = 16;

			p1 = &pData1[(ty * 16 * step1) + (tx * 16 * 4)];
			p2 = &pData2[(ty * 16 * step2) + (tx * 16 * 4)];

			for (k = 0; k < th; k++)
			{
				if (memcmp(p1, p2, tw * 4) != 0)
				{
					equal = false;
					break;
				}

				p1 += step1;
				p2 += step2;
			}

			if (!equal)
			{
				rows[ty] = false;

				if (l > tx)
					l = tx;

				if (r < tx)
					r = tx;
			}
		}

		if (!rows[ty])
		{
			allEqual = false;

			if (t > ty)
				t = ty;

			if (b < ty)
				b = ty;
		}
	}

	if (allEqual)
	{
		rect->left = rect->top = 0;
		rect->right = rect->bottom = 0;
		return 0;
	}

	left = l * 16;
	top = t * 16;
	right = (r + 1) * 16;
	bottom = (b + 1) * 16;

	if (right > width)
		right = width;

	if (bottom > height)
		bottom = height;

	rect->left = left;
	rect->top = top;
	rect->right = right;
	rect->bottom = bottom;

	return 1;
}

int NowCodec_Compare32(uint8_t* pData1, int step1, uint8_t* pData2, int step2, int width, int height,
		       DUC_EDGE_RECT* rect)
{
	NowPrimitives* primitives = NowPrimitives_Get();
	return primitives->Compare32(pData1, step1, pData2, step2, width, height, rect);
}

int NowCodec_Compare8_c(uint8_t* pData1, int step1, uint8_t* pData2, int step2, int width, int height,
			DUC_EDGE_RECT* rect)
{
	int x, y;
	int width4 = (width & ~0x3);
	int l, r, t, b;
	uint8_t* p1 = pData1;
	uint8_t* p2 = pData2;

	l = width + 1;
	r = -1;
	t = height + 1;
	b = -1;

	if (width4 != width)
	{
		for (y = 0; y < height; y++)
		{
			for (x = 0; x < width4; x += 4)
			{
				if (*(uint32_t *)&p1[x] != *(uint32_t *)&p2[x])
				{
					if (x < l)
						l = x;

					if (x > r)
						r = x;

					if (y < t)
						t = y;

					if (y > b)
						b = y;
				}
			}
			for (x = width4; x < width; x++)
			{
				if (p1[x] != p2[x])
				{
					if (x < l)
						l = x;

					if (x > r)
						r = x;

					if (y < t)
						t = y;

					if (y > b)
						b = y;
				}
			}

			p1 += step1;
			p2 += step2;
		}
	}
	else
	{
		for (y = 0; y < height; y++)
		{
			for (x = 0; x < width; x += 4)
			{
				if (*(uint32_t *)&p1[x] != *(uint32_t *)&p2[x])
				{
					if (x < l)
						l = x;

					if (x > r)
						r = x;

					if (y < t)
						t = y;

					if (y > b)
						b = y;
				}
			}

			p1 += step1;
			p2 += step2;
		}
	}

	if ((r == -1) && (b == -1))
	{
		rect->left = 0;
		rect->top = 0;
		rect->right = 0;
		rect->bottom = 0;
		return 0;
	}

	r++;
	b++;

	l &= ~0x3;
	t &= ~0x3;
	r = (r + 3) & ~0x3;
	b = (b + 3) & ~0x3;

	rect->left = l;
	rect->top = t;
	rect->right = r;
	rect->bottom = b;

	return 1;
}

int NowCodec_Compare8(uint8_t* pData1, int step1, uint8_t* pData2, int step2, int width, int height,
		      DUC_EDGE_RECT* rect)
{
	NowPrimitives* primitives = NowPrimitives_Get();
	return primitives->Compare8(pData1, step1, pData2, step2, width, height, rect);
}

int NowCodec_Compare(uint8_t* pData1, int nStep1, int nWidth, int nHeight, uint8_t* pData2, int nStep2,
		     DUC_EDGE_RECT* rect)
{
	return NowCodec_Compare32(pData1, nStep1, pData2, nStep2, nWidth, nHeight, rect);
}
