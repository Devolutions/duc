
#include <duc/core.h>

extern DUC_PIXEL_FORMAT g_DucPixelFormats[];

DUC_PIXEL_FORMAT* Duc_FindPixelFormat(uint16_t id)
{
	bool found = false;
	DUC_PIXEL_FORMAT* format = NULL;
	DUC_PIXEL_FORMAT* formats = g_DucPixelFormats;

	while (formats->id)
	{
		if (id == formats->id)
		{
			format = formats;
			found = true;
		}
		formats++;
	}

	return found ? format : NULL;
}

bool Duc_GetPixelFormat(uint16_t id, DUC_PIXEL_FORMAT* format)
{
	DUC_PIXEL_FORMAT* found;

	if (!format)
		return false;

	found = Duc_FindPixelFormat(id);

	if (!found)
	{
		duc_memset(format, 0, sizeof(DUC_PIXEL_FORMAT));
		return false;
	}

	duc_memcpy(format, found, sizeof(DUC_PIXEL_FORMAT));
	return true;
}

uint16_t Duc_GetPixelFormatId(const char* name)
{
	bool found = false;
	DUC_PIXEL_FORMAT* format = NULL;
	DUC_PIXEL_FORMAT* formats = g_DucPixelFormats;

	while (formats->id)
	{
		if (!strcmp(name, (const char*) formats->name))
		{
			format = formats;
			found = true;
		}
		formats++;
	}

	return found ? format->id : 0;
}

