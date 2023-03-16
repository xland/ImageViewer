#include "App.h"
#include "MainWindow.h"
#include "Tip.h"
#include <memory>
#include "resource.h"
namespace {
	static App* app{ nullptr };
}
App::App(HINSTANCE hinstance):
	hinstance{hinstance}
{

}
App::~App()
{

}
void App::initIconFont() {
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
	auto fontData = SkData::MakeWithoutCopy(resPointer, resSize);
	auto fontFace = SkTypeface::MakeFromData(fontData);
	iconFont = new SkFont(fontFace);
}
void App::init(HINSTANCE hinstance) {
	app = new App(hinstance);
	app->mainWindow = std::make_unique<MainWindow>();
	app->initIconFont();
	app->tip = std::make_unique<Tip>();
	app->bottomBar = std::make_unique<BottomBar>();
	app->navigateBar = std::make_unique<NavigateBar>();
	app->mainWindow->Show();
}
App* App::get() {
	return app;
}