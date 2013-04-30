#ifndef _SAFE_EX_H_
#  define _SAFE_EX_H_

#ifdef SAFE_EXTERN
#  undef SAFE_EXTERN
#  define SAFE_EXTERN
#else
#  define SAFE_EXTERN extern
#endif

#include <windows.h>

typedef HMODULE (WINAPI *_NtLoadLibraryExW)(LPCWSTR lpFileName,
									    HANDLE hFile, 
									    DWORD dwFlags);

typedef BOOL (WINAPI *_NtSHGetSpecialFolderPathW)(HWND hwndOwner,
									    LPWSTR lpszPath,
									    int csidl,
									    BOOL fCreate);

SAFE_EXTERN _NtLoadLibraryExW		   TrueLoadLibraryExW;
SAFE_EXTERN _NtSHGetSpecialFolderPathW TrueSHGetSpecialFolderPathW;

#ifdef __cplusplus
extern "C" {
#endif
SAFE_EXTERN unsigned WINAPI init_safed(void * pParam);
SAFE_EXTERN void safe_end(void);
#ifdef __cplusplus
}
#endif 

#endif   /* end _SAFE_EX_H_ */