#pragma once
#include <Windows.h>
#include "include/core/SkSurface.h"
#include "ImageViewer.h"
struct GrGLInterface;
class GrDirectContext;
class MainWindow
{
public:
	MainWindow(HINSTANCE hinstance);
	~MainWindow();
	HWND hwnd{ nullptr };
	unsigned clientWidth{ 0 };
	unsigned clientHeight{ 0 };
	unsigned toolBarHeight{ 60 };
private:
	friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool creatreNativeWindow();
	LRESULT CALLBACK  winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void paint();
	sk_sp<SkSurface> getSurface();
	void disposeSurfaceResource();
	void initSurface();
	int stencilBits{ 0 };
	HGLRC hglrc{ nullptr };
	const GrGLInterface* backendContext{ nullptr };
	GrDirectContext* directContext{ nullptr };
	HINSTANCE hinstance;
	unsigned width{ 1000 };
	unsigned height{ 700 };
	unsigned minWidth{ 800 };
	unsigned minHeight{ 600 };
	std::shared_ptr<ImageViewer> imageViewer;
};

