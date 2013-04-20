#ifndef _HEAD_ER_H_
#  define _HEAD_ER_H_

#include <windows.h>
#include <shlwapi.h>
#include <psapi.h>

#define CREATE_PROCESS_BREAKAWAY_FROM_JOB	0x0001
#define CREATE_PROCESS_INHERIT_HANDLES		0x0004
#define CREATE_PROCESS_PROTECTED			0x0040

#define CREATE_THREAD_SUSPENDED				0x0001
#define STATUS_ERROR						((NTSTATUS)0x80070000L)
#define NT_SUCCESS(Status)					(((NTSTATUS)(Status)) >= 0)
#define STATUS_SUCCESS						((NTSTATUS)0x00000000)
#define STATUS_OBJECT_PATH_NOT_FOUND		((NTSTATUS)0xC000003A)
#define _Inout_
#define _In_

#define NtCurrentProcess() ( (HANDLE)(LONG_PTR) -1 )

#define ProcThreadAttributeValue( p1, p2, p3, p4 ) \
        (((p1) & PROC_THREAD_ATTRIBUTE_NUMBER) | \
        ((p2 != FALSE) ? PROC_THREAD_ATTRIBUTE_THREAD : 0) | \
        ((p3 != FALSE) ? PROC_THREAD_ATTRIBUTE_INPUT : 0) | \
        ((p4 != FALSE) ? PROC_THREAD_ATTRIBUTE_ADDITIVE : 0))

#define PROC_THREAD_ATTRIBUTE_PCLIENTID ProcThreadAttributeValue (8, FALSE, FALSE, FALSE)

#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004)

#ifndef _NTSTATUS_PSDK
#define _NTSTATUS_PSDK
  typedef LONG NTSTATUS;
#endif

#ifndef __UNICODE_STRING_DEFINED
#define __UNICODE_STRING_DEFINED
  typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
  } UNICODE_STRING;
  typedef UNICODE_STRING *PUNICODE_STRING;
#endif

#ifndef __OBJECT_ATTRIBUTES_DEFINED
#define __OBJECT_ATTRIBUTES_DEFINED
  typedef struct _OBJECT_ATTRIBUTES {
    ULONG Length;
#ifdef _WIN64
    ULONG pad1;
#endif
    HANDLE RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG Attributes;
#ifdef _WIN64
    ULONG pad2;
#endif
    PVOID SecurityDescriptor;
    PVOID SecurityQualityOfService;
  } OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;
#endif

#ifndef __NT_PROC_THREAD_ATTRIBUTE_ENTRY
#define __NT_PROC_THREAD_ATTRIBUTE_ENTRY
typedef struct _NT_PROC_THREAD_ATTRIBUTE_ENTRY {
    ULONG Attribute;    /* PROC_THREAD_ATTRIBUTE_XXX���μ�MSDN��UpdateProcThreadAttribute��˵�� */
    SIZE_T Size;        /* Value�Ĵ�С */
    ULONG_PTR Value;    /* ����4�ֽ����ݣ�����һ��Handle��������ָ�� */
    ULONG Unknown;      /* ����0�������������������ݸ������� */
} PROC_THREAD_ATTRIBUTE_ENTRY, *PPROC_THREAD_ATTRIBUTE_ENTRY;
#endif

#ifndef __NT_PROC_THREAD_ATTRIBUTE_LIST
#define __NT_PROC_THREAD_ATTRIBUTE_LIST
typedef struct _NT_PROC_THREAD_ATTRIBUTE_LIST {
    ULONG Length;       /* �ܵĽṹ��С */
    PROC_THREAD_ATTRIBUTE_ENTRY Entry[1];
} NT_PROC_THREAD_ATTRIBUTE_LIST;
typedef NT_PROC_THREAD_ATTRIBUTE_LIST *PNT_PROC_THREAD_ATTRIBUTE_LIST;
#endif

#ifndef __CURDIR
#define __CURDIR
typedef struct _CURDIR
{
     UNICODE_STRING DosPath;
     PVOID Handle;
} CURDIR, *PCURDIR;
#endif

#ifndef __STRING
#define __STRING
typedef struct _STRING
{
     WORD Length;
     WORD MaximumLength;
     CHAR * Buffer;
} STRING, *PSTRING;
#endif

