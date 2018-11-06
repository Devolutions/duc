#ifndef DUC_CORE_H
#define DUC_CORE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <limits.h>

#include <string.h>
#include <stdlib.h>

#include <stdio.h>
#include <errno.h>

#ifdef __GNUC__
#define duc_align(_n)	__attribute__((aligned(_n)))
#else
#define duc_align(_n)	__declspec(align(_n))
#endif

#define duc_is_aligned(_pointer, _alignment) \
	!(((size_t) _pointer) % _alignment)

#pragma pack(push, 1)

enum duc_layout_type
{
	DucLayoutSerial = 0,
	DucLayoutOffset = 1,
	DucLayoutPacked = 2,
	DucLayoutSparse = 3
};
typedef enum duc_layout_type DucLayoutType;

union _DUC_PTR
{
	void* ptr;
	uint64_t val;
};
typedef duc_align(8) union _DUC_PTR DUC_PTR;

#define DUC_PTR_DEF(_type, _name) \
union _DUC_ ## _name ## _PTR { \
	_type * ptr; \
	uint64_t val; \
}; \
typedef duc_align(8) union _DUC_ ## _name ## _PTR DUC_ ## _name ## _PTR

DUC_PTR_DEF(uint8_t, UINT8);

struct _DUC_POINT		/* [size(4)] */
{
	int16_t x;		/* [size(2), offset(0)] */
	int16_t y;		/* [size(2), offset(2)] */
};
typedef duc_align(4) struct _DUC_POINT DUC_POINT;

struct _DUC_SIZE		/* [size(4)] */
{
	uint16_t width;		/* [size(2), offset(0)] */
	uint16_t height;	/* [size(2), offset(2)] */
};
typedef duc_align(4) struct _DUC_SIZE DUC_SIZE;

struct _DUC_RECT		/* [size(8)] */
{
	int16_t x;		/* [size(2), offset(0)] */
	int16_t y;		/* [size(2), offset(2)] */
	uint16_t width;		/* [size(2), offset(4)] */
	uint16_t height;	/* [size(2), offset(6)] */
};
typedef duc_align(8) struct _DUC_RECT DUC_RECT;

#define DUC_PIXEL_CHANNEL_SIZE	8

struct _DUC_PIXEL_CHANNEL	/* [size(8)] */
{
	uint8_t name[4];	/* [size(4), offset(0)] */
	uint8_t flags;		/* [size(1), offset(4)] */
	uint8_t depth;		/* [size(1), offset(5)] */
	uint8_t unit;		/* [size(1), offset(6)] */
	uint8_t offset;		/* [size(1), offset(7)] */
};
typedef duc_align(8) struct _DUC_PIXEL_CHANNEL DUC_PIXEL_CHANNEL;

/* pixel format ids */

#define DUC_PIXEL_FORMAT_ID_NONE			0x0000
#define DUC_PIXEL_FORMAT_ID_ARGB32			0x0001
#define DUC_PIXEL_FORMAT_ID_XRGB32			0x0002
#define DUC_PIXEL_FORMAT_ID_ABGR32			0x0003
#define DUC_PIXEL_FORMAT_ID_XBGR32			0x0004
#define DUC_PIXEL_FORMAT_ID_BGRA32			0x0005
#define DUC_PIXEL_FORMAT_ID_BGRX32			0x0006
#define DUC_PIXEL_FORMAT_ID_RGBA32			0x0007
#define DUC_PIXEL_FORMAT_ID_RGBX32			0x0008
#define DUC_PIXEL_FORMAT_ID_RGB24			0x0009
#define DUC_PIXEL_FORMAT_ID_BGR24			0x000A
#define DUC_PIXEL_FORMAT_ID_RGB565			0x000B
#define DUC_PIXEL_FORMAT_ID_BGR565			0x000C
#define DUC_PIXEL_FORMAT_ID_RGB555			0x000D
#define DUC_PIXEL_FORMAT_ID_BGR555			0x000E
#define DUC_PIXEL_FORMAT_ID_ARGB555			0x000F
#define DUC_PIXEL_FORMAT_ID_BGRA555			0x0010
#define DUC_PIXEL_FORMAT_ID_RGB				0x0020

#define DUC_PIXEL_FORMAT_ID_DYNAMIC			0x8000
#define DUC_PIXEL_FORMAT_ID_EXTENDED			0xC000

