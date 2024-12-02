#pragma once
#include "Windows.h"
#include <dinput.h>
#include <wrl.h>
#include <cstdint>

class WinApp {
public:
	void Initialize();

	void Finalize();

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	HWND GetHwnd() const { return hwnd; }

	HINSTANCE GetWCInStance() const { return wc.hInstance; }

	bool ProcessMessage();

public:

	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;


private:

	HWND hwnd = nullptr;

	WNDCLASS wc{};

};