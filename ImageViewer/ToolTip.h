#pragma once
#include <string>
#include <Windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <vector>
#include <map>
class ToolTip
{
public:
	ToolTip();
	~ToolTip();
	void RegToolTip(const char* key, RECT rect, unsigned id);
	void SetToolTipText(unsigned id, const char* key);
private:
	HWND hwnd;
	std::map<unsigned,std::pair<std::string, TOOLINFO>> tooltipInfos;
};

