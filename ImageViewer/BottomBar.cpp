#include "BottomBar.h"
#include "MainWindow.h"
#include "Color.h"
#include "include/core/SkFont.h"
#include "include/core/SkMaskFilter.h"
#include "resource.h"
#include "App.h"
#include <math.h>
#include "Converter.h"
#include "Tip.h"
#include "ImageViewer.h"
#include "FileHelper.h"
#include "ImageDownloader.h"
#include "ToolTip.h"

BottomBar::BottomBar()
{
}
BottomBar::~BottomBar()
{
}
void BottomBar::Resize(const unsigned& w, const unsigned& h)
{
	
	auto win = App::get()->mainWindow.get();
	float btnWidth{ 68.f };
	auto x = (float)(w - btns.size() * btnWidth) / 2;
	auto y = (float)(h - win->bottomBarHeight);
	for (size_t i = 0; i < btns.size(); i++) {
		auto& rect = std::get<2>(btns[i]);
		auto& key = std::get<0>(btns[i]);
		rect.setXYWH(x, y, btnWidth, win->bottomBarHeight);
		RECT r{ (long)x, (long)y, (long)rect.right(), (long)rect.bottom() };
		App::get()->tooltip->RegToolTip(key, r);
		x += btnWidth;
	}
}
void BottomBar::CheckMouseDown(int mouseX, int mouseY)
{
	if (mouseEnterIndex == -1) return;
	else if (mouseEnterIndex == 0) {
		App::get()->imageDownloader->ShowUrlDialog();
	}
	else if (mouseEnterIndex == 1) {
		App::get()->fileHelper->Open();
	}
	else if (mouseEnterIndex == 2) {
		App::get()->fileHelper->ShowPrev();
	}
	else if (mouseEnterIndex == 3) {
		App::get()->fileHelper->ShowNext();
	}
	else if (mouseEnterIndex == 4) {
		App::get()->imageViewer->Zoom(0.98f);
	}
	else if (mouseEnterIndex == 5) {
		App::get()->imageViewer->Zoom(1.02f);
	}
	else if (mouseEnterIndex == 6) {
		auto& code = std::get<1>(btns[6]);
		if (code == (const char*)u8"\ue6f8") 
		{
			code = (const char*)u8"\ue6be";
			App::get()->imageViewer->AutoSize();
		}
		else
		{
			App::get()->imageViewer->Zoom(1.f);
		}
	}
	else if (mouseEnterIndex == 7) {
		auto& code = std::get<1>(btns[6]);
		code = (const char*)u8"\ue6be";
		App::get()->imageViewer->Rotate();
	}
	else if (mouseEnterIndex == 8) {
		App::get()->fileHelper->Save();;
	}
}
void BottomBar::CheckMouseEnter(int x, int y)
{
	int index = -1;
	for (size_t i = 0; i < btns.size(); i++)
	{
		auto& rect = std::get<2>(btns[i]);
		if (x > rect.x() && y > rect.y() && x < rect.right() && y < rect.bottom()) {
			index = i;
			break;
		}
	}
	if (index != mouseEnterIndex) {
		OutputDebugStringA("refresh");
		mouseEnterIndex = index;
		App::get()->mainWindow->Refresh(); //hover
	}
}
void BottomBar::Paint(SkCanvas* canvas)
{
	auto win = App::get()->mainWindow.get();
	SkPaint paint;
	{
		paint.setColor(ColorWhite);
		SkRect rect;
		rect.setXYWH(0, std::get<2>(btns[0]).y(), (float)win->clientWidth, (float)win->bottomBarHeight);
		canvas->drawRect(rect, paint);
	}	
	paint.setColor(ColorBlack);
	paint.setAntiAlias(true);
	auto font = App::get()->iconFont;
	font->setSize(fontSize);
	for (size_t i = 0; i < btns.size(); i++)
	{
		auto& rect = std::get<2>(btns[i]);
		if (i == mouseEnterIndex) {
			paint.setColor(GetColor(230,230,230));
			canvas->drawRect(rect, paint);
			paint.setColor(ColorBlack);
		}
		auto x = rect.x() + fontSize;
		auto y = rect.y() + fontSize / 2 + win->bottomBarHeight / 2;
		canvas->drawString(std::get<1>(btns[i]), x, y, *font, paint);
	}
}