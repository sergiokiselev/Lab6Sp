// Plugin2.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "Plugin2.h"

#ifdef __cplusplus
extern "C" {
#endif

	int language = 0;
const char* locale[2][3] = {{"\nAuthor: Sergio Kiselev \n","\nDescription: Video hardware \n",""},{"\nАвтор: Сергей Киселев \n","\nОписание: Видео утройства \n",""}};

BOOLEAN GetAuthor(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten){
		strcpy_s(buffer, dwBufferSize, locale[language][0]);

		if(strlen(buffer) > 0){
			*pdwBytesWritten = strlen(buffer);
			return TRUE;
		}
		else
			return false;
	}// автор плагина

VOID SetLanguage(int lang){
		language = lang;
	}

BOOLEAN GetDescription(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten){
		strcpy_s(buffer, dwBufferSize, locale[language][1]);

		if(strlen(buffer) > 0){
			*pdwBytesWritten = strlen(buffer);
			return TRUE;
		}
		else
			return false;
	}// описание плагин

VOID Execute(){
	HKEY hSubK;
	DWORD ValType, Index, NumSubKs, ValNameLen, ValLen;
	DWORD MaxSubKLen, NumVals, MaxValNameLen, MaxValLen;
	FILETIME LastWriteTime;
	LPTSTR SubKName, ValName;
	LPBYTE Val;
	
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\VIDEO", 0, KEY_READ, &hSubK);
	
	RegQueryInfoKey(hSubK, NULL, NULL, NULL, &NumSubKs, &MaxSubKLen, NULL, &NumVals, &MaxValNameLen, &MaxValLen, NULL, &LastWriteTime);
	SubKName = (LPTSTR)malloc (MaxSubKLen+1); /* Размер без учета завершающего нулевого символа. */
	ValName = (LPTSTR)malloc(MaxValNameLen+1); /* Учесть нулевой символ. */
	Val = (LPBYTE)malloc(MaxValLen); /* Размер в байтах. */
	
	for (Index = 0; Index < NumVals; Index++) {
		ValNameLen = MaxValNameLen + 1; /* Устанавливается каждый раз! */
		ValLen = MaxValLen + 1;
		RegEnumValue(hSubK, Index, ValName, &ValNameLen, NULL, &ValType, Val, &ValLen);
		DisplayPair(ValName, ValType, Val, ValLen);
	}
	printf("\n");
	free(SubKName);
	free(ValName);
	free(Val);
	RegCloseKey(hSubK);
}

BOOL DisplayPair(LPTSTR ValueName, DWORD ValueType, LPBYTE Value, DWORD ValueLen)
	/* Функция, отображающая пары "имя-значение". */
{
	LPBYTE pV = Value;
	DWORD i;
	printf("\nValue: %s = ", ValueName);
	switch (ValueType) {
	case REG_FULL_RESOURCE_DESCRIPTOR: /* 9: описание оборудования. */
	case REG_BINARY: /* 3: Любые двоичные данные. */
		for (i = 0; i < ValueLen; i++, pV++) printf(" %x", *pV);
		break;
	case REG_DWORD: /* 4: 32-битовое число. */
		printf("%x", (DWORD)*Value);
		break;
		//case REG_MULTI SZ: /*7: массив строк, завершающихся нулевым символом.*/
	case REG_SZ: /* 1: строка, завершающаяся нулевым символом. */
		printf("%s", (LPTSTR)Value);
		break;
		/* … Несколько других типов … */
	}
	return TRUE;
}

#ifdef __cplusplus
}
#endif 