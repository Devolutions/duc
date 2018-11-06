
#include <duc/time.h>

#include <duc/tape.h>

#ifdef WITH_LZ4
#include "lz4.h"
#endif

struct duc_tape
{
	FILE* fp;
	bool write;
	DUC_PIXEL_FORMAT pixelFormat;
};

bool DucTape_Read(DucTape* tape, void* ptr, size_t size)
{
	return (fread(ptr, size, 1, tape->fp) == 1);
}

bool DucTape_Write(DucTape* tape, void* ptr, size_t size)
{
	return (fwrite(ptr, size, 1, tape->fp) == 1);
}

bool DucTape_Seek(DucTape* tape, int offset, int origin)
{
	return (fseek(tape->fp, offset, origin) == 0);
}

void DucTape_SetPixelFormat(DucTape* tape, DUC_PIXEL_FORMAT* pixelFormat)
{
	duc_memcpy(&tape->pixelFormat, pixelFormat, DUC_PIXEL_FORMAT_SIZE);
}

void DucTape_GetPixelFormat(DucTape* tape, DUC_PIXEL_FORMAT* pixelFormat)
{
	duc_memcpy(pixelFormat, &tape->pixelFormat, DUC_PIXEL_FORMAT_SIZE);
}

bool DucTape_ReadBlock(DucTape* tape, DUC_BLOCK** ppBlock)
{
	bool compressed;
	DUC_BLOCK* block = NULL;
	DUC_BLOCK_HEADER header;

	if (!ppBlock)
		return false;

	if (!DucTape_Read(tape, &header, DUC_BLOCK_HEADER_SIZE))
		return false;

	if (header.blockSize < DUC_BLOCK_HEADER_SIZE)
		return false;

	compressed = header.blockFlags & DUC_BLOCK_FLAG_COMPRESSED;

	if (compressed)
	{
		int zstatus;
		int blockSize;
		uint8_t* data;
		uint8_t* zdata;
		int dataSize;
		int zdataSize;
		DUC_ZBLOCK* zblock;

		zblock = (DUC_ZBLOCK*) duc_malloc(header.blockSize);

		if (!zblock)
			return false;

		duc_memcpy(zblock, &header, DUC_BLOCK_HEADER_SIZE);

		if (!DucTape_Read(tape, &((uint8_t*) zblock)[DUC_BLOCK_HEADER_SIZE],
				header.blockSize - DUC_BLOCK_HEADER_SIZE))
		{
			return false;
		}

		blockSize = zblock->uncompressedSize + DUC_BLOCK_HEADER_SIZE;

		block = (DUC_BLOCK*) duc_malloc(blockSize);

		if (!block)
			return false;

		block->blockType = zblock->blockType;
		block->blockFlags = zblock->blockFlags & ~DUC_BLOCK_FLAG_COMPRESSED;
		block->blockSize = blockSize;

		data = &((uint8_t*) block)[DUC_BLOCK_HEADER_SIZE];
		zdata = &((uint8_t*) zblock)[DUC_ZBLOCK_HEADER_SIZE];

		dataSize = blockSize - DUC_BLOCK_HEADER_SIZE;
		zdataSize = zblock->blockSize - DUC_ZBLOCK_HEADER_SIZE;

		zstatus = LZ4_decompress_safe((const char*) zdata, (char*) data, zdataSize, dataSize);

		free(zblock);

		*ppBlock = block;
	}
	else
	{
		block = (DUC_BLOCK*) duc_malloc(header.blockSize);

		if (!block)
			return false;

		duc_memcpy(block, &header, DUC_BLOCK_HEADER_SIZE);

		if (!DucTape_Read(tape, &((uint8_t*) block)[DUC_BLOCK_HEADER_SIZE],
				header.blockSize - DUC_BLOCK_HEADER_SIZE))
		{
			return false;
		}

		*ppBlock = block;
	}

	return true;
}

bool DucTape_WriteBlock(DucTape* tape, DUC_BLOCK* block)
{
	bool compressed = true;

	if (compressed)
	{
		int zstatus;
		uint8_t* data;
		uint8_t* zdata;
		DUC_ZBLOCK* zblock;
		int blockSize = (int) block->blockSize;
		int dataSize = blockSize - DUC_BLOCK_HEADER_SIZE;

		zblock = (DUC_ZBLOCK*) duc_malloc(blockSize);

		if (!zblock)
			return false;

		duc_memcpy(zblock, block, DUC_BLOCK_HEADER_SIZE);

		data = &((uint8_t*) block)[DUC_BLOCK_HEADER_SIZE];
		zdata = &((uint8_t*) zblock)[DUC_ZBLOCK_HEADER_SIZE];

		zstatus = LZ4_compress_fast((const char*) data, (char*) zdata, dataSize, dataSize, 1);

		zblock->compressionType = 0;
		zblock->compressionFlags = 0;
		zblock->uncompressedSize = dataSize;

		zblock->blockFlags |= DUC_BLOCK_FLAG_COMPRESSED;
		zblock->blockSize = DUC_ZBLOCK_HEADER_SIZE + zstatus;

		if (!DucTape_Write(tape, zblock, zblock->blockSize))
			return false;

		free(zblock);
	}
	else
	{
		if (!DucTape_Write(tape, block, block->blockSize))
			return false;
	}

	return true;
}

