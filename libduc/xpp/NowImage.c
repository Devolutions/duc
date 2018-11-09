
#include <Wayk/NowBmp.h>
#include <Wayk/NowPng.h>
#include <Wayk/NowFile.h>

#include <Wayk/NowImage.h>

int wayk_bmp_write(const char* filename, uint8_t* data, int width, int height, int bpp)
{
	return NowBmp_WriteToFile(filename, data, width, height, bpp);
}

int wayk_png_write(const char* filename, uint8_t* data, int width, int height, int bpp)
{
	return NowPng_WriteToFile(filename, data, width, height);
}

int wayk_y4m_write(wayk_image_t* img, const char* filename)
{
	img->fp = NowFile_Open(filename, "wb");

	if (!img->fp)
		return -1;

	fprintf(img->fp, "YUV4MPEG2 W%d H%d F30:1 Ip A1:1 C420jpeg XYSCSS=420JPEG\n",
		img->width, img->height);

	return 1;
}

int wayk_y4m_write_frame(wayk_image_t* img, uint8_t* pSrc[3], int32_t srcStep[3])
{
	uint32_t lumaSize;
	uint32_t chromaSize;

	lumaSize = img->width * img->height;
	chromaSize = lumaSize / 4;

	fprintf(img->fp, "FRAME\n");

	if (fwrite(pSrc[0], 1, lumaSize, img->fp) != lumaSize)
		return -1;

	if (fwrite(pSrc[1], 1, chromaSize, img->fp) != chromaSize)
		return -1;

	if (fwrite(pSrc[2], 1, chromaSize, img->fp) != chromaSize)
		return -1;

	img->frameCount++;

	return 1;
}

int wayk_yuv_write(wayk_image_t* img, const char* filename)
{
	img->fp = NowFile_Open(filename, "wb");

	if (!img->fp)
		return -1;

	return 1;
}

int wayk_yuv_write_frame(wayk_image_t* img, uint8_t* pSrc[3], int32_t srcStep[3])
{
	uint32_t lumaSize;
	uint32_t chromaSize;

	lumaSize = img->width * img->height;
	chromaSize = lumaSize / 4;

	if (fwrite(pSrc[0], 1, lumaSize, img->fp) != lumaSize)
		return -1;

	if (fwrite(pSrc[1], 1, chromaSize, img->fp) != chromaSize)
		return -1;

	if (fwrite(pSrc[2], 1, chromaSize, img->fp) != chromaSize)
		return -1;

	img->frameCount++;

	return 1;
}

int wayk_rgb_write(wayk_image_t* img, const char* filename)
{
	img->fp = NowFile_Open(filename, "wb");

	if (!img->fp)
		return -1;

	return 1;
}

int wayk_rgb_write_frame(wayk_image_t* img, uint8_t* pSrc, int32_t srcStep)
{
	int32_t srcSize = srcStep * img->height;

	if (fwrite(pSrc, 1, srcSize, img->fp) != srcSize)
		return -1;

	img->frameCount++;

	return 1;
}

int wayk_image_write(wayk_image_t* img, const char* filename)
{
	int status = -1;

	if (img->type == WAYK_IMAGE_RGB)
	{
		status = wayk_rgb_write(img, filename);
	}
	else if (img->type == WAYK_IMAGE_YUV)
	{
		status = wayk_yuv_write(img, filename);
	}
	else if (img->type == WAYK_IMAGE_BMP)
	{
		status = wayk_bmp_write(filename, img->data, img->width, img->height, img->bitsPerPixel);
	}
	else if (img->type == WAYK_IMAGE_PNG)
	{
		status = wayk_png_write(filename, img->data, img->width, img->height, img->bitsPerPixel);
	}
	else if (img->type == WAYK_IMAGE_Y4M)
	{
		status = wayk_y4m_write(img, filename);
	}

	return status;
}

int wayk_image_png_read_fp(wayk_image_t* img, FILE* fp)
{
	size_t size;
	int status = -1;
	int32_t width;
	int32_t height;
	uint8_t* data = NULL;

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	data = (uint8_t*) malloc(size);

	if (!data)
		return -1;

	if (fread(data, size, 1, fp) != 1)
	{
		status = -1;
		goto exit;
	}

	status = NowPng_ReadFromMemory(&(img->data), &width, &height, data, size);

	free(data);
	data = NULL;

	img->width = width;
	img->height = height;

	img->bitsPerPixel = 32;
	img->bytesPerPixel = 4;
	img->scanline = img->bytesPerPixel * img->width;

exit:
	return status;
}

int wayk_image_png_read_buffer(wayk_image_t* img, uint8_t* data, int size)
{
	int status = -1;
	int32_t width = 0;
	int32_t height = 0;

	img->type = WAYK_IMAGE_PNG;

	status = NowPng_ReadFromMemory(&(img->data), &width, &height, data, size);

	if (status < 0)
		return -1;

	img->width = width;
	img->height = height;

	img->bitsPerPixel = 32;
	img->bytesPerPixel = 4;
	img->scanline = img->bytesPerPixel * img->width;

	return 1;
}

