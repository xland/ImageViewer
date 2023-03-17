#include "App.h"
#include "MainWindow.h"
#include "Tip.h"
#include <memory>
#include "resource.h"
#include "ImageViewer.h"
#include "BottomBar.h"
#include "NavigateBar.h"
#include "FileHelper.h"
#include "ImageDownloader.h"
namespace {
	static App* app{ nullptr };
}
App::App(HINSTANCE hinstance):
	hinstance{hinstance}
	, iconFont{nullptr}
{
	initIconFont();
	initTextFont();
}
App::~App()
{
	delete iconFont;
}
void App::initIconFont() {
	HRSRC resID = FindResource(hinstance, MAKEINTRESOURCE(IDR_BTNFONT1), L"BTNFONT");
	if (resID == 0) {
		return;
	}
	size_t resSize = SizeofResource(hinstance, resID);
	HGLOBAL res = LoadResource(hinstance, resID);
	if (res == 0) {
		return;
	}
	void* resPointer = LockResource(res);
	auto fontData = SkData::MakeWithoutCopy(resPointer, resSize);
	auto fontFace = SkTypeface::MakeFromData(fontData);
	iconFont = new SkFont(fontFace);
}
void App::initTextFont() {
	auto fontFace = SkTypeface::MakeFromName("Microsoft YaHei", SkFontStyle::Normal());
	textFont = new SkFont(fontFace);
}
void App::init(HINSTANCE hinstance) {
	app = new App(hinstance);
	app->mainWindow = std::make_unique<MainWindow>();
	app->initIconFont();
	app->tip = std::make_unique<Tip>();
	app->bottomBar = std::make_unique<BottomBar>();
	app->navigateBar = std::make_unique<NavigateBar>();
	app->fileHelper = std::make_unique<FileHelper>();
	app->imageDownloader = std::make_unique<ImageDownloader>();
	app->mainWindow->Show();
}
App* App::get() {
	return app;
}
void App::dispose()
{
	delete app;
}
void App::Paint(SkCanvas* canvas)
{
	if (imageViewer) {
		imageViewer->Paint(canvas);
	}
	navigateBar->Paint(canvas);
	bottomBar->Paint(canvas);
	tip->Paint(canvas);
}
void App::CheckMouseEnter(int x, int y)
{
	bottomBar->CheckMouseEnter(x, y);
	navigateBar->CheckMouseEnter(x, y);
	if (imageViewer) {
		imageViewer->CheckMouseEnter(x, y);
	}	
}
void App::CheckMouseUp(int x, int y)
{
	if (imageViewer) {
		imageViewer->CheckMouseUp(x, y);
	}
}
void App::CheckMouseDown(int x, int y)
{
	bottomBar->CheckMouseDown(x, y);
	navigateBar->CheckMouseDown(x, y);
	if (imageViewer) {
		imageViewer->CheckMouseDown(x, y);
	}
}