bool DucTape_ReadHeaderBlock(DucTape* tape)
{
	struct {
		DUC_HEADER_BLOCK block;
	} st;
	DUC_HEADER_BLOCK* block = &st.block;

	if (!DucTape_Read(tape, block, DUC_HEADER_BLOCK_SIZE))
		return false;

	if (block->blockType != DUC_HEADER_BLOCK_TYPE)
		return false;

	if (block->blockSize != DUC_HEADER_BLOCK_SIZE)
		return false;

	if (block->version != DUC_VERSION_1_0)
		return false;

	if (block->endianness != DUC_HEADER_BLOCK_TYPE)
		return false;

	return true;
}

bool DucTape_WriteHeaderBlock(DucTape* tape)
{
	struct {
		DUC_HEADER_BLOCK block;
	} st;
	DUC_HEADER_BLOCK* block = &st.block;

	memset(&st, 0, sizeof(st));

	block->blockType = DUC_HEADER_BLOCK_TYPE;
	block->blockFlags = 0;
	block->blockSize = DUC_HEADER_BLOCK_SIZE;
	block->version = DUC_VERSION_1_0;
	block->endianness = DUC_HEADER_BLOCK_TYPE;
	block->flags = 0;

	if (!DucTape_Write(tape, block, block->blockSize))
		return false;

	return true;
}

bool DucTape_WriteFormatBlock(DucTape* tape)
{
	size_t blockSize;
	struct {
		DUC_FORMAT_BLOCK block;
	} st;
	DUC_FORMAT_BLOCK* srcBlock = &st.block;
	DUC_FORMAT_BLOCK* dstBlock = NULL;

	memset(&st, 0, sizeof(st));

	srcBlock->blockType = DUC_FORMAT_BLOCK_TYPE;
	srcBlock->blockFlags = 0;
	srcBlock->blockSize = DUC_FORMAT_BLOCK_SIZE;
	srcBlock->count = 1;
	srcBlock->formats.ptr = &tape->pixelFormat;

	blockSize = Duc_GetBlockSize((DUC_BLOCK*) srcBlock);

	dstBlock = (DUC_FORMAT_BLOCK*) duc_calloc(1, blockSize);

	if (!dstBlock)
		return false;

	dstBlock->blockType = DUC_FORMAT_BLOCK_TYPE;
	dstBlock->blockFlags = 0;
	dstBlock->blockSize = (uint32_t) blockSize;

	if (!Duc_WriteBlock((DUC_BLOCK*) srcBlock, (DUC_BLOCK*) dstBlock))
		return false;

	if (!DucTape_Write(tape, dstBlock, blockSize))
		return false;

	free(dstBlock);

	return true;
}

bool DucTape_CreateSurface(DucTape* tape, uint16_t surfaceId, DUC_RECT* surfaceRect)
{
	struct {
		DUC_SURFACE_BLOCK block;
	} st;
	DUC_SURFACE_BLOCK* block = &st.block;
	DUC_PIXEL_FORMAT* pixelFormat = &tape->pixelFormat;

	memset(&st, 0, sizeof(st));

	block->blockType = DUC_SURFACE_BLOCK_TYPE;
	block->blockFlags = 0;
	block->blockSize = DUC_SURFACE_BLOCK_SIZE;

	block->surfaceId = surfaceId;
	block->formatId = pixelFormat->id;
	block->surfaceRect.x = surfaceRect->x;
	block->surfaceRect.y = surfaceRect->y;
	block->surfaceRect.width = surfaceRect->width;
	block->surfaceRect.height = surfaceRect->height;
	block->surfaceFlags = DUC_SURFACE_BLOCK_FLAG_CREATE;

	if (!DucTape_Write(tape, block, block->blockSize))
		return false;

	return true;
}

bool DucTape_DeleteSurface(DucTape* tape, uint16_t surfaceId, DUC_RECT* surfaceRect)
{
	struct {
		DUC_SURFACE_BLOCK block;
	} st;
	DUC_SURFACE_BLOCK* block = &st.block;
	DUC_PIXEL_FORMAT* pixelFormat = &tape->pixelFormat;

	memset(&st, 0, sizeof(st));

	block->blockType = DUC_SURFACE_BLOCK_TYPE;
	block->blockFlags = 0;
	block->blockSize = DUC_SURFACE_BLOCK_SIZE;

	block->surfaceId = surfaceId;
	block->formatId = pixelFormat->id;
	block->surfaceRect.x = surfaceRect->x;
	block->surfaceRect.y = surfaceRect->y;
	block->surfaceRect.width = surfaceRect->width;
	block->surfaceRect.height = surfaceRect->height;
	block->surfaceFlags = DUC_SURFACE_BLOCK_FLAG_DELETE;
	block->surfaceTime = Duc_GetTime();

	if (!DucTape_Write(tape, block, block->blockSize))
		return false;

	return true;
}

bool DucTape_Open(DucTape* tape, const char* filename, bool write)
{
	tape->write = write;

	tape->fp = fopen(filename, tape->write ? "w+b" : "rb");

	if (!tape->fp)
		return false;

	return true;
}

bool DucTape_Close(DucTape* tape)
{
	if (tape->fp)
	{
		fclose(tape->fp);
		tape->fp = NULL;
	}

	return true;
}

DucTape* DucTape_New()
{
	DucTape* tape;

	tape = (DucTape*) calloc(1, sizeof(DucTape));

	if (!tape)
		return NULL;

	return tape;
}

void DucTape_Free(DucTape* tape)
{
	if (!tape)
		return;

	DucTape_Close(tape);

	free(tape);
}