/* pixel format flags */

#define DUC_PIXEL_FORMAT_FLAG_PLANAR			0x0001
#define DUC_PIXEL_FORMAT_FLAG_INDEXED			0x0002
#define DUC_PIXEL_FORMAT_FLAG_PACKED			0x0004
#define DUC_PIXEL_FORMAT_FLAG_GRAYSCALE			0x0008
#define DUC_PIXEL_FORMAT_FLAG_RGB			0x0010
#define DUC_PIXEL_FORMAT_FLAG_ALPHA			0x0020
#define DUC_PIXEL_FORMAT_FLAG_OPAQUE			0x0040
#define DUC_PIXEL_FORMAT_FLAG_LUMA_CHROMA		0x0080
#define DUC_PIXEL_FORMAT_FLAG_SUBSAMPLING		0x0100

#define DUC_PIXEL_FORMAT_ALIGNMENT	8
#define DUC_PIXEL_FORMAT_SIZE		64

struct _DUC_PIXEL_FORMAT		/* [size(64)] */
{
	uint8_t name[16];		/* [size(16), offset(0)] */
	uint16_t id;			/* [size(2), offset(16)] */
	uint16_t flags;			/* [size(2), offset(18)] */
	uint8_t bitsPerPixel;		/* [size(1), offset(20)] */
	uint8_t bytesPerPixel;		/* [size(1), offset(21)] */
	uint8_t planeFlags;		/* [size(1), offset(22)] */
	uint8_t planeCount;		/* [size(1), offset(23)] */
	uint8_t channelFlags;		/* [size(1), offset(24)] */
	uint8_t channelDepth;		/* [size(1), offset(25)] */
	uint8_t channelUnit;		/* [size(1), offset(26)] */
	uint8_t channelCount;		/* [size(1), offset(27)] */
	uint8_t subsampling[4];		/* [size(4), offset(28)] */
	DUC_PIXEL_CHANNEL channels[4];	/* [size(32), offset(32)] */
};
typedef duc_align(8) struct _DUC_PIXEL_FORMAT DUC_PIXEL_FORMAT;

DUC_PTR_DEF(DUC_PIXEL_FORMAT, PIXEL_FORMAT);

/* pixel buffer flags */

#define DUC_PIXEL_BUFFER_FLAG_REGION			0x0001
#define DUC_PIXEL_BUFFER_FLAG_OFFSET			0x0002
#define DUC_PIXEL_BUFFER_FLAG_BOTTOM_UP			0x0010

#define DUC_PIXEL_BUFFER_ALIGNMENT	16
#define DUC_PIXEL_BUFFER_SIZE		64

struct _DUC_PIXEL_BUFFER		/* [size(64)] */
{
	DUC_RECT rect;			/* [size(8), offset(0)] */
	uint16_t step[4];		/* [size(8), offset(8)] */
	uint16_t vstep[4];		/* [size(8), offset(16)] */
	uint16_t formatId;		/* [size(2), offset(24)] */
	uint16_t flags;			/* [size(2), offset(26)] */
	uint32_t reserved;		/* [size(4), offset(28)] */
	DUC_UINT8_PTR data[4];		/* [size(32), offset(32)] */
};
typedef duc_align(16) struct _DUC_PIXEL_BUFFER DUC_PIXEL_BUFFER;

DUC_PTR_DEF(DUC_PIXEL_BUFFER, PIXEL_BUFFER);

/* block types */

#define DUC_HEADER_BLOCK_TYPE				0xDC00
#define DUC_FORMAT_BLOCK_TYPE				0xDC01
#define DUC_SURFACE_BLOCK_TYPE				0xDC02
#define DUC_FRAME_BLOCK_TYPE				0xDC03

/* block flags */

#define DUC_BLOCK_FLAG_COMPRESSED			0x8000

#define DUC_BLOCK_HEADER_ALIGNMENT	16
#define DUC_BLOCK_HEADER_SIZE		8

struct _DUC_BLOCK_HEADER		/* [size(8)] */
{
	uint16_t blockType;		/* [size(2), offset(0)] */
	uint16_t blockFlags;		/* [size(2), offset(2)] */
	uint32_t blockSize;		/* [size(4), offset(4)] */
};
typedef duc_align(16) struct _DUC_BLOCK_HEADER DUC_BLOCK_HEADER;
typedef duc_align(16) struct _DUC_BLOCK_HEADER DUC_BLOCK;

