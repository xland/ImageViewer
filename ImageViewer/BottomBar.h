#pragma once
#include "include/core/SkCanvas.h"
#include <variant>
class MainWindow;
class BottomBar
{
public:
	BottomBar();
	~BottomBar();
	void Paint(SkCanvas* canvas);
	void CheckMouseEnter(int mouseX, int mouseY);
	void CheckMouseDown(int mouseX, int mouseY);
	std::vector<const char*> btnCodes{ (const char*)u8"\ue610" 
		,(const char*)u8"\ueabe"
		,(const char*)u8"\ue6bf"
		,(const char*)u8"\ue6ad"
		,(const char*)u8"\ue6bd"
		,(const char*)u8"\ue6c0"
		,(const char*)u8"\ue6be"
		,(const char*)u8"\ue6c2"
		,(const char*)u8"\ue6c1" };
private:
	unsigned fontSize{ 22 };
	std::vector<SkRect> btnRects;
	int mouseEnterIndex{ -1 };
};