DUC_PIXEL_FORMAT g_DucPixelFormats[] = {
{
	"ARGB32", DUC_PIXEL_FORMAT_ID_ARGB32,
	DUC_PIXEL_FORMAT_FLAG_RGB | DUC_PIXEL_FORMAT_FLAG_ALPHA,
	32, 4, 0, 1, 0, 8, 1, 4, { 4, 4, 4, 4 }, {
		{ { 'A', 0, 0, 0 }, 0, 8, 1, 0  },
		{ { 'R', 0, 0, 0 }, 0, 8, 1, 8  },
		{ { 'G', 0, 0, 0 }, 0, 8, 1, 16 },
		{ { 'B', 0, 0, 0 }, 0, 8, 1, 24 }
	}
},
{
	"XRGB32", DUC_PIXEL_FORMAT_ID_XRGB32,
	DUC_PIXEL_FORMAT_FLAG_RGB | DUC_PIXEL_FORMAT_FLAG_ALPHA | DUC_PIXEL_FORMAT_FLAG_OPAQUE,
	32, 4, 0, 1, 0, 8, 1, 4, { 4, 4, 4, 4 }, {
		{ { 'X', 0, 0, 0 }, 0, 8, 1, 0  },
		{ { 'R', 0, 0, 0 }, 0, 8, 1, 8  },
		{ { 'G', 0, 0, 0 }, 0, 8, 1, 16 },
		{ { 'B', 0, 0, 0 }, 0, 8, 1, 24 }
	}
},
{
	"ABGR32", DUC_PIXEL_FORMAT_ID_ABGR32,
	DUC_PIXEL_FORMAT_FLAG_RGB | DUC_PIXEL_FORMAT_FLAG_ALPHA,
	32, 4, 0, 1, 0, 8, 1, 4, { 4, 4, 4, 4 }, {
		{ { 'A', 0, 0, 0 }, 0, 8, 1, 0  },
		{ { 'B', 0, 0, 0 }, 0, 8, 1, 8  },
		{ { 'G', 0, 0, 0 }, 0, 8, 1, 16 },
		{ { 'R', 0, 0, 0 }, 0, 8, 1, 24 }
	}
},
{
	"XBGR32", DUC_PIXEL_FORMAT_ID_XBGR32,
	DUC_PIXEL_FORMAT_FLAG_RGB | DUC_PIXEL_FORMAT_FLAG_ALPHA | DUC_PIXEL_FORMAT_FLAG_OPAQUE,
	32, 4, 0, 1, 0, 8, 1, 4, { 4, 4, 4, 4 }, {
		{ { 'X', 0, 0, 0 }, 0, 8, 1, 0  },
		{ { 'B', 0, 0, 0 }, 0, 8, 1, 8  },
		{ { 'G', 0, 0, 0 }, 0, 8, 1, 16 },
		{ { 'R', 0, 0, 0 }, 0, 8, 1, 24 }
	}
},
{
	"BGRA32", DUC_PIXEL_FORMAT_ID_BGRA32,
	DUC_PIXEL_FORMAT_FLAG_RGB | DUC_PIXEL_FORMAT_FLAG_ALPHA,
	32, 4, 0, 1, 0, 8, 1, 4, { 4, 4, 4, 4 }, {
		{ { 'B', 0, 0, 0 }, 0, 8, 1, 0  },
		{ { 'G', 0, 0, 0 }, 0, 8, 1, 8  },
		{ { 'R', 0, 0, 0 }, 0, 8, 1, 16 },
		{ { 'A', 0, 0, 0 }, 0, 8, 1, 24 }
	}
},
{
	"BGRX32", DUC_PIXEL_FORMAT_ID_BGRX32,
	DUC_PIXEL_FORMAT_FLAG_RGB | DUC_PIXEL_FORMAT_FLAG_ALPHA | DUC_PIXEL_FORMAT_FLAG_OPAQUE,
	32, 4, 0, 1, 0, 8, 1, 4, { 4, 4, 4, 4 }, {
		{ { 'B', 0, 0, 0 }, 0, 8, 1, 0  },
		{ { 'G', 0, 0, 0 }, 0, 8, 1, 8  },
		{ { 'R', 0, 0, 0 }, 0, 8, 1, 16 },
		{ { 'X', 0, 0, 0 }, 0, 8, 1, 24 }
	}
},
{
	"RGBA32", DUC_PIXEL_FORMAT_ID_RGBA32,
	DUC_PIXEL_FORMAT_FLAG_RGB | DUC_PIXEL_FORMAT_FLAG_ALPHA,
	32, 4, 0, 1, 0, 8, 1, 4, { 4, 4, 4, 4 }, {
		{ { 'R', 0, 0, 0 }, 0, 8, 1, 0  },
		{ { 'G', 0, 0, 0 }, 0, 8, 1, 8  },
		{ { 'B', 0, 0, 0 }, 0, 8, 1, 16 },
		{ { 'A', 0, 0, 0 }, 0, 8, 1, 24 }
	}
},
{
	"RGBX32", DUC_PIXEL_FORMAT_ID_RGBX32,
	DUC_PIXEL_FORMAT_FLAG_RGB | DUC_PIXEL_FORMAT_FLAG_ALPHA | DUC_PIXEL_FORMAT_FLAG_OPAQUE,
	32, 4, 0, 1, 0, 8, 1, 4, { 4, 4, 4, 4 }, {
		{ { 'R', 0, 0, 0 }, 0, 8, 1, 0  },
		{ { 'G', 0, 0, 0 }, 0, 8, 1, 8  },
		{ { 'B', 0, 0, 0 }, 0, 8, 1, 16 },
		{ { 'X', 0, 0, 0 }, 0, 8, 1, 24 }
	}
},
{
	"RGB", DUC_PIXEL_FORMAT_ID_RGB,
	DUC_PIXEL_FORMAT_FLAG_RGB | DUC_PIXEL_FORMAT_FLAG_PLANAR,
	24, 3, 0, 3, 0, 8, 1, 3, { 4, 4, 4, 4 }, {
		{ { 'R', 0, 0, 0 }, 0, 8, 1, 0  },
		{ { 'G', 0, 0, 0 }, 0, 8, 1, 8  },
		{ { 'B', 0, 0, 0 }, 0, 8, 1, 16 },
		{ {  0,  0, 0, 0 }, 0, 0, 0, 0 }
	}
},
{
	"", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, { 0, 0, 0, 0 }, {
		{ { 0, 0, 0, 0 }, 0, 0, 0, 0 },
		{ { 0, 0, 0, 0 }, 0, 0, 0, 0 },
		{ { 0, 0, 0, 0 }, 0, 0, 0, 0 },
		{ { 0, 0, 0, 0 }, 0, 0, 0, 0 }
	}
},
};
