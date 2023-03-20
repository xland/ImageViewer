#include "App.h"
#include "MainWindow.h"
#include "Tip.h"
#include <memory>
#include <map>
#include "resource.h"
#include "ImageViewer.h"
#include "BottomBar.h"
#include "NavigateBar.h"
#include "FileHelper.h"
#include "ImageDownloader.h"
#include "ToolTip.h"
namespace {
	static App* app{ nullptr };
	/// <summary>
	/// 0:chinese
	/// 1:english
	/// </summary>
	static unsigned languageIndex{ 0 };
	const static std::map<std::string, std::vector<std::wstring>> languageDic {
		{{"url"},{{L"打开URL路径"},{L"Open Image Url"}}},
		{{"loading"},{{L"加载中，请稍后"},{L"Loading, please wait"}}},
		{{"zoomIn"},{{L"放大"},{L"Zoom"}}},
		{{"zoomOut"},{{L"缩小"},{L"Shrink"}}},
		{{"autoSize"},{{L"自动大小"},{L"Automatic size"}}},
		{{"originSize"},{{L"原始大小"},{L"Original size"}}},
		{{"rotate"},{{L"旋转"},{L"Rotate"}}},
		{{"download"},{{L"下载"},{L"Download"}}},
		{{"isLastOne"},{{L"已是最后一张"},{L"This is the last one"}}},
		{{"isFirstOne"},{{L"已是第一张"},{L"This is the first one"}}},
		{{"copy"},{{L"复制"},{L"Copy"}}},
		{{"transfer"},{{L"转发"},{L"Transfer"}}},
		{{"revoke"},{{L"消息已被撤回"},{L"The message has been revoked"}}},
		{{"Recognize"},{{L"识别文字"},{L"Recognize text"}}},
		{{"previousOne"},{{L"上一张"},{L"Previous"}}},
		{{"nextOne"},{{L"下一张"},{L"Next"}}}
	};
}
App::App(HINSTANCE hinstance):
	hinstance{hinstance}
	, iconFont{nullptr}
{
	initIconFont();
	initTextFont();
	LANGID lid = GetSystemDefaultLangID();;
	if (lid == 0x0804)
	{
		languageIndex = 0;
	}
	else  //0x0404 繁体中文 ，0x0409美国英语
	{
		languageIndex = 1;
	}
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
std::wstring App::getText(std::string&& key)
{
	auto _key = key;
	return languageDic.at(_key)[languageIndex];
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
	RegToolTip(L"Allen", { 0,0,100,100 });
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