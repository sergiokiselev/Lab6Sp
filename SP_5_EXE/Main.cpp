#include <stdio.h>
#include <Windows.h>
#include "consoleMenuDLL.h"
#include <DbgHelp.h>
#include <locale>

LONG WINAPI TopExceptionFilter(PEXCEPTION_POINTERS pExceptionPtrs);
void CreateMiniDump( EXCEPTION_POINTERS*);

int main()
{	
	setlocale(LC_ALL, "russian");
	__try
	{
		print_menu();
	}
	__except(TopExceptionFilter(GetExceptionInformation()))
	{

	}
}

LONG WINAPI  TopExceptionFilter(PEXCEPTION_POINTERS pExceptionPtrs)
{
	CreateMiniDump(pExceptionPtrs);
	return EXCEPTION_EXECUTE_HANDLER; 
}


void CreateMiniDump( EXCEPTION_POINTERS* pep ) 
{
	// Open the file 

	HANDLE hFile = CreateFile( "D:\\dumpForLab5\\Dump.dmp", GENERIC_READ | GENERIC_WRITE, 
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ); 
	HANDLE hNamedPipe;
	char lpszAppName[] = "D:\\SP_LAB_5_DLL\\Debug\\Server.exe";
	STARTUPINFO si;
	PROCESS_INFORMATION piApp;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	DWORD dwBytesWrite = 0;
	int size = 0;
	char* buffer;
	FILE* f;

	if( ( hFile != NULL ) && ( hFile != INVALID_HANDLE_VALUE ) ) 
	{

		if (!CreateProcess(lpszAppName, NULL, NULL, NULL, FALSE,
			CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp))
		{
			printf("Cannot create process\n");
		}
		Sleep(100);

		hNamedPipe = CreateFile("\\\\.\\pipe\\debug_pipe", GENERIC_WRITE, FILE_SHARE_READ,(LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, 0, (HANDLE)NULL);

		MINIDUMP_EXCEPTION_INFORMATION mdei; 

		mdei.ThreadId           = GetCurrentThreadId(); 
		mdei.ExceptionPointers  = pep; 
		mdei.ClientPointers     = FALSE; 

		MINIDUMP_TYPE mdt       = (MINIDUMP_TYPE)(MiniDumpWithFullMemory | 
			MiniDumpWithFullMemoryInfo | 
			MiniDumpWithHandleData | 
			MiniDumpWithThreadInfo | 
			MiniDumpWithUnloadedModules ); 

		BOOL rv = MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), 
			hFile, mdt, (pep != 0) ? &mdei : 0, 0, 0 ); 
		if( !rv ) 
			printf("MiniDumpWriteDump failed. Error: %u \n", GetLastError() ); 
		else
			printf("Minidump created.\n" ); 
		CloseHandle( hFile );

		if(rv){
			fopen_s(&f, "D:\\dumpForLab5\\Dump.dmp", "rb");

			fseek(f, 0, SEEK_END);
			size = ftell(f);
			fseek(f, 0, SEEK_SET);
			buffer = (char*)calloc(size + 1, sizeof(char));
			fread(buffer, size, 1, f);

			WriteFile(hNamedPipe, &size, sizeof(size), &dwBytesWrite, (LPOVERLAPPED)NULL);
			WriteFile(hNamedPipe, buffer, size, &dwBytesWrite, (LPOVERLAPPED)NULL);

		}
		WaitForSingleObject(piApp.hProcess, INFINITE);


		CloseHandle(piApp.hThread);
		CloseHandle(piApp.hProcess); 
		CloseHandle(hNamedPipe);
		if(buffer)
			free(buffer);
		fclose(f);
	}
	else 
	{
		printf( "CreateFile failed. Error: %u \n", GetLastError() ); 
	}

}
