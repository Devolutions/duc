
#include <duc/core.h>

struct duc_stream
{
	size_t size;
	uint8_t* buffer;
	uint8_t* pointer;
};
typedef struct duc_stream DucStream;

static const char tab[] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";

size_t Duc_AlignSize(size_t size, size_t align)
{
	return size + ((align - (size % align)) % align);
}

DucStream* DucStream_Init(DucStream* s, void* buffer, size_t size)
{
	s->buffer = (uint8_t*) buffer;
	s->pointer = s->buffer;
	s->size = size;
	return s;
}

static bool DucStream_Safe(DucStream* s, size_t size)
{
	size_t safe = (s->size - (s->pointer - s->buffer));

	if (!s->size)
		return true;

	if (safe < size)
	{
		fprintf(stderr, "safe: %d size: %d\n", (int) safe, (int) size);
		return false;
	}

	return true;
}

static bool DucStream_Seek(DucStream* s, size_t size)
{
	s->pointer += size;
	return true;
}

static void* DucStream_WritePtr(DucStream* s, size_t size)
{
	uint8_t* pointer = s->pointer;

	if (!DucStream_Safe(s, size))
		return false;

	s->pointer += size;
	return pointer;
}

static void* DucStream_ReadPtr(DucStream* s, DUC_PTR* ptr)
{
	uint64_t base = (uint64_t) (size_t) s->buffer;
	uint64_t offset = (uint64_t) (size_t) (s->pointer - s->buffer);

	if (ptr->val == offset)
		return &((uint8_t*) base)[ptr->val];

	return ptr->ptr;
}

void* DucPtr_Get(DUC_PTR* ptr, void* base)
{
	return &((uint8_t*) base)[ptr->val];
}

bool DucPtr_Rebase(DUC_PTR* ptr, uint64_t srcBase, uint64_t dstBase)
{
	if (ptr->val < srcBase)
		return false;

	ptr->val -= srcBase;
	ptr->val += dstBase;

	return true;
}

bool DucPtr_Switch(DUC_PTR* ptr, uint64_t base, uint64_t size, bool offset)
{
	if (offset)
	{
		if (ptr->val >= base)
			ptr->val -= base;
		else
			return false;
	}
	else
	{
		if (ptr->val <= size)
			ptr->val += base;
		else
			return false;
	}

	return true;
}

/**
 * Pixel Format
 */

void Duc_PrintPixelFormat(DUC_PIXEL_FORMAT* t, int lvl)
{
	const char* fmt =
		"%.*s{\n"
		"%.*sname: %s\n"
		"%.*sid: 0x%04X\n"
		"%.*sflags: 0x%04X\n"
		"%.*sbitsPerPixel: %d\n"
		"%.*sbytesPerPixel: %d\n"
		"%.*splaneFlags: 0x%02X\n"
		"%.*splaneCount: %d\n"
		"%.*schannelFlags: 0x%02X\n"
		"%.*schannelDepth: %d\n"
		"%.*schannelUnit: %d\n"
		"%.*schannelCount: %d\n"
		"%.*ssubsampling: %d:%d:%d:%d\n"
		"%.*schannels[0]: name: %s flags: 0x%02X depth: %d unit: %d offset: %d\n"
		"%.*schannels[1]: name: %s flags: 0x%02X depth: %d unit: %d offset: %d\n"
		"%.*schannels[2]: name: %s flags: 0x%02X depth: %d unit: %d offset: %d\n"
		"%.*schannels[3]: name: %s flags: 0x%02X depth: %d unit: %d offset: %d\n"
		"%.*s}\n";

	printf(fmt,
		lvl, tab,
		lvl + 1, tab, t->name,
		lvl + 1, tab, t->id,
		lvl + 1, tab, t->flags,
		lvl + 1, tab, t->bitsPerPixel,
		lvl + 1, tab, t->bytesPerPixel,
		lvl + 1, tab, t->planeFlags,
		lvl + 1, tab, t->planeCount,
		lvl + 1, tab, t->channelFlags,
		lvl + 1, tab, t->channelDepth,
		lvl + 1, tab, t->channelUnit,
		lvl + 1, tab, t->channelCount,
		lvl + 1, tab, t->subsampling[0], t->subsampling[1], t->subsampling[2], t->subsampling[3],
		lvl + 1, tab, t->channels[0].name, t->channels[0].flags, t->channels[0].depth, t->channels[0].unit, t->channels[0].offset,
		lvl + 1, tab, t->channels[1].name, t->channels[1].flags, t->channels[1].depth, t->channels[1].unit, t->channels[1].offset,
		lvl + 1, tab, t->channels[2].name, t->channels[2].flags, t->channels[2].depth, t->channels[2].unit, t->channels[2].offset,
		lvl + 1, tab, t->channels[3].name, t->channels[3].flags, t->channels[3].depth, t->channels[3].unit, t->channels[3].offset,
		lvl, tab);
}

