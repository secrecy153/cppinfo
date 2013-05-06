#define SAFE_EXTERN

#include "safe_ex.h"
#include "inipara.h"
#include "header.h"
#include "mhook-lib/mhook.h"
#include <process.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>

#ifdef _DEBUG
extern void __cdecl logmsg(const char * format, ...);
#endif

_NtSHGetSpecialFolderPathW TrueSHGetSpecialFolderPathW = NULL;
_NtLoadLibraryExW		   TrueLoadLibraryExW = NULL;

HANDLE NtCreateRemoteThread(HANDLE hProcess, 
							LPTHREAD_START_ROUTINE lpRemoteThreadStart, 
							LPVOID lpRemoteCallback
						   )
{
	 NT_PROC_THREAD_ATTRIBUTE_LIST Buffer; 
	 ZeroMemory(&Buffer, sizeof(NT_PROC_THREAD_ATTRIBUTE_LIST));
	 Buffer.Length = sizeof (NT_PROC_THREAD_ATTRIBUTE_LIST); 

	 _NtCreateThreadEx Win7CreateThread = (_NtCreateThreadEx)GetProcAddress
						   (GetModuleHandleW(L"ntdll.dll"), "NtCreateThreadEx"); 

	 if(Win7CreateThread == NULL) 
		return NULL;

	 HANDLE  hRemoteThread = NULL; 
	 HRESULT hRes = 0; 

	 if(!NT_SUCCESS(Win7CreateThread( 
				  &hRemoteThread, 
				  0x001FFFFF, // all access 
				  NULL, 
				  hProcess, 
				  (LPTHREAD_START_ROUTINE)lpRemoteThreadStart, 
				  lpRemoteCallback, 
				  0, 
				  0, 
				  0, 
				  0, 
				  &Buffer 
				  )))
	 { 
		return NULL; 
	 } 
	 return hRemoteThread; 
}

unsigned WINAPI InjectDll(void *mpara)
{
	BOOL		bRet		= FALSE;
	SIZE_T		size		= 0;
	PVOID		dll_buff	= NULL;
	NTSTATUS	status;
	wchar_t		dll_name[VALUE_LEN+1];
	PROCESS_INFORMATION pi = *(LPPROCESS_INFORMATION)mpara;
	if ( GetModuleFileNameW(dll_module,dll_name,VALUE_LEN) >0 )
	{
		size = (1 + wcslen(dll_name)) * sizeof(wchar_t);
	}
	if (!size)
	{
		return bRet;
	}
	status = TrueNtAllocateVirtualMemory(pi.hProcess,&dll_buff,0,&size,MEM_COMMIT | MEM_TOP_DOWN,
										PAGE_EXECUTE_READWRITE);
	if ( NT_SUCCESS(status) )
	{
		status = TrueNtWriteVirtualMemory(pi.hProcess,dll_buff,dll_name,size,(PULONG)&size);
		if ( NT_SUCCESS(status) )
		{
			HANDLE hRemote;
			if (!RemoteLoadW)
			{
				RemoteLoadW  = (_NtRemoteLoadW)GetProcAddress(GetModuleHandleW(L"kernel32.dll"),
								"LoadLibraryW");	
			}
			hRemote = CreateRemoteThread(pi.hProcess,NULL,0,
					 (LPTHREAD_START_ROUTINE)RemoteLoadW,(LPVOID)dll_buff,0,NULL);
			if ( NULL == hRemote && 0x5 == GetLastError())
			{
				/* NtCreateThreadEx (Vista or Win7 and above is supported) */
				hRemote = NtCreateRemoteThread(pi.hProcess, (LPTHREAD_START_ROUTINE)RemoteLoadW, 
						 (LPVOID)dll_buff);
			}
			if (hRemote)
			{
				bRet = TRUE;
				WaitForSingleObject(hRemote,INFINITE);
				CloseHandle( hRemote );
			}
		}
		#ifdef _DEBUG
		else
		{
			logmsg("NtWriteProcessMemory() false,error code = %lu\n",status);
		}
		#endif
		size = 0;
		TrueNtFreeVirtualMemory(pi.hProcess,&dll_buff,&size,MEM_RELEASE);
		if ( !NT_SUCCESS(TrueNtResumeThread(pi.hThread,NULL)) )
		{
		#ifdef _DEBUG
			logmsg("TrueNtResumeThread() false\n");
		#endif
		}
	}
	#ifdef _DEBUG
	else
	{
		logmsg("NtVirtualAllocEx() false,error code = %lu\n",status);
	}
	#endif
	return (bRet);
}

