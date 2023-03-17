#pragma once
#include <Windows.h>
#include "include/core/SkFont.h"
#include "include/core/SkCanvas.h"
#include <memory>

class MainWindow;
class Tip;
class ImageViewer;
class BottomBar;
class NavigateBar;
class Tip;
class FileHelper;
class ImageDownloader;
class App
{
public:	
	~App();
	void Paint(SkCanvas* canvas);
	void CheckMouseEnter(int x, int y);
	void CheckMouseDown(int x, int y);
	void CheckMouseUp(int x, int y);
	static void init(HINSTANCE hinstance);
	static App* get();
	static void dispose();
public:
	std::unique_ptr<MainWindow> mainWindow;
	std::unique_ptr<Tip> tip;
	std::shared_ptr<ImageViewer> imageViewer {nullptr};
	std::unique_ptr<BottomBar> bottomBar;
	std::unique_ptr<NavigateBar> navigateBar;
	std::unique_ptr<FileHelper> fileHelper;
	std::unique_ptr<ImageDownloader> imageDownloader;
	SkFont* iconFont;
	SkFont* textFont;
	HINSTANCE hinstance;
private:
	App(HINSTANCE hinstance);
	void initIconFont();
	void initTextFont();
private:
};

