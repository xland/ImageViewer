#include "ImageDownloader.h"
#include <Windows.h>
#include "App.h"
#include "resource.h"
#include "MainWindow.h"
#include <Urlmon.h>
#include <functional>
#include <future>

namespace {
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
		STDMETHOD(OnStartBinding)(
			/* [in] */ DWORD dwReserved,
			/* [in] */ IBinding __RPC_FAR* pib)
		{
			return E_NOTIMPL;
		}

		STDMETHOD(GetPriority)(
			/* [out] */ LONG __RPC_FAR* pnPriority)
		{
			return E_NOTIMPL;
		}

		STDMETHOD(OnLowResource)(
			/* [in] */ DWORD reserved)
		{
			return E_NOTIMPL;
		}

		STDMETHOD(OnProgress)(
			/* [in] */ ULONG ulProgress,
			/* [in] */ ULONG ulProgressMax,
			/* [in] */ ULONG ulStatusCode,
			/* [in] */ LPCWSTR wszStatusText);

		STDMETHOD(OnStopBinding)(
			/* [in] */ HRESULT hresult,
			/* [unique][in] */ LPCWSTR szError)
		{
			return E_NOTIMPL;
		}

		STDMETHOD(GetBindInfo)(
			/* [out] */ DWORD __RPC_FAR* grfBINDF,
			/* [unique][out][in] */ BINDINFO __RPC_FAR* pbindinfo)
		{
			return E_NOTIMPL;
		}

		STDMETHOD(OnDataAvailable)(
			/* [in] */ DWORD grfBSCF,
			/* [in] */ DWORD dwSize,
			/* [in] */ FORMATETC __RPC_FAR* pformatetc,
			/* [in] */ STGMEDIUM __RPC_FAR* pstgmed)
		{
			return E_NOTIMPL;
		}

		STDMETHOD(OnObjectAvailable)(
			/* [in] */ REFIID riid,
			/* [iid_is][in] */ IUnknown __RPC_FAR* punk)
		{
			return E_NOTIMPL;
		}

		// IUnknown methods.  Note that IE never calls any of these methods, since
		// the caller owns the IBindStatusCallback interface, so the methods all
		// return zero/E_NOTIMPL.

		STDMETHOD_(ULONG, AddRef)()
		{
			return 0;
		}

		STDMETHOD_(ULONG, Release)()
		{
			return 0;
		}

		STDMETHOD(QueryInterface)(
			/* [in] */ REFIID riid,
			/* [iid_is][out] */ void __RPC_FAR* __RPC_FAR* ppvObject)
		{
			return E_NOTIMPL;
		}
	};
	HRESULT DownloadStatus::OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR wszStatusText)
	{
		//float ps = 0;
		//if (ulProgressMax != 0) {
		//	ps = (float)ulProgress * 100 / ulProgressMax;
		//}
		return S_OK;
	}

}


ImageDownloader::ImageDownloader()
{

}
ImageDownloader::~ImageDownloader()
{

}
void ImageDownloader::ShowUrlDialog()
{
	BOOL result = DialogBox(App::get()->hinstance, MAKEINTRESOURCE(IDD_DIALOG1), App::get()->mainWindow->hwnd, (DLGPROC)PromptProc);
}
void ImageDownloader::DownloadImage(std::wstring&& url)
{
    DownloadStatus ds;
	LPTSTR userInputValue = new TCHAR[MAX_PATH];
	URLDownloadToCacheFile(nullptr, url.c_str(), userInputValue, MAX_PATH, 0,&ds);
	int a = 1;	
}