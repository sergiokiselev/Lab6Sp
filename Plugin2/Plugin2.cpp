// Plugin2.cpp: ���������� ���������������� ������� ��� ���������� DLL.
//

#include "stdafx.h"
#include "Plugin2.h"

#ifdef __cplusplus
extern "C" {
#endif

	int language = 0;
const char* locale[2][3] = {{"\nAuthor: Sergio Kiselev \n","\nDescription: Video hardware \n",""},{"\n�����: ������ ������� \n","\n��������: ����� ��������� \n",""}};

BOOLEAN GetAuthor(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten){
		strcpy_s(buffer, dwBufferSize, locale[language][0]);

		if(strlen(buffer) > 0){
			*pdwBytesWritten = strlen(buffer);
			return TRUE;
		}
		else
			return false;
	}// ����� �������

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
	}// �������� ������

VOID Execute(){
	HKEY hSubK;
	DWORD ValType, Index, NumSubKs, ValNameLen, ValLen;
	DWORD MaxSubKLen, NumVals, MaxValNameLen, MaxValLen;
	FILETIME LastWriteTime;
	LPTSTR SubKName, ValName;
	LPBYTE Val;
	
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\VIDEO", 0, KEY_READ, &hSubK);
	
	RegQueryInfoKey(hSubK, NULL, NULL, NULL, &NumSubKs, &MaxSubKLen, NULL, &NumVals, &MaxValNameLen, &MaxValLen, NULL, &LastWriteTime);
	SubKName = (LPTSTR)malloc (MaxSubKLen+1); /* ������ ��� ����� ������������ �������� �������. */
	ValName = (LPTSTR)malloc(MaxValNameLen+1); /* ������ ������� ������. */
	Val = (LPBYTE)malloc(MaxValLen); /* ������ � ������. */
	
	for (Index = 0; Index < NumVals; Index++) {
		ValNameLen = MaxValNameLen + 1; /* ��������������� ������ ���! */
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
	/* �������, ������������ ���� "���-��������". */
{
	LPBYTE pV = Value;
	DWORD i;
	printf("\nValue: %s = ", ValueName);
	switch (ValueType) {
	case REG_FULL_RESOURCE_DESCRIPTOR: /* 9: �������� ������������. */
	case REG_BINARY: /* 3: ����� �������� ������. */
		for (i = 0; i < ValueLen; i++, pV++) printf(" %x", *pV);
		break;
	case REG_DWORD: /* 4: 32-������� �����. */
		printf("%x", (DWORD)*Value);
		break;
		//case REG_MULTI SZ: /*7: ������ �����, ������������� ������� ��������.*/
	case REG_SZ: /* 1: ������, ������������� ������� ��������. */
		printf("%s", (LPTSTR)Value);
		break;
		/* � ��������� ������ ����� � */
	}
	return TRUE;
}

#ifdef __cplusplus
}
#endif 