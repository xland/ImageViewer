#include "MainWindow.h"
#include <shobjidl.h> 
#include <locale>
#include <string>
#include "include/core/SkCanvas.h"
#include "include/core/SkImage.h"
#include "include/core/SkStream.h"
#include "include/core/SkPicture.h"
#include "include/codec/SkCodec.h"
#include "Color.h"
#include "Converter.h"
#include "App.h"
#include "ImageViewer.h"


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

MainWindow::MainWindow()
{
	auto flag = creatreNativeWindow();
	if (!flag || !hwnd) {
		//todo error
		return;
	}
	//initToolTip();
	initSurface();


}
MainWindow::~MainWindow()
{
	disposeSurfaceResource();
}
void MainWindow::Show()
{
	ShowWindow(hwnd, SW_SHOW);
}
void MainWindow::Refresh()
{
	InvalidateRect(hwnd, nullptr, false);
}
void MainWindow::setTracking(bool track)
{
	TRACKMOUSEEVENT mouseEvent;
	mouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
	mouseEvent.dwFlags = track ? TME_LEAVE : TME_CANCEL;
	mouseEvent.hwndTrack = hwnd;
	mouseEvent.dwHoverTime = HOVER_DEFAULT;
	TrackMouseEvent(&mouseEvent);
}
bool MainWindow::creatreNativeWindow()
{
	static std::wstring windowClassName = L"RRS_Window_Class";
	static WNDCLASSEX wcex;
	static bool wcexInit = false;
	auto hinstance = App::get()->hinstance;
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
			return 0;
		}
		case WM_PAINT:
		{
			BeginPaint(hwnd, &ps);
			paint();
			EndPaint(hwnd, &ps);
			return 0;
		}
		case WM_ERASEBKGND: {
			return 0; // InvalidateRect(win->hwnd, nullptr, false);
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
			App::get()->Resize(clientWidth, clientHeight);
			return 0;
		}
		case WM_MOUSELEAVE: {
			auto x = GET_X_LPARAM(lParam);
			auto y = GET_Y_LPARAM(lParam);
			App::get()->CheckMouseEnter(x, y);
			//HideToolTip();
			setTracking(false);
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			auto delta = GET_WHEEL_DELTA_WPARAM(wParam); //上120，下-120
			if (delta > 0) 
			{
				App::get()->imageViewer->Zoom(1.02f);
			}
			else
			{
				App::get()->imageViewer->Zoom(0.98f);
			}
			return 0;
		}
		case WM_MOUSEMOVE: 
		{
			auto x = GET_X_LPARAM(lParam);
			auto y = GET_Y_LPARAM(lParam);
			App::get()->CheckMouseEnter(x, y);
			setTracking(true);
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			//leftBtnDownTime = std::chrono::system_clock::now();
			auto x = GET_X_LPARAM(lParam);
			auto y = GET_Y_LPARAM(lParam);
			SetCapture(hwnd);
			App::get()->CheckMouseDown(x, y);
			return 0;
		}
		case WM_LBUTTONUP:
		{
			//GetDoubleClickTime();
			auto x = GET_X_LPARAM(lParam);
			auto y = GET_Y_LPARAM(lParam);
			ReleaseCapture();
			App::get()->CheckMouseUp(x, y);
			//OutputDebugStringA("CheckMouseUp");
			//OutputDebugStringA("\r\n");

			//return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
//void MainWindow::ShowToolTip(std::string&& key,unsigned x,unsigned y)
//{
//	auto tipText = App::getText(std::move(key));
//	ti.lpszText = (LPWSTR)tipText.c_str();
//	POINT pt = { x, y };
//	ClientToScreen(hwnd, &pt);
//	SendMessage(hwndToolTip, TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(pt.x + 10, pt.y - 20));
//	SendMessage(hwndToolTip, TTM_SETTOOLINFO, 0, (LPARAM)&ti);
//	SendMessage(hwndToolTip, TTM_TRACKACTIVATE, (WPARAM)TRUE, (LPARAM)&ti);
//}
//void MainWindow::HideToolTip()
//{
//	SendMessage(hwndToolTip, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)&ti);
//	ShowWindow(hwndToolTip, SW_HIDE);
//}
void MainWindow::initToolTip()
{
	hwndToolTip = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		hwnd, NULL, App::get()->hinstance, NULL);
	SetWindowPos(hwndToolTip, HWND_TOPMOST, 0, 0, 0, 0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	TOOLINFO ti = { 0 };
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = TTF_SUBCLASS;
	ti.hwnd = hwnd;
	ti.hinst = App::get()->hinstance;
	ti.lpszText = (LPWSTR)L"This is your tooltip string.";
	GetClientRect(hwnd, &ti.rect);
	SendMessage(hwndToolTip, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
}
void MainWindow::paint()
{
	auto surface = getSurface();
	auto canvas = surface->getCanvas();
	canvas->clear(GetColor(248,248,248));
	App::get()->Paint(canvas);	
	surface->flushAndSubmit();
	HDC dc = GetDC(hwnd);
	SwapBuffers(dc);
	ReleaseDC(hwnd, dc);
}