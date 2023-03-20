#pragma once
#include <string>
#include <Windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <map>
class ToolTip
{
public:
	ToolTip();
	~ToolTip();
	void RegToolTip(const char* key, RECT rect, unsigned id);
private:
	HWND hwnd;
	std::map<std::string, TOOLINFO> tooltipInfos;
};

