// consoleMenuDLL.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "consoleMenuDLL.h"

// mainDLL.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include <stdlib.h>
#include <malloc.h>


HMODULE hModule;
int language;
//bool loaded;
int plugins_count;
char** plugins_list;
const char* locale[2][12] = {{"Plugins list:\n","Plugin unloaded\n","No plugins were loaded yet\n","\n1 Show plugins list", "\n2 Show info about plugins",
	"\n3 Load plugin", "\n4 Unload plugin", "\n5 Execute","\nEnter plugin name\n", "\nPlugin loaded\n","\nPlugin can't be loaded\n","\nChoose ation\n"},
{"Список плагинов:\n","Плагин выгружен\n","Нет загруженных плагинов\n","\n1 Показать список плагинов","\n2 Показать информацию о плагинах","\n3 Загрузить плагин",
"\n4 Выгрузить пллагин","\n5 Выполнить","\nВведите имя плагина\n","\nПлагин загружен\n","\nПлагин не может быть загружен\n","\nВыберите действие:\n"}};


typedef BOOLEAN (*GetAuthorType)(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten);
typedef BOOLEAN (*GetDescriptionType)(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten);
typedef VOID (*ExecuteType)();
typedef VOID (*SetLanguageType)(int lang);


GetDescriptionType GetDescription;
GetAuthorType GetAuthor;
ExecuteType Execute;
SetLanguageType SetLanguage;

bool load_plugin(const char* plugin_name);  //
void print_menu();
void show_info_about_plugin(const char* plugin_name);  //
void show_info_about_plugins();  //
void show_plugins_list();  //
void unload_plugin();   //
void update_plugins_list();   //
int get_dlls_number();   //
void perform();  //



void print_menu(){
	char* action = (char*)calloc(50,sizeof(char));
	int act = 0;
	char* name = (char*)calloc(50,sizeof(char));

	printf("\nChoose language\n 0 - English\n 1 - Русский\n");
	char* lan = (char*)calloc(10, sizeof(char));

	gets_s(lan,10);

	language = atoi(lan);

	while(strcmp(action, "6") != 0){
		printf(locale[language][3]);
		printf(locale[language][4]);
		printf(locale[language][5]);
		printf(locale[language][6]);
		printf(locale[language][7]);
		printf(locale[language][11]);

		gets_s(action, 50);

		act = atoi(action);

		switch(act){
		case 1: 
			show_plugins_list();
			break;
		case 2:
			show_info_about_plugins();
			break;
		case 3:
			
			printf(locale[language][8]);
			gets_s(name, 50);

			if(load_plugin(name))
				printf(locale[language][9]);
			else
				printf(locale[language][10]);
			break;
		case 4:
			unload_plugin();
			break;
		case 5:
			perform();
			break;
		}
	}
}

void show_info_about_plugins(){
	int i = 0;
		
	update_plugins_list();
	for(i = 0; i < plugins_count; i++){
		show_info_about_plugin(plugins_list[i]);
	}
}

void show_info_about_plugin(const char* plugin_name){
	char* name = (char*)calloc(50, sizeof(char));
	HMODULE hModule2 = LoadLibrary(plugin_name);
	int error = GetLastError();
	if(hModule2){
		printf("%s\n",plugin_name);
		GetAuthor = (GetAuthorType)GetProcAddress(hModule2, "GetAuthor");
		GetDescription = (GetDescriptionType)GetProcAddress(hModule2, "GetDescription");
		char* authorname = (char*)calloc(50, sizeof(char));
		char* description = (char*)calloc(200, sizeof(char));
		DWORD dwAuthorLen = 0;
		DWORD dwDescrLen = 0;

		GetAuthor(authorname, 50, &dwAuthorLen);
		GetDescription(description, 50, &dwDescrLen);

		printf("%s", authorname);
		printf("%s\n", description);
		FreeLibrary(hModule2);
	}
}

void perform(){
	Execute();
}

bool load_plugin(const char* plugin_name){
	hModule = LoadLibrary(plugin_name);

	if(hModule){
		Execute = (ExecuteType)GetProcAddress(hModule, "Execute");
		SetLanguage = (SetLanguageType)GetProcAddress(hModule, "SetLanguage");

		SetLanguage(language);

		return true;
	}
	else return false;
}

void show_plugins_list(){
	update_plugins_list();
	int i = 0;
	printf("\n\n%s", locale[language][0]);
	for(i = 0; i < plugins_count; i++){
		printf("%s\n",plugins_list[i]);
	}
}

void unload_plugin(){
	if(hModule){
		FreeLibrary(hModule);
		printf("%s\n",locale[language][1]);
	}
	else
		printf("%s\n",locale[language][2]);
}

int get_dlls_number(){
	WIN32_FIND_DATA FindFileData;
	HANDLE hf;
	int result = 0;
	int len = 0;
	hf=FindFirstFile("d:\\SP_LAB_5_DLL\\Debug\\*", &FindFileData);
	char* filename = (char*)calloc(30, sizeof(char));
	char* filetype =(char*)calloc(20, sizeof(char));
	if (hf!=INVALID_HANDLE_VALUE)
	{
		do
		{
			strcpy_s(filename,30, (char*)FindFileData.cFileName);
			len = strlen(filename);
			if(len >  3){
				strcpy_s(filetype,10, &filename[len - 3]);
				if(strcmp(filetype, "dll") == 0){
					result++;
				}
			}
		}
		while (FindNextFile(hf,&FindFileData)!=0);
		FindClose(hf);
	}
	return result;
}

void update_plugins_list(){
	int number = get_dlls_number();
	plugins_count = number;
	int counter = 0;
	plugins_list = (char**)calloc(number, sizeof(char*));
	WIN32_FIND_DATA FindFileData;
	HANDLE hf;
	int result = 0;
	int len = 0;
	hf=FindFirstFile("d:\\SP_LAB_5_DLL\\Debug\\*", &FindFileData);
	char* filename = new char[30];
	char* filetype = new char[20];
	if (hf!=INVALID_HANDLE_VALUE)
	{
		do
		{
			strcpy_s(filename,30, (char*)FindFileData.cFileName);
			len = strlen(filename);
			if(len >  3){
				strcpy_s(filetype,10, &filename[len - 3]);
				if(strcmp(filetype, "dll") == 0 && strcmp(filename, "consoleMenuDLL.dll") != 0){
					plugins_list[counter] = (char*)calloc(strlen(filename)+1, sizeof(char));
					strcpy_s(plugins_list[counter],strlen(filename)+1, filename);
					counter++;
				}
			}
		}
		while (FindNextFile(hf,&FindFileData)!=0);
		FindClose(hf);
	}
}


