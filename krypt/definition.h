#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include "dxgi.h"
#include "d3d11.h"
#include "windef.h"
#include "xor.h"
#include <d3dcompiler.h>
#include <minwindef.h>
#include <random>
#include <chrono>
#include <thread>
#include <WinUser.h>
#include <vector>
#include "bhop.h"
#include "flight.h"
#include "Velocity.h"

namespace v
{
	inline bool left_toggle = false;
	inline bool left_down = false;
	inline bool left_first_click = false;
	inline float left_min = 5.f;
	inline float left_max = 5.f;
	inline int left_bind = 0x0;

	inline bool right_toggle = false;
	inline bool right_down = false;
	inline bool right_first_click = false;
	inline float right_min = 5.f;
	inline float right_max = 5.f;
	inline int right_bind = 0x0;

	inline bool inventory = false;
	inline bool inv_open = false;
	inline bool blatant = false;
	inline bool rblatant = false;
	inline bool minecraft_only = true;
	inline bool right_lock = true;
	inline bool right_held = false;
	inline float fBlatant = 20.f;
	inline float fBlatantR = 20.f;
	
	inline bool customRandomization = false;
	inline float customFloatMin = 0.1;
	inline float customFloatMax = 2.0;
	
	inline bool selfDestruct = false;

	inline HWND clicker_hwnd = nullptr;
	inline POINT clicker_pt = { 0 , 0 };
	inline HWND gui_hwnd = nullptr;

	inline bool gui_rainbow = false;
	inline float col[3] = { 0.48f, 0.01f, 0.85f };

	inline HHOOK mouse_hook = nullptr;
	inline HHOOK keyboard_hook = nullptr;

	inline std::vector<unsigned int> binds = { 0, 0 };
	inline std::vector<std::string> binds_text = { "NONE", "NONE" };

}
