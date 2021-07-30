// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#include "targetver.h"
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>

// GdiPlus
#ifdef GDIPVER
#undef GDIPVER
#endif
#define GDIPVER 0x0110
#include <gdiplus.h>

// Dui
#include "common/dui_refptr.h"
#include "common/dui_refptr_object.h"
#include "render/render_interface.h"

// Render 
#include "bitmap.h"
#include "brush.h"
#include "clip.h"
#include "matrix.h"
#include "path.h"
#include "pen.h"
#include "render.h"
#include "factory.h"