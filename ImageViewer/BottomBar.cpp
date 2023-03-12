#include "BottomBar.h"
#include "MainWindow.h"
#include "Color.h"
#include "include/core/SkFont.h"
#include "resource.h"
#include <math.h>
#include "IconFont.h"
BottomBar::BottomBar(MainWindow* win):win{win}
{
	
}
BottomBar::~BottomBar()
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
	auto font = IconFont::Get();
	font->setSize(fontSize);
	for (size_t i = 0; i < btnCodes.size(); i++)
	{
		if (i == mouseEnterIndex) {
			paint.setColor(GetColor(230,230,230));
			SkRect rect;
			rect.setXYWH(tempX-fontSize, y, btnWidth, (float)win->bottomBarHeight);
			canvas->drawRect(rect, paint);
			paint.setColor(ColorBlack);
		}
		canvas->drawString(btnCodes[i], tempX, tempY, *font, paint);
		tempX += btnWidth;
	}
}