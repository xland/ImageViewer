#include "ImageDownloader.h"
#include <Windows.h>
#include "App.h"
#include "resource.h"
#include "MainWindow.h"

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