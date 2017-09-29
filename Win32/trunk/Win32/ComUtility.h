#pragma once
#include <Windows.h>
namespace MyCOM
{
	class ComUtility
	{
	public:
		ComUtility(void);
		void Initialize();
		void Uninitialize();
		LPWSTR OpenFileDialog(bool use_custom);
		~ComUtility(void);
	private:
		bool com_init_sucess;
		template <class T> void SafeRelease(T **ppT);
		
	};
}
