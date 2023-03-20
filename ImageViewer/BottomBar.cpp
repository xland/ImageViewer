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

BottomBar::BottomBar()
{
	auto win = App::get()->mainWindow.get();
	float btnWidth{ 68.f };
	auto x = (float)(win->clientWidth - btnCodes.size()*btnWidth) / 2;
	auto y = (float)(win->clientHeight - win->bottomBarHeight);
	for (size_t i = 0; i < btnCodes.size(); i++) {
		SkRect rect;
		rect.setXYWH(x, y, btnWidth, win->bottomBarHeight);
		btnRects.push_back(rect);
		x += btnWidth;
	}
}
BottomBar::~BottomBar()
{
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
		if (btnCodes[6] == (const char*)u8"\ue6f8") 
		{
			btnCodes[6] = (const char*)u8"\ue6be";
			App::get()->imageViewer->AutoSize();
		}
		else
		{
			App::get()->imageViewer->Zoom(1.f);
		}
		
	}
	else if (mouseEnterIndex == 7) {
		btnCodes[6] = (const char*)u8"\ue6be";
		App::get()->imageViewer->Rotate();
	}
	else if (mouseEnterIndex == 8) {
		App::get()->fileHelper->Save();;
	}
}
void BottomBar::CheckMouseEnter(int x, int y)
{
	int index = -2;
	for (size_t i = 0; i < btnRects.size(); i++)
	{
		if (x > btnRects[i].x() && y > btnRects[i].y() && x < btnRects[i].right() && y < btnRects[i].bottom()) {
			index = i;
			break;
		}
	}
	index = -1;
	if (index != mouseEnterIndex) {
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
		rect.setXYWH(0, btnRects[0].y(), (float)win->clientWidth, (float)win->bottomBarHeight);
		canvas->drawRect(rect, paint);
	}	
	paint.setColor(ColorBlack);
	paint.setAntiAlias(true);
	auto font = App::get()->iconFont;
	font->setSize(fontSize);
	for (size_t i = 0; i < btnCodes.size(); i++)
	{
		if (i == mouseEnterIndex) {
			paint.setColor(GetColor(230,230,230));
			canvas->drawRect(btnRects[i], paint);
			paint.setColor(ColorBlack);
		}
		auto x = btnRects[i].x() + fontSize;
		auto y = btnRects[i].y() + fontSize / 2 + win->bottomBarHeight / 2;
		canvas->drawString(btnCodes[i], x, y, *font, paint);
	}
}