size_t Duc_PixelFormatSize(DUC_PIXEL_FORMAT* pixelFormat)
{
	size_t size = DUC_PIXEL_FORMAT_SIZE;
	return size;
}

/**
 * Pixel Buffer
 */

void Duc_PrintPixelBuffer(DUC_PIXEL_BUFFER* t, int lvl)
{
	const char* fmt =
		"%.*s{\n"
		"%.*srect: %d,%d %dx%d\n"
		"%.*sstep: %d, %d, %d, %d\n"
		"%.*svstep: %d, %d, %d, %d\n"
		"%.*sformatId: %d\n"
		"%.*sflags: 0x%04X\n"
		"%.*s}\n";

	printf(fmt,
		lvl, tab,
		lvl + 1, tab, t->rect.x, t->rect.y, t->rect.width, t->rect.height,
		lvl + 1, tab, t->step[0], t->step[1], t->step[2], t->step[3],
		lvl + 1, tab, t->vstep[0], t->vstep[1], t->vstep[2], t->vstep[3],
		lvl + 1, tab, t->formatId,
		lvl + 1, tab, t->flags,
		lvl, tab);
}

/**
 * Header Block
 */

void Duc_PrintHeaderBlock(DUC_HEADER_BLOCK* block, int lvl)
{
	const char* fmt =
		"%.*s{\n"
		"%.*sversion: %d.%d\n"
		"%.*sflags: 0x%08X\n"
		"%.*s}\n";

	printf(fmt,
		lvl, tab,
		lvl + 1, tab, (block->version >> 8), (block->version & 0xFF),
		lvl + 1, tab, block->flags,
		lvl, tab);
}

size_t Duc_GetHeaderBlockSize(DUC_HEADER_BLOCK* srcBlock)
{
	size_t size = DUC_HEADER_BLOCK_SIZE;
	return size;
}

size_t Duc_WriteHeaderBlock(DUC_HEADER_BLOCK* srcBlock, DUC_HEADER_BLOCK* dstBlock)
{
	duc_memcpy(dstBlock, srcBlock, DUC_HEADER_BLOCK_SIZE);
	dstBlock->blockFlags = 0;
	return DUC_HEADER_BLOCK_SIZE;
}

/**
 * Format Block
 */

void Duc_PrintFormatBlock(DUC_FORMAT_BLOCK* block, int lvl)
{
	size_t index, count;
	DucStream _src, *src;
	DUC_PIXEL_FORMAT* formats;

	const char* fmt =
		"%.*s{\n"
		"%.*scount: %d\n";

	src = DucStream_Init(&_src, block, block->blockSize);
	DucStream_Seek(src, DUC_FORMAT_BLOCK_SIZE);

	formats = DucStream_ReadPtr(src, (DUC_PTR*) &block->formats);

	printf(fmt,
		lvl, tab,
		lvl + 1, tab, block->count);

	count = (size_t) block->count;

	for (index = 0; index < count; index++)
	{
		Duc_PrintPixelFormat(&formats[index], lvl + 1);
	}

	printf("%.*s}\n", lvl, tab);
}

bool Duc_SwitchFormatBlock(DUC_FORMAT_BLOCK* block, bool offset)
{
	bool success;
	uint64_t base = (uint64_t) (size_t) block;
	uint64_t size = (uint64_t) block->blockSize;

	success = DucPtr_Switch((DUC_PTR*) &block->formats, base, size, offset);

	return success;
}

size_t Duc_GetFormatBlockSize(DUC_FORMAT_BLOCK* srcBlock)
{
	size_t size = DUC_FORMAT_BLOCK_SIZE;

	size += srcBlock->count * DUC_PIXEL_FORMAT_SIZE;

	return size;
}

