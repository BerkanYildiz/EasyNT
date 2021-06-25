#pragma once

typedef struct _RTL_PROCESS_MODULE_INFORMATION
{
    HANDLE Section;
    PVOID MappedBase;
    PVOID ImageBase;
    ULONG ImageSize;
    ULONG Flags;
    USHORT LoadOrderIndex;
    USHORT InitOrderIndex;
    USHORT LoadCount;
    USHORT OffsetToFileName;
    UCHAR FullPathName[256];
} RTL_PROCESS_MODULE_INFORMATION, *PRTL_PROCESS_MODULE_INFORMATION;

typedef struct _RTL_PROCESS_MODULES
{
    ULONG NumberOfModules;
    RTL_PROCESS_MODULE_INFORMATION Modules[1];
} RTL_PROCESS_MODULES, *PRTL_PROCESS_MODULES;

typedef struct _LDR_DATA_TABLE_ENTRY {
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG Flags;
	USHORT LoadCount;
	USHORT TlsIndex;
	union {
		LIST_ENTRY HashLinks;
		struct {
			PVOID SectionPointer;
			ULONG CheckSum;
		};
	};
	union {
		struct {
			ULONG TimeDateStamp;
		};
		struct {
			PVOID LoadedImports;
		};
	};
	
	PVOID* EntryPointActivationContext;
	PVOID PatchInformation;

} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

typedef struct _PEB_LDR_DATA
{
    ULONG Length;
    BOOLEAN Initialized;
    HANDLE SsHandle;
    LIST_ENTRY InLoadOrderModuleList;
    LIST_ENTRY InMemoryOrderModuleList;
    LIST_ENTRY InInitializationOrderModuleList;
    PVOID EntryInProgress;
    BOOLEAN ShutdownInProgress;
    HANDLE ShutdownThreadId;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _PEB64 {
	CHAR Reserved[0x10];
	PVOID ImageBaseAddress;
	PPEB_LDR_DATA Ldr;
} PEB64, *PPEB64;

typedef PEB64 PEB;

EXTERN_C NTKERNELAPI ERESOURCE PsLoadedModuleResource;
EXTERN_C NTKERNELAPI LIST_ENTRY PsLoadedModuleList;

/// <summary>
/// Gets information about every modules loaded into the given process.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="OutModuleEntries">The modules.</param>
/// <param name="OutNumberOfModules">The number of modules.</param>
NTSTATUS PsGetProcessModules(CONST PEPROCESS InProcess, OUT RTL_PROCESS_MODULE_INFORMATION** OutModuleEntries, OUT ULONG* OutNumberOfModules);

/// <summary>
/// Gets information about a module with the given filename.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InModuleFilename">The filename of the module.</param>
/// <param name="OutModuleInformation">The result.</param>
NTSTATUS PsGetProcessModuleInformation(CONST PEPROCESS InProcess, CONST CHAR* InModuleFilename, OUT OPTIONAL RTL_PROCESS_MODULE_INFORMATION* OutModuleInformation = nullptr);

/// <summary>
/// Gets information about a module with the given filename.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InModuleFilename">The filename of the module.</param>
/// <param name="OutModuleInformation">The result.</param>
NTSTATUS PsGetProcessModuleInformation(CONST PEPROCESS InProcess, CONST WCHAR* InModuleFilename, OUT OPTIONAL RTL_PROCESS_MODULE_INFORMATION* OutModuleInformation = nullptr);

/// <summary>
/// Gets information about a module that matches the given address range.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InModuleAddress">A virtual address pointing inside a module.</param>
/// <param name="OutModuleInformation">The result.</param>
NTSTATUS PsGetProcessModuleInformationByAddress(CONST PEPROCESS InProcess, CONST PVOID InModuleAddress, OUT OPTIONAL RTL_PROCESS_MODULE_INFORMATION* OutModuleInformation = nullptr);
