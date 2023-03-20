#pragma once
#include "include/core/SkCanvas.h"
#include <tuple>
class MainWindow;
class BottomBar
{
public:
	BottomBar();
	~BottomBar();
	void Paint(SkCanvas* canvas);
	void CheckMouseEnter(int mouseX, int mouseY);
	void CheckMouseDown(int mouseX, int mouseY);
	void Resize(const unsigned& w, const unsigned& h);
	std::vector<std::tuple<const char*,const char*,SkRect>> btns {
		{"url",(const char*)u8"\ue610",SkRect::MakeEmpty()},
		{"folder",(const char*)u8"\ueabe",SkRect::MakeEmpty()},
		{"previousOne",(const char*)u8"\ue6bf",SkRect::MakeEmpty()},
		{"nextOne",(const char*)u8"\ue6ad",SkRect::MakeEmpty()},
		{"zoomOut",(const char*)u8"\ue6bd",SkRect::MakeEmpty()},
		{"zoomIn",(const char*)u8"\ue6c0",SkRect::MakeEmpty()},
		{"originSize",(const char*)u8"\ue6be",SkRect::MakeEmpty()},
		{"rotate",(const char*)u8"\ue6c2",SkRect::MakeEmpty()},
		{"download",(const char*)u8"\ue6c1",SkRect::MakeEmpty()}
	};
private:
	unsigned fontSize{ 22 };
	int mouseEnterIndex{ -1 };
};

