#include "NavigateBar.h"
#include "MainWindow.h"
#include "Color.h"
#include "App.h"
#include "include/core/SkFont.h"
#include "BottomBar.h"
NavigateBar::NavigateBar()
{

}
NavigateBar::~NavigateBar()
{

}
void NavigateBar::CheckMouseDown(int x, int y)
{
	if (mouseEnterLeft) {
		App::get()->bottomBar->loopFile(false);
	}
	if (mouseEnterRight) {
		App::get()->bottomBar->loopFile(true);
	}
}
void NavigateBar::CheckMouseEnter(int x, int y)
{
	auto win = App::get()->mainWindow.get();
	if (x > 0 && x < w && y>0 && y < win->clientHeight - win->bottomBarHeight) 
	{
		if (!mouseEnterLeft) 
		{
			mouseEnterLeft = true;
			App::get()->mainWindow->Refresh();
		}
	}
	else
	{
		if (mouseEnterLeft) 
		{
			mouseEnterLeft = false;
			App::get()->mainWindow->Refresh();
		}		
	}
	if (x > win->clientWidth-w && x < win->clientWidth && y>0 && y < win->clientHeight - win->bottomBarHeight) 
	{
		if (!mouseEnterRight) {
			mouseEnterRight = true;
			App::get()->mainWindow->Refresh();
		}		
	}
	else
	{
		if (mouseEnterRight) {
			mouseEnterRight = false;
			App::get()->mainWindow->Refresh();
		}		
	}
}
void NavigateBar::Paint(SkCanvas* canvas)
{	
	//todo paint border
	if (!mouseEnterLeft && !mouseEnterRight) return;
	if (!App::get()->imageViewer) return;
	auto win = App::get()->mainWindow.get();
	auto font = App::get()->iconFont;
	font->setSize(26);
	y = ((float)win->clientHeight - (float)win->bottomBarHeight) / 2;
	SkPaint paint;
	paint.setColor(GetColor(0, 0, 0, 80));	
	if (mouseEnterLeft) {
		canvas->drawCircle({ x1,y }, r, paint);
		paint.setColor(ColorWhite);
		canvas->drawString((const char*)u8"\ue60e", x1 - 13, y + 10, *font, paint);
		paint.setColor(GetColor(255, 255, 255, 80));
		paint.setStroke(true);
		paint.setStrokeWidth(2.f);
		canvas->drawCircle({ x1,y }, r, paint);
	}
	else
	{
		x2 = (float)win->clientWidth - w + r;
		canvas->drawCircle({ x2,y }, r, paint);
		paint.setColor(ColorWhite);
		canvas->drawString((const char*)u8"\ue638", x2 - 10, y + 10, *font, paint);
		paint.setColor(GetColor(255, 255, 255, 80));
		paint.setStroke(true);
		paint.setStrokeWidth(2.f);
		canvas->drawCircle({ x2,y }, r, paint);
	}	
}