#define DUC_ZBLOCK_HEADER_ALIGNMENT	16
#define DUC_ZBLOCK_HEADER_SIZE		16

struct _DUC_ZBLOCK_HEADER		/* [size(8)] */
{
	uint16_t blockType;		/* [size(2), offset(0)] */
	uint16_t blockFlags;		/* [size(2), offset(2)] */
	uint32_t blockSize;		/* [size(4), offset(4)] */

	uint16_t compressionType;	/* [size(2), offset(8)] */
	uint16_t compressionFlags;	/* [size(2), offset(10)] */
	uint32_t uncompressedSize;	/* [size(4), offset(12)] */
};
typedef duc_align(16) struct _DUC_ZBLOCK_HEADER DUC_ZBLOCK_HEADER;
typedef duc_align(16) struct _DUC_ZBLOCK_HEADER DUC_ZBLOCK;

/**
 * Header Block
 */

#define DUC_VERSION_1_0					0x0100

#define DUC_HEADER_BLOCK_ALIGNMENT	16
#define DUC_HEADER_BLOCK_SIZE		32

struct _DUC_HEADER_BLOCK		/* [size(32)] */
{
	uint16_t blockType;		/* [size(2), offset(0)] */
	uint16_t blockFlags;		/* [size(2), offset(2)] */
	uint32_t blockSize;		/* [size(4), offset(4)] */

	uint16_t version;		/* [size(2), offset(8)] */
	uint16_t endianness;		/* [size(2), offset(10)] */
	uint32_t flags;			/* [size(4), offset(12)] */
	uint32_t reserved1;		/* [size(4), offset(16)] */
	uint32_t reserved2;		/* [size(4), offset(20)] */
	uint32_t reserved3;		/* [size(4), offset(24)] */
	uint32_t reserved4;		/* [size(4), offset(28)] */
};
typedef duc_align(16) struct _DUC_HEADER_BLOCK DUC_HEADER_BLOCK;

/**
 * Format Block
 */

#define DUC_FORMAT_BLOCK_ALIGNMENT	16
#define DUC_FORMAT_BLOCK_SIZE		24

struct _DUC_FORMAT_BLOCK		/* [size(24)] */
{
	uint16_t blockType;		/* [size(2), offset(0)] */
	uint16_t blockFlags;		/* [size(2), offset(2)] */
	uint32_t blockSize;		/* [size(4), offset(4)] */

	uint32_t reserved1;		/* [size(4), offset(8)] */
	uint16_t reserved2;		/* [size(2), offset(12)] */
	uint16_t count;			/* [size(2), offset(14)] */
	DUC_PIXEL_FORMAT_PTR formats;	/* [size(8), offset(16)] */
};
typedef duc_align(16) struct _DUC_FORMAT_BLOCK DUC_FORMAT_BLOCK;

/**
 * Surface Block
 */

/* surface block flags */

#define DUC_SURFACE_BLOCK_FLAG_CREATE			0x0001
#define DUC_SURFACE_BLOCK_FLAG_DELETE			0x0002

#define DUC_SURFACE_BLOCK_ALIGNMENT	16
#define DUC_SURFACE_BLOCK_SIZE		32

struct _DUC_SURFACE_BLOCK		/* [size(32)] */
{
	uint16_t blockType;		/* [size(2), offset(0)] */
	uint16_t blockFlags;		/* [size(2), offset(2)] */
	uint32_t blockSize;		/* [size(4), offset(4)] */

	uint16_t surfaceId;		/* [size(2), offset(8)] */
	uint16_t formatId;		/* [size(2), offset(10)] */
	uint16_t surfaceFlags;		/* [size(2), offset(12)] */
	uint16_t reserved1;		/* [size(2), offset(14)] */
	DUC_RECT surfaceRect;		/* [size(8), offset(16)] */
	uint32_t surfaceTime;		/* [size(4), offset(24)] */
	uint32_t reserved3;		/* [size(4), offset(28)] */
};
typedef duc_align(16) struct _DUC_SURFACE_BLOCK DUC_SURFACE_BLOCK;

/**
 * Frame Block
 */

#define DUC_FRAME_BLOCK_ALIGNMENT	16
#define DUC_FRAME_BLOCK_SIZE		32

