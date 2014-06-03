#include <windows.h>
#include <conio.h>
#include <iostream>
#include <DbgHelp.h>

using namespace std;

int main()
{
	printf("Start of the server\n");
	while (true) 
	{
		BOOL fConnected = FALSE; 
		HANDLE hPipe = CreateNamedPipe("\\\\.\\pipe\\debug_pipe",PIPE_ACCESS_INBOUND,PIPE_TYPE_MESSAGE | PIPE_WAIT,1,0,0,INFINITE,(LPSECURITY_ATTRIBUTES)NULL);
		cout << hPipe << endl;
		//if(!ConnectNamedPipe(
		//	hPipe,    // дескриптор канала
		//	(LPOVERLAPPED)NULL  // связь синхронная
		//	))
		//{
		//	printf("Connection to the server failed\n");
		//}
		fConnected = ConnectNamedPipe(hPipe,NULL);
		cout << fConnected << endl;
		FILE* f;
		if (fConnected) 
		{
			printf("Connected\n");
			printf("%d\n",fopen_s(&f,"temp.dmp","w+b"));
			DWORD dwBytesRead, dwBytesWritten;
			DWORD size;
			int t = 0;
			ReadFile(hPipe,&size,sizeof(size),&dwBytesRead,(LPOVERLAPPED)NULL);
			printf("Dump size is %d bytes\n",size);
			char* buffer = new char[size + 1];
			ReadFile(hPipe,buffer,size,&dwBytesRead,(LPOVERLAPPED)NULL);
			printf("Dump obtained\n");
			fwrite(buffer, size, 1, f);
			fclose(f);
			cout << sizeof(buffer) << endl;
			system("cdb.exe -z temp.dmp -c \"!analyze -v; q\" -logo \"dump.log\" -y srv*C:\\Windows\\symbols*http://msdl.microsoft.com//download//symbols");
			delete [] buffer;
		}
		CloseHandle(hPipe);
		printf("===============================================================\n");
	}
	return 0;
}