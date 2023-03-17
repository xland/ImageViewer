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

BottomBar::BottomBar()
{
	//todo cursor style
}
BottomBar::~BottomBar()
{
}
void BottomBar::CheckMouseDown(int mouseX, int mouseY)
{

	if (mouseEnterIndex == -1) return;
	else if (mouseEnterIndex == 0) {
		App::get()->fileHelper->Open();
	}
	else if (mouseEnterIndex == 1) {
		App::get()->fileHelper->ShowPrev();
	}
	else if (mouseEnterIndex == 2) {
		App::get()->fileHelper->ShowNext();
	}
	else if (mouseEnterIndex == 3) {
		App::get()->imageViewer->Zoom(0.98f);
	}
	else if (mouseEnterIndex == 4) {
		App::get()->imageViewer->Zoom(1.02f);
	}
	else if (mouseEnterIndex == 5) {
		if (btnCodes[5] == (const char*)u8"\ue6f8") 
		{
			btnCodes[5] = (const char*)u8"\ue6be";
			App::get()->imageViewer->AutoSize();
		}
		else
		{
			App::get()->imageViewer->Zoom(1.f);
		}
		
	}
	else if (mouseEnterIndex == 6) {
		btnCodes[5] = (const char*)u8"\ue6be";
		App::get()->imageViewer->Rotate();
	}
	else if (mouseEnterIndex == 7) {
		App::get()->fileHelper->Save();;
	}
}
void BottomBar::CheckMouseEnter(int mouseX, int mouseY)
{
	int index = -2;
	auto win = App::get()->mainWindow.get();
	auto x = (float)(win->clientWidth - w) / 2;
	auto y = (float)(win->clientHeight - win->bottomBarHeight);
	if (mouseX > x && mouseY > y && mouseX < x + w && mouseY < win->clientHeight) {
		index = (unsigned)std::floor((mouseX - x) / btnWidth);
	}
	else {
		index = -1;
	}
	if (index != mouseEnterIndex) {
		mouseEnterIndex = index;
		App::get()->mainWindow->Refresh();
	}
}
void BottomBar::Paint(SkCanvas* canvas)
{
	auto win = App::get()->mainWindow.get();
	auto x = (float)(win->clientWidth - w) / 2;
	auto y = (float)(win->clientHeight - win->bottomBarHeight);
	SkPaint paint;
	{
		//»­µ×²¿À¸µÄ°×É«±³¾°
		paint.setColor(ColorWhite);
		SkRect rect;
		rect.setXYWH(0, y, (float)win->clientWidth, (float)win->bottomBarHeight);
		canvas->drawRect(rect, paint);
	}	
	float tempX = x + fontSize;
	float tempY = y + fontSize / 2 + win->bottomBarHeight / 2;
	paint.setColor(ColorBlack);
	paint.setAntiAlias(true);
	auto font = App::get()->iconFont;
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