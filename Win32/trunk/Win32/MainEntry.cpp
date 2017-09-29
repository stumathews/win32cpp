// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "ComUtility.h"

using namespace MyCOM;

static TCHAR szWindowClass[] = TEXT("win32app"); // The main window class name.
static TCHAR szTitle[] = TEXT("Win32 Guided Tour Application"); // The string that appears in the application's title bar.
HINSTANCE hInst;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // Forward declarations of functions included in this code module:

struct MyWindowData
{	
	LPWSTR fish = TEXT("Baracudda");
	DWORD_PTR age = 65;
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL, TEXT("Call to RegisterClassEx failed!"), TEXT("Win32 Guided Tour"), NULL);
        return TRUE;
    }

    hInst = hInstance; // Store instance handle in our global variable
	MyWindowData* myWinData = new MyWindowData;
	if (myWinData == NULL)
		return 0;

	myWinData->age = 13;	

		HWND hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        500, 100,
        NULL,
        NULL,
        hInstance,
        &myWinData
    );

    if (!hWnd)
    {
        MessageBox(NULL, TEXT("Call to CreateWindow failed!"), TEXT("Win32 Guided Tour"), NULL);
        return TRUE;
    }
	    
    ShowWindow(hWnd, nCmdShow); // Show the window that we created earlier
    UpdateWindow(hWnd);

	// We're going to show to call a COM object my using a class to isolate some COM functionality.
	ComUtility com_util = ComUtility();
	com_util.Initialize();

	LPWSTR pszFilePath = com_util.OpenFileDialog(true); // true means use the IUnknown->QueryInterface method within...
	
	if( lstrcmpiW( pszFilePath,L"") != 0 )
	{
		MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
		CoTaskMemFree(pszFilePath); // Interesting note here about CoTaskMemFree is that its used to deallocate the memory that was assigned to the 
		// pszFilePath variable by the call to OpenFileDialog. Basically if a com object adds memory on the heap, we need to
		// deallocate it via the CoTaskMemFree call.
	}
	com_util.Uninitialize();

    // Main message loop, which will call LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); defined above.
	// Get message will look for messages and block if none are available for this application.
	// Dispatch message will actually call the Wndproc function and when it returns(wndProc) the while loop continues.
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))  // to infinity and beyond, well until PostQuitMessage() ends GetMessage's fun!
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

inline MyWindowData* GetAppState(HWND hwnd)
{
	LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
	MyWindowData *pData = reinterpret_cast<MyWindowData*>(ptr);
	return pData;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = TEXT("Hello, World!");
	MyWindowData* myWinData;

	if (message == WM_CREATE) //first message sent to this winodws procedure before the window is shown
	{
		// Extract our custom data from lparam and use it locally perhaps for initialization(we passed it in CreatWindow() )
		CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		myWinData = reinterpret_cast<MyWindowData*>(pCreate->lpCreateParams);

		// stores this in the instance data for this window
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)myWinData);

		// We can get it back anytime by calling:
		myWinData = GetAppState(hWnd);
		
	}

    switch (message)
    {	
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
		//ps.rcPaint ius the update region

        // Here your application's layout is setup.
		
        TextOut(hdc, 5, 5, greeting, _tcslen(greeting));
        // End application-specific layout section.

        EndPaint(hWnd, &ps);
        break;
	case WM_MOUSEWHEEL:
		MessageBox(hWnd, TEXT("You used the mouse wheel"), TEXT("Caption goes here"), NULL);
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_CLOSE:		
		if (MessageBox(hWnd, L"Do you really want to quit this wonderful c++ application?", L"Quit?", MB_OKCANCEL) == IDOK)
		{
			DestroyWindow(hWnd);
		}
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}