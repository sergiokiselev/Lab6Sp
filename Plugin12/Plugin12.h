#include "stdafx.h"

#ifdef __cplusplus
extern "C" {
#endif

BOOLEAN GetAuthor(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten); // ����� �������

BOOLEAN GetDescription(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten); // �������� ������

VOID Execute(); 

BOOL TraverseRegistry();
BOOL DisplayPair(LPTSTR ValueName, DWORD ValueType, LPBYTE Value, DWORD ValueLen);

#ifdef __cplusplus
}
#endif 