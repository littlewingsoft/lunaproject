// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#pragma once
#ifndef LUNAUI_STDAFX_H
#define LUNAUI_STDAFX_H

#ifndef STRICT
#define STRICT
#endif

// If app hasn't choosen, set to work with Windows 98, Windows Me, Windows 2000, Windows XP and beyond
#ifndef WINVER
#define WINVER         0x0410
#endif
#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410 
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT   0x0500 
#endif


#include <windows.h>
#include <assert.h>
#include <wchar.h>
#include <mmsystem.h>
#include <commctrl.h> // for InitCommonControls() 
#include <shellapi.h> // for ExtractIcon()
#include <new.h>      // for placement new
#include <math.h>      
#include <limits.h>      
#include <stdio.h>
#include <XInput.h> // Header for XInput APIs

// CRT's memory leak detection
#if defined(DEBUG) || defined(_DEBUG)
#include <crtdbg.h>
#endif


// Direct3D includes
#include "dxcommon/dxstdafx.h"

// TODO: stl 관련헤더
//#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <list>

extern "C" 
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
};
#include "lua_tinker.h"

#include "uiCore.h"



// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#endif