int wayk_image_bmp_read_fp(wayk_image_t* img, FILE* fp)
{
	int index;
	int vflip;
	uint8_t* pDstData;
	NOW_BITMAP_FILE_HEADER bf;
	NOW_BITMAP_INFO_HEADER bi;

	if (fread(&bf, sizeof(NOW_BITMAP_FILE_HEADER), 1, fp) != 1)
		return -1;

	if ((bf.bfType[0] != 'B') || (bf.bfType[1] != 'M'))
		return -1;

	img->type = WAYK_IMAGE_BMP;

	if (fread(&bi, sizeof(NOW_BITMAP_INFO_HEADER), 1, fp) != 1)
		return -1;

	if (ftell(fp) != bf.bfOffBits)
	{
		fseek(fp, bf.bfOffBits, SEEK_SET);
	}

	// Lengths of zero do not make sense and can cause a division by zero
	// below.
	if (bi.biHeight == 0 || bi.biWidth == 0)
		return -1;

	img->width = bi.biWidth;

	if (bi.biHeight < 0)
	{
		vflip = 0;
		img->height = -1 * bi.biHeight;
	}
	else
	{
		vflip = 1;
		img->height = bi.biHeight;
	}

	img->bitsPerPixel = bi.biBitCount;
	img->bytesPerPixel = (img->bitsPerPixel / 8);
	img->scanline = (bi.biSizeImage / img->height);

	img->data = (uint8_t*) malloc(bi.biSizeImage);

	if (!img->data)
		return -1;

	if (!vflip)
	{
		if (fread(img->data, bi.biSizeImage, 1, fp) != 1)
		{
			free(img->data);
			img->data = NULL;
			return -1;
		}
	}
	else
	{
		pDstData = &(img->data[(img->height - 1) * img->scanline]);

		for (index = 0; index < img->height; index++)
		{
			if (fread(pDstData, img->scanline, 1, fp) != 1)
			{
				free(img->data);
				img->data = NULL;
				return -1;
			}
			pDstData -= img->scanline;
		}
	}

	return 1;
}

int wayk_image_y4m_read_fp(wayk_image_t* img, FILE* fp)
{
	int pos;
	int len;
	int num;
	int den;
	char* end;
	char buf[256];
	double fps = 30;
	uint32_t width = 0;
	uint32_t height = 0;
	uint32_t lumaSize;
	uint32_t chromaSize;
	uint32_t frameSize;
	uint32_t fileSize;

	len = (int) fread(buf, 1, sizeof(buf), fp);
	buf[255] = '\0';

	if (strncmp(buf, "YUV4MPEG2 ", 10) != 0)
		return -1;

	pos = 10;

	while ((pos < len) && (buf[pos] != '\n'))
	{
		switch (buf[pos++])
		{
			case 'W':
				width = strtol(buf + pos, &end, 10);
				pos = (int) (end - buf + 1);
				break;
			case 'H':
				height = strtol(buf + pos, &end, 10);
				pos = (int) (end - buf + 1);
				break;
			case 'F':
				den = strtol(buf + pos, &end, 10);
				pos = (int) (end - buf + 1);
				num = strtol(buf + pos, &end, 10);

				// Avoid division by zero.
				if (num == 0)
					return -1;

				pos = (int) (end - buf + 1);
				fps = (double) (den / num);
				break;
			case 'I':
				if (buf[pos] != 'p') {
					fprintf(stderr, "Only progressive input supported\n");
					return -1;
				}
				break;
			case 'C':
				if (strcmp(buf+pos, "C420")) {
				}
				/* Fallthrough */
			case 'A': /* Ignored */
			case 'X':
			default:
				while (buf[pos] != ' ' && buf[pos] != '\n' && pos < len)
					pos++;
				break;
		}
	}

	if (strncmp(buf + pos, "\nFRAME\n", 7) != 0)
		return -1;

	img->y4m_fps = fps;
	img->y4m_file_hdrlen = pos + 1;
	img->y4m_frame_hdrlen = 6;

	img->width = width;
	img->height = height;

	lumaSize = img->width * img->height;
	chromaSize = lumaSize / 4;
	frameSize = img->y4m_frame_hdrlen + lumaSize + (chromaSize * 2);

	fseek(fp, 0, SEEK_END);
	fileSize = (uint32_t) ftell(fp);
	fseek(fp, img->y4m_file_hdrlen, SEEK_SET);

	// Avoid division by zero.
	if (frameSize == 0)
		return -1;

	img->frameIndex = 0;
	img->frameCount = (fileSize - img->y4m_file_hdrlen) / frameSize;

	return 1;
}

