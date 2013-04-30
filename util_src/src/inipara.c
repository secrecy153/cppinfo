#define INI_EXTERN

#include "inipara.h"
#include <shlwapi.h>
#include <stdio.h>

extern HMODULE dll_module;

BOOL WINAPI ini_ready(LPWSTR inifull_name,size_t str_len)
{
	BOOL rect = FALSE;
	GetModuleFileNameW(dll_module,inifull_name,str_len);
	PathRemoveFileSpecW(inifull_name);
	PathAppendW(inifull_name,L"portable.ini");
	rect = PathFileExistsW(inifull_name);
	if (!rect)
	{
		if ( PathRemoveFileSpecW(inifull_name) )
		{
			PathAppendW(inifull_name,L"tmemutil.ini");
			rect = PathFileExistsW(inifull_name);
		}
	}
	return rect;
}

BOOL read_appkey(LPCWSTR lpappname,              /* 区段名 */
				 LPCWSTR lpkey,					 /* 键名  */	
				 LPWSTR  prefstring,			 /* 保存值缓冲区 */	
				 size_t  bufsize				 /* 缓冲区大小 */
				 )
{
	DWORD  res = 0;
	LPWSTR lpstring;
	wchar_t inifull_name[MAX_PATH+1];
	if (ini_ready(inifull_name,MAX_PATH))
	{
		lpstring = (LPWSTR)SYS_MALLOC(bufsize);
		res = GetPrivateProfileStringW(lpappname, lpkey ,L"", lpstring, bufsize, inifull_name);
		if (res == 0 && GetLastError() != 0x0)
		{
			SYS_FREE(lpstring);
			printf("this ini config file not found\n");
			return FALSE;
		}
		wcsncpy(prefstring,lpstring,bufsize/sizeof(wchar_t)-1);
		prefstring[res] = '\0';
		SYS_FREE(lpstring);
	}
	return ( res>0 );
}	

int read_appint(LPCWSTR cat,LPCWSTR name)
{
	int res = 0;
	wchar_t inifull_name[MAX_PATH];
	if (ini_ready(inifull_name,MAX_PATH))
	{
		res = GetPrivateProfileIntW(cat, name, 0, inifull_name);
		if(res == 0 && GetLastError() == 0x2)
			printf("this ini config file not found\n");
	}
	return res;
}

BOOL for_eachSection(LPCWSTR cat,						/* ini 区段 */
					 wchar_t (*lpdata)[VALUE_LEN+1],	/* 二维数组首地址,保存多个段值 */
					 int m								/* 二维数组行数 */
					 )
{
	DWORD  res = 0;
	LPWSTR lpstring;
	LPWSTR strKey;
	int  i = 0;
	const wchar_t delim[] = L"=";
	wchar_t inifull_name[MAX_PATH];
	if (ini_ready(inifull_name,MAX_PATH))
	{
		size_t num = VALUE_LEN*sizeof(wchar_t)*m;
		lpstring = (LPWSTR)SYS_MALLOC(num);
		res = GetPrivateProfileSectionW(cat, lpstring, num, inifull_name);
		if (res == 0 && GetLastError() != 0x0)
		{
			SYS_FREE(lpstring);
			printf("this ini config file not found\n");
			return FALSE;
		}
		ZeroMemory(*lpdata,num);
		strKey = lpstring;
		while(*strKey != L'\0'&& i < m) 
		{
			LPWSTR strtmp;
			wchar_t t_str[VALUE_LEN] = {0};
			wcsncpy(t_str,strKey,VALUE_LEN-1);
			strtmp = StrStrW(t_str, delim);
			if (strtmp)
			{
				wcsncpy(lpdata[i],&strtmp[1],VALUE_LEN-1);
			}
			strKey += wcslen(strKey)+1;
			++i;
		}
		SYS_FREE(lpstring);
	}
	return TRUE;
}

LPWSTR stristrW(LPCWSTR Str, LPCWSTR Pat)
{
    wchar_t *pptr, *sptr, *start;

    for (start = (wchar_t *)Str; *start != '\0'; start++)
    {
        for ( ; ((*start!='\0') && (toupper(*start) != toupper(*Pat))); start++);
        if ('\0' == *start) return NULL;
        pptr = (wchar_t *)Pat;
        sptr = (wchar_t *)start;
        while (toupper(*sptr) == toupper(*pptr))
        {
            sptr++;
            pptr++;
            if ('\0' == *pptr) return (start);
        }
    }
    return NULL;
}

