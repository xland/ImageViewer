#include "FileHelper.h"
#include <Windows.h>
#include <shobjidl.h> 
#include "Converter.h"
#include "App.h"
#include "MainWindow.h"
#include "ImageViewer.h"
#include "Tip.h"
FileHelper::FileHelper()
{

}
FileHelper::~FileHelper()
{

}

void FileHelper::Open()
{
	showFileDialog(false);
}
void FileHelper::Save()
{
	if (currentPath.empty()) return;
	showFileDialog(true);
}
void FileHelper::ShowNext()
{
	loopFile(true);
}
void FileHelper::ShowPrev()
{
	loopFile(false);
}
void FileHelper::loopFile(bool isNext)
{
	if (currentPath.empty()) return;
	std::filesystem::path folder{ currentPath };
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
		if (currentPath == temp) {
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
			App::get()->tip->Show(App::getText("isFirstOne"));
		}
		return;
	}
	if (currentPath == resultPath) {
		if (isNext) {
			App::get()->tip->Show(App::getText("isLastOne"));
		}
		return;
	}
	currentPath = resultPath;
	auto path = ConvertWideToUtf8(resultPath.wstring());
	ImageViewer::MakeImageViewer(path);
}

void FileHelper::showFileDialog(bool isSave)
{
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
				pFileSave->SetFileName(currentPath.filename().wstring().c_str());
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
						auto path = ConvertWideToUtf8(ss.str());
						if (isSave) {
							App::get()->imageViewer->SaveImage(path);
						}
						else
						{
							currentPath = std::filesystem::path(ss.str());
							ImageViewer::MakeImageViewer(path);
						}						
					}
					pItem->Release();
				}
			}
			pFileSave->Release();
		}
		CoUninitialize();
	}
}