
#include <duc/core.h>

#define MUNIT_ENABLE_ASSERT_ALIASES
#include <munit.h>

int test_dump_to_file(const char* filename, uint8_t* data, size_t size)
{
	FILE* fp;

	fp = fopen(filename, "w+");

	if (!fp)
		return -1;

	fwrite(data, size, 1, fp);
	fclose(fp);

	return 1;
}

int test_fill_image_inc(uint8_t* pSrc, int srcStep, int width, int height)
{
	int x, y, val;
	uint8_t* src = pSrc;

	for (y = 0; y < height; y++)
	{
		val = y % 0xFF;

		for (x = 0; x < width * 4; x++)
		{
			src[x] = (uint8_t) val;
			val = (val + 1) % 0xFF;
		}

		src += srcStep;
	}

	return 1;
}

MunitResult test_duc_header_block(const MunitParameter params[], void* user_data)
{
	size_t size;
	struct {
		DUC_HEADER_BLOCK srcBlock;
		DUC_HEADER_BLOCK xfrBlock;
		DUC_HEADER_BLOCK dstBlock;
	} st;
	DUC_HEADER_BLOCK* srcBlock = &st.srcBlock;
	DUC_HEADER_BLOCK* xfrBlock = &st.xfrBlock;
	DUC_HEADER_BLOCK* dstBlock = &st.dstBlock;

	memset(&st, 0, sizeof(st));

	srcBlock->blockType = DUC_HEADER_BLOCK_TYPE;
	srcBlock->blockFlags = 0;

	srcBlock->version = DUC_VERSION_1_0;

	size = Duc_GetBlockSize((DUC_BLOCK*) srcBlock);

	fprintf(stderr, "header block sparse -> serial size: %d\n", (int) size);

	if (size != 32)
	{
		fprintf(stderr, "unexpected header block size: Actual: %d, Expected: %d\n",
				(int) size, 32);
		return MUNIT_FAIL;
	}

	xfrBlock->blockType = DUC_HEADER_BLOCK_TYPE;
	xfrBlock->blockFlags = 0;
	xfrBlock->blockSize = (uint32_t) size;

	Duc_WriteBlock((DUC_BLOCK*) srcBlock, (DUC_BLOCK*) xfrBlock);

	size = Duc_GetBlockSize((DUC_BLOCK*) xfrBlock);

	fprintf(stderr, "header block serial -> packed size: %d\n", (int) size);

	if (size != 32)
	{
		fprintf(stderr, "unexpected header block size: Actual: %d, Expected: %d\n",
				(int) size, 32);
		return MUNIT_FAIL;
	}

	dstBlock->blockType = DUC_HEADER_BLOCK_TYPE;
	dstBlock->blockFlags = 0;
	dstBlock->blockSize = (uint32_t) size;

	Duc_WriteBlock((DUC_BLOCK*) xfrBlock, (DUC_BLOCK*) dstBlock);

	Duc_PrintHeaderBlock(dstBlock, 0);

	return MUNIT_OK;
}

