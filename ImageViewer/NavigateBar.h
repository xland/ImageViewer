#pragma once
#include "include/core/SkCanvas.h"
class MainWindow;
class NavigateBar
{
public:
	NavigateBar(MainWindow* win);
	~NavigateBar();
	void Paint(SkCanvas* canvas);
	void CheckMouseEnter(int mouseX, int mouseY);
	MainWindow* win{ nullptr };
};

