#pragma once
#include "include/core/SkCanvas.h"
class MainWindow;
class BottomBar
{
public:
	BottomBar(MainWindow* win);
	~BottomBar();
	void Paint(SkCanvas* canvas);
	void CheckMouseEnter(int mouseX, int mouseY);
	MainWindow* win{ nullptr };
private:
	void paintBtn(const char* btn,float& x,float& y, SkPaint& paint);
	std::unique_ptr<SkFont> font;
	unsigned fontSize{ 22 };
	float btnWidth{ 68.f };
	float x{ 0.f };
	float y{ 0.f };
	float w{ 7*btnWidth };
	int mouseEnterIndex{ -1 };
};

