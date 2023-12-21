#pragma once
#include "../definition.h"
#include "clicker.h"

auto leftclick()
{
	if (GetForegroundWindow() != v::gui_hwnd && GetForegroundWindow() == FindWindow("LWJGL", nullptr))
	{
		if (!v::inventory && !v::inv_open)
		{
			GetCursorPos(&v::clicker_pt);
			v::clicker_hwnd = WindowFromPoint(v::clicker_pt);
			//std::cout << "left" << std::endl;
			float delay = c::randomizer(v::left_min - 2.5, v::left_max - 1.6);
			float up_delay = delay * c::random_float(c::random_float(0.3f, 0.55f), c::random_float(0.65f, 0.85f));
			float down_delay = delay - up_delay;
			PostMessage(v::clicker_hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(v::clicker_pt.x, v::clicker_pt.y));
			Sleep(down_delay);
			PostMessage(v::clicker_hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(v::clicker_pt.x, v::clicker_pt.y));
			Sleep(up_delay);
		}
		if(v::inv_open && (GetAsyncKeyState(VK_SHIFT) & 0x8000) && v::inventory)
		{
			GetCursorPos(&v::clicker_pt);
			v::clicker_hwnd = WindowFromPoint(v::clicker_pt);
			//std::cout << "left" << std::endl;
			float delay = c::randomizer(v::left_min - 2.5, v::left_max - 1.6);
			float up_delay = delay * c::random_float(c::random_float(0.3f, 0.55f), c::random_float(0.65f, 0.85f));
			float down_delay = delay - up_delay;
			PostMessage(v::clicker_hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(v::clicker_pt.x, v::clicker_pt.y));
			Sleep(down_delay);
			PostMessage(v::clicker_hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(v::clicker_pt.x, v::clicker_pt.y));
			Sleep(up_delay);
		}
		if (v::inventory && !v::inv_open)
		{
			{
				GetCursorPos(&v::clicker_pt);
				v::clicker_hwnd = WindowFromPoint(v::clicker_pt);
				//std::cout << "left" << std::endl;
				float delay = c::randomizer((v::left_min - 2.5), v::left_max - 1.6);
				float up_delay = delay * c::random_float(c::random_float(0.3f, 0.55f), c::random_float(0.65f, 0.85f));
				float down_delay = delay - up_delay;
				PostMessage(v::clicker_hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(v::clicker_pt.x, v::clicker_pt.y));
				Sleep(down_delay);
				PostMessage(v::clicker_hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(v::clicker_pt.x, v::clicker_pt.y));
				Sleep(up_delay);
			}
		}

	}
}

auto rightclick()
{
	if (GetForegroundWindow() != v::gui_hwnd && GetForegroundWindow() == FindWindow("LWJGL", nullptr))
	{
		GetCursorPos(&v::clicker_pt);
		v::clicker_hwnd = WindowFromPoint(v::clicker_pt);
		//std::cout << "right" << std::endl;
		float delay = c::randomizer(v::right_min - 1, v::right_max + 1);
		float up_delay = delay * c::random_float(c::random_float(0.3f, 0.55f), c::random_float(0.65f, 0.85f));
		float down_delay = delay - up_delay;
		PostMessage(v::clicker_hwnd, WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(v::clicker_pt.x, v::clicker_pt.y));
		Sleep(down_delay);
		PostMessage(v::clicker_hwnd, WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(v::clicker_pt.x, v::clicker_pt.y));
		Sleep(up_delay);
	}
}

