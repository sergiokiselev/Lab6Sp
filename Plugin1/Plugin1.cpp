// SPDLL.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "Plugin1.h"

#ifdef __cplusplus
extern "C" {
#endif

	int language = 0;
	const char* locale[2][3] = {{"\nAuthor: Sergio Kiselev \n","\nDescription: Windows version\n",""},{"\nАвтор: Сергей Киселев","\nОписание: Версия Windows",""}};

	BOOLEAN GetAuthor(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten){
		strcpy_s(buffer, dwBufferSize, locale[language][0]);

		if(strlen(buffer) > 0){
			*pdwBytesWritten = strlen(buffer);
			return TRUE;
		}
		else
			return false;
	}

	BOOLEAN GetDescription(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten){
		strcpy_s(buffer, dwBufferSize, locale[language][0]);

		if(strlen(buffer) > 0){
			*pdwBytesWritten = strlen(buffer);
			return TRUE;
		}
		else
			return false;
	}

	VOID Execute(){
		OSVERSIONINFO osversioninfo;
		osversioninfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&osversioninfo);

		printf("\nWindows version: %d.%d \n",osversioninfo.dwMajorVersion, osversioninfo.dwMinorVersion);
		printf("Build: %d\n", osversioninfo.dwBuildNumber);

	}

	VOID SetLanguage(int lang){
		language = lang;
	}
#ifdef __cplusplus
}
#endif 