int wayk_y4m_read_frame(wayk_image_t* img, uint8_t* pDst[3], int32_t dstStep[3])
{
	uint32_t lumaSize;
	uint32_t chromaSize;

	if (fseek(img->fp, img->y4m_frame_hdrlen, SEEK_CUR) != 0)
		return -1;

	lumaSize = img->width * img->height;
	chromaSize = lumaSize / 4;

	if (fread(pDst[0], 1, lumaSize, img->fp) != lumaSize)
		return -1;

	if (fread(pDst[1], 1, chromaSize, img->fp) != chromaSize)
		return -1;

	if (fread(pDst[2], 1, chromaSize, img->fp) != chromaSize)
		return -1;

	img->frameIndex++;

	return 1;
}

int wayk_yuv_read_frame(wayk_image_t* img, uint8_t* pDst[3], int32_t dstStep[3])
{
	uint32_t lumaSize;
	uint32_t chromaSize;

	lumaSize = img->width * img->height;
	chromaSize = lumaSize / 4;

	if (fread(pDst[0], 1, lumaSize, img->fp) != lumaSize)
		return -1;

	if (fread(pDst[1], 1, chromaSize, img->fp) != chromaSize)
		return -1;

	if (fread(pDst[2], 1, chromaSize, img->fp) != chromaSize)
		return -1;

	img->frameIndex++;

	return 1;
}

int wayk_rgb_read_frame(wayk_image_t* img, uint8_t* pDst, int32_t dstStep)
{
	uint32_t frameSize = img->scanline * img->height;

	if (fread(pDst, 1, frameSize, img->fp) != frameSize)
		return -1;

	img->frameIndex++;

	return 1;
}

int wayk_image_read(wayk_image_t* img, const char* filename)
{
	FILE* fp;
	uint8_t sig[10];
	size_t fileSize;
	size_t frameSize;
	int status = -1;

	fp = NowFile_Open(filename, "rb");

	if (!fp)
		return -1;

	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if (!img->type)
	{
		if (strstr(filename, ".png"))
			img->type = WAYK_IMAGE_PNG;
		else if (strstr(filename, ".bmp"))
			img->type = WAYK_IMAGE_BMP;
		else if (strstr(filename, ".y4m"))
			img->type = WAYK_IMAGE_Y4M;
		else if (strstr(filename, ".yuv"))
			img->type = WAYK_IMAGE_YUV;
		else if (strstr(filename, ".rgb"))
			img->type = WAYK_IMAGE_RGB;
	}

	if (img->type == WAYK_IMAGE_RGB)
	{
		img->fp = fp;
		frameSize = img->scanline * img->height;
		img->frameCount = (int) (fileSize / frameSize);
		return 1;
	}
	else if (img->type == WAYK_IMAGE_YUV)
	{
		img->fp = fp;
		frameSize = (img->width * img->height) + ((img->width * img->height) / 2);
		img->frameCount = (int) (fileSize / frameSize);
		return 1;
	}

	if (fread(&sig, sizeof(sig), 1, fp) != 1 || fseek(fp, 0, SEEK_SET) < 0)
	{
		fclose(fp);
		return -1;
	}

	if ((sig[0] == 'B') && (sig[1] == 'M'))
	{
		img->type = WAYK_IMAGE_BMP;
		status = wayk_image_bmp_read_fp(img, fp);
		fclose(fp);
	}
	else if ((sig[0] == 0x89) && (sig[1] == 'P') && (sig[2] == 'N') && (sig[3] == 'G') &&
		 (sig[4] == '\r') && (sig[5] == '\n') && (sig[6] == 0x1A) && (sig[7] == '\n'))
	{
		img->type = WAYK_IMAGE_PNG;
		status = wayk_image_png_read_fp(img, fp);
		fclose(fp);
	}
	else if ((sig[0] == 'Y') && (sig[1] == 'U') && (sig[2] == 'V') && (sig[3] == '4') &&
		 (sig[4] == 'M') && (sig[5] == 'P') && (sig[6] == 'E') && (sig[7] == 'G') &&
		 (sig[8] == '2') && (sig[9] == ' '))
	{
		img->fp = fp;
		img->type = WAYK_IMAGE_Y4M;
		status = wayk_image_y4m_read_fp(img, fp);
	}
	else
	{
		img->type = WAYK_IMAGE_NONE;
		fclose(fp);
	}

	if (!img->type)
		return -1;

	return status;
}

wayk_image_t* wayk_image_new()
{
	wayk_image_t * image;

	image = (wayk_image_t *) calloc(1, sizeof(wayk_image_t));

	if (!image)
		return NULL;

	return image;
}

void wayk_image_free(wayk_image_t* img, int freeBuffer)
{
	if (!img)
		return;

	if (img->fp)
	{
		fclose(img->fp);
		img->fp = NULL;
	}

	if (freeBuffer)
	{
		free(img->data);
		img->data = NULL;
	}

	free(img);
}