void c::left_thread()
{
	
	while (true)
	{
		if (GetAsyncKeyState(v::left_bind) & 0x8000)
		{
			v::left_toggle = !v::left_toggle;
			Sleep(200);
		}

		if (v::left_toggle && GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			leftclick();
		}
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void c::right_thread()
{

	while (true)
	{
		if (v::right_toggle && GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		{
			rightclick();
		}
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

float c::randomizer(float min_cps, float max_cps)
{
	float cps_min = min_cps + c::random_float(0.5f, 0.8f);
	float cps_max = max_cps + c::random_float(0.6f, 1.f);
	float random_delay = c::random_float((float)cps_min, (float)cps_max);
	float random_deviation = c::random_float(750.f, 850.f);

	int drop_value = c::random_int(0, 60);
	int spike_value = c::random_int(0, 60);
	int cps_drop_chance = 8;
	int cps_spike_chance = 50;
	float drop_min = 1.f;
	float drop_max = 1.5f;
	float spike_min = 1.f;
	float spike_max = 2.2f;

	random_delay = random_deviation / random_delay;

	if (random_deviation > 800)
	{
		if (spike_value < cps_spike_chance)
		{
			random_delay -= c::random_float(spike_min, spike_max);
			switch (c::random_int(1, 2))
			{
			case 1:
				random_delay *= c::random_float(0.8f, 1.1f);
				break;
			case 2:
				random_delay /= c::random_float(0.73f, 1.43f);
				break;
			}
		}
	}
	else if (random_deviation < 800)
	{
		if (drop_value < cps_drop_chance)
		{
			random_delay += c::random_float(drop_min, drop_max);
			switch (c::random_int(1, 2))
			{
			case 1:
				random_delay *= c::random_float(1.25f, 1.5f);
				break;
			case 2:
				random_delay /= c::random_float(0.75f, 1.5f);
				break;
			}
		}
	}
	if (!v::customRandomization)
	{
		return random_delay;
	}
	else
	{
		switch (c::random_int(1, 2))
		{
		case 1:
			return random_delay + c::random_float(v::customFloatMin, v::customFloatMin);
			break;
		case 2:
			return random_delay - c::random_float(v::customFloatMin, v::customFloatMin);
			break;
		}
	}
	
}

LRESULT __stdcall c::keyboard_cb(int code, WPARAM param, LPARAM lparam)
{
	auto key_struct = reinterpret_cast<PKBDLLHOOKSTRUCT>(lparam);
	if ((key_struct->flags == LLKHF_INJECTED) || (key_struct->flags == LLKHF_LOWER_IL_INJECTED))
	{
		return false;
	}
	else if ((key_struct->flags & LLKHF_INJECTED) == LLKHF_INJECTED)
	{
		return false;
	}
	else if (key_struct != nullptr)
	{
		if (param == WM_KEYDOWN && code == HC_ACTION)
		{
			if (key_struct->vkCode == v::binds[0])
			{
				v::left_toggle = !v::left_toggle;
			}
			if (key_struct->vkCode == v::binds[1])
			{
				v::right_toggle = !v::right_toggle;
			}
		}
	}
	return CallNextHookEx(v::keyboard_hook, code, param, lparam);
}

LRESULT __stdcall c::mouse_cb(int code, WPARAM param, LPARAM lparam)
{
	
	if (code < 0) // do not process message 
		return CallNextHookEx(v::mouse_hook, code, param, lparam);

	auto mouse_struct = reinterpret_cast<PMSLLHOOKSTRUCT>(lparam);
	if ((mouse_struct->flags == LLMHF_INJECTED) || (mouse_struct->flags == LLMHF_LOWER_IL_INJECTED))
	{
		return false;
	}
	else if ((mouse_struct->flags & LLMHF_INJECTED) == LLMHF_INJECTED)
	{
		return false;
	}
	else if (mouse_struct != nullptr)
	{
		if (param != WM_MOUSEMOVE && code == HC_ACTION)
		{
				switch (param)
				{
				case WM_LBUTTONDOWN:
				{
					v::left_down = true;
					v::left_first_click = true;
					break;
				}
				case WM_LBUTTONUP:
				{
					v::left_down = false;
					break;
				}
				case WM_RBUTTONDOWN:
				{
					v::right_down = true;
					v::right_first_click = true;
					if (v::right_lock)
					{
						v::right_held = true;
					}
					if (v::left_down)
					{
						v::right_held = false;
					}
					break;
				}
				case WM_RBUTTONUP:
				{
					v::right_down = false;
					if (v::right_lock && v::right_held)
					{
						v::right_held = false;
					}
					break;
				}
			}
		}
	}
	return CallNextHookEx(v::mouse_hook, code, param, lparam);
}


int c::random_int(int min, int max)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max);
	return dist6(rng);;
}

float c::random_float(float min, float max)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_real_distribution<float> dist6(min, max);
	return dist6(rng);;
}

void c::hook()
{
	
	v::keyboard_hook = SetWindowsHookEx(WH_KEYBOARD_LL, c::keyboard_cb, nullptr, 0);
	//v::mouse_hook = SetWindowsHookEx(WH_MOUSE_LL, c::mouse_cb, nullptr, 0);

	MSG message;

	while (GetMessage(&message, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	
	UnhookWindowsHookEx(v::keyboard_hook);
	//UnhookWindowsHookEx(v::mouse_hook);

}

void c::inventory_thread()
{
	while (true)
	{
		CURSORINFO ci = { sizeof(CURSORINFO) };
		if (GetCursorInfo(&ci))
		{
			HCURSOR hc = ci.hCursor;
			if ((int)hc > 50000 && (int)hc < 100000)
			{
				v::inv_open = true;
				
			}
			else
			{
				v::inv_open = false;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}