MunitResult test_duc_format_block(const MunitParameter params[], void* user_data)
{
	size_t size;
	struct {
		DUC_FORMAT_BLOCK srcBlock;
		DUC_PIXEL_FORMAT formats[1];
	} st;
	DUC_FORMAT_BLOCK* srcBlock = &st.srcBlock;
	DUC_FORMAT_BLOCK* xfrBlock;
	DUC_FORMAT_BLOCK* dstBlock;
	uint16_t formatId = DUC_PIXEL_FORMAT_ID_BGRX32;

	memset(&st, 0, sizeof(st));

	srcBlock->blockType = DUC_FORMAT_BLOCK_TYPE;
	srcBlock->blockFlags = 0;
	srcBlock->blockSize = DUC_FORMAT_BLOCK_SIZE;

	srcBlock->count = 1;
	srcBlock->formats.ptr = st.formats;
	Duc_GetPixelFormat(formatId, &srcBlock->formats.ptr[0]);

	size = Duc_GetBlockSize((DUC_BLOCK*) srcBlock);

	fprintf(stderr, "format block sparse -> serial size: %d\n", (int) size);

	if (size != 88)
	{
		fprintf(stderr, "unexpected format block size: Actual: %d, Expected: %d\n",
				(int) size, 88);
		return MUNIT_FAIL;
	}

	xfrBlock = (DUC_FORMAT_BLOCK*) malloc(size);

	if (!xfrBlock)
		return MUNIT_FAIL;

	xfrBlock->blockType = DUC_FORMAT_BLOCK_TYPE;
	xfrBlock->blockFlags = 0;
	xfrBlock->blockSize = (uint32_t) size;

	Duc_WriteBlock((DUC_BLOCK*) srcBlock, (DUC_BLOCK*) xfrBlock);

	size = Duc_GetBlockSize((DUC_BLOCK*) xfrBlock);

	fprintf(stderr, "format block serial -> packed size: %d\n", (int) size);

	dstBlock = (DUC_FORMAT_BLOCK*) malloc(size);

	if (!dstBlock)
		return MUNIT_FAIL;

	dstBlock->blockType = DUC_FORMAT_BLOCK_TYPE;
	dstBlock->blockFlags = 0;
	dstBlock->blockSize = (uint32_t) size;

	Duc_WriteBlock((DUC_BLOCK*) xfrBlock, (DUC_BLOCK*) dstBlock);

	Duc_PrintFormatBlock(dstBlock, 0);

	if (0)
	{
		test_dump_to_file("srcFormat.block", (uint8_t*) srcBlock, srcBlock->blockSize);
		test_dump_to_file("xfrFormat.block", (uint8_t*) xfrBlock, xfrBlock->blockSize);
		test_dump_to_file("dstFormat.block", (uint8_t*) dstBlock, dstBlock->blockSize);
	}

	free(xfrBlock);
	free(dstBlock);

	return MUNIT_OK;
}

MunitResult test_duc_surface_block(const MunitParameter params[], void* user_data)
{
	size_t size;
	struct {
		DUC_SURFACE_BLOCK srcBlock;
		DUC_SURFACE_BLOCK xfrBlock;
		DUC_SURFACE_BLOCK dstBlock;
	} st;
	DUC_SURFACE_BLOCK* srcBlock = &st.srcBlock;
	DUC_SURFACE_BLOCK* xfrBlock = &st.xfrBlock;
	DUC_SURFACE_BLOCK* dstBlock = &st.dstBlock;

	memset(&st, 0, sizeof(st));

	srcBlock->blockType = DUC_SURFACE_BLOCK_TYPE;
	srcBlock->blockFlags = 0;

	size = Duc_GetBlockSize((DUC_BLOCK*) srcBlock);

	fprintf(stderr, "surface block sparse -> serial size: %d\n", (int) size);

	if (size != 32)
	{
		fprintf(stderr, "unexpected surface block size: Actual: %d, Expected: %d\n",
				(int) size, 32);
		return MUNIT_FAIL;
	}

	xfrBlock->blockType = DUC_SURFACE_BLOCK_TYPE;
	xfrBlock->blockFlags = 0;
	xfrBlock->blockSize = (uint32_t) size;

	Duc_WriteBlock((DUC_BLOCK*) srcBlock, (DUC_BLOCK*) xfrBlock);

	size = Duc_GetBlockSize((DUC_BLOCK*) xfrBlock);

	fprintf(stderr, "surface block serial -> packed size: %d\n", (int) size);

	if (size != 32)
	{
		fprintf(stderr, "unexpected surface block size: Actual: %d, Expected: %d\n",
				(int) size, 32);
		return MUNIT_FAIL;
	}

	dstBlock->blockType = DUC_SURFACE_BLOCK_TYPE;
	dstBlock->blockFlags = 0;
	dstBlock->blockSize = (uint32_t) size;

	Duc_WriteBlock((DUC_BLOCK*) xfrBlock, (DUC_BLOCK*) dstBlock);

	return MUNIT_OK;
}

