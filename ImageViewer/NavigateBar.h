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
	void CheckMouseUp(int mouseX, int mouseY);
	MainWindow* win{ nullptr };
private:
	float r = 35.f;
	float w = 175.f;
	float x1 = w - r;
	float x2 = 0.f;
	float y = 0.f;
	bool mouseEnterLeft{ false };
	bool mouseEnterRight{ false };
};