BOOL WINAPI in_whitelist(LPCWSTR lpfile)
{
	wchar_t white_list[EXCLUDE_NUM][VALUE_LEN+1];
	int		i;
	BOOL    ret = FALSE;
	LPCWSTR pname = lpfile;
	/* iceweasel,plugin-container,plugin-hang-ui进程的路径 */
	GetModuleFileNameW(NULL,white_list[0],VALUE_LEN);
	GetModuleFileNameW(dll_module,white_list[1],VALUE_LEN);
	PathRemoveFileSpecW(white_list[1]);
	PathAppendW(white_list[1],L"plugin-container.exe");
	GetModuleFileNameW(dll_module,white_list[2],VALUE_LEN);
	PathRemoveFileSpecW(white_list[2]);
	PathAppendW(white_list[2],L"plugin-hang-ui.exe");
	if (lpfile[0] == L'"')
	{
		pname = &lpfile[1];
	}
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
			if (_wcsnicmp(white_list[i],pname,wcslen(white_list[i]))==0)
			{
				ret = TRUE;
				break;
			}
		}
	}
	return ret;
}

ULONG_PTR WINAPI GetParentProcess(HANDLE hProcess)
{
	ULONG_PTR	dwParentPID;
	NTSTATUS	status;
	PROCESS_BASIC_INFORMATION pbi;
	status = TrueNtQueryInformationProcess( hProcess,
											ProcessBasicInformation,
											(PVOID)&pbi,
											sizeof(PROCESS_BASIC_INFORMATION),
											NULL );

	if ( NT_SUCCESS(status) )
		dwParentPID = pbi.InheritedFromUniqueProcessId;
	else
		dwParentPID = 0;
	return dwParentPID;
}

BOOL WINAPI ProcessIsCUI(LPCWSTR lpfile)
{
	wchar_t lpname[VALUE_LEN+1];
	LPCWSTR sZfile = lpfile;
	int     n;
	if (lpfile[0] == L'"')
	{
		sZfile = &lpfile[1];
	}
	if (GetModuleFileNameW(NULL,lpname,VALUE_LEN))
	{
		if ( _wcsnicmp(lpname,sZfile,wcslen(lpname)) == 0 )
			return FALSE;
		PathRemoveFileSpecW(lpname);
		PathAppendW(lpname,L"plugin-container.exe");
		if ( _wcsnicmp(lpname,sZfile,wcslen(lpname)) == 0 )
			return FALSE;
		PathRemoveFileSpecW(lpname);
		PathAppendW(lpname,L"plugin-hang-ui.exe");
		if ( _wcsnicmp(lpname,sZfile,wcslen(lpname)) == 0 )
			return FALSE;
	}
	ZeroMemory(lpname,sizeof(lpname));
	if ( lpfile[0] == L'"' )
	{
		for ( n = 0; *sZfile != L'"'; ++n )
		{
			lpname[n] = *sZfile;
			sZfile++;
		}
	}
	else
	{
		wcsncpy(lpname,sZfile,VALUE_LEN);
	}
	if ( wcslen(lpname)>3 )
	{
		return ( !IsGUI(lpname) );
	}
	return TRUE;
}

