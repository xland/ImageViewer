#pragma once
#include "include/core/SkCanvas.h"
class MainWindow;
class NavigateBar
{
public:
	NavigateBar();
	~NavigateBar();
	void Paint(SkCanvas* canvas);
	void CheckMouseEnter(int mouseX, int mouseY);
	void CheckMouseDown(int mouseX, int mouseY);
	void Resize(const unsigned& w, const unsigned& h);
private:
	bool isInsideCircle(SkPoint& circleCenter, int x, int y);
private:
	float r = 35.f;
	float regionW = 175.f;
	SkPoint leftCircleCenter;
	SkPoint rightCircleCenter;
	SkRect leftButtonRect;
	SkRect rightButtonRect;
	bool mouseEnterLeft{ false };
	bool mouseEnterRight{ false };
};