size_t Duc_WriteFormatBlock(DUC_FORMAT_BLOCK* srcBlock, DUC_FORMAT_BLOCK* dstBlock)
{
	size_t count;
	DucStream _src, *src;
	DucStream _dst, *dst;
	DUC_PIXEL_FORMAT* srcFormats;
	DUC_PIXEL_FORMAT* dstFormats;

	if (!srcBlock || !dstBlock)
		return 0;

	src = DucStream_Init(&_src, srcBlock, srcBlock->blockSize);
	dst = DucStream_Init(&_dst, dstBlock, dstBlock->blockSize);

	duc_memcpy(dstBlock, srcBlock, DUC_FORMAT_BLOCK_SIZE);
	DucStream_Seek(src, DUC_FORMAT_BLOCK_SIZE);
	DucStream_Seek(dst, DUC_FORMAT_BLOCK_SIZE);

	dstBlock->blockType = DUC_FORMAT_BLOCK_TYPE;
	dstBlock->blockSize = (uint32_t) dst->size;

	count = srcBlock->count;

	srcFormats = (DUC_PIXEL_FORMAT*) DucStream_ReadPtr(src, (DUC_PTR*) &srcBlock->formats);
	dstFormats = dstBlock->formats.ptr = (DUC_PIXEL_FORMAT*) DucStream_WritePtr(dst, count * DUC_PIXEL_FORMAT_SIZE);

	duc_memcpy(dstFormats, srcFormats, count * DUC_PIXEL_FORMAT_SIZE);

	Duc_SwitchFormatBlock(dstBlock, true);

	return (size_t) dstBlock->blockSize;
}

/**
 * Surface Block
 */

void Duc_PrintSurfaceBlock(DUC_SURFACE_BLOCK* block, int lvl)
{
	const char* fmt =
		"%.*s{\n"
		"%.*ssurfaceId: %d\n"
		"%.*sformatId: %d\n"
		"%.*ssurfaceRect: (%d,%d) %dx%d\n"
		"%.*ssurfaceFlags: 0x%04X\n"
		"%.*s}\n";

	printf(fmt,
		lvl, tab,
		lvl + 1, tab, block->surfaceId,
		lvl + 1, tab, block->formatId,
		lvl + 1, tab, block->surfaceRect.x, block->surfaceRect.y,
			block->surfaceRect.width, block->surfaceRect.height,
		lvl + 1, tab, block->surfaceFlags,
		lvl, tab);
}

size_t Duc_GetSurfaceBlockSize(DUC_SURFACE_BLOCK* srcBlock)
{
	size_t size = DUC_SURFACE_BLOCK_SIZE;
	return size;
}

size_t Duc_WriteSurfaceBlock(DUC_SURFACE_BLOCK* srcBlock, DUC_SURFACE_BLOCK* dstBlock)
{
	duc_memcpy(dstBlock, srcBlock, DUC_SURFACE_BLOCK_SIZE);
	dstBlock->blockFlags = 0;
	return DUC_SURFACE_BLOCK_SIZE;
}

/**
 * Frame Block
 */

void Duc_PrintFrameBlock(DUC_FRAME_BLOCK* block, int lvl)
{
	size_t index, count;
	DucStream _src, *src;
	DUC_PIXEL_BUFFER* buffers;

	const char* fmt =
		"%.*s{\n"
		"%.*ssurfaceId: %d\n"
		"%.*sformatId: %d\n"
		"%.*sframeSize: %dx%d\n"
		"%.*sframeTime: %d\n"
		"%.*sframeId: %d\n"
		"%.*scount: %d\n";

	printf(fmt,
		lvl, tab,
		lvl + 1, tab, block->surfaceId,
		lvl + 1, tab, block->formatId,
		lvl + 1, tab, block->frameSize.width, block->frameSize.height,
		lvl + 1, tab, block->frameTime,
		lvl + 1, tab, block->frameId,
		lvl + 1, tab, block->count);

	count = (size_t) block->count;

	src = DucStream_Init(&_src, block, block->blockSize);
	DucStream_Seek(src, DUC_FRAME_BLOCK_SIZE);

	buffers = DucStream_ReadPtr(src, (DUC_PTR*) &block->buffers);

	for (index = 0; index < count; index++)
	{
		Duc_PrintPixelBuffer(&buffers[index], lvl + 1);
	}

	printf("%.*s}\n", lvl, tab);
}

