#ifndef DUC_TAPE_H
#define DUC_TAPE_H

#include <duc/core.h>

typedef struct duc_tape DucTape;

#ifdef __cplusplus
extern "C" {
#endif

bool DucTape_ReadHeaderBlock(DucTape* tape);
bool DucTape_WriteHeaderBlock(DucTape* tape);

bool DucTape_WriteFormatBlock(DucTape* tape);

void DucTape_SetPixelFormat(DucTape* tape, DUC_PIXEL_FORMAT* pixelFormat);
void DucTape_GetPixelFormat(DucTape* tape, DUC_PIXEL_FORMAT* pixelFormat);

bool DucTape_CreateSurface(DucTape* tape, uint16_t surfaceId, DUC_RECT* surfaceRect);
bool DucTape_DeleteSurface(DucTape* tape, uint16_t surfaceId, DUC_RECT* surfaceRect);

bool DucTape_ReadBlock(DucTape* tape, DUC_BLOCK** ppBlock);
bool DucTape_WriteBlock(DucTape* tape, DUC_BLOCK* block);

bool DucTape_Open(DucTape* duc, const char* filename, bool write);
bool DucTape_Close(DucTape* duc);

DucTape* DucTape_New();
void DucTape_Free(DucTape* duc);

#ifdef __cplusplus
}
#endif

#endif /* DUC_TAPE_H */

