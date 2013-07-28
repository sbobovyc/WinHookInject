#include <windows.h>
#include "resource.h"

BOOL CALLBACK DialogProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

HINSTANCE TheInstance = 0;

int WINAPI WinMain
   (HINSTANCE hInst, HINSTANCE hPrevInst, char * cmdParam, int cmdShow)
{
    TheInstance = hInst;

    HWND hDialog = 0;

    hDialog = CreateDialog (hInst, 
                            MAKEINTRESOURCE (DLG_MAIN), 
                            0, 
                            DialogProc);

    if (!hDialog)
    {
        char buf [100];
        wsprintf (buf, "Error x%x", GetLastError ());
        MessageBox (0, buf, "CreateDialog", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    MSG  msg;
    int status;
    while ((status = GetMessage (& msg, 0, 0, 0)) != 0)
    {
        if (status == -1)
            return -1;
        if (!IsDialogMessage (hDialog, & msg))
        {
            TranslateMessage ( & msg );
            DispatchMessage ( & msg );
        }
    }

    return msg.wParam;
}

BOOL CALLBACK DialogProc (HWND hwnd, 
                          UINT message, 
                          WPARAM wParam, 
                          LPARAM lParam)
{
	BOOL status = FALSE;
    switch (message)
    {
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDOK:
					OutputDebugString("In OK");
                    DestroyWindow(hwnd);
					PostQuitMessage(0);
					STARTUPINFO StartupInfo;	
					PROCESS_INFORMATION ProcessInformation;
					ZeroMemory(&StartupInfo, sizeof(StartupInfo));
					StartupInfo.cb = sizeof(StartupInfo);
					ZeroMemory(&ProcessInformation, sizeof(ProcessInformation));

					status = CreateProcessA(NULL, "directInputTest.exe", NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInformation);
					if(!status) {
				        char buf [100];
						wsprintf (buf, "Error x%x", GetLastError ());
						MessageBox (0, buf, "CreateProcessA", MB_ICONEXCLAMATION | MB_OK);
					}

					break;
                case IDCANCEL:
					OutputDebugString("In Cancel");
                    DestroyWindow(hwnd);
					PostQuitMessage(0);
					break;
            }
        break;
        default:
            return FALSE;
    }
    return FALSE;
}