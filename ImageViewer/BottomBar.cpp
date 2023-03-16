#include "BottomBar.h"
#include "MainWindow.h"
#include "Color.h"
#include "include/core/SkFont.h"
#include "include/core/SkMaskFilter.h"
#include "resource.h"
#include "App.h"
#include <math.h>
#include <windows.h>
#include <shobjidl.h> 
#include "Converter.h"
#include "Tip.h"
#include "ImageViewer.h"

BottomBar::BottomBar()
{
	//todo cursor style
}
BottomBar::~BottomBar()
{
}

void BottomBar::loopFile(bool isNext)
{
	//todo 不是图片，给出提示
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
			App::get()->tip->Show(L"已是第一张");
		}
		return;
	}
	if (imagePath == resultPath) {
		if (isNext) {
			App::get()->tip->Show(L"已是最后一张");
		}
		return;
	}
	imagePath = resultPath;
	auto path = ConvertWideToUtf8(resultPath.wstring());
	ImageViewer::MakeImageViewer(path.c_str());
	btnCodes[5] = (const char*)u8"\ue6be";
	App::get()->mainWindow->Refresh();
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
			hr = pFileSave->Show(App::get()->mainWindow.get()->hwnd);
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
void BottomBar::CheckMouseDown(int mouseX, int mouseY)
{

	if (mouseEnterIndex == -1) return;
	else if (mouseEnterIndex == 0) {
		auto path = openFileDialog(false);
		if (path.empty()) return;
		ImageViewer::MakeImageViewer(path.c_str());
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
		App::get()->imageViewer->Zoom(0.98f);
	}
	else if (mouseEnterIndex == 4) {
		App::get()->imageViewer->Zoom(1.02f);
	}
	else if (mouseEnterIndex == 5) {
		if (btnCodes[5] == (const char*)u8"\ue6f8") 
		{
			App::get()->imageViewer->IsAutoSize = true;
			btnCodes[5] = (const char*)u8"\ue6be";
			App::get()->mainWindow->Refresh();
		}
		else
		{
			App::get()->imageViewer->Zoom(1.f);
		}
		
	}
	else if (mouseEnterIndex == 6) {
		btnCodes[5] = (const char*)u8"\ue6be";
		App::get()->imageViewer->Rotate();
	}
	else if (mouseEnterIndex == 7) {
		auto path =	openFileDialog(true);
		if (path.empty()) return;
		App::get()->imageViewer->SaveImage(path);
	}
}
void BottomBar::CheckMouseEnter(int mouseX, int mouseY)
{
	int index = -2;
	auto win = App::get()->mainWindow.get();
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
		App::get()->mainWindow->Refresh();
	}
}
void BottomBar::Paint(SkCanvas* canvas)
{
	auto win = App::get()->mainWindow.get();
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
	auto font = App::get()->iconFont;
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