#include "Tip.h"
#include <Windows.h>
#include "App.h"
#include "MainWindow.h"
#include "include/core/SkMaskFilter.h"
#include "Color.h"

Tip::Tip()
{
	//todo more tip
}
Tip::~Tip()
{
	if (waitingTread.joinable()) {
		waitingTread.join();
	}
}
void Tip::waitingFunc()
{
	while (remainTime > 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		remainTime -= 20;
	}
	this->tipText = L"";
	App::get()->mainWindow->Refresh();
}
void Tip::Show(std::wstring&& tipText)
{
	if (this->tipText == tipText)
	{
		remainTime = 2000;
		return;
	}
	this->tipText = tipText;
	textLength = wcslen(tipText.data()) * 2;
	auto font = App::get()->textFont;
	font->setSize(20.f);
	SkRect rectText;
	font->measureText(tipText.data(), textLength, SkTextEncoding::kUTF16, &rectText);
	textX = (300 - rectText.width()) / 2 + rectText.x();
	textY = (60 - rectText.height()) / 2 - rectText.y();
	if (remainTime > 0) {
		remainTime = 2000;
	}
	else
	{
		remainTime = 2000;
		if (waitingTread.joinable()) {
			waitingTread.join();
		}
		waitingTread = std::thread(&Tip::waitingFunc, this);		
	}
	App::get()->mainWindow->Refresh();
}
void Tip::Paint(SkCanvas* canvas)
{
	if (remainTime <= 0) return;
	auto win = App::get()->mainWindow.get();
	auto x = ((float)win->clientWidth - 300.f) / 2;
	auto y = ((float)win->clientHeight - (float)win->bottomBarHeight - 60.f) / 2;
	SkRect rect;
	rect.setXYWH(x, y, 300.f, 60.f);
	SkPaint paint;
	paint.setColor(ColorBlack);
	{
		SkPaint blur;
		blur.setColor(ColorWhite);
		blur.setAlpha(127);
		blur.setMaskFilter(SkMaskFilter::MakeBlur(kNormal_SkBlurStyle, 3.4f, false));
		canvas->drawRoundRect(rect, 6.f, 6.f, blur);
	}
	canvas->drawRoundRect(rect, 6.f, 6.f, paint);
	paint.setColor(ColorWhite);
	auto font = App::get()->textFont;
	font->setSize(20.f);
	paint.setAntiAlias(true);
	canvas->drawSimpleText(tipText.data(), textLength, SkTextEncoding::kUTF16, x+textX, y+textY, *font, paint);
}