#ifndef __RTL_DRIVE_LETTER_CURDIR
#define __RTL_DRIVE_LETTER_CURDIR
typedef struct _RTL_DRIVE_LETTER_CURDIR
{
     WORD Flags;
     WORD Length;
     ULONG TimeStamp;
     STRING DosPath;
} RTL_DRIVE_LETTER_CURDIR, *PRTL_DRIVE_LETTER_CURDIR;
#endif

#ifndef __RTL_USER_PROCESS_PARAMETERS
#define __RTL_USER_PROCESS_PARAMETERS
typedef struct _RTL_USER_PROCESS_PARAMETERS
{
     ULONG MaximumLength;
     ULONG Length;
     ULONG Flags;
     ULONG DebugFlags;
     PVOID ConsoleHandle;
     ULONG ConsoleFlags;
     PVOID StandardInput;
     PVOID StandardOutput;
     PVOID StandardError;
     CURDIR CurrentDirectory;
     UNICODE_STRING DllPath;
     UNICODE_STRING ImagePathName;
     UNICODE_STRING CommandLine;
     PVOID Environment;
     ULONG StartingX;
     ULONG StartingY;
     ULONG CountX;
     ULONG CountY;
     ULONG CountCharsX;
     ULONG CountCharsY;
     ULONG FillAttribute;
     ULONG WindowFlags;
     ULONG ShowWindowFlags;
     UNICODE_STRING WindowTitle;
     UNICODE_STRING DesktopInfo;
     UNICODE_STRING ShellInfo;
     UNICODE_STRING RuntimeData;
     RTL_DRIVE_LETTER_CURDIR CurrentDirectores[32];
     ULONG EnvironmentSize;
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;
#endif

typedef enum _SECTION_INHERIT {
  ViewShare = 1,
  ViewUnmap = 2
} SECTION_INHERIT;

typedef enum _FILE_INFORMATION_CLASS {
  FileDirectoryInformation = 1,
  FileFullDirectoryInformation,
  FileBothDirectoryInformation,
  FileBasicInformation,
  FileStandardInformation,
  FileInternalInformation,
  FileEaInformation,
  FileAccessInformation,
  FileNameInformation,
  FileRenameInformation,
  FileLinkInformation,
  FileNamesInformation,
  FileDispositionInformation,
  FilePositionInformation,
  FileFullEaInformation,
  FileModeInformation,
  FileAlignmentInformation,
  FileAllInformation,
  FileAllocationInformation,
  FileEndOfFileInformation,
  FileAlternateNameInformation,
  FileStreamInformation,
  FilePipeInformation,
  FilePipeLocalInformation,
  FilePipeRemoteInformation,
  FileMailslotQueryInformation,
  FileMailslotSetInformation,
  FileCompressionInformation,
  FileObjectIdInformation,
  FileCompletionInformation,
  FileMoveClusterInformation,
  FileQuotaInformation,
  FileReparsePointInformation,
  FileNetworkOpenInformation,
  FileAttributeTagInformation,
  FileTrackingInformation,
  FileIdBothDirectoryInformation,
  FileIdFullDirectoryInformation,
  FileValidDataLengthInformation,
  FileShortNameInformation = 40,
  FileSfioReserveInformation = 44,
  FileSfioVolumeInformation = 45,
  FileHardLinkInformation = 46,
  FileNormalizedNameInformation = 48,
  FileIdGlobalTxDirectoryInformation = 50,
  FileStandardLinkInformation = 54,
  FileMaximumInformation
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;

typedef struct _IO_STATUS_BLOCK {
  __C89_NAMELESS union {
    NTSTATUS Status;
    PVOID Pointer;
  };
  ULONG_PTR Information;
} IO_STATUS_BLOCK,*PIO_STATUS_BLOCK;

#ifndef _LARGE_INTEGER_DEFINED
#define _LARGE_INTEGER_DEFINED
/* Large Integer Unions */
#if defined(MIDL_PASS)
typedef struct _LARGE_INTEGER {
#else
typedef union _LARGE_INTEGER {
  __C89_NAMELESS struct {
    ULONG LowPart;
    LONG HighPart;
  } DUMMYSTRUCTNAME;
  struct {
    ULONG LowPart;
    LONG HighPart;
  } u;
#endif
  LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;
#endif

typedef struct _FILE_NAME_INFORMATION {
  ULONG FileNameLength;
  wchar_t FileName[1];
} FILE_NAME_INFORMATION, *PFILE_NAME_INFORMATION;

typedef enum _PROCESSINFOCLASS {
  ProcessBasicInformation,
  ProcessQuotaLimits,
  ProcessIoCounters,
  ProcessVmCounters,
  ProcessTimes,
  ProcessBasePriority,
  ProcessRaisePriority,
  ProcessDebugPort,
  ProcessExceptionPort,
  ProcessAccessToken,
  ProcessLdtInformation,
  ProcessLdtSize,
  ProcessDefaultHardErrorMode,
  ProcessIoPortHandlers,
  ProcessPooledUsageAndLimits,
  ProcessWorkingSetWatch,
  ProcessUserModeIOPL,
  ProcessEnableAlignmentFaultFixup,
  ProcessPriorityClass,
  ProcessWx86Information,
  ProcessHandleCount,
  ProcessAffinityMask,
  ProcessPriorityBoost,
  ProcessDeviceMap,
  ProcessSessionInformation,
  ProcessForegroundInformation,
  ProcessWow64Information,
  ProcessImageFileName,
  ProcessLUIDDeviceMapsEnabled,
  ProcessBreakOnTermination,
  ProcessDebugObjectHandle,
  ProcessDebugFlags,
  ProcessHandleTracing,
  ProcessIoPriority,
  ProcessExecuteFlags,
  ProcessTlsInformation,
  ProcessCookie,
  ProcessImageInformation,
  ProcessCycleTime,
  ProcessPagePriority,
  ProcessInstrumentationCallback,
  ProcessThreadStackAllocation,
  ProcessWorkingSetWatchEx,
  ProcessImageFileNameWin32,
  ProcessImageFileMapping,
  ProcessAffinityUpdateMode,
  ProcessMemoryAllocationMode,
  ProcessGroupInformation,
  ProcessTokenVirtualizationEnabled,
  ProcessConsoleHostProcess,
  ProcessWindowInformation,
  MaxProcessInfoClass
} PROCESSINFOCLASS;

extern	HMODULE  dll_module;
static  UINT_PTR m_dwUser32Low;						/* dll �ļ��ػ�ַ */
static  UINT_PTR m_dwUser32Hi;						/* dll �ļ��ػ�ַ+ImageSize */

typedef NTSTATUS (NTAPI *_NtQueryObject)(HANDLE ObjectHandle,
										ULONG  ObjectInformationClass,
										PVOID  ObjectInformation,
										ULONG  ObjectInformationLength,
										PULONG ReturnLength);
typedef NTSTATUS (NTAPI *_NtQuerySection) (HANDLE SectionHandle, 
										ULONG SectionInformationClass,
										PVOID SectionInformation,
										ULONG SectionInformationLength,
										PULONG ResultLength);
typedef  NTSTATUS (NTAPI *_NtCreateSection)(OUT PHANDLE SectionHandle,
										IN ACCESS_MASK DesiredAccess,
										IN POBJECT_ATTRIBUTES ObjectAttributes,
										IN PLARGE_INTEGER SectionSize OPTIONAL,
										IN ULONG Protect,
										IN ULONG Attributes,
										IN HANDLE FileHandle);
typedef  NTSTATUS (NTAPI *_NtCreateUserProcess)(OUT PHANDLE ProcessHandle,
										OUT PHANDLE ThreadHandle,
										IN ACCESS_MASK ProcessDesiredAccess,
										IN ACCESS_MASK ThreadDesiredAccess,
										IN POBJECT_ATTRIBUTES ProcessObjectAttributes OPTIONAL,
										IN POBJECT_ATTRIBUTES ThreadObjectAttributes OPTIONAL,
										IN ULONG CreateProcessFlags,
										IN ULONG CreateThreadFlags,
										IN PRTL_USER_PROCESS_PARAMETERS ProcessParameters,
										IN PVOID Parameter9,
										IN PNT_PROC_THREAD_ATTRIBUTE_LIST AttributeList);
typedef NTSTATUS (NTAPI *_NtMapViewOfSection)(HANDLE SectionHandle,
									    HANDLE ProcessHandle,
									    PVOID *BaseAddress,
									    ULONG_PTR ZeroBits,
									    SIZE_T CommitSize,
										PLARGE_INTEGER SectionOffset,
										PSIZE_T ViewSize,
										SECTION_INHERIT InheritDisposition,
										ULONG AllocationType,
										ULONG Win32Protect);
typedef NTSTATUS (NTAPI *_NtQueryInformationFile)(HANDLE hFile, 
									    PIO_STATUS_BLOCK IoStatusBlock,
									    PVOID FileInformation,
                                        ULONG Length,
									    FILE_INFORMATION_CLASS FileInformationClass);
typedef NTSTATUS (NTAPI *_NtTerminateProcess)(HANDLE hProcess,
									    NTSTATUS ExitStatus);
typedef NTSTATUS (NTAPI *_NtUnmapViewOfSection)( HANDLE ProcessHandle,
										PVOID BaseAddress );
typedef NTSTATUS (NTAPI *_NtCLOSE) ( HANDLE ); 
typedef NTSTATUS (NTAPI *_NtAllocateVirtualMemory)(_In_  HANDLE ProcessHandle,
										_Inout_  PVOID *BaseAddress,
										_In_     ULONG_PTR ZeroBits,
										_Inout_  PSIZE_T RegionSize,
										_In_     ULONG AllocationType,
										_In_     ULONG Protect);
typedef NTSTATUS (NTAPI *_NtFreeVirtualMemory)(_In_ HANDLE ProcessHandle,
										_Inout_  PVOID *BaseAddress,
										_Inout_  PSIZE_T RegionSize,
										_In_     ULONG FreeType);
typedef NTSTATUS (NTAPI *_NtWriteVirtualMemory)(IN HANDLE ProcessHandle,
										IN PVOID BaseAddress,
										IN PVOID Buffer, 
										IN ULONG NumberOfBytesToWrite,
										OUT PULONG NumberOfBytesWritten);
typedef NTSTATUS (NTAPI *_NtCreateThreadEx)(OUT PHANDLE ThreadHandle,
										IN ACCESS_MASK DesiredAccess,
										IN POBJECT_ATTRIBUTES ObjectAttributes,
										IN HANDLE ProcessHandle,
										IN PTHREAD_START_ROUTINE StartRoutine,
										IN PVOID StartContext,
										IN ULONG CreateThreadFlags,
										IN ULONG ZeroBits OPTIONAL,
										IN ULONG StackSize OPTIONAL,
										IN ULONG MaximumStackSize OPTIONAL,
										IN PNT_PROC_THREAD_ATTRIBUTE_LIST AttributeList);
typedef NTSTATUS (NTAPI *_NtCreateProcessEx)(OUT PHANDLE ProcessHandle,
										IN ACCESS_MASK DesiredAccess,
										IN POBJECT_ATTRIBUTES ObjectAttributes,
										IN HANDLE ParentProcess,
										IN ULONG Flags,
										IN HANDLE SectionHandle,
										IN HANDLE DebugPort,
										IN HANDLE ExceptionPort,
										IN BOOLEAN InJob);
typedef NTSTATUS (WINAPI *_NtQueryInformationProcess)(HANDLE ProcessHandle,
										PROCESSINFOCLASS ProcessInformationClass,
										PVOID ProcessInformation,
										ULONG ProcessInformationLength,
										PULONG ReturnLength);
typedef  HMODULE (WINAPI *_NtRemoteLoadW)(LPCWSTR lpFileName);

static _NtCLOSE							TrueNtclose							= NULL;
static _NtQueryInformationFile			TrueNtQueryInformationFile			= NULL;
static _NtQuerySection					TrueNtQuerySection					= NULL;
static _NtTerminateProcess				TrueNtTerminateProcess				= NULL;
static _NtCreateSection					TrueNtCreateSection					= NULL;
static _NtMapViewOfSection              TrueNtMapViewOfSection				= NULL;
static _NtCreateUserProcess             TrueCreateUserProcess				= NULL;
static _NtUnmapViewOfSection			TrueNtUnmapViewOfSection			= NULL;
static _NtWriteVirtualMemory            TrueNtWriteVirtualMemory			= NULL;
static _NtAllocateVirtualMemory         TrueNtAllocateVirtualMemory			= NULL;
static _NtFreeVirtualMemory				TrueNtFreeVirtualMemory				= NULL;
static _NtCreateProcessEx				TrueNtCreateProcessEx				= NULL;
static _NtQueryInformationProcess		TrueNtQueryInformationProcess		= NULL;
static _NtRemoteLoadW					RemoteLoadW							= NULL;

#endif  // _HEAD_ER_H_