size_t Duc_GetFrameBlockSize(DUC_FRAME_BLOCK* srcBlock)
{
	size_t size = 0;
	size_t index, count;
	DucStream _src, *src;
	DUC_PIXEL_BUFFER* buffer;
	DUC_PIXEL_BUFFER* buffers;

	count = (size_t) srcBlock->count;

	src = DucStream_Init(&_src, srcBlock, 0);
	DucStream_Seek(src, DUC_FRAME_BLOCK_SIZE);
	size += DUC_FRAME_BLOCK_SIZE;

	size += count * DUC_PIXEL_BUFFER_SIZE;

	buffers = DucStream_ReadPtr(src, (DUC_PTR*) &srcBlock->buffers);

	for (index = 0; index < count; index++)
	{
		buffer = &buffers[index];
		size += Duc_GetPlaneSizes(buffer->step, buffer->vstep, NULL);
	}

	return size;
}

bool Duc_SwitchFrameBlock(DUC_FRAME_BLOCK* block, bool offset)
{
	bool success;
	size_t index, count;
	DucStream _src, *src;
	DUC_PIXEL_BUFFER* buffer;
	DUC_PIXEL_BUFFER* buffers;
	uint64_t base = (uint64_t) (size_t) block;
	uint64_t size = (uint64_t) block->blockSize;

	count = (size_t) block->count;

	src = DucStream_Init(&_src, block, 0);
	DucStream_Seek(src, DUC_FRAME_BLOCK_SIZE);
	size += DUC_FRAME_BLOCK_SIZE;

	buffers = DucStream_ReadPtr(src, (DUC_PTR*) &block->buffers);
	DucStream_Seek(src, count * DUC_PIXEL_BUFFER_SIZE);
	success = DucPtr_Switch((DUC_PTR*) &block->buffers, base, size, offset);

	for (index = 0; index < count; index++)
	{
		buffer = &buffers[index];
		size += Duc_GetPlaneSizes(buffer->step, buffer->vstep, NULL);
		DucPtr_Switch((DUC_PTR*) &buffer->data[0], base, size, offset);
		DucPtr_Switch((DUC_PTR*) &buffer->data[1], base, size, offset);
		DucPtr_Switch((DUC_PTR*) &buffer->data[2], base, size, offset);
		success = DucPtr_Switch((DUC_PTR*) &buffer->data[3], base, size, offset);
	}

	return success;
}

size_t Duc_WriteFrameBlock(DUC_FRAME_BLOCK* srcBlock, DUC_FRAME_BLOCK* dstBlock)
{
	size_t index, count;
	DucStream _src, *src;
	DucStream _dst, *dst;
	uint8_t* srcData[4];
	uint8_t* dstData[4];
	size_t srcTotalSize;
	size_t srcPlaneSize[4];
	size_t dstTotalSize;
	size_t dstPlaneSize[4];
	DUC_PIXEL_BUFFER* srcBuffer;
	DUC_PIXEL_BUFFER* dstBuffer;
	DUC_PIXEL_BUFFER* srcBuffers;
	DUC_PIXEL_BUFFER* dstBuffers;

	src = DucStream_Init(&_src, srcBlock, srcBlock->blockSize);
	dst = DucStream_Init(&_dst, dstBlock, dstBlock->blockSize);

	duc_memcpy(dstBlock, srcBlock, DUC_FRAME_BLOCK_SIZE);
	DucStream_Seek(src, DUC_FRAME_BLOCK_SIZE);
	DucStream_Seek(dst, DUC_FRAME_BLOCK_SIZE);

	dstBlock->blockType = DUC_FRAME_BLOCK_TYPE;
	dstBlock->blockSize = (uint32_t) dst->size;

	count = (size_t) srcBlock->count;

	srcBuffers = DucStream_ReadPtr(src, (DUC_PTR*) &srcBlock->buffers);
	DucStream_Seek(src, count * DUC_PIXEL_BUFFER_SIZE);
	dstBuffers = dstBlock->buffers.ptr = (DUC_PIXEL_BUFFER*) DucStream_WritePtr(dst, count * DUC_PIXEL_BUFFER_SIZE);

	duc_memcpy(dstBuffers, srcBuffers, count * DUC_PIXEL_BUFFER_SIZE);

	for (index = 0; index < count; index++)
	{
		srcBuffer = &srcBuffers[index];
		dstBuffer = &dstBuffers[index];

		srcTotalSize = Duc_GetPlaneSizes(srcBuffer->step, srcBuffer->vstep, srcPlaneSize);
		dstTotalSize = Duc_GetPlaneSizes(dstBuffer->step, dstBuffer->vstep, dstPlaneSize);

		srcData[0] = DucStream_ReadPtr(src, (DUC_PTR*) &srcBuffer->data[0]);
		srcData[1] = DucStream_ReadPtr(src, (DUC_PTR*) &srcBuffer->data[1]);
		srcData[2] = DucStream_ReadPtr(src, (DUC_PTR*) &srcBuffer->data[2]);
		srcData[3] = DucStream_ReadPtr(src, (DUC_PTR*) &srcBuffer->data[3]);

		dstData[0] = dstBuffer->data[0].ptr = (uint8_t*) DucStream_WritePtr(dst, srcPlaneSize[0]);
		dstData[1] = dstBuffer->data[1].ptr = (uint8_t*) DucStream_WritePtr(dst, srcPlaneSize[1]);
		dstData[2] = dstBuffer->data[2].ptr = (uint8_t*) DucStream_WritePtr(dst, srcPlaneSize[2]);
		dstData[3] = dstBuffer->data[3].ptr = (uint8_t*) DucStream_WritePtr(dst, srcPlaneSize[3]);

		if (srcData[0])
			duc_memcpy(dstData[0], srcData[0], dstPlaneSize[0]);
		if (srcData[1])
			duc_memcpy(dstData[1], srcData[1], dstPlaneSize[1]);
		if (srcData[2])
			duc_memcpy(dstData[2], srcData[2], dstPlaneSize[2]);
		if (srcData[3])
			duc_memcpy(dstData[3], srcData[3], dstPlaneSize[3]);
	}

	Duc_SwitchFrameBlock(dstBlock, true);

	return (size_t) dstBlock->blockSize;
}

