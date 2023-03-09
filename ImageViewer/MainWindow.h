#pragma once
#include <Windows.h>
struct GrGLInterface;
class GrDirectContext;
class SkSurface;
class MainWindow
{
public:
	MainWindow(HINSTANCE hinstance);
	~MainWindow();
private:
	friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool creatreNativeWindow();
	LRESULT CALLBACK  winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void paint();
	SkSurface* getSurface();
	void disposeSurfaceResource();
	void initSurface();
	int stencilBits{ 0 };
	HGLRC hglrc{ nullptr };
	const GrGLInterface* backendContext{ nullptr };
	GrDirectContext* directContext{ nullptr };
	HWND hwnd{nullptr};
	HINSTANCE hinstance;
	unsigned width{ 1000 };
	unsigned height{ 700 };
	unsigned minWidth{ 800 };
	unsigned minHeight{ 600 };
	unsigned clientWidth{ 0 };
	unsigned clientHeight{ 0 };
};

