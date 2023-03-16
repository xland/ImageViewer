#pragma once
#include <Windows.h>
#include "include/core/SkFont.h"
#include <memory>

class MainWindow;
class Tip;
class ImageViewer;
class BottomBar;
class NavigateBar;
class App
{
public:	
	~App();
	static void init(HINSTANCE hinstance);
	static App* get();
public:
	std::unique_ptr<MainWindow> mainWindow;
	std::unique_ptr<Tip> tip;
	std::unique_ptr<ImageViewer> imageViewer {nullptr};
	std::unique_ptr<BottomBar> bottomBar;
	std::unique_ptr<NavigateBar> navigateBar;
	SkFont* iconFont;
	HINSTANCE hinstance;
private:
	App(HINSTANCE hinstance);
	void initIconFont();
private:
};

