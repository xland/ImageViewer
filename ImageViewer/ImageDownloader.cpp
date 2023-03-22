#include "ImageDownloader.h"
#include <Windows.h>
#include "App.h"
#include "resource.h"
#include "MainWindow.h"
#include <Urlmon.h>
#include <functional>
#include <future>
#include "ImageViewer.h"
#include "Converter.h"
#include "Loading.h"
#include "FileHelper.h"

namespace {

	//todo 输入框怎么是个多行文本框
    BOOL CALLBACK PromptProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) {
        BOOL result = FALSE;
        switch (message)
        {
        case WM_INITDIALOG: {
            HWND hwndInput = GetDlgItem(hwndDlg, IDC_URL);
            SetFocus(hwndInput);
            break;
        }
        case WM_COMMAND: {
            switch (LOWORD(wParam))
            {
            case IDOK: {
                HWND hwndInput = GetDlgItem(hwndDlg, IDC_URL);
                int outLength = GetWindowTextLength(hwndInput) + 1;
                LPTSTR userInputValue = new TCHAR[outLength];
                GetDlgItemText(hwndDlg, IDC_URL, userInputValue, outLength);
                std::wstringstream ss;
                ss << userInputValue;
                App::get()->imageDownloader->DownloadImage(std::move(ss.str()));
                delete[] userInputValue;
                EndDialog(hwndDlg, wParam);
                result = TRUE;
                break;
            }
            case IDCANCEL: {
                EndDialog(hwndDlg, wParam);
                result = FALSE;
                break;
            }
            }
            break;
        }
        }
        return FALSE;
    }
	class DownloadStatus :public IBindStatusCallback {
	public:
		ULONG presize{ 0 };
		STDMETHOD(OnStartBinding)(DWORD dwReserved,IBinding __RPC_FAR* pib)
		{
			return App::get()->imageDownloader->abortDownloadFlag ? E_ABORT : E_NOTIMPL;
		}

		STDMETHOD(GetPriority)(LONG __RPC_FAR* pnPriority)
		{
			return E_NOTIMPL;
		}
		STDMETHOD(OnLowResource)(DWORD reserved)
		{
			return E_NOTIMPL;
		}
		STDMETHOD(OnProgress)(ULONG ulProgress,ULONG ulProgressMax,ULONG ulStatusCode,LPCWSTR wszStatusText)
		{
			//float ps = 0;
			//if (ulProgressMax != 0) {
			//	ps = (float)ulProgress * 100 / ulProgressMax;
			//}
			return App::get()->imageDownloader->abortDownloadFlag ? E_ABORT : E_NOTIMPL;
		}
		STDMETHOD(OnStopBinding)(HRESULT hresult,LPCWSTR szError)
		{
			return E_NOTIMPL;
		}
		STDMETHOD(GetBindInfo)(DWORD __RPC_FAR* grfBINDF,BINDINFO __RPC_FAR* pbindinfo)
		{
			return E_NOTIMPL;
		}
		STDMETHOD(OnDataAvailable)(DWORD grfBSCF,DWORD dwSize,FORMATETC __RPC_FAR* pformatetc,STGMEDIUM __RPC_FAR* pstgmed)
		{
			return App::get()->imageDownloader->abortDownloadFlag ? E_ABORT : E_NOTIMPL;
		}
		STDMETHOD(OnObjectAvailable)(REFIID riid,IUnknown __RPC_FAR* punk)
		{
			return App::get()->imageDownloader->abortDownloadFlag ? E_ABORT : E_NOTIMPL;
		}
		STDMETHOD_(ULONG, AddRef)()
		{
			return E_NOTIMPL;
		}
		STDMETHOD_(ULONG, Release)()
		{
			return E_NOTIMPL;
		}
		STDMETHOD(QueryInterface)(REFIID riid,void __RPC_FAR* __RPC_FAR* ppvObject)
		{
			return E_NOTIMPL;
		}
	};
}


ImageDownloader::ImageDownloader()
{

}
ImageDownloader::~ImageDownloader()
{
	AbortDownload();
}
void ImageDownloader::ShowUrlDialog()
{
	AbortDownload();
	BOOL result = DialogBox(App::get()->hinstance, MAKEINTRESOURCE(IDD_DIALOG1), App::get()->mainWindow->hwnd, (DLGPROC)PromptProc);
}
void ImageDownloader::AbortDownload()
{
	if (!downloadThread.joinable()) return;
	abortDownloadFlag = true;
	downloadThread.join();
}
void ImageDownloader::DownloadImage(std::wstring&& url)
{
	AbortDownload();
	imageUrl = url;
	abortDownloadFlag = false;
	App::get()->loading->Show();
	downloadThread = std::thread([this]() {
		DownloadStatus ds;
		LPTSTR cacheFilePath = new TCHAR[MAX_PATH];
		const HRESULT hr = URLDownloadToCacheFile(nullptr, this->imageUrl.c_str(), cacheFilePath, URLOSTRM_USECACHEDCOPY, 0, &ds);
		if (FAILED(hr))
		{
			delete[] cacheFilePath;
			App::get()->loading->Hide();
			MessageBox(App::get()->mainWindow->hwnd,
				App::get()->getText("downloadError").c_str(),
				App::get()->getText("SysInfo").c_str(),
				MB_ICONWARNING | MB_OK);
			return;
		}
		std::wstringstream ss;
		ss << cacheFilePath;
		delete[] cacheFilePath;
		auto path = ConvertWideToUtf8(ss.str());
		if (path.empty()) {
			//todo
		}
		App::get()->fileHelper->currentPath = std::filesystem::path(path);
		App::get()->loading->Hide();
		ImageViewer::MakeImageViewer(path);
	});
}