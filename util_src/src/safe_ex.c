#define SAFE_EXTERN

#include "safe_ex.h"
#include "inipara.h"
#include "header.h"
#include "mhook-lib/mhook.h"
#include <tchar.h>
#include <process.h>
#include <stdio.h>

#ifdef _DEBUG
extern void __cdecl logmsg(const char * format, ...);
#endif

BOOL WINAPI GetFileByHandle(PHANDLE phandle,LPWSTR lpFileName)
{
	BOOL	ret = FALSE;
	HANDLE  h_handle = *phandle;
	if (TrueNtQueryInformationProcess)
	{
		NTSTATUS	status;
		ULONG		returnedLength = 0;
		PVOID       pi = NULL;
		status = TrueNtQueryInformationProcess( h_handle,
                                    ProcessImageFileName,
                                    NULL,
                                    0,
				                    &returnedLength);
		if (STATUS_INFO_LENGTH_MISMATCH != status && returnedLength ==0) 
		{
			return ret;
		}
		pi = SYS_MALLOC(returnedLength);
		if (NT_SUCCESS(TrueNtQueryInformationProcess( h_handle,ProcessImageFileName,pi,
					  returnedLength,&returnedLength)) )
		{
			UNICODE_STRING buf = *(PUNICODE_STRING)pi;
			if (buf.Length>0)
            {
				UINT			DriveStrLen = 0;
				wchar_t			dummy[1];
				wchar_t			szVolumeName[3] = {0};
				DriveStrLen =	GetLogicalDriveStringsW (0, dummy);
                wchar_t			szTemp[DriveStrLen+1];
                if(GetLogicalDriveStringsW(DriveStrLen,szTemp))
                {
                    BOOL bFound = FALSE;
                    wchar_t *p = szTemp;
                    do
                    {
						wchar_t szDrive[3]={0};
						wchar_t szName[VALUE_LEN+1];
                        wcsncpy(szDrive,p,2);
                        if(QueryDosDeviceW(szDrive,szName,VALUE_LEN))
                        {
                            UINT uNameLen = wcslen(szName);
                            if(uNameLen<VALUE_LEN)
                            {
                                bFound=wcsnicmp(buf.Buffer,szName,uNameLen)==0;
                                if(bFound)
                                {
                                    _snwprintf(lpFileName,MAX_PATH,L"%ls%ls",szDrive,buf.Buffer+uNameLen);
                                    ret = TRUE;
                                }
                            }
                        }
                        while(*p++)
							;
                    } while(!bFound&&*p);
                }
            }
		}
		if (pi)
		{
			SYS_FREE(pi);
		}
	}
	return ret;
}

BOOL WINAPI in_whitelist(LPCWSTR lpfile)
{
	wchar_t white_list[EXCLUDE_NUM][VALUE_LEN+1];
	int		i;
	BOOL    ret = FALSE;
	/* iceweasel,plugin-container,plugin-hang-ui进程的路径 */
	GetModuleFileNameW(NULL,white_list[0],VALUE_LEN);
	GetModuleFileNameW(dll_module,white_list[1],VALUE_LEN);
	PathRemoveFileSpecW(white_list[1]);
	PathAppendW(white_list[1],L"plugin-container.exe");
	GetModuleFileNameW(dll_module,white_list[2],VALUE_LEN);
	PathRemoveFileSpecW(white_list[2]);
	PathAppendW(white_list[2],L"plugin-hang-ui.exe");
	if ( for_eachSection(L"whitelist", &white_list[3], EXCLUDE_NUM-3) )
	{
		for ( i=0; i<EXCLUDE_NUM ; i++ )
		{
			if (wcslen(white_list[i]) == 0)
			{
				continue;
			}
			if (white_list[i][1] != L':')
			{
				PathToCombineW(white_list[i],VALUE_LEN);
			}
			if (_wcsnicmp(white_list[i],lpfile,wcslen(lpfile))==0)
			{
				ret = TRUE;
				break;
			}
		}
	}
	return ret;
}

