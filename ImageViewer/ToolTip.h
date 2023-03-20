#pragma once
#include <string>
#include <Windows.h>
#include <windowsx.h>
#include <commctrl.h>
class ToolTip
{
public:
	ToolTip();
	~ToolTip();
	void RegToolTip(std::string&& key, RECT rect);
private:
	HWND hwnd;
};