BOOL WINAPI ChangeEnviromentVariablesW(LPCWSTR szname,LPWSTR sz_newval,int dw_flag)
{
	DWORD  dw_err;
	LPWSTR sz_val;
	DWORD  dw_result;
	DWORD  new_valsize;
	BOOL   result = FALSE;
	/* 附加到指定环境变量末尾 */
	if(dw_flag == VARIABLES_APPEND)
	{
		new_valsize = wcslen(sz_newval)+1; 
		sz_val = (LPWSTR)SYS_MALLOC(BUFSIZE+new_valsize);
		/* 获取指定环境变量的值 */
		dw_result = GetEnvironmentVariableW(szname,sz_val,BUFSIZE);
		if(dw_result == 0) /* 出错处理 */
		{
			dw_err = GetLastError();
			if( ERROR_ENVVAR_NOT_FOUND == dw_err )
			{
				printf("Environment variable %s does not exist.\n", szname);
				if( SetEnvironmentVariableW(szname,sz_newval) )
				{
					result = TRUE;
				}
			}
			SYS_FREE(sz_val);
			return result;
		}
		/* 缓冲区太小 */
		if( BUFSIZE <= dw_result )
		{
			SYS_FREE(sz_val);
			dw_result += new_valsize;
			sz_val = (LPWSTR)SYS_MALLOC(dw_result);
			if(NULL == sz_val)
			{
				printf("Memory error\n");
				return result;
			}
			dw_result = GetEnvironmentVariableW(szname, sz_val, dw_result);
			if(!dw_result)
			{
				SYS_FREE(sz_val);
				return result;
			}
		}
		wcscat(sz_val,L";");   /* 分隔符 */
		wcscat(sz_val,sz_newval);
		if( SetEnvironmentVariableW(szname,sz_val) )
		{
			result = TRUE;
		}
		SYS_FREE(sz_val);
	}
	/* 设置新的环境变量 */
	else if(dw_flag == VARIABLES_RESET)
	{
	    if( SetEnvironmentVariableW(szname,sz_newval) )
	    {
		    result = TRUE;
	    }
	}
	/* 指定环境变量清零 */
	else if(dw_flag == VARIABLES_NULL)
	{
	    if( SetEnvironmentVariableW(szname,NULL) )
		{
			result = TRUE;
		}
	}
	return result;
}

BOOL WINAPI ChangeEnviromentVariablesA(LPCSTR szname,LPSTR sz_newval,int dw_flag)
{
	DWORD  dw_err;
	LPSTR  sz_val;
	DWORD  dw_result;
	DWORD  new_valsize;
	BOOL   result = FALSE;
	/* 附加到指定环境变量末尾 */
	if(dw_flag == VARIABLES_APPEND)
	{
		new_valsize = strlen(sz_newval)+1; 
		sz_val = (LPSTR)SYS_MALLOC(BUFSIZE+new_valsize);
		/* 获取指定环境变量的值 */
		dw_result = GetEnvironmentVariableA(szname,sz_val,BUFSIZE);
		if(dw_result == 0) /* 出错处理 */
		{
			dw_err = GetLastError();
			if( ERROR_ENVVAR_NOT_FOUND == dw_err )
			{
				/* Environment variable does not exist */
				if( SetEnvironmentVariableA(szname,sz_newval) )
				{
					result = TRUE;
				}
			}
			SYS_FREE(sz_val);
			return result;
		}
		/* 缓冲区太小 */
		if( BUFSIZE <= dw_result )
		{
			SYS_FREE(sz_val);
			dw_result += new_valsize;
			sz_val = (LPSTR)SYS_MALLOC(dw_result);
			if(NULL == sz_val)
			{
				printf("Memory error\n");
				return result;
			}
			dw_result = GetEnvironmentVariableA(szname, sz_val, dw_result);
			if(!dw_result)
			{
				SYS_FREE(sz_val);
				return result;
			}
		}
		strcat(sz_val,";");   /* 分隔符 */
		strcat(sz_val,sz_newval);
		if( SetEnvironmentVariableA(szname,sz_val) )
		{
			result = TRUE;
		}
		SYS_FREE(sz_val);
	}
	/* 设置新的环境变量 */
	else if(dw_flag == VARIABLES_RESET)
	{
	    if( SetEnvironmentVariableA(szname,sz_newval) )
	    {
		    result = TRUE;
	    }
	}
	/* 指定环境变量清零 */
	else if(dw_flag == VARIABLES_NULL)
	{
	    if( SetEnvironmentVariableA(szname,NULL) )
		{
			result = TRUE;
		}
	}
	return result;
}