NTSTATUS WINAPI HookNtCreateUserProcess(PHANDLE ProcessHandle,PHANDLE ThreadHandle,
								  ACCESS_MASK ProcessDesiredAccess,ACCESS_MASK ThreadDesiredAccess,
								  POBJECT_ATTRIBUTES ProcessObjectAttributes,
								  POBJECT_ATTRIBUTES ThreadObjectAttributes,
								  ULONG CreateProcessFlags,
								  ULONG CreateThreadFlags,
								  PRTL_USER_PROCESS_PARAMETERS ProcessParameters,
								  PVOID CreateInfo,
								  PNT_PROC_THREAD_ATTRIBUTE_LIST AttributeList)
{
	RTL_USER_PROCESS_PARAMETERS mY_ProcessParameters;
	ZeroMemory(&mY_ProcessParameters,sizeof(RTL_USER_PROCESS_PARAMETERS));
	if ( ProcessParameters->ImagePathName.Length > 0 && 
		in_whitelist((LPCWSTR)ProcessParameters->ImagePathName.Buffer) )
	{
	#ifdef _DEBUG
		logmsg("the process %ls in whitelist\n",ProcessParameters->ImagePathName.Buffer);
	#endif
	}
	else
	{
	#ifdef _DEBUG
		logmsg("the process %ls disabled-runes\n",ProcessParameters->ImagePathName.Buffer);
	#endif
		ProcessParameters = &mY_ProcessParameters;
	}
	return TrueCreateUserProcess(ProcessHandle, ThreadHandle,
								  ProcessDesiredAccess, ThreadDesiredAccess,
								  ProcessObjectAttributes, ThreadObjectAttributes,
								  CreateProcessFlags, CreateThreadFlags, ProcessParameters,
								  CreateInfo, AttributeList);
}

NTSTATUS WINAPI HookNtCreateProcessEx(OUT PHANDLE ProcessHandle,
									  IN ACCESS_MASK DesiredAccess,
									  IN POBJECT_ATTRIBUTES ObjectAttributes,
									  IN HANDLE ParentProcess,
									  IN ULONG	Flags,
									  IN HANDLE SectionHandle,
									  IN HANDLE DebugPort,
									  IN HANDLE ExceptionPort,
									  IN BOOLEAN InJob)
{
	NTSTATUS status = TrueNtCreateProcessEx(ProcessHandle,DesiredAccess,
					  ObjectAttributes,ParentProcess,Flags,SectionHandle,
					  DebugPort,ExceptionPort,InJob);
	if (NT_SUCCESS(status) && DesiredAccess == PROCESS_ALL_ACCESS)
	{
		wchar_t lpFileName[MAX_PATH+1] = {0};
		if (GetFileByHandle(ProcessHandle,lpFileName))
		{
			if ( wcslen(lpFileName) > 0 && in_whitelist((LPCWSTR)lpFileName) )
			{
			#ifdef _DEBUG
				logmsg("the process %ls in whitelist\n",lpFileName);
			#endif
			}
			else
			{
			#ifdef _DEBUG
				logmsg("the process %ls disabled-runes\n",lpFileName);
			#endif
				TrueNtTerminateProcess(*ProcessHandle, 0);
				return STATUS_ERROR;
			}
		}
	}
	return status;
}

BOOL WINAPI iSAuthorized(LPCWSTR lpFileName)
{
	BOOL	ret = FALSE;
	LPWSTR	filename = NULL;
	wchar_t *szAuthorizedList[] = {L"comctl32.dll", L"uxtheme.dll", L"indicdll.dll",
								   L"msctf.dll",L"shell32.dll", L"imageres.dll",
								   L"winmm.dll",L"ole32.dll", L"oleacc.dll", 
								   L"oleaut32.dll",L"secur32.dll",L"shlwapi.dll",
								   L"ImSCTip.DLL",L"gdi32.dll"
								  };
	WORD line = sizeof(szAuthorizedList)/sizeof(szAuthorizedList[0]);
	if (lpFileName[1] == L':')
	{
		wchar_t sysdir[MAX_PATH];
		if ( (GetEnvironmentVariableW(L"SystemRoot",sysdir,sizeof(sysdir)) > 0) )
		{
			PathAppendW(sysdir,L"system32");
			if ( _wcsnicmp(lpFileName,sysdir,wcslen(sysdir)) == 0 )
			{
				filename = PathFindFileNameW(lpFileName);
			}
		}
	}
	else
	{
		filename = (LPWSTR)lpFileName;
	}
	if (filename)
	{
		WORD  i;
		for(i=0;i<line;i++)
		{
			if ( _wcsicmp(filename,szAuthorizedList[i]) == 0 )
			{
				ret = TRUE;
				break;
			}
		}
	}
	return ret;
}

