#include "NavigateBar.h"
#include "MainWindow.h"
#include "Color.h"
#include "App.h"
#include "include/core/SkFont.h"
#include "BottomBar.h"
#include "FileHelper.h"
#include "ToolTip.h"
NavigateBar::NavigateBar()
{
}
NavigateBar::~NavigateBar()
{

}
void NavigateBar::Resize(const unsigned& w, const unsigned& h)
{
	auto win = App::get()->mainWindow.get();
	auto y = ((float)win->clientHeight - (float)win->bottomBarHeight) / 2;
	leftCircleCenter.set(regionW - r, y);
	rightCircleCenter.set(w - regionW + r, y);
	leftButtonRect.setXYWH(regionW - 2 * r, y - r, 2*r, 2*r);
	rightButtonRect.setXYWH(w - regionW,y-r, 2 * r, 2 * r);
	RECT r2{ (long)rightButtonRect.x(), (long)rightButtonRect.y(), (long)rightButtonRect.right(), (long)rightButtonRect.bottom() };
	App::get()->tooltip->RegToolTip("none", r2, 202);
	RECT r1{ (long)leftButtonRect.x(), (long)leftButtonRect.y(), (long)leftButtonRect.right(), (long)leftButtonRect.bottom() };
	App::get()->tooltip->RegToolTip("none", r1, 201);
}
bool NavigateBar::isInsideCircle(SkPoint& circleCenter, int x, int y) {
	int dist = (x - circleCenter.x()) * (x - circleCenter.x()) + (y - circleCenter.y()) * (y - circleCenter.y());
	if (dist <= r * r)
		return true;
	else
		return false;
}
void NavigateBar::CheckMouseDown(int mouseX, int mouseY)
{
	if (mouseEnterLeft) {
		if (isInsideCircle(leftCircleCenter, mouseX, mouseY)) {
			App::get()->fileHelper->ShowPrev();
		}		
	}
	if (mouseEnterRight) {
		if (isInsideCircle(rightCircleCenter, mouseX, mouseY)) {
			App::get()->fileHelper->ShowNext();
		}
	}
}
void NavigateBar::CheckMouseEnter(int x, int y)
{
	if (!App::get()->imageViewer) return;
	auto win = App::get()->mainWindow.get();
	if (x > 0 && x < regionW && y>0 && y < win->clientHeight - win->bottomBarHeight)
	{
		if (!mouseEnterLeft) 
		{
			mouseEnterLeft = true;
			App::get()->tooltip->SetToolTipText(201, "previousOne");
			App::get()->mainWindow->Refresh();
		}
	}
	else
	{
		if (mouseEnterLeft) 
		{
			mouseEnterLeft = false;
			App::get()->tooltip->SetToolTipText(201, "none");
			App::get()->mainWindow->Refresh();
		}		
	}
	if (x > win->clientWidth- regionW && x < win->clientWidth && y>0 && y < win->clientHeight - win->bottomBarHeight)
	{
		if (!mouseEnterRight) {
			mouseEnterRight = true;
			App::get()->tooltip->SetToolTipText(202, "nextOne");
			App::get()->mainWindow->Refresh();
		}		
	}
	else
	{
		if (mouseEnterRight) {
			mouseEnterRight = false;
			App::get()->tooltip->SetToolTipText(202, "none");
			App::get()->mainWindow->Refresh();
		}		
	}
}
void NavigateBar::Paint(SkCanvas* canvas)
{	
	if (!mouseEnterLeft && !mouseEnterRight) return;
	if (!App::get()->imageViewer) return;
	auto win = App::get()->mainWindow.get();
	auto font = App::get()->iconFont;
	font->setSize(26);
	SkPaint paint;
	paint.setColor(GetColor(0, 0, 0, 80));	
	if (mouseEnterLeft) {
		canvas->drawCircle(leftCircleCenter, r, paint);
		paint.setColor(ColorWhite);
		canvas->drawString((const char*)u8"\ue60e", leftCircleCenter.x() - 13, leftCircleCenter.y() + 10, *font, paint);
		paint.setColor(GetColor(255, 255, 255, 80));
		paint.setStroke(true);
		paint.setStrokeWidth(2.f);
		canvas->drawCircle(leftCircleCenter, r, paint);
	}
	else
	{
		canvas->drawCircle(rightCircleCenter, r, paint);
		paint.setColor(ColorWhite);
		canvas->drawString((const char*)u8"\ue638", rightCircleCenter.x() - 10, rightCircleCenter.y() + 10, *font, paint);
		paint.setColor(GetColor(255, 255, 255, 80));
		paint.setStroke(true);
		paint.setStrokeWidth(2.f);
		canvas->drawCircle(rightCircleCenter, r, paint);
	}	
}