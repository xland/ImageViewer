#include "BottomBar.h"
#include "MainWindow.h"
#include "Color.h"
#include "include/core/SkFont.h"
#include "resource.h"
#include <math.h>
#include "IconFont.h"
#include <windows.h>
#include <shobjidl.h> 
#include "Converter.h"

BottomBar::BottomBar(MainWindow* win):win{win}
{
	
}
BottomBar::~BottomBar()
{

}

void BottomBar::loopFile(bool isNext)
{
	//todo 轮询完成后给给提示
	if (imagePath.empty()) return;
	std::filesystem::path folder{ imagePath };
	folder.remove_filename();
	std::filesystem::path resultPath;
	bool nextBreakFlag = false;
	for (auto const& file : std::filesystem::directory_iterator{ folder })
	{
		if (file.is_directory()) continue;
		auto temp = file.path();
		if (nextBreakFlag) {
			resultPath = temp;
			break;
		}
		if (imagePath == temp) {
			if (!isNext) {
				break;
			}
			else
			{
				nextBreakFlag = true;
			}			
		}
		resultPath = temp;
	}
	if (resultPath.empty()) {
		return;
	}
	imagePath = resultPath;
	auto path = ConvertWideToUtf8(resultPath.wstring());
	if (path.ends_with(".gif")) {
		int i = 0;
	}
	win->imageViewer = ImageViewer::MakeImageViewer(path.c_str(), win);
	InvalidateRect(win->hwnd, nullptr, false);
}


void BottomBar::openFile()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileOpenDialog* pFileOpen;
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
		if (SUCCEEDED(hr))
		{
			hr = pFileOpen->Show(win->hwnd);
			if (SUCCEEDED(hr))
			{
				IShellItem* pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
					if (SUCCEEDED(hr))
					{
						std::wstringstream ss;
						ss << pszFilePath;
						CoTaskMemFree(pszFilePath);
						imagePath = std::filesystem::path(ss.str());
						auto path = ConvertWideToUtf8(ss.str());
						win->imageViewer = ImageViewer::MakeImageViewer(path.c_str(), win);
						InvalidateRect(win->hwnd, nullptr, false);
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}
}
void BottomBar::CheckMouseUp(int mouseX, int mouseY)
{
	if (mouseEnterIndex == -1) return;
	if (mouseEnterIndex == 0) {
		openFile();
	}
	else if (mouseEnterIndex == 1) {
		loopFile(false);
	}
	else if (mouseEnterIndex == 2) {
		loopFile(true);
	}
}
void BottomBar::CheckMouseEnter(int mouseX, int mouseY)
{
	int index = -2;
	if (mouseX > x && mouseY > y && mouseX < x + w && mouseY < win->clientHeight) {
		index = (unsigned)std::floor((mouseX - x) / btnWidth);
	}
	else {
		index = -1;
	}
	if (index != mouseEnterIndex) {
		mouseEnterIndex = index;
		InvalidateRect(win->hwnd, nullptr, false);
	}
}
void BottomBar::caculatePosition()
{
	x = (float)(win->clientWidth - w) / 2 ;
	y = (float)(win->clientHeight - win->bottomBarHeight);
}
void BottomBar::Paint(SkCanvas* canvas)
{
	SkPaint paint;
	{
		paint.setColor(ColorWhite);
		SkRect rect;
		rect.setXYWH(0, win->clientHeight - win->bottomBarHeight, (float)win->clientWidth, (float)win->bottomBarHeight);
		canvas->drawRect(rect, paint);
	}	
	caculatePosition();
	float tempX = x + fontSize;
	float tempY = y + fontSize / 2 + win->bottomBarHeight / 2;
	paint.setColor(ColorBlack);
	paint.setAntiAlias(true);
	auto font = IconFont::Get();
	font->setSize(fontSize);
	for (size_t i = 0; i < btnCodes.size(); i++)
	{
		if (i == mouseEnterIndex) {
			paint.setColor(GetColor(230,230,230));
			SkRect rect;
			rect.setXYWH(tempX-fontSize, y, btnWidth, (float)win->bottomBarHeight);
			canvas->drawRect(rect, paint);
			paint.setColor(ColorBlack);
		}
		canvas->drawString(btnCodes[i], tempX, tempY, *font, paint);
		tempX += btnWidth;
	}
}