struct _DUC_FRAME_BLOCK			/* [size(32)] */
{
	uint16_t blockType;		/* [size(2), offset(0)] */
	uint16_t blockFlags;		/* [size(2), offset(2)] */
	uint32_t blockSize;		/* [size(4), offset(4)] */

	uint16_t surfaceId;		/* [size(2), offset(8)] */
	uint16_t formatId;		/* [size(2), offset(10)] */
	DUC_SIZE frameSize;		/* [size(4), offset(12)] */
	uint32_t frameTime;		/* [size(4), offset(16)] */
	uint16_t frameId;		/* [size(2), offset(20)] */
	uint8_t reserved;		/* [size(1), offset(22)] */
	uint8_t count;			/* [size(1), offset(23)] */
	DUC_PIXEL_BUFFER_PTR buffers;	/* [size(8), offset(24)] */
};
typedef duc_align(16) struct _DUC_FRAME_BLOCK DUC_FRAME_BLOCK;

#define duc_malloc(_size)		malloc(_size)
#define duc_calloc(_num, _size)		calloc(_num, _size)
#define duc_realloc(_ptr, _size)	realloc(_ptr, _size)
#define duc_free(_ptr)			free(_ptr)

#define duc_memcpy(_dst, _src, _size)	memcpy(_dst, _src, _size)
#define duc_memset(_ptr, _val, _size)	memset(_ptr, _val, _size)

#define duc_min(_a, _b)			((_a < _b) ? _a : _b)
#define duc_max(_a, _b)			((_a > _b) ? _a : _b)

typedef struct duc_tape DucTape;

#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

size_t Duc_GetRegionOffset(int16_t x, int16_t y, uint16_t step, uint8_t bytesPerPixel);
uint8_t* Duc_GetRegionPointer(uint8_t* pImage, int16_t x, int16_t y, uint16_t step, uint8_t bytesPerPixel);
uint8_t* Duc_GetImagePointer(uint8_t* pRegion, int16_t x, int16_t y, uint16_t step, uint8_t bytesPerPixel);

size_t Duc_GetPlaneSize(uint16_t step, uint16_t vstep);
size_t Duc_GetPlaneSizes(uint16_t step[4], uint16_t vstep[4], size_t planeSize[4]);

int Duc_Copy_8u_C1R(uint8_t* pSrc, int srcStep, uint8_t* pDst, int dstStep, int width, int height);
int Duc_Copy_8u_C2R(uint8_t* pSrc, int srcStep, uint8_t* pDst, int dstStep, int width, int height);
int Duc_Copy_8u_C3R(uint8_t* pSrc, int srcStep, uint8_t* pDst, int dstStep, int width, int height);
int Duc_Copy_8u_C4R(uint8_t* pSrc, int srcStep, uint8_t* pDst, int dstStep, int width, int height);

int Duc_XRGB32ToRGB(uint8_t* pSrc, int srcStep, uint8_t* pDst[3], int dstStep[3], int width, int height);
int Duc_RGBToXRGB32(uint8_t* pSrc[3], int srcStep[3], uint8_t* pDst, int dstStep, int width, int height);

void* DucPtr_Get(DUC_PTR* ptr, void* base);
size_t Duc_AlignSize(size_t size, size_t align);

bool Duc_SwitchBlock(DUC_BLOCK* block, bool offset);

size_t Duc_GetBlockSize(DUC_BLOCK* block);
size_t Duc_WriteBlock(DUC_BLOCK* srcBlock, DUC_BLOCK* dstBlock);

uint16_t Duc_GetPixelFormatId(const char* name);
bool Duc_GetPixelFormat(uint16_t id, DUC_PIXEL_FORMAT* format);

void Duc_PrintHeaderBlock(DUC_HEADER_BLOCK* block, int lvl);
void Duc_PrintFormatBlock(DUC_FORMAT_BLOCK* block, int lvl);
void Duc_PrintFrameBlock(DUC_FRAME_BLOCK* block, int lvl);
void Duc_PrintSurfaceBlock(DUC_SURFACE_BLOCK* block, int lvl);
void Duc_PrintPixelFormat(DUC_PIXEL_FORMAT* t, int lvl);

#ifdef __cplusplus
}
#endif

#endif /* DUC_CORE_H */

