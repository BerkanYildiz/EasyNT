#pragma once

EXTERN_C NTSYSAPI NTSTATUS NTAPI ZwQuerySystemInformation(
	IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
	OUT PVOID SystemInformation,
	IN ULONG SystemInformationLength,
	OUT PULONG ReturnLength OPTIONAL
);

EXTERN_C NTKERNELAPI NTSTATUS ZwQueryInformationProcess(
	_In_      HANDLE           ProcessHandle,
	_In_      PROCESSINFOCLASS ProcessInformationClass,
	_Out_     PVOID            ProcessInformation,
	_In_      ULONG            ProcessInformationLength,
	_Out_opt_ PULONG           ReturnLength
);

EXTERN_C NTKERNELAPI PPEB NTAPI PsGetProcessPeb(
    IN PEPROCESS Process
);

#define PROCESS_TERMINATE 0x0001
#define PROCESS_CREATE_THREAD 0x0002
#define PROCESS_SET_SESSIONID 0x0004
#define PROCESS_VM_OPERATION 0x0008
#define PROCESS_VM_READ 0x0010
#define PROCESS_VM_WRITE 0x0020
#define PROCESS_CREATE_PROCESS 0x0080
#define PROCESS_SET_QUOTA 0x0100
#define PROCESS_SET_INFORMATION 0x0200
#define PROCESS_QUERY_INFORMATION 0x0400
#define PROCESS_SET_PORT 0x0800
#define PROCESS_SUSPEND_RESUME 0x0800
#define PROCESS_QUERY_LIMITED_INFORMATION 0x1000

/// <summary>
/// Gets every processes information on the system.
/// </summary>
/// <param name="OutProcessEntries">The process entries.</param>
/// <param name="OutNumberOfProcessEntries">The number of entries in the buffer.</param>
NTSTATUS PsGetProcesses(OUT SYSTEM_PROCESS_INFORMATION** OutProcessEntries, OPTIONAL OUT ULONG* OutNumberOfProcessEntries = nullptr);

/// <summary>
/// Gets every processes information on the system matching a certain image file name.
/// </summary>
/// <param name="InProcessName">The process image filename.</param>
/// <param name="OutProcessEntries">The process entries.</param>
/// <param name="OutNumberOfProcessEntries">The number of entries in the buffer.</param>
NTSTATUS PsGetProcesses(UNICODE_STRING InProcessName, OUT SYSTEM_PROCESS_INFORMATION** OutProcessEntries, OPTIONAL OUT ULONG* OutNumberOfProcessEntries = nullptr);

/// <summary>
/// Gets every processes information on the system matching a certain image file name.
/// </summary>
/// <param name="InProcessName">The process image filename.</param>
/// <param name="OutProcessEntries">The process entries.</param>
/// <param name="OutNumberOfProcessEntries">The number of entries in the buffer.</param>
NTSTATUS PsGetProcesses(CONST WCHAR* InProcessName, OUT SYSTEM_PROCESS_INFORMATION** OutProcessEntries, OPTIONAL OUT ULONG* OutNumberOfProcessEntries = nullptr);

/// <summary>
/// Gets every processes information on the system matching a certain image file name.
/// </summary>
/// <param name="InProcessName">The process image filename.</param>
/// <param name="OutProcessEntries">The process entries.</param>
/// <param name="OutNumberOfProcessEntries">The number of entries in the buffer.</param>
NTSTATUS PsGetProcesses(CONST CHAR* InProcessName, OUT SYSTEM_PROCESS_INFORMATION** OutProcessEntries, OPTIONAL OUT ULONG* OutNumberOfProcessEntries = nullptr);

/// <summary>
/// Gets information about the process with the given filename.
/// </summary>
/// <param name="InProcessName">The process image filename.</param>
/// <param name="OutProcessInformation">The returned process information.</param>
NTSTATUS PsGetProcessInformation(UNICODE_STRING InProcessName, OUT SYSTEM_PROCESS_INFORMATION* OutProcessInformation);

/// <summary>
/// Gets information about the process with the given filename.
/// </summary>
/// <param name="InProcessName">The process image filename.</param>
/// <param name="OutProcessInformation">The returned process information.</param>
NTSTATUS PsGetProcessInformation(CONST WCHAR* InProcessName, OUT SYSTEM_PROCESS_INFORMATION* OutProcessInformation);

/// <summary>
/// Gets information about the process with the given filename.
/// </summary>
/// <param name="InProcessName">The process image filename.</param>
/// <param name="OutProcessInformation">The returned process information.</param>
NTSTATUS PsGetProcessInformation(CONST CHAR* InProcessName, OUT SYSTEM_PROCESS_INFORMATION* OutProcessInformation);

/// <summary>
/// Gets the image file path of the given process.
/// </summary>
/// <param name="InProcess">The process object.</param>
/// <param name="OutProcessName">The process image file path.</param>
NTSTATUS PsGetProcessImageFilePath(CONST PEPROCESS InProcess, OUT WCHAR** OutProcessName);

/// <summary>
/// Gets the image filename of the given process.
/// </summary>
/// <param name="InProcess">The process object.</param>
/// <param name="OutProcessName">The process image filename.</param>
NTSTATUS PsGetProcessImageFileName(CONST PEPROCESS InProcess, OUT WCHAR** OutProcessName);

/// <summary>
/// Terminates a process by its object with the given exit status.
/// </summary>
/// <param name="InProcess">The process object.</param>
/// <param name="InExitStatus">The exist status.</param>
NTSTATUS PsTerminateProcess(CONST PEPROCESS InProcess, NTSTATUS InExitStatus = STATUS_SUCCESS);

/// <summary>
/// Gets a value indicating whether the specified process is terminating.
/// </summary>
/// <param name="InProcess">The process.</param>
BOOLEAN PsProcessIsTerminating(CONST PEPROCESS InProcess);