HMODULE WINAPI HookLoadLibraryExW(LPCWSTR lpFileName,HANDLE hFile,DWORD dwFlags)  
{  
    /* 获取函数的返回地址 */
	UINT_PTR dwCaller;
	wchar_t sysdir[MAX_PATH];
	/* 是否信任的dll */
	if ( iSAuthorized(lpFileName) )
		return TrueLoadLibraryExW(lpFileName, hFile, dwFlags);
	#ifdef __GNUC__
		dwCaller = (UINT_PTR)__builtin_return_address(0);
	#else
		__asm push dword ptr [ebp+4]
		__asm pop  dword ptr [dwCaller]
	#endif
    /* 判断是否是从User32.dll调用 */
    if(dwCaller > m_dwUser32Low && dwCaller < m_dwUser32Hi)  
    {
	#ifdef _DEBUG
		logmsg("the  %ls disable load\n",lpFileName);
	#endif
        return NULL;  
    } 
    return TrueLoadLibraryExW(lpFileName, hFile, dwFlags);  
}


unsigned WINAPI init_safed(void * pParam)
{
	MODULEINFO	user32ModInfo;
	HMODULE		hNtdll;
	DWORD		ver = GetOsVersion();
	ZeroMemory(&user32ModInfo,sizeof(user32ModInfo));
    if (GetModuleInformation(GetCurrentProcess(), GetModuleHandleA("user32.dll"),   
                         &user32ModInfo, sizeof(user32ModInfo)))
    {
		m_dwUser32Low = (UINT_PTR)user32ModInfo.lpBaseOfDll; 
		m_dwUser32Hi = (UINT_PTR)user32ModInfo.lpBaseOfDll+user32ModInfo.SizeOfImage; 
	}
	TrueLoadLibraryExW = (_NtLoadLibraryExW)GetProcAddress(GetModuleHandleW(L"kernel32.dll"),
						 "LoadLibraryExW");
	hNtdll = GetModuleHandleW(L"ntdll.dll");
	if (hNtdll)
	{
		TrueNtclose						= (_NtCLOSE)GetProcAddress
										  (hNtdll, "NtClose");
	    TrueNtQuerySection				= (_NtQuerySection)GetProcAddress 
										  (hNtdll, "NtQuerySection");
		TrueNtTerminateProcess			= (_NtTerminateProcess)GetProcAddress
										  (hNtdll, "NtTerminateProcess");
		TrueNtQueryInformationProcess	= (_NtQueryInformationProcess)GetProcAddress(hNtdll,
										  "NtQueryInformationProcess");
		if (ver>502)
		{
			TrueCreateUserProcess       = (_NtCreateUserProcess)GetProcAddress
										  (hNtdll, "NtCreateUserProcess");
		}
		else
		{
			TrueNtCreateProcessEx			= (_NtCreateProcessEx)GetProcAddress(hNtdll,
											  "NtCreateProcessEx");
		}
	}
	if (TrueLoadLibraryExW)
	{
		Mhook_SetHook((PVOID*)&TrueLoadLibraryExW, (PVOID)HookLoadLibraryExW);
	}
	if (ver>502)
	{
		if (TrueCreateUserProcess)
		{
			Mhook_SetHook((PVOID*)&TrueCreateUserProcess, (PVOID)HookNtCreateUserProcess);
		}
	}
	else
	{
		if (TrueNtCreateProcessEx)
		{
			Mhook_SetHook((PVOID*)&TrueNtCreateProcessEx, (PVOID)HookNtCreateProcessEx);
		}
	}
	return (1);
}

void safe_end(void)
{
	if (TrueLoadLibraryExW)
	{
		Mhook_Unhook((PVOID*)&TrueLoadLibraryExW);
	}
	if (TrueNtCreateProcessEx)
	{
		Mhook_Unhook((PVOID*)&TrueNtCreateProcessEx);
	}
	if (TrueCreateUserProcess)
	{
		Mhook_Unhook((PVOID*)&TrueCreateUserProcess);
	}
	return;
}