bool Duc_SwitchBlock(DUC_BLOCK* block, bool offset)
{
	bool success = false;

	switch (block->blockType)
	{
		case DUC_HEADER_BLOCK_TYPE:
			success = true; /* nothing to do */
			break;

		case DUC_FORMAT_BLOCK_TYPE:
			success = Duc_SwitchFormatBlock((DUC_FORMAT_BLOCK*) block, offset);
			break;

		case DUC_SURFACE_BLOCK_TYPE:
			success = true; /* nothing to do */
			break;

		case DUC_FRAME_BLOCK_TYPE:
			success = Duc_SwitchFrameBlock((DUC_FRAME_BLOCK*) block, offset);
			break;
	}

	return success;
}

size_t Duc_GetBlockSize(DUC_BLOCK* block)
{
	size_t size = 0;

	switch (block->blockType)
	{
		case DUC_HEADER_BLOCK_TYPE:
			size = Duc_GetHeaderBlockSize((DUC_HEADER_BLOCK*) block);
			break;

		case DUC_FORMAT_BLOCK_TYPE:
			size = Duc_GetFormatBlockSize((DUC_FORMAT_BLOCK*) block);
			break;

		case DUC_SURFACE_BLOCK_TYPE:
			size = Duc_GetSurfaceBlockSize((DUC_SURFACE_BLOCK*) block);
			break;

		case DUC_FRAME_BLOCK_TYPE:
			size = Duc_GetFrameBlockSize((DUC_FRAME_BLOCK*) block);
			break;
	}

	return size;
}

size_t Duc_WriteBlock(DUC_BLOCK* srcBlock, DUC_BLOCK* dstBlock)
{
	size_t status = 0;

	switch (srcBlock->blockType)
	{
		case DUC_HEADER_BLOCK_TYPE:
			status = Duc_WriteHeaderBlock((DUC_HEADER_BLOCK*) srcBlock, (DUC_HEADER_BLOCK*) dstBlock);
			break;

		case DUC_FORMAT_BLOCK_TYPE:
			status = Duc_WriteFormatBlock((DUC_FORMAT_BLOCK*) srcBlock, (DUC_FORMAT_BLOCK*) dstBlock);
			break;

		case DUC_SURFACE_BLOCK_TYPE:
			status = Duc_WriteSurfaceBlock((DUC_SURFACE_BLOCK*) srcBlock, (DUC_SURFACE_BLOCK*) dstBlock);
			break;

		case DUC_FRAME_BLOCK_TYPE:
			status = Duc_WriteFrameBlock((DUC_FRAME_BLOCK*) srcBlock, (DUC_FRAME_BLOCK*) dstBlock);
			break;
	}

	return status;
}
