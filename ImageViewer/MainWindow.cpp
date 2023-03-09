#include "MainWindow.h"
#include <Windows.h>
#include <windowsx.h>
#include <string>
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "include/core/SkImage.h"
#include "include/core/SkStream.h"
#include "include/core/SkPicture.h"
#include "include/codec/SkCodec.h"
#include "Color.h"

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
	ShowWindow(hwnd, SW_SHOW);
}
MainWindow::~MainWindow()
{

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
	case WM_PAINT:
		BeginPaint(hwnd, &ps);
		paint();
		EndPaint(hwnd, &ps);
	case WM_NCCALCSIZE: {
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	case WM_GETMINMAXINFO: {
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
	case WM_MOUSEMOVE: {
		auto x = GET_X_LPARAM(lParam);
		auto y = GET_Y_LPARAM(lParam);
	}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
void MainWindow::paint()
{
	SkSurface* surface = getSurface();
	auto canvas = surface->getCanvas();
	canvas->clear(ColorWhite);

	std::string filePath = "";
	SkFILEStream fileStream(filePath.c_str());
	auto skData = SkData::MakeFromFileName("C:\\Users\\liuxiaolun\\Pictures\\gif.gif");
	auto skImage = SkImage::MakeFromEncoded(skData);
	std::unique_ptr<SkCodec> codec = SkCodec::MakeFromData(skData);
	if (!codec) {
		return;
	}
	SkString ext;
	switch (codec->getEncodedFormat()) {
	case SkEncodedImageFormat::kBMP:  ext = "bmp"; break;
	case SkEncodedImageFormat::kGIF:  ext = "gif"; break;
	case SkEncodedImageFormat::kICO:  ext = "ico"; break;
	case SkEncodedImageFormat::kJPEG: ext = "jpg"; break;
	case SkEncodedImageFormat::kPNG:  ext = "png"; break;
	case SkEncodedImageFormat::kDNG:  ext = "dng"; break;
	case SkEncodedImageFormat::kWBMP: ext = "wbmp"; break;
	case SkEncodedImageFormat::kWEBP: ext = "webp"; break;
	default:
		// This should be unreachable because we cannot create a codec if we do not know
		// the image type.
		SkASSERT(false);
	}
	canvas->drawImage(skImage, 0, 0);
	surface->flushAndSubmit();
	HDC dc = GetDC(hwnd);
	SwapBuffers(dc);
	ReleaseDC(hwnd, dc);
	delete surface;
}