NTSTATUS WINAPI HookNtWriteVirtualMemory(IN HANDLE ProcessHandle,
										IN PVOID BaseAddress,
										IN PVOID Buffer, 
										IN ULONG NumberOfBytesToWrite,
										OUT PULONG NumberOfBytesWritten)
{
	if ( GetCurrentProcessId() == GetProcessId(ProcessHandle) )
	{
		return STATUS_ERROR;
	}
	return TrueNtWriteVirtualMemory(ProcessHandle,
									BaseAddress,
									Buffer, 
									NumberOfBytesToWrite,
									NumberOfBytesWritten);
}

NTSTATUS NTAPI HookNtReadVirtualMemory (IN HANDLE ProcessHandle,
										IN PVOID BaseAddress,
										OUT PVOID Buffer,
										IN ULONG NumberOfBytesToRead,
										OUT PULONG NumberOfBytesRead)
{
	ULONG_PTR        dwCurrentPid	= GetCurrentProcessId();
	ULONG_PTR        dwParent		= GetParentProcess(ProcessHandle);
	if ( (dwCurrentPid>4 && dwParent == dwCurrentPid) )
	{
		return TrueNtReadVirtualMemory(ProcessHandle,BaseAddress,Buffer,
				NumberOfBytesToRead,NumberOfBytesRead);
	}
	return STATUS_ERROR;
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
	PROCESS_INFORMATION ProcessInformation;
	NTSTATUS	status;
	BOOL		tohook	= FALSE;
	ZeroMemory(&mY_ProcessParameters,sizeof(RTL_USER_PROCESS_PARAMETERS));
	if ( stristrW(ProcessParameters->ImagePathName.Buffer, L"SumatraPDF.exe") || 
		 stristrW(ProcessParameters->ImagePathName.Buffer, L"java.exe") ||
		 stristrW(ProcessParameters->ImagePathName.Buffer, L"jp2launcher.exe"))
	{
		tohook = TRUE;
	}
	else if ( read_appint(L"General",L"EnableWhiteList") )
	{
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
	}
	else if ( !IsGUI((LPCWSTR)ProcessParameters->ImagePathName.Buffer) )
	{
	#ifdef _DEBUG
		logmsg("CUI process,disabled-runes\n");
	#endif
		ProcessParameters = &mY_ProcessParameters;
	}
	status = TrueNtCreateUserProcess(ProcessHandle, ThreadHandle,
								  ProcessDesiredAccess, ThreadDesiredAccess,
								  ProcessObjectAttributes, ThreadObjectAttributes,
								  CreateProcessFlags, CreateThreadFlags, ProcessParameters,
								  CreateInfo, AttributeList);
	if ( NT_SUCCESS(status)&&tohook)
	{
		ULONG Suspend;
		ZeroMemory(&ProcessInformation,sizeof(PROCESS_INFORMATION));
		ProcessInformation.hProcess = *ProcessHandle;
		ProcessInformation.hThread = *ThreadHandle;
		if ( NT_SUCCESS(TrueNtSuspendThread(ProcessInformation.hThread,&Suspend)) )
		{
			InjectDll(&ProcessInformation);
		#ifdef _DEBUG
			logmsg("ready to  dll hook .\n");
		#endif
		}
	}
	return status;
}

