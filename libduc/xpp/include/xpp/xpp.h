#ifndef DUC_XPP_H
#define DUC_XPP_H

#include <duc/core.h>

/**
 * XPP: eXtreme Performance Primitives
 */

/**
 * Intel Performance Primitives (reference):
 *
 * Structures and Enumerators:
 * https://software.intel.com/en-us/node/503715
 */

#define DUC_EXPORT 

typedef struct {
	int x;
	int y;
} XppPoint;

typedef struct {
	int width;
	int height;
} XppSize;

typedef struct {
	int x;
	int y;
	int width;
	int height;
} XppRect;

typedef enum xpp_interpolation_mode {
	XppInterpolationNearest = 0,
	XppInterpolationLinear = 1,
	XppInterpolationBilinear = 2,
	XppInterpolationBox = 3
} XppInterpolationMode;

#endif /* DUC_XPP_H */
