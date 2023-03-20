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
		{{"url"},{{L"��URL·��"},{L"Open Image Url"}}},
		{{"loading"},{{L"�����У����Ժ�"},{L"Loading, please wait"}}},
		{{"zoomIn"},{{L"�Ŵ�"},{L"Zoom"}}},
		{{"zoomOut"},{{L"��С"},{L"Shrink"}}},
		{{"autoSize"},{{L"�Զ���С"},{L"Automatic size"}}},
		{{"originSize"},{{L"ԭʼ��С"},{L"Original size"}}},
		{{"rotate"},{{L"��ת"},{L"Rotate"}}},
		{{"download"},{{L"����"},{L"Download"}}},
		{{"isLastOne"},{{L"�������һ��"},{L"This is the last one"}}},
		{{"isFirstOne"},{{L"���ǵ�һ��"},{L"This is the first one"}}},
		{{"copy"},{{L"����"},{L"Copy"}}},
		{{"transfer"},{{L"ת��"},{L"Transfer"}}},
		{{"revoke"},{{L"��Ϣ�ѱ�����"},{L"The message has been revoked"}}},
		{{"Recognize"},{{L"ʶ������"},{L"Recognize text"}}},
		{{"previousOne"},{{L"��һ��"},{L"Previous"}}},
		{{"nextOne"},{{L"��һ��"},{L"Next"}}}
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
	else  //0x0404 �������� ��0x0409����Ӣ��
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