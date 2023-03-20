#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include "include/core/SkSurface.h"
#include <chrono>
struct GrGLInterface;
class GrDirectContext;
class MainWindow
{
public:
	MainWindow();
	~MainWindow();
	void Show();
	void Refresh();
	//void ShowToolTip(std::string&& key, unsigned x, unsigned y);
	//void HideToolTip();
public:
	HWND hwnd{ nullptr };
	HWND hwndToolTip{ nullptr };
	unsigned clientWidth{ 0 };
	unsigned clientHeight{ 0 };
	unsigned bottomBarHeight{ 60 };	
private:
	friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool creatreNativeWindow();
	void setTracking(bool track);
	LRESULT CALLBACK  winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void paint();
	sk_sp<SkSurface> getSurface();
	void disposeSurfaceResource();
	void initSurface();
	//void initToolTip();
private:
	int stencilBits{ 0 };
	HGLRC hglrc{ nullptr };
	const GrGLInterface* backendContext{ nullptr };
	GrDirectContext* directContext{ nullptr };
	unsigned width{ 1000 };
	unsigned height{ 700 };
	unsigned minWidth{ 800 };
	unsigned minHeight{ 600 };
	TOOLINFO ti = { 0 };
	//std::chrono::system_clock::time_point leftBtnDownTime;
};

