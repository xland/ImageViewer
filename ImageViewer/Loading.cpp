#include "Loading.h"
#include "App.h"
#include "MainWindow.h"
#include "Color.h"
Loading::Loading()
{

}
Loading::~Loading()
{
    running = false;
    if (rotateThread.joinable()) {
        rotateThread.join();
    }
}
void Loading::Show()
{
    if (running) return;
    running = true;
    App::get()->mainWindow->Refresh();
    rotateThread = std::thread([this]() {
        while (running)
        {
            roateAngle += 6.0f;
            if (roateAngle > 360) {
                roateAngle -= 360;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(18));
            App::get()->mainWindow->Refresh();
        }
    });
}
void Loading::Hide()
{
    if (!running) return;
    running = false;
    App::get()->mainWindow->Refresh();
    if (rotateThread.joinable()) {
        rotateThread.join();
    }
}
void Loading::Paint(SkCanvas* canvas)
{
    if (!running) return;
	auto win = App::get()->mainWindow.get();
	auto font = App::get()->iconFont;
	font->setSize(42);
	SkPaint paint;
	paint.setColor(GetColor(100,100,100));
    canvas->save(); 
    auto x = (float)win->clientWidth / 2;
    auto y = ((float)win->clientHeight-(float)win->bottomBarHeight) / 2;
    canvas->rotate(roateAngle,x,y);
    canvas->drawString((const char*)u8"\ue891", x-20, y+16, *font, paint);
    canvas->restore();
}