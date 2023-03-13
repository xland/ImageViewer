#include "NavigateBar.h"
#include "MainWindow.h"
#include "Color.h"
#include "IconFont.h"
NavigateBar::NavigateBar(MainWindow* win) :win{ win }
{

}
NavigateBar::~NavigateBar()
{

}
void NavigateBar::CheckMouseEnter(int x, int y)
{
	if (x > 0 && x < w && y>0 && y < win->clientHeight - win->bottomBarHeight) 
	{
		mouseEnterLeft = true;
	}
	else
	{
		mouseEnterLeft = false;
	}
	if (x > win->clientWidth-w && x < win->clientWidth && y>0 && y < win->clientHeight - win->bottomBarHeight) 
	{
		mouseEnterRight = true;
	}
	else
	{
		mouseEnterRight = false;
	}
}
void NavigateBar::Paint(SkCanvas* canvas)
{	
	if (!mouseEnterLeft && !mouseEnterRight) return;

	auto font = IconFont::Get();
	font->setSize(26);
	y = ((float)win->clientHeight - (float)win->bottomBarHeight) / 2;
	SkPaint paint;
	paint.setColor(GetColor(0, 0, 0, 80));	
	if (mouseEnterLeft) {
		canvas->drawCircle({ x1,y }, r, paint);
		paint.setColor(ColorWhite);
		canvas->drawString((const char*)u8"\ue60e", x1 - 13, y + 10, *font, paint);
	}
	else
	{
		x2 = (float)win->clientWidth - w + r;
		canvas->drawCircle({ x2,y }, r, paint);
		paint.setColor(ColorWhite);
		canvas->drawString((const char*)u8"\ue638", x2 - 10, y + 10, *font, paint);
	}	
}