void WINAPI charTochar(LPWSTR path)
{
	LPWSTR lp = NULL;
	int post;
	do
	{
		lp =  StrChrW(path,L'/');
		if (lp)
		{
			post = lp-path;
			path[post] = L'\\';
		}
	}
	while (lp!=NULL);
	return;
}

BOOL PathToCombineW(IN LPWSTR lpfile, IN size_t str_len)
{
	BOOL	ret = FALSE;
	wchar_t buf_modname[VALUE_LEN+1] = {0};
	wchar_t tmp_path[MAX_PATH] = {0};
	if ( dll_module && lpfile[1] != L':' )
	{
		charTochar(lpfile);
		if ( GetModuleFileNameW( dll_module, buf_modname, VALUE_LEN) > 0)
		{
			PathRemoveFileSpecW(buf_modname);
			if ( PathCombineW(tmp_path,buf_modname,lpfile) )
			{
				int n = _snwprintf(lpfile,str_len,L"%ls",tmp_path);
				lpfile[n] = L'\0';
				ret = TRUE;
			}
		}
	}
	return ret;
}

unsigned WINAPI SetPluginPathW(void * pParam)
{
	wchar_t lpfile[VALUE_LEN+1];
	if ( read_appkey(L"Env",L"NpluginPath",lpfile,sizeof(lpfile)) )
	{
		if (PathToCombineW(lpfile, VALUE_LEN))
		{
			ChangeEnviromentVariablesW(L"MOZ_PLUGIN_PATH", lpfile, VARIABLES_APPEND);
		}
	}
	return (1);
}

unsigned WINAPI SetPluginPathA(void * pParam)
{
	wchar_t lpfile[VALUE_LEN+1];
	if ( read_appkey(L"Env",L"NpluginPath",lpfile,sizeof(lpfile)) )
	{
		char	*szpath = NULL;
		if (PathToCombineW(lpfile, VALUE_LEN))
		{
			int  len = WideCharToMultiByte( CP_ACP, 0, lpfile, -1, NULL, 0, NULL, NULL );
			szpath = (char *)SYS_MALLOC(len+1);
			if (szpath)
			{
				if (WideCharToMultiByte( CP_ACP, 0, lpfile, -1, szpath, len, NULL, NULL ))
				{
					ChangeEnviromentVariablesA("MOZ_PLUGIN_PATH",szpath, VARIABLES_APPEND);
				}
			}
		}
		if (szpath)
		{
			SYS_FREE(szpath);
		}
	}
	return (1);
}

BOOL WINAPI IsGUI(LPCWSTR lpFileName)
{
	IMAGE_DOS_HEADER dos_header;
	IMAGE_NT_HEADERS pe_header;
	DWORD	readed;
	BOOL	ret     = FALSE;
	HANDLE	hFile	=  CreateFileW(lpFileName,GENERIC_READ,
						FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return ret;
	}
	SetFilePointer(hFile,0,0,FILE_BEGIN);

	ReadFile(hFile,&dos_header,sizeof(IMAGE_DOS_HEADER),&readed,NULL);
	if(readed != sizeof(IMAGE_DOS_HEADER))
	{
		CloseHandle(hFile);
		return ret;
	}
	if(dos_header.e_magic != 0x5a4d)
	{
		CloseHandle(hFile);
		return ret;
	}
	SetFilePointer(hFile,dos_header.e_lfanew,NULL,FILE_BEGIN);
	ReadFile(hFile,&pe_header,sizeof(IMAGE_NT_HEADERS),&readed,NULL);
	if(readed != sizeof(IMAGE_NT_HEADERS))
	{
		CloseHandle(hFile);
		return ret;
	}
	CloseHandle(hFile);
	if(pe_header.OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_WINDOWS_GUI)
	{
		ret = TRUE;
	}
	return ret;
} 

DWORD WINAPI GetOsVersion(void)
{
	OSVERSIONINFOEXA	osvi;
	BOOL				bOs = FALSE;
	DWORD				ver = 0L;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEXA));
	
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);
	if( GetVersionExA((OSVERSIONINFOA*)&osvi) ) 
	{
		if ( VER_PLATFORM_WIN32_NT==osvi.dwPlatformId && 
			osvi.dwMajorVersion > 4 )
		{
			char pszOS[4] = {0};
			_snprintf(pszOS, 3, "%lu%d%lu", osvi.dwMajorVersion,0,osvi.dwMinorVersion);
			ver = strtol(pszOS, NULL, 10);
		}
	}
	return ver;
}
