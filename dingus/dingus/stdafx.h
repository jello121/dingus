// --------------------------------------------------------------------------
// Dingus project - a collection of subsystems for game/graphics applications
// --------------------------------------------------------------------------

#ifndef __DINGUS_STDAFX_H
#define __DINGUS_STDAFX_H

// precompiled headers stuff

//#pragma warning(disable:4786)
//#pragma warning(disable:4018)
#pragma warning(disable:4511) // copy constructor can't be generated
#pragma warning(disable:4512) // assignment operator can't be generated
#pragma warning(disable:4100) // unreferenced formal parameter
#pragma warning(disable:4201) // nonstandard extension used: nameless struct
#pragma warning(disable:4702) // unreachable code (lots of that in STL)

// synch unicode settings
#ifdef _UNICODE
#define UNICODE
#endif


// windows
#ifdef _WINDLL

#define VC_EXTRALEAN
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif

#else

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#endif


// STL
#include <list>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <string>

// String type, depending on Unicode settings
#ifdef _UNICODE
typedef std::wstring	unistring;
#else
typedef std::string		unistring;
#endif


// C libs
#include <cassert>
#include <cmath>
#include <cstring>
#include <malloc.h>
// do NOT include these, so it's less tempting to use rand() etc.
//#include <cstdio>
//#include <cstdlib>
//#include <cstdarg>

// D3D
#include <d3d9.h>
#include <d3dx9.h>

// We have various API changes in D3DX SDKs...
#undef DINGUS_HAVE_D3DX_SUMMER_2004
#undef DINGUS_HAVE_D3DX_FEB_2005

#if D3DX_VERSION >= 0x0902 && D3DX_SDK_VERSION >= 22
#define DINGUS_HAVE_D3DX_SUMMER_2004
#endif
#if D3DX_VERSION >= 0x0902 && D3DX_SDK_VERSION >= 25 // don't know about 25 for sure, just guessing!
#define DINGUS_HAVE_D3DX_FEB_2005
#endif


// smart pointers
#include <boost/intrusive_ptr.hpp>
#include <boost/noncopyable.hpp>


// debug memory manager
#define NO_MMGR
#include "utils/mmgr.h"

// common
#include "utils/MemUtil.h"
#include "utils/STLUtils.h"
#include "utils/fastvector.h"
#include "utils/AssertHelper.h"
#include "utils/RefCounted.h"
#include "utils/Timer.h"


#endif
