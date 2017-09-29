#include "ComUtility.h"
#include <shobjidl.h> // This gets the CLSID GUID constants for the COM object IFileOpenDialog
#include <windows.h>
#include <atlbase.h> // Contains the declaration of CComPtr.

namespace MyCOM 
{
	ComUtility::ComUtility(void)
	{
	}


	ComUtility::~ComUtility(void)
	{
	}

	
	//Initialise the COM system and set the result obejct internally
	void ComUtility::Initialize()
	{
		// Usually use ApartmenThread if you bring up a window in this thread and use MultiThreaded in all other instances
		HRESULT com_init_outcome = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE ); // Not disabling OLD linking as last bitwise OR
		if (SUCCEEDED(com_init_outcome))
		{
			// The function succeeded.
			com_init_sucess = true;
		}
		else
		{
			// Handle the error.
			com_init_sucess = false;
		}		
	}

	LPWSTR ComUtility::OpenFileDialog(bool use_custom)
	{
		Initialize();
		if( !this->com_init_sucess )
			return L"";

		IFileOpenDialog *pFileOpen;
		HRESULT com_result = CoCreateInstance( __uuidof( FileOpenDialog ) , NULL, CLSCTX_ALL, IID_PPV_ARGS(&pFileOpen)); //use of IID_PPV_ARGS to eliminate reinterpre_cast and get the GUID! Yay!
		if( FAILED(com_result))
			return L"";
		com_result = pFileOpen->Show(NULL);
		if( FAILED( com_result ) )
			return L"";

		if( use_custom )
		{
			// This demonstrates how you can use an existing COM object to act as a conduit to return a
			// nother interface of the object - especially when the object inherites from two or more interfaces:
			CComPtr<IFileDialogCustomize> pCustom; // cunning use of a self releasing COM object pointer curtosy of ATL
			com_result = pFileOpen->QueryInterface( IID_PPV_ARGS(&pCustom)); // use of IID_PPV_ARGS to eliminate reinterpre_cast and get the GUID! Yay!
			if( SUCCEEDED(com_result))
			{
				// use the pCustom object 
							
			
			}
		} //pCustom goes out of scope and CComPtr nicely calls Release() if its valid to do so...so cool.

		// Get the file name from the dialog box.
		IShellItem* pItem;
		com_result = pFileOpen->GetResult(&pItem);
		if( FAILED( com_result ))
			return L"";

		LPWSTR pszFilePath;
        com_result = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
		if( FAILED(com_result))
			return L"";	
        
		// Superbly cunning use of the SafeRelease Pattern
		SafeRelease<IShellItem>(&pItem); // we could even have used CComPtrs here! Even better
		SafeRelease<IFileOpenDialog>(&pFileOpen);

		return pszFilePath;
		

	}

	void ComUtility::Uninitialize()
	{
		CoUninitialize();
	}

	// Safe release pattern in use here - 
	template <class T> void ComUtility::SafeRelease(T **ppT)
	{
		if (*ppT) // check if pointer valid, if so release it and null it
		{
			(*ppT)->Release();
			*ppT = NULL;
		}
	}
	

}