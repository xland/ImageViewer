#include "BottomBar.h"
#include "MainWindow.h"
#include "Color.h"
#include "include/core/SkFont.h"
#include "include/core/SkMaskFilter.h"
#include "resource.h"
#include <math.h>
#include "IconFont.h"
#include <windows.h>
#include <shobjidl.h> 
#include "Converter.h"

BottomBar::BottomBar(MainWindow* win):win{win}
{
	//todo cursor style
}
BottomBar::~BottomBar()
{
	if (lastFirstWaitingTread && lastFirstWaitingTread->joinable()) {
		lastFirstWaitingTread->join();
	}
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
		if (!isNext) {
			lastFirstWaitingTime = 2000;
			if (lastFirstFlag == 0) {
				if (lastFirstWaitingTread && lastFirstWaitingTread->joinable()) {
					lastFirstWaitingTread->join();
				}
				lastFirstWaitingTread = std::make_shared<std::thread>(&BottomBar::lastFirstWaitingFunc, this);
			}

			if (lastFirstFlag != 1) {
				lastFirstFlag = 1;
				InvalidateRect(win->hwnd, nullptr, false);
			}
		}
		return;
	}
	if (imagePath == resultPath) {
		if (isNext) {
			lastFirstWaitingTime = 2000;
			if (lastFirstFlag == 0) {				
				if (lastFirstWaitingTread && lastFirstWaitingTread->joinable()) {
					lastFirstWaitingTread->join();
				}
				lastFirstWaitingTread = std::make_shared<std::thread>(&BottomBar::lastFirstWaitingFunc, this);				
			}
			if (lastFirstFlag != 2) {
				lastFirstFlag = 2;
				InvalidateRect(win->hwnd, nullptr, false);
			}
		}
		return;
	}
	imagePath = resultPath;
	auto path = ConvertWideToUtf8(resultPath.wstring());
	win->imageViewer = ImageViewer::MakeImageViewer(path.c_str(), win);
	btnCodes[5] = (const char*)u8"\ue6be";
	InvalidateRect(win->hwnd, nullptr, false);
}
void BottomBar::lastFirstWaitingFunc()
{
	while (lastFirstWaitingTime > 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		lastFirstWaitingTime -= 20;
	}
	lastFirstFlag = 0;
	InvalidateRect(win->hwnd, nullptr, false);
}
std::string BottomBar::openFileDialog(bool isSave) {
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileOpenDialog* pFileSave;
		CLSID param1 = CLSID_FileOpenDialog, param2 = IID_IFileOpenDialog; 
		if (isSave) {
			param1 = CLSID_FileSaveDialog;
			param2 = IID_IFileSaveDialog;
		}
		hr = CoCreateInstance(param1, NULL, CLSCTX_ALL, param2, reinterpret_cast<void**>(&pFileSave));
		if (SUCCEEDED(hr))
		{
			COMDLG_FILTERSPEC FileTypes[] = {
{ L"All Pictures", L"*.png;*.bmp;*.gif;*.jpg;*.jpeg;*.svg;*.ico;*.jfif;*.jpe;*.dib;*.rle;*.emz;*.wmz;*.tif;*.tiff;*.emf;*.wmf;" },
{ L"All files", L"*.*" }
			};
			if (isSave) {
				pFileSave->SetFileName(imagePath.filename().wstring().c_str());
			}
			pFileSave->SetFileTypes(2, FileTypes);
			hr = pFileSave->Show(win->hwnd);
			if (SUCCEEDED(hr))
			{
				IShellItem* pItem;
				hr = pFileSave->GetResult(&pItem);
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
						return path;
					}
					pItem->Release();
				}
			}
			pFileSave->Release();
		}
		CoUninitialize();
	}
	return std::string();
}
void BottomBar::CheckMouseUp(int mouseX, int mouseY)
{

	if (mouseEnterIndex == -1) return;
	else if (mouseEnterIndex == 0) {
		auto path = openFileDialog(false);
		if (path.empty()) return;
		win->imageViewer = ImageViewer::MakeImageViewer(path.c_str(), win);
		InvalidateRect(win->hwnd, nullptr, false);
		return;
	}
	if (imagePath.empty())return;
	else if (mouseEnterIndex == 1) {
		loopFile(false);
	}
	else if (mouseEnterIndex == 2) {
		loopFile(true);
	}
	else if (mouseEnterIndex == 3) {
		win->imageViewer->Zoom(0.98);
	}
	else if (mouseEnterIndex == 4) {
		win->imageViewer->Zoom(1.02);
	}
	else if (mouseEnterIndex == 5) {
		if (btnCodes[5] == (const char*)u8"\ue6f8") 
		{
			win->imageViewer->IsAutoSize = true;
			btnCodes[5] = (const char*)u8"\ue6be";
			InvalidateRect(win->hwnd, nullptr, false);
		}
		else
		{
			win->imageViewer->Zoom(1.f);
		}
		
	}
	else if (mouseEnterIndex == 6) {
		btnCodes[5] = (const char*)u8"\ue6be";
		win->imageViewer->Rotate();
	}
	else if (mouseEnterIndex == 7) {
		auto path =	openFileDialog(true);
		if (path.empty()) return;
		win->imageViewer->SaveImage(path);
	}
}
void BottomBar::CheckMouseEnter(int mouseX, int mouseY)
{
	int index = -2;
	auto x = (float)(win->clientWidth - w) / 2;
	auto y = (float)(win->clientHeight - win->bottomBarHeight);
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
void BottomBar::Paint(SkCanvas* canvas)
{
	SkPaint paint;
	{
		paint.setColor(ColorWhite);
		SkRect rect;
		rect.setXYWH(0, win->clientHeight - win->bottomBarHeight, (float)win->clientWidth, (float)win->bottomBarHeight);
		canvas->drawRect(rect, paint);
	}	
	auto x = (float)(win->clientWidth - w) / 2;
	auto y = (float)(win->clientHeight - win->bottomBarHeight);
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
	if (lastFirstFlag>0) {
		auto x = (float)(win->clientWidth - 300) / 2;
		auto y = (float)(win->clientHeight - win->bottomBarHeight - 60)/2;
		SkRect rect;
		rect.setXYWH(x, y, 300, 60);
		paint.setColor(ColorBlack);
		{
			SkPaint blur;
			blur.setColor(ColorWhite);
			blur.setAlpha(127);
			blur.setMaskFilter(SkMaskFilter::MakeBlur(kNormal_SkBlurStyle, 3.4f, false));
			canvas->drawRoundRect(rect, 6, 6, blur);
		}
		canvas->drawRoundRect(rect, 6,6,paint);
		paint.setColor(ColorWhite);
		SkFont font(SkTypeface::MakeFromName("Microsoft YaHei", SkFontStyle::Normal()), 20);
		paint.setAntiAlias(true);
		std::wstring Text = lastFirstFlag == 1 ? L"已是第一张":L"已是最后一张";
		SkRect rectText;
		font.measureText(Text.data(), wcslen(Text.data()) * 2, SkTextEncoding::kUTF16, &rectText);
		x = x+(300-rectText.width())/2+ rectText.x();
		y = y+(60 - rectText.height()) / 2 - rectText.y();
		canvas->drawSimpleText(Text.data(), wcslen(Text.data()) * 2, SkTextEncoding::kUTF16, x, y, font, paint);
	}
}