BOOL WINAPI HookCreateProcessInternalW (HANDLE hToken,
										LPCWSTR lpApplicationName,
										LPWSTR lpCommandLine,
										LPSECURITY_ATTRIBUTES lpProcessAttributes,
										LPSECURITY_ATTRIBUTES lpThreadAttributes,
										BOOL bInheritHandles,
										DWORD dwCreationFlags,
										LPVOID lpEnvironment,
										LPCWSTR lpCurrentDirectory,
										LPSTARTUPINFOW lpStartupInfo,
										LPPROCESS_INFORMATION lpProcessInformation,
										PHANDLE hNewToken)
{
	BOOL	ret		= FALSE;
	LPWSTR	lpfile	= lpCommandLine;
	BOOL    tohook	= FALSE;
	if (lpApplicationName && wcslen(lpApplicationName)>1)
	{
		lpfile = (LPWSTR)lpApplicationName;
	}
	/* 禁止启动16位程序 */
	if (dwCreationFlags&CREATE_SHARED_WOW_VDM || dwCreationFlags&CREATE_SEPARATE_WOW_VDM)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
        return ret;
	}
	/* 存在不安全插件,注入保护 */
	if ( stristrW(lpfile, L"SumatraPDF.exe") || 
		 stristrW(lpfile, L"java.exe") ||
		 stristrW(lpfile, L"jp2launcher.exe"))
	{
		dwCreationFlags |= CREATE_SUSPENDED;
		tohook = TRUE;
	}
	/* 如果启用白名单制度(严格检查) */
	else if ( read_appint(L"General",L"EnableWhiteList") )
	{
		if ( !in_whitelist((LPCWSTR)lpfile) )
		{
		#ifdef _DEBUG
			logmsg("the process %ls disabled-runes\n",lpfile);
		#endif
			SetLastError( TrueRtlNtStatusToDosError(STATUS_ERROR) );
			return ret;
		}
	}
	/* 如果不启用白名单,则自动阻止命令行程序启动 */
	else if ( ProcessIsCUI(lpfile) )
	{
		{
		#ifdef _DEBUG
			logmsg("CUI process, disabled-runes\n");
		#endif
			SetLastError( TrueRtlNtStatusToDosError(STATUS_ERROR) );
			return ret;
		}
	}
	ret =  TrueCreateProcessInternalW(hToken,lpApplicationName,lpCommandLine,lpProcessAttributes,
		   lpThreadAttributes,bInheritHandles,dwCreationFlags,lpEnvironment,lpCurrentDirectory,
		   lpStartupInfo,lpProcessInformation,hNewToken);
	/* 远程注入进程 */
	if ( ret && tohook )
	{
		InjectDll(lpProcessInformation);
	#ifdef _DEBUG
		logmsg("ready to  dll hook .\n");
	#endif
	}
	return ret;
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
		if (!TrueSHGetSpecialFolderPathW)
		{
			TrueSHGetSpecialFolderPathW = (_NtSHGetSpecialFolderPathW)GetProcAddress
										  (GetModuleHandleW(L"shell32.dll"),"SHGetSpecialFolderPathW");
		}
	#ifdef _M_IX86
		if ( TrueSHGetSpecialFolderPathW(NULL, sysdir, 0x0029, FALSE) )
	#elif defined _M_X64
		if ( TrueSHGetSpecialFolderPathW(NULL, sysdir, 0x0025, FALSE) )
	#endif
		{
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
    UINT_PTR dwCaller;
	/* 是否信任的dll */
	if ( iSAuthorized(lpFileName) )
	{
		return TrueLoadLibraryExW(lpFileName, hFile, dwFlags);
	}
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
	wchar_t		process_name[VALUE_LEN+1];
	DWORD		ver = GetOsVersion();
	GetModuleFileNameW(NULL,process_name,VALUE_LEN);
	if (stristrW(process_name, L"Iceweasel.exe")||
		stristrW(process_name, L"firefox.exe")	||
		stristrW(process_name, L"lawlietfox.exe") )
	{
		ZeroMemory(&user32ModInfo,sizeof(user32ModInfo));
		TrueLoadLibraryExW = (_NtLoadLibraryExW)GetProcAddress(GetModuleHandleW(L"kernel32.dll"),
							 "LoadLibraryExW");
		TrueGetModuleInformation = (_NtGetModuleInformation)GetProcAddress(
									TrueLoadLibraryExW(L"psapi.dll",NULL,LOAD_LIBRARY_AS_DATAFILE),
									"GetModuleInformation");
		if (TrueGetModuleInformation(GetCurrentProcess(), GetModuleHandleA("user32.dll"),   
							 &user32ModInfo, sizeof(user32ModInfo)))
		{
			m_dwUser32Low = (UINT_PTR)user32ModInfo.lpBaseOfDll; 
			m_dwUser32Hi = (UINT_PTR)user32ModInfo.lpBaseOfDll+user32ModInfo.SizeOfImage; 
		}
	}
	hNtdll = GetModuleHandleW(L"ntdll.dll");
	if (hNtdll)
	{
		TrueNtclose						= (_NtCLOSE)GetProcAddress
										  (hNtdll, "NtClose");
		TrueNtSuspendThread				= (_NtSuspendThread)GetProcAddress
										  (hNtdll, "NtSuspendThread");
		TrueNtResumeThread				= (_NtResumeThread)GetProcAddress
										  (hNtdll, "NtResumeThread");
		TrueNtTerminateProcess			= (_NtTerminateProcess)GetProcAddress
										  (hNtdll, "NtTerminateProcess");
		TrueNtQueryInformationProcess	= (_NtQueryInformationProcess)GetProcAddress(hNtdll,
										  "NtQueryInformationProcess");
		TrueNtWriteVirtualMemory		= (_NtWriteVirtualMemory)GetProcAddress(hNtdll,
										  "NtWriteVirtualMemory");
		TrueNtFreeVirtualMemory			= (_NtFreeVirtualMemory)GetProcAddress(hNtdll,
										  "NtFreeVirtualMemory");
		TrueNtAllocateVirtualMemory		= (_NtAllocateVirtualMemory)GetProcAddress(hNtdll,
										  "NtAllocateVirtualMemory");
		TrueNtReadVirtualMemory			= (_NtReadVirtualMemory)GetProcAddress(hNtdll,
										  "NtReadVirtualMemory");
		TrueRtlNtStatusToDosError		= (_RtlNtStatusToDosError)GetProcAddress(hNtdll,
										  "RtlNtStatusToDosError");
		if (ver>601)  /* win8 */
		{
			TrueNtCreateUserProcess      = (_NtCreateUserProcess)GetProcAddress(hNtdll, "NtCreateUserProcess");
			if (TrueNtCreateUserProcess)
			{
				Mhook_SetHook((PVOID*)&TrueNtCreateUserProcess, (PVOID)HookNtCreateUserProcess);
			}
		}
		else
		{
			TrueCreateProcessInternalW		= (_CreateProcessInternalW)GetProcAddress(
											  GetModuleHandleW(L"kernel32.dll"), "CreateProcessInternalW");
			if (TrueCreateProcessInternalW)
			{
				Mhook_SetHook((PVOID*)&TrueCreateProcessInternalW, (PVOID)HookCreateProcessInternalW);
			}
		}
	}
	if (TrueLoadLibraryExW)
	{
		Mhook_SetHook((PVOID*)&TrueLoadLibraryExW, (PVOID)HookLoadLibraryExW);
	}
	if (TrueNtReadVirtualMemory)
	{
		Mhook_SetHook((PVOID*)&TrueNtReadVirtualMemory, (PVOID)HookNtReadVirtualMemory);
	}
	if (TrueNtWriteVirtualMemory)
	{
		Mhook_SetHook((PVOID*)&TrueNtWriteVirtualMemory, (PVOID)HookNtWriteVirtualMemory);
	}
	return (1);
}

void safe_end(void)
{
	if (TrueLoadLibraryExW)
	{
		Mhook_Unhook((PVOID*)&TrueLoadLibraryExW);
	}
	if (TrueCreateProcessInternalW)
	{
		Mhook_Unhook((PVOID*)&TrueCreateProcessInternalW);
	}
	if (TrueNtCreateUserProcess)
	{
		Mhook_Unhook((PVOID*)&TrueNtCreateUserProcess);
	}
	if (TrueNtReadVirtualMemory)
	{
		Mhook_Unhook((PVOID*)&TrueNtReadVirtualMemory);
	}
	if (TrueNtWriteVirtualMemory)
	{
		Mhook_Unhook((PVOID*)&TrueNtWriteVirtualMemory);
	}
	return;
}