#include "ToolTip.h"
#include "MainWindow.h"
#include "App.h"
ToolTip::ToolTip()
{
    static auto toolTipWindowStyle = WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP;
    hwnd = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, toolTipWindowStyle, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        App::get()->mainWindow->hwnd, NULL, App::get()->hinstance, NULL);
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}
void ToolTip::RegToolTip(std::string&& key, RECT rect)
{
    TOOLINFO ti = { 0 };
    ti.cbSize = sizeof(TOOLINFO);
    ti.uFlags = TTF_SUBCLASS;
    ti.hwnd = hwnd;
    ti.hinst = App::get()->hinstance;
    auto tipText = App::getText(std::move(key));
    ti.lpszText = (LPWSTR)tipText.c_str();
    ti.rect = rect;
    SendMessage(hwnd, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
}