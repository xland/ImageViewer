#include "MainWindow.h"
#include <Windows.h>
#include <windowsx.h>
#include <locale>
#include <string>
#include "include/core/SkCanvas.h"
#include "include/core/SkImage.h"
#include "include/core/SkStream.h"
#include "include/core/SkPicture.h"
#include "include/codec/SkCodec.h"
#include "GifViewer.h"
#include "Color.h"
#include "Converter.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	MainWindow* win;
	if (msg == WM_CREATE) {
		win = (MainWindow*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)win);
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	else {
		win = (MainWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (!win) {
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}
	return win->winProc(hwnd, msg, wParam, lParam);
}

MainWindow::MainWindow(HINSTANCE hinstance):hinstance{hinstance}
{
	auto flag = creatreNativeWindow();
	if (!flag || !hwnd) {
		//todo error
		return;
	}		
	initSurface();
	auto path = ConvertWideToUtf8(L"C:\\Users\\liuxiaolun\\Desktop\\图片\\girl.jpg");//D:\\gif\\gif2.gif D:\\gif\\c.jpg
	imageViewer = ImageViewer::MakeImageViewer(path.c_str(),this);
	bottomBar = std::make_unique<BottomBar>(this);
	ShowWindow(hwnd, SW_SHOW);
}
MainWindow::~MainWindow()
{
	disposeSurfaceResource();
}
bool MainWindow::creatreNativeWindow()
{
	static std::wstring windowClassName = L"RRS_Window_Class";
	static WNDCLASSEX wcex;
	static bool wcexInit = false;
	if (!wcexInit) {
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wcex.lpfnWndProc = WindowProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hinstance;
		wcex.hIcon = LoadIcon(hinstance, (LPCTSTR)IDI_WINLOGO);
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = windowClassName.c_str();
		wcex.hIconSm = LoadIcon(hinstance, (LPCTSTR)IDI_WINLOGO);
		if (!RegisterClassEx(&wcex)) {
			//todo log
			return false;
		}
		wcexInit = true;
	}
	RECT screenRect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &screenRect, 0);
	auto x = (screenRect.right - width) / 2;
	auto y = (screenRect.bottom - height) / 2;
	hwnd = CreateWindow(windowClassName.c_str(), L"Image Viewer", WS_OVERLAPPEDWINDOW, x, y, width, height,
		nullptr, nullptr, hinstance, nullptr);
	if (!hwnd)
	{
		//todo log
		return false;
	}
	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
	return true;
}
LRESULT CALLBACK  MainWindow::winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	switch (msg) {
		case WM_CLOSE:
		{
			PostQuitMessage(0);
		}
		case WM_PAINT:
		{
			BeginPaint(hwnd, &ps);
			paint();
			EndPaint(hwnd, &ps);
		}
		case WM_NCCALCSIZE: 
		{
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		case WM_GETMINMAXINFO: 
		{
			MINMAXINFO* mminfo;
			mminfo = (PMINMAXINFO)lParam;
			mminfo->ptMinTrackSize.x = minWidth;
			mminfo->ptMinTrackSize.y = minHeight;
			mminfo->ptMaxPosition.x = 0;
			mminfo->ptMaxPosition.y = 0;
			return 0;
		}
		case WM_SIZE: {
			clientWidth = LOWORD(lParam);
			clientHeight = HIWORD(lParam);
			return 0;
		}
		case WM_MOUSEMOVE: 
		{
			auto x = GET_X_LPARAM(lParam);
			auto y = GET_Y_LPARAM(lParam);
			bottomBar->CheckMouseEnter(x, y);
			return 0;
		}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
void MainWindow::paint()
{
	auto surface = getSurface();
	auto canvas = surface->getCanvas();
	canvas->clear(GetColor(248,248,248));
	imageViewer->Paint(canvas);
	bottomBar->Paint(canvas);
	surface->flushAndSubmit();
	HDC dc = GetDC(hwnd);
	SwapBuffers(dc);
	ReleaseDC(hwnd, dc);
}