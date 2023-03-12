#include "BottomBar.h"
#include "MainWindow.h"
#include "Color.h"
#include "include/core/SkFont.h"
#include "resource.h"
#include <math.h>
BottomBar::BottomBar(MainWindow* win):win{win}
{
	HMODULE instance = GetModuleHandle(NULL);
	HRSRC resID = FindResource(instance, MAKEINTRESOURCE(IDR_BTNFONT1), L"BTNFONT");
	if (resID == 0) {
		return;
	}
	size_t resSize = SizeofResource(instance, resID);
	HGLOBAL res = LoadResource(instance, resID);
	if (res == 0) {
		return;
	}
	void* resPointer = LockResource(res);
	auto fontData = SkData::MakeWithoutCopy(resPointer,resSize);
	auto fontTypeFace = SkTypeface::MakeFromData(fontData);
	font = std::make_unique<SkFont>(fontTypeFace, fontSize);
}
BottomBar::~BottomBar()
{

}
void BottomBar::paintBtn(const char* btn, float& x, float& y, SkPaint& paint)
{

}
void BottomBar::CheckMouseEnter(int mouseX, int mouseY)
{
	int index = -2;
	if (mouseX > x && mouseY > y && mouseX < x + w && mouseY < win->clientHeight) {
		index = (unsigned)std::floor((mouseX - x) / btnWidth);
	}
	else {
		index = -1;
	}
	if (index != mouseEnterIndex) {
		mouseEnterIndex = index;
		InvalidateRect(win->hwnd, nullptr, false);
	}
}
void BottomBar::Paint(SkCanvas* canvas)
{
	SkPaint paint;
	paint.setColor(ColorWhite);
	x = (float)(win->clientWidth - w) / 2;
	y = (float)(win->clientHeight - win->bottomBarHeight);
	{
		SkRect rect;
		rect.setXYWH(0, y, (float)win->clientWidth, (float)win->bottomBarHeight);
		canvas->drawRect(rect, paint);
	}

	auto tempY = y+fontSize/2+ win->bottomBarHeight /2;
	paint.setColor(ColorBlack);
	paint.setAntiAlias(true);
	float tempX = x + fontSize;
	canvas->drawString((const char*)u8"\ue6bf", tempX, tempY, *font.get(), paint);
	tempX = x+ btnWidth;
	canvas->drawString((const char*)u8"\ue6ad", tempX, tempY, *font.get(), paint);
	tempX += btnWidth;
	canvas->drawString((const char*)u8"\ue6bd", tempX, tempY, *font.get(), paint);
	tempX += btnWidth;
	canvas->drawString((const char*)u8"\ue6c0", tempX, tempY, *font.get(), paint);
	tempX += btnWidth;
	canvas->drawString((const char*)u8"\ue6be", tempX, tempY, *font.get(), paint);
	tempX += btnWidth;
	canvas->drawString((const char*)u8"\ue6c2", tempX, tempY, *font.get(), paint);
	tempX += btnWidth;
	canvas->drawString((const char*)u8"\ue6c1", tempX, tempY, *font.get(), paint);
}