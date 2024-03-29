#pragma once
#include <Windows.h>
#include "include/core/SkFont.h"
#include "include/core/SkCanvas.h"
#include <memory>
#include <string>

class MainWindow;
class Tip;
class ImageViewer;
class BottomBar;
class NavigateBar;
class Tip;
class FileHelper;
class ImageDownloader;
class ToolTip;
class Loading;
class App
{
public:	
	~App();
	void Paint(SkCanvas* canvas);
	void CheckMouseEnter(int x, int y);
	void Resize(const unsigned& w, const unsigned& h);
	void CheckMouseDown(int x, int y);
	void CheckMouseUp(int x, int y);
	static void init(HINSTANCE hinstance);
	static App* get();
	static std::wstring getText(const char* key);
	static void dispose();
public:
	std::unique_ptr<MainWindow> mainWindow;
	std::unique_ptr<Tip> tip;
	std::shared_ptr<ImageViewer> imageViewer {nullptr};
	std::unique_ptr<BottomBar> bottomBar;
	std::unique_ptr<NavigateBar> navigateBar;
	std::unique_ptr<FileHelper> fileHelper;
	std::unique_ptr<ImageDownloader> imageDownloader;
	std::unique_ptr<ToolTip> tooltip;
	std::unique_ptr<Loading> loading;
	SkFont* iconFont;
	SkFont* textFont;
	HINSTANCE hinstance;
private:
	App(HINSTANCE hinstance);
	void initIconFont();
	void initTextFont();
private:
};

