#include "ToolTip.h"
#include "MainWindow.h"
#include "App.h"
ToolTip::ToolTip()
{
    hwnd = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        App::get()->mainWindow->hwnd, NULL, App::get()->hinstance, NULL);
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}
ToolTip::~ToolTip()
{

}
void ToolTip::RegToolTip(const char* key, RECT rect,unsigned id)
{
    if (tooltipInfos.contains(key)) {
        auto ti = tooltipInfos.at(key);
        ti.rect = rect;
        SendMessage(hwnd, TTM_NEWTOOLRECT, 0, (LPARAM)(LPTOOLINFO)&ti);
    }
    else
    {
        TOOLINFO ti = { 0 };
        ti.cbSize = sizeof(TOOLINFO);
        ti.uFlags = TTF_SUBCLASS;
        ti.hwnd = App::get()->mainWindow->hwnd;
        ti.hinst = App::get()->hinstance;
        ti.uId = id;
        auto tipText = App::getText(key);
        ti.lpszText = (LPWSTR)tipText.c_str();
        ti.rect = rect;
        tooltipInfos.insert({ key, ti });
        SendMessage(hwnd, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
    }
    
    
}