MunitResult test_duc_frame_block(const MunitParameter params[], void* user_data)
{
	size_t size;
	size_t totalSize;
	size_t planeSize[4];
	uint8_t* srcData[4];
	uint8_t* xfrData[4];
	uint8_t* frameData;
	int frameWidth = 64;
	int frameHeight = 64;
	int frameStep = frameWidth * 4;
	struct {
		DUC_FRAME_BLOCK srcBlock;
		DUC_PIXEL_BUFFER srcBuffers[1];
		DUC_PIXEL_BUFFER dstBuffers[1];
	} st;
	DUC_FRAME_BLOCK* srcBlock = &st.srcBlock;
	DUC_FRAME_BLOCK* xfrBlock;
	DUC_FRAME_BLOCK* dstBlock;
	DUC_PIXEL_FORMAT format;
	DUC_PIXEL_BUFFER* srcBuffer;
	DUC_PIXEL_BUFFER* xfrBuffer;
	uint16_t formatId = DUC_PIXEL_FORMAT_ID_BGRX32;

	memset(&st, 0, sizeof(st));

	srcBlock->blockType = DUC_FRAME_BLOCK_TYPE;
	srcBlock->blockFlags = 0;
	srcBlock->blockSize = DUC_FRAME_BLOCK_SIZE;

	srcBlock->surfaceId = 0;
	srcBlock->formatId = formatId;
	Duc_GetPixelFormat(formatId, &format);

	srcBlock->frameSize.width = frameWidth;
	srcBlock->frameSize.height = frameHeight;
	srcBlock->frameTime = 0;
	srcBlock->frameId = 0;

	frameData = (uint8_t*) calloc(1, frameStep * frameHeight);

	if (!frameData)
		return MUNIT_FAIL;

	test_fill_image_inc(frameData, frameStep, frameWidth, frameHeight);

	srcBlock->count = 1;
	srcBlock->buffers.ptr = st.srcBuffers;

	srcBuffer = &srcBlock->buffers.ptr[0];
	srcBuffer->rect.x = 16;
	srcBuffer->rect.y = 16;
	srcBuffer->rect.width = 32;
	srcBuffer->rect.height = 32;

	if (0)
	{
		srcBuffer->rect.x = 0;
		srcBuffer->rect.y = 0;
		srcBuffer->rect.width = 32;
		srcBuffer->rect.height = 32;
	}

	srcBuffer->step[0] = srcBuffer->rect.width * format.bytesPerPixel;
	srcBuffer->vstep[0] = srcBuffer->rect.height;
	srcBuffer->formatId = formatId;
	srcBuffer->flags = 0;
	srcBuffer->reserved = 0;

	totalSize = Duc_GetPlaneSizes(srcBuffer->step, srcBuffer->vstep, planeSize);

	srcBuffer->data[0].ptr = NULL;
	srcBuffer->data[1].ptr = NULL;
	srcBuffer->data[2].ptr = NULL;
	srcBuffer->data[3].ptr = NULL;

	size = Duc_GetBlockSize((DUC_BLOCK*) srcBlock);

	fprintf(stderr, "frame block sparse -> serial size: %d\n", (int) size);

	if (size != 4192)
	{
		fprintf(stderr, "unexpected frame block size: Actual: %d, Expected: %d\n",
				(int) size, 4192);
		return MUNIT_FAIL;
	}

	Duc_PrintFrameBlock(srcBlock, 0);

	xfrBlock = (DUC_FRAME_BLOCK*) malloc(size);

	if (!xfrBlock)
		return MUNIT_FAIL;

	xfrBlock->blockType = DUC_FRAME_BLOCK_TYPE;
	xfrBlock->blockFlags = 0;
	xfrBlock->blockSize = (uint32_t) size;

	Duc_WriteBlock((DUC_BLOCK*) srcBlock, (DUC_BLOCK*) xfrBlock);

	xfrBuffer = (DUC_PIXEL_BUFFER*) DucPtr_Get((DUC_PTR*) &xfrBlock->buffers, (void*) xfrBlock);

	srcData[0] = srcBuffer->data[0].ptr = Duc_GetRegionPointer(frameData, srcBuffer->rect.x, srcBuffer->rect.y, frameStep, 4);
	xfrData[0] = DucPtr_Get((DUC_PTR*) &xfrBuffer->data[0], (void*) xfrBlock);

	Duc_Copy_8u_C4R(srcData[0], srcBuffer->step[0], xfrData[0], xfrBuffer->step[0], xfrBuffer->rect.width, xfrBuffer->rect.height);

	Duc_PrintFrameBlock(xfrBlock, 0);

	size = Duc_GetBlockSize((DUC_BLOCK*) xfrBlock);

	fprintf(stderr, "frame block serial -> packed size: %d\n", (int) size);

	dstBlock = (DUC_FRAME_BLOCK*) malloc(size);

	if (!dstBlock)
		return MUNIT_FAIL;

	dstBlock->blockType = DUC_FRAME_BLOCK_TYPE;
	dstBlock->blockFlags = 0;
	dstBlock->blockSize = (uint32_t) size;

	Duc_WriteBlock((DUC_BLOCK*) xfrBlock, (DUC_BLOCK*) dstBlock);

	Duc_PrintFrameBlock(dstBlock, 0);

	if (0)
	{
		test_dump_to_file("srcFrame.block", (uint8_t*) srcBlock, srcBlock->blockSize);
		test_dump_to_file("xfrFrame.block", (uint8_t*) xfrBlock, xfrBlock->blockSize);
		test_dump_to_file("dstFrame.block", (uint8_t*) dstBlock, dstBlock->blockSize);
	}

	free(xfrBlock);
	free(dstBlock);
	free(frameData);

	return MUNIT_OK;
}

