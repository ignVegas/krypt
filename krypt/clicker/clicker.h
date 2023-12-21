
#pragma once


namespace c
{

	LRESULT __stdcall keyboard_cb(int n_code, WPARAM w_param, LPARAM l_praram);
	LRESULT __stdcall mouse_cb(int n_code, WPARAM w_param, LPARAM l_praram);
	float randomizer(float min_cps, float max_cps);
	void left_thread();
	void right_thread();
	void inventory_thread();
	void hook();
	float random_float(float min, float max);
	int random_int(int min, int max);

}