MunitResult test_duc_alignment(const MunitParameter params[], void* user_data)
{
	DUC_POINT point;
	DUC_SIZE size;
	DUC_RECT rect;
	DUC_PIXEL_CHANNEL pixelChannel;
	DUC_PIXEL_BUFFER pixelBuffer;
	DUC_PIXEL_FORMAT pixelFormat;
	DUC_BLOCK_HEADER blockHeader;
	DUC_HEADER_BLOCK headerBlock;
	DUC_FORMAT_BLOCK formatBlock;
	DUC_SURFACE_BLOCK surfaceBlock;
	DUC_FRAME_BLOCK frameBlock;

	assert(sizeof(DUC_POINT) == 4);
	assert(duc_is_aligned(&point, 4));
	assert(duc_is_aligned(&point.x, 2));
	assert(duc_is_aligned(&point.y, 2));

	assert(sizeof(DUC_SIZE) == 4);
	assert(duc_is_aligned(&size, 4));
	assert(duc_is_aligned(&size.width, 2));
	assert(duc_is_aligned(&size.height, 2));

	assert(sizeof(DUC_RECT) == 8);
	assert(duc_is_aligned(&rect, 8));
	assert(duc_is_aligned(&rect.x, 2));
	assert(duc_is_aligned(&rect.y, 2));
	assert(duc_is_aligned(&rect.width, 2));
	assert(duc_is_aligned(&rect.height, 2));

	assert(sizeof(DUC_PIXEL_CHANNEL) == DUC_PIXEL_CHANNEL_SIZE);
	assert(duc_is_aligned(&pixelChannel, 8));
	assert(duc_is_aligned(&pixelChannel.name, 4));
	assert(duc_is_aligned(&pixelChannel.flags, 1));
	assert(duc_is_aligned(&pixelChannel.depth, 1));
	assert(duc_is_aligned(&pixelChannel.unit, 1));
	assert(duc_is_aligned(&pixelChannel.offset, 1));

	assert(sizeof(pixelBuffer) == DUC_PIXEL_BUFFER_SIZE);
	assert(duc_is_aligned(&pixelBuffer, 8));
	assert(duc_is_aligned(&pixelBuffer.rect, 8));
	assert(duc_is_aligned(&pixelBuffer.step, 8));
	assert(duc_is_aligned(&pixelBuffer.vstep, 8));
	assert(duc_is_aligned(&pixelBuffer.formatId, 2));
	assert(duc_is_aligned(&pixelBuffer.flags, 2));
	assert(duc_is_aligned(&pixelBuffer.reserved, 4));
	assert(duc_is_aligned(&pixelBuffer.data, 8));

	assert(sizeof(pixelFormat) == DUC_PIXEL_FORMAT_SIZE);
	assert(duc_is_aligned(&pixelFormat.name, 8));
	assert(duc_is_aligned(&pixelFormat.id, 2));
	assert(duc_is_aligned(&pixelFormat.flags, 2));
	assert(duc_is_aligned(&pixelFormat.bitsPerPixel, 1));
	assert(duc_is_aligned(&pixelFormat.bytesPerPixel, 1));
	assert(duc_is_aligned(&pixelFormat.planeFlags, 1));
	assert(duc_is_aligned(&pixelFormat.planeCount, 1));
	assert(duc_is_aligned(&pixelFormat.channelFlags, 1));
	assert(duc_is_aligned(&pixelFormat.channelDepth, 1));
	assert(duc_is_aligned(&pixelFormat.channelUnit, 1));
	assert(duc_is_aligned(&pixelFormat.channelCount, 1));
	assert(duc_is_aligned(&pixelFormat.subsampling, 4));
	assert(duc_is_aligned(&pixelFormat.channels, 8));

	assert(sizeof(blockHeader) == DUC_BLOCK_HEADER_SIZE);
	assert(duc_is_aligned(&blockHeader, 16));
	assert(duc_is_aligned(&blockHeader.blockType, 2));
	assert(duc_is_aligned(&blockHeader.blockFlags, 2));
	assert(duc_is_aligned(&blockHeader.blockSize, 4));

	assert(sizeof(headerBlock) == DUC_HEADER_BLOCK_SIZE);
	assert(duc_is_aligned(&headerBlock, 16));
	assert(duc_is_aligned(&headerBlock.version, 2));
	assert(duc_is_aligned(&headerBlock.endianness, 2));
	assert(duc_is_aligned(&headerBlock.flags, 4));
	assert(duc_is_aligned(&headerBlock.reserved1, 4));
	assert(duc_is_aligned(&headerBlock.reserved2, 4));
	assert(duc_is_aligned(&headerBlock.reserved3, 4));
	assert(duc_is_aligned(&headerBlock.reserved4, 4));

	assert(sizeof(formatBlock) == DUC_FORMAT_BLOCK_SIZE);
	assert(duc_is_aligned(&formatBlock.reserved1, 4));
	assert(duc_is_aligned(&formatBlock.reserved2, 2));
	assert(duc_is_aligned(&formatBlock.count, 2));
	assert(duc_is_aligned(&formatBlock.formats, 8));

	assert(sizeof(surfaceBlock) == DUC_SURFACE_BLOCK_SIZE);
	assert(duc_is_aligned(&surfaceBlock.surfaceId, 2));
	assert(duc_is_aligned(&surfaceBlock.formatId, 2));
	assert(duc_is_aligned(&surfaceBlock.surfaceFlags, 2));
	assert(duc_is_aligned(&surfaceBlock.reserved1, 2));
	assert(duc_is_aligned(&surfaceBlock.surfaceRect, 8));
	assert(duc_is_aligned(&surfaceBlock.surfaceTime, 4));
	assert(duc_is_aligned(&surfaceBlock.reserved3, 4));

	assert(sizeof(DUC_FRAME_BLOCK) == DUC_FRAME_BLOCK_SIZE);
	assert(duc_is_aligned(&frameBlock.surfaceId, 2));
	assert(duc_is_aligned(&frameBlock.formatId, 2));
	assert(duc_is_aligned(&frameBlock.frameSize, 4));
	assert(duc_is_aligned(&frameBlock.frameTime, 4));
	assert(duc_is_aligned(&frameBlock.frameId, 2));
	assert(duc_is_aligned(&frameBlock.reserved, 1));
	assert(duc_is_aligned(&frameBlock.count, 1));
	assert(duc_is_aligned(&frameBlock.buffers, 8));

	return MUNIT_OK;
}

static MunitTest tests[] = {
	{ "TestDucHeaderBlock", test_duc_header_block, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
	{ "TestDucFormatBlock", test_duc_format_block, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
	{ "TestDucSurfaceBlock", test_duc_surface_block, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
	{ "TestDucFrameBlock", test_duc_frame_block, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
	{ "TestDucAlignment", test_duc_alignment, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
	{ NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite test_suite = { "", tests, NULL, 1, MUNIT_SUITE_OPTION_NONE };

int main(int argc, char** argv)
{
	return munit_suite_main(&test_suite, (void*) "munit", argc, argv);
}

