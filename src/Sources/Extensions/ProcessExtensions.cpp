#include "../../Headers/EasyNT.h"

/// <summary>
/// Gets every processes information on the system.
/// </summary>
/// <param name="OutProcessEntries">The process entries.</param>
/// <param name="OutNumberOfProcessEntries">The count of entries in the buffer.</param>
NTSTATUS PsGetProcesses(OUT SYSTEM_PROCESS_INFORMATION** OutProcessEntries, OPTIONAL OUT ULONG* OutNumberOfProcessEntries)
{
	NTSTATUS Status = { };

	// 
	// Verify the passed parameters.
	// 

	if (OutProcessEntries == NULL)
		return STATUS_INVALID_PARAMETER_2;

	// 
	// Retrieve the process entries.
	// 

    PVOID ProcessEntries = nullptr;
	ULONG ProcessEntriesLength = 0;
	Status = ZwQuerySystemInformation(SystemProcessInformation, ProcessEntries, ProcessEntriesLength, &ProcessEntriesLength);

	while (Status == STATUS_BUFFER_TOO_SMALL ||
		   Status == STATUS_INFO_LENGTH_MISMATCH)
	{
		// 
		// Round up the size just in case a process has been started during the query.
		// 

        ProcessEntriesLength = (ULONG) PAGE_ROUND_UP(ProcessEntriesLength /* + 1 */ );

		// 
		// Allocate memory for the entries.
		// 

		if (ProcessEntries != nullptr)
		{
            CkFreePool(ProcessEntries);
            ProcessEntries = nullptr;
		}
		
        ProcessEntries = CkAllocatePool(NonPagedPoolNx, ProcessEntriesLength);

		if (ProcessEntries == nullptr)
		{
            Status = STATUS_INSUFFICIENT_RESOURCES;
            break;
		}

		// 
		// Attempt to retrieve the processes again.
		// 

        Status = ZwQuerySystemInformation(SystemProcessInformation, ProcessEntries, ProcessEntriesLength, &ProcessEntriesLength);
	}

	// 
	// If we encountered any errors...
	// 

	if (NT_ERROR(Status))
	{
        if (ProcessEntries != nullptr)
            CkFreePool(ProcessEntries);
		
        return Status;
	}
	
	// 
	// Count the number of processes in the list.
	// 

	if (OutNumberOfProcessEntries != NULL)
	{
		ULONG NumberOfProcessEntries = 0;

		for (auto* ProcessEntry = (SYSTEM_PROCESS_INFORMATION*) OutProcessEntries; ; ProcessEntry = (SYSTEM_PROCESS_INFORMATION*) RtlAddOffsetToPointer(ProcessEntry, ProcessEntry->NextEntryOffset))
		{
			NumberOfProcessEntries++;

			// 
			// Check if this is the last entry in the list.
			// 

			if (ProcessEntry->NextEntryOffset == 0)
				break;
		}

        *OutNumberOfProcessEntries = NumberOfProcessEntries;
	}

    *OutProcessEntries = (SYSTEM_PROCESS_INFORMATION*) ProcessEntries;
	return STATUS_SUCCESS;
}

/// <summary>
/// Gets every processes information on the system matching a certain image file name.
/// </summary>
/// <param name="InProcessName">The process image filename.</param>
/// <param name="OutProcessEntries">The process entries.</param>
/// <param name="OutNumberOfProcessEntries">The number of entries in the buffer.</param>
NTSTATUS PsGetProcesses(UNICODE_STRING InProcessName, OUT SYSTEM_PROCESS_INFORMATION** OutProcessEntries, OPTIONAL OUT ULONG* OutNumberOfProcessEntries)
{
	NTSTATUS Status = { };

	// 
	// Verify the passed parameters.
	// 
    
	if (InProcessName.Buffer == NULL)
		return STATUS_INVALID_PARAMETER_1;

	// 
	// Retrieve every process entries on this system.
	// 

	SYSTEM_PROCESS_INFORMATION* ProcessEntries = NULL;
	ULONG NumberOfProcessEntries = 0;

	if (!NT_SUCCESS(Status = PsGetProcesses(&ProcessEntries, &NumberOfProcessEntries)))
		return Status;

	// 
	// Count the number of processes that matches the search.
	// 
	
	ULONG NumberOfMatchingProcessEntries = 0;

	for (auto* ProcessEntry = ProcessEntries; ; ProcessEntry = (SYSTEM_PROCESS_INFORMATION*) RtlAddOffsetToPointer(ProcessEntry, ProcessEntry->NextEntryOffset))
	{
		// 
		// Check if this is the process we are searching for.
		// 
        
		if (RtlEqualUnicodeString(&InProcessName, &ProcessEntry->ImageName, TRUE))
		{
			NumberOfMatchingProcessEntries++;
		}

		// 
		// Check if this is the last entry in the list.
		// 

		if (ProcessEntry->NextEntryOffset == 0)
			break;
	}

	// 
	// If there are no processes matching our search...
	// 

	if (NumberOfMatchingProcessEntries == 0)
	{
		CkFreePool(ProcessEntries);
		return STATUS_NOT_FOUND;
	}

	// 
	// ...else, allocate memory to store the matching processes.
	// 

	auto* MatchingProcessEntries = (SYSTEM_PROCESS_INFORMATION*) CkAllocatePool(NonPagedPoolNx, NumberOfMatchingProcessEntries * sizeof(SYSTEM_PROCESS_INFORMATION));

	if (MatchingProcessEntries == nullptr)
	{
		if (OutNumberOfProcessEntries != nullptr)
			*OutNumberOfProcessEntries = NumberOfMatchingProcessEntries;
		
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	// 
	// Copy the matching processes to the new allocated buffer.
	// 

	ULONG CurrentMatchingProcessIndex = 0;
	
	for (auto* ProcessEntry = ProcessEntries; ; ProcessEntry = (SYSTEM_PROCESS_INFORMATION*) RtlAddOffsetToPointer(ProcessEntry, ProcessEntry->NextEntryOffset))
	{
		// 
		// Check if this is the process we are searching for.
		// 
        
		if (RtlEqualUnicodeString(&InProcessName, &ProcessEntry->ImageName, TRUE))
		{
			RtlCopyMemory(&MatchingProcessEntries[CurrentMatchingProcessIndex], ProcessEntry, sizeof(SYSTEM_PROCESS_INFORMATION));
			RtlZeroMemory(&MatchingProcessEntries[CurrentMatchingProcessIndex].ImageName, sizeof(UNICODE_STRING));

			if (++CurrentMatchingProcessIndex == NumberOfMatchingProcessEntries)
				MatchingProcessEntries[CurrentMatchingProcessIndex].NextEntryOffset = 0;
			else
				MatchingProcessEntries[CurrentMatchingProcessIndex].NextEntryOffset = sizeof(SYSTEM_PROCESS_INFORMATION);
		}

		// 
		// Check if this is the last entry in the list (or the last matching entry).
		// 

		if (ProcessEntry->NextEntryOffset == 0 ||
			CurrentMatchingProcessIndex >= NumberOfMatchingProcessEntries)
		{
			break;
		}
	}
	
	// 
	// Release the memory allocated for the process entries.
	// 

    CkFreePool(ProcessEntries);

	// 
	// Return the result.
	// 

	*OutProcessEntries = MatchingProcessEntries;

	if (OutNumberOfProcessEntries != NULL)
		*OutNumberOfProcessEntries = NumberOfMatchingProcessEntries;

	return NumberOfMatchingProcessEntries > 0 ? STATUS_SUCCESS : STATUS_NOT_FOUND;
}

/// <summary>
/// Gets every processes information on the system matching a certain image file name.
/// </summary>
/// <param name="InProcessName">The process image filename.</param>
/// <param name="OutProcessEntries">The process entries.</param>
/// <param name="OutNumberOfProcessEntries">The number of entries in the buffer.</param>
NTSTATUS PsGetProcesses(CONST WCHAR* InProcessName, OUT SYSTEM_PROCESS_INFORMATION** OutProcessEntries, OPTIONAL OUT ULONG* OutNumberOfProcessEntries)
{
    UNICODE_STRING ProcessName;
    RtlInitUnicodeString(&ProcessName, InProcessName);
	
    return PsGetProcesses(ProcessName, OutProcessEntries, OutNumberOfProcessEntries);
}

/// <summary>
/// Gets every processes information on the system matching a certain image file name.
/// </summary>
/// <param name="InProcessName">The process image filename.</param>
/// <param name="OutProcessEntries">The process entries.</param>
/// <param name="OutNumberOfProcessEntries">The number of entries in the buffer.</param>
NTSTATUS PsGetProcesses(CONST CHAR* InProcessName, OUT SYSTEM_PROCESS_INFORMATION** OutProcessEntries, OPTIONAL OUT ULONG* OutNumberOfProcessEntries)
{
    ANSI_STRING ProcessNameToConvert;
    RtlInitAnsiString(&ProcessNameToConvert, InProcessName);

    UNICODE_STRING ProcessName;
    RtlAnsiStringToUnicodeString(&ProcessName, &ProcessNameToConvert, TRUE);
	
    CONST NTSTATUS Status = PsGetProcesses(ProcessName, OutProcessEntries, OutNumberOfProcessEntries);
    RtlFreeUnicodeString(&ProcessName);
    return Status;
}


/// <summary>
/// Gets information about the process with the given filename.
/// </summary>
/// <param name="InProcessName">The process image filename.</param>
/// <param name="OutProcessInformation">The returned process information.</param>
NTSTATUS PsGetProcessInformation(UNICODE_STRING InProcessName, OUT SYSTEM_PROCESS_INFORMATION* OutProcessInformation)
{
	NTSTATUS Status = { };

	// 
	// Verify the passed parameters.
	// 
    
	if (InProcessName.Buffer == NULL)
		return STATUS_INVALID_PARAMETER_1;

	// 
	// Retrieve every process entries on this system.
	// 

	SYSTEM_PROCESS_INFORMATION* ProcessEntries = NULL;
	ULONG NumberOfProcessEntries = 0;

	if (!NT_SUCCESS(Status = PsGetProcesses(&ProcessEntries, &NumberOfProcessEntries)))
		return Status;

	// 
	// Loop thru each process entries...
	// 

	BOOLEAN WasSearchSuccessful = FALSE;
	SYSTEM_PROCESS_INFORMATION ProcessInformation = { };

	for (auto* ProcessEntry = ProcessEntries; ; ProcessEntry = (SYSTEM_PROCESS_INFORMATION*) RtlAddOffsetToPointer(ProcessEntry, ProcessEntry->NextEntryOffset))
	{
		// 
		// Check if this is the process we are searching for.
		// 
        
		if (RtlEqualUnicodeString(&InProcessName, &ProcessEntry->ImageName, TRUE))
		{
			WasSearchSuccessful = TRUE;
			ProcessInformation = *ProcessEntry;
			break;
		}

		// 
		// Check if this is the last entry in the list.
		// 

		if (ProcessEntry->NextEntryOffset == 0)
			break;
	}

	// 
	// Release the memory allocated for the process entries.
	// 

    CkFreePool(ProcessEntries);

	// 
	// Return the result.
	// 

	if (WasSearchSuccessful)
	{
		if (OutProcessInformation != NULL)
			*OutProcessInformation = ProcessInformation;

		// 
		// The image name was in the buffer we just released.
		// 

		if (OutProcessInformation != NULL)
			RtlZeroMemory(&OutProcessInformation->ImageName, sizeof(OutProcessInformation->ImageName));
	}

	return WasSearchSuccessful ? STATUS_SUCCESS : STATUS_NOT_FOUND;
}

/// <summary>
/// Gets information about the process with the given filename.
/// </summary>
/// <param name="InProcessName">The process image filename.</param>
/// <param name="OutProcessInformation">The returned process information.</param>
NTSTATUS PsGetProcessInformation(CONST WCHAR* InProcessName, OUT SYSTEM_PROCESS_INFORMATION* OutProcessInformation)
{
    UNICODE_STRING ProcessName;
    RtlInitUnicodeString(&ProcessName, InProcessName);
	
    return PsGetProcessInformation(ProcessName, OutProcessInformation);
}

/// <summary>
/// Gets information about the process with the given filename.
/// </summary>
/// <param name="InProcessName">The process image filename.</param>
/// <param name="OutProcessInformation">The returned process information.</param>
NTSTATUS PsGetProcessInformation(CONST CHAR* InProcessName, OUT SYSTEM_PROCESS_INFORMATION* OutProcessInformation)
{
    ANSI_STRING ProcessNameToConvert;
    RtlInitAnsiString(&ProcessNameToConvert, InProcessName);

    UNICODE_STRING ProcessName;
    RtlAnsiStringToUnicodeString(&ProcessName, &ProcessNameToConvert, TRUE);
	
    CONST NTSTATUS Status = PsGetProcessInformation(ProcessName, OutProcessInformation);
    RtlFreeUnicodeString(&ProcessName);
    return Status;
}

/// <summary>
/// Gets information about the process with the given process id.
/// </summary>
/// <param name="InProcessId">The process identifier.</param>
/// <param name="OutProcessInformation">The returned process information.</param>
NTSTATUS PsGetProcessInformation(CONST HANDLE InProcessId, OUT SYSTEM_PROCESS_INFORMATION* OutProcessInformation)
{
	NTSTATUS Status = { };

	// 
	// Verify the passed parameters.
	// 
    
	if (InProcessId == NULL)
		return STATUS_INVALID_PARAMETER_1;

	// 
	// Retrieve every process entries on this system.
	// 

	SYSTEM_PROCESS_INFORMATION* ProcessEntries = NULL;
	ULONG NumberOfProcessEntries = 0;

	if (!NT_SUCCESS(Status = PsGetProcesses(&ProcessEntries, &NumberOfProcessEntries)))
		return Status;

	// 
	// Loop thru each process entries...
	// 
	
	BOOLEAN WasSearchSuccessful = FALSE;
	SYSTEM_PROCESS_INFORMATION ProcessInformation = { };

	for (auto* ProcessEntry = ProcessEntries; ; ProcessEntry = (SYSTEM_PROCESS_INFORMATION*) RtlAddOffsetToPointer(ProcessEntry, ProcessEntry->NextEntryOffset))
	{
		// 
		// Check if this is the process we are searching for.
		// 
        
		if (ProcessEntry->UniqueProcessId == InProcessId)
		{
			WasSearchSuccessful = TRUE;
			ProcessInformation = *ProcessEntry;
			break;
		}

		// 
		// Check if this is the last entry in the list.
		// 

		if (ProcessEntry->NextEntryOffset == 0)
			break;
	}

	// 
	// Release the memory allocated for the process entries.
	// 

    CkFreePool(ProcessEntries);

	// 
	// Return the result.
	// 

	if (WasSearchSuccessful)
	{
		if (OutProcessInformation != NULL)
			*OutProcessInformation = ProcessInformation;

		// 
		// The image name was in the buffer we just released.
		// 

		if (OutProcessInformation != NULL)
			RtlZeroMemory(&OutProcessInformation->ImageName, sizeof(OutProcessInformation->ImageName));
	}

	return WasSearchSuccessful ? STATUS_SUCCESS : STATUS_NOT_FOUND;
}

/// <summary>
/// Gets information about the process with the given process object.
/// </summary>
/// <param name="InProcess">The process object.</param>
/// <param name="OutProcessInformation">The returned process information.</param>
NTSTATUS PsGetProcessInformation(CONST PEPROCESS InProcess, OUT SYSTEM_PROCESS_INFORMATION* OutProcessInformation)
{
	// 
	// Verify the passed parameters.
	// 

	if (InProcess == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	return PsGetProcessInformation(PsGetProcessId(InProcess), OutProcessInformation);
}

/// <summary>
/// Gets the image file path of the given process.
/// </summary>
/// <param name="InProcess">The process object.</param>
/// <param name="OutProcessName">The process image file path.</param>
NTSTATUS PsGetProcessImageFilePath(CONST PEPROCESS InProcess, OUT WCHAR** OutProcessName)
{
	NTSTATUS Status = { };

	// 
	// Verify the passed arguments.
	// 

	if (InProcess == NULL)
		return STATUS_INVALID_PARAMETER_1;

	if (OutProcessName == NULL)
		return STATUS_INVALID_PARAMETER_2;

	// 
	// If this is a special process...
	// 
	
	if (PsGetProcessId(InProcess) == NULL)
	{
		constexpr WCHAR ProcessName[] = L"System Idle Process";
		WCHAR* Pool = (WCHAR*) CkAllocatePool(NonPagedPoolNx, sizeof(ProcessName));
		RtlCopyMemory(Pool, ProcessName, sizeof(ProcessName));
		*OutProcessName = Pool;
		return STATUS_SUCCESS;
	}
	else if (InProcess == PsInitialSystemProcess)
	{
		constexpr WCHAR ProcessName[] = L"System Process";
		WCHAR* Pool = (WCHAR*) CkAllocatePool(NonPagedPoolNx, sizeof(ProcessName));
		RtlCopyMemory(Pool, ProcessName, sizeof(ProcessName));
		*OutProcessName = Pool;
		return STATUS_SUCCESS;
	}

	// 
	// Open a handle to the process.
	// 

	auto* ProcessHandle = ZwCurrentProcess();

	if (PsGetCurrentProcess() != InProcess)
		if (!NT_SUCCESS(Status = ObOpenObjectByPointer(InProcess, OBJ_KERNEL_HANDLE, NULL, GENERIC_ALL, *PsProcessType, KernelMode, &ProcessHandle)))
			return Status;

	// 
	// Retrieve the name of the process with the given PID.
	// 

    PVOID Buffer = nullptr;
	ULONG BufferLength = 0;
	Status = ZwQueryInformationProcess(ProcessHandle, ProcessImageFileName, Buffer, BufferLength, &BufferLength);

	while (Status == STATUS_BUFFER_TOO_SMALL ||
		   Status == STATUS_INFO_LENGTH_MISMATCH)
	{
		// 
		// Add 8 characters to the buffer.
		// 

		BufferLength += sizeof(WCHAR) * 8;

		// 
		// Allocate memory for the process name.
		// 

		if (Buffer != nullptr)
		{
            CkFreePool(Buffer);
			Buffer = nullptr;
		}
		
		Buffer = CkAllocatePool(NonPagedPoolNx, BufferLength);

		if (Buffer == nullptr)
		{
            Status = STATUS_INSUFFICIENT_RESOURCES;
            break;
		}

		// 
		// Attempt to retrieve the process name again.
		// 

        Status = ZwQueryInformationProcess(ProcessHandle, ProcessImageFileName, Buffer, BufferLength, &BufferLength);
	}

	// 
	// If we encountered any errors...
	// 

	if (NT_ERROR(Status))
	{
        if (Buffer != nullptr)
            CkFreePool(Buffer);

		if (ProcessHandle != ZwCurrentProcess())
			ZwClose(ProcessHandle);
		
        return Status;
	}

	// 
	// Close the handle to the process.
	// 

	if (ProcessHandle != ZwCurrentProcess())
		ZwClose(ProcessHandle);

	// 
	// Move the string to the beginning of the buffer.
	// 

	CONST UNICODE_STRING* ProcessName = (UNICODE_STRING*) Buffer;
	CONST WCHAR* ProcessNameBuffer = ProcessName->Buffer;
	CONST USHORT ProcessNameBufferLength = ProcessName->Length;

	// 
	// Add a null terminator character.
	// 
	
	RtlMoveMemory(Buffer, ProcessNameBuffer, ProcessNameBufferLength);
	RtlZeroMemory(RtlAddOffsetToPointer(Buffer, ProcessNameBufferLength), sizeof(WCHAR));
	
	// 
	// Return the result.
	// 

	if (OutProcessName != nullptr)
		*OutProcessName = (WCHAR*) Buffer;
	else
		CkFreePool(Buffer);
	
	return Status;
}

/// <summary>
/// Gets the image filename of the given process.
/// </summary>
/// <param name="InProcess">The process object.</param>
/// <param name="OutProcessName">The process image filename.</param>
NTSTATUS PsGetProcessImageFileName(CONST PEPROCESS InProcess, OUT WCHAR** OutProcessName)
{
	NTSTATUS Status = { };

	// 
	// Retrieve the process's full image file path.
	// 

	WCHAR* FullProcessFilePath;

	if (!NT_SUCCESS(Status = PsGetProcessImageFilePath(InProcess, &FullProcessFilePath)))
		return Status;

	// 
	// Format the path to only get the actual filename with its extension.
	// 
	
	CONST WCHAR* FileName = wcschr(FullProcessFilePath, OBJ_NAME_PATH_SEPARATOR);

	if (FileName != NULL)
	{
		FileName = (WCHAR*) RtlAddOffsetToPointer(FileName, sizeof(WCHAR));
		CONST SIZE_T LengthOfFileName = RtlStringLength(FileName);
		WCHAR* Pool = (WCHAR*) CkAllocatePool(NonPagedPoolNx, (LengthOfFileName + 2) * sizeof(WCHAR));
		wcscpy_s(Pool, LengthOfFileName + 1, FileName);
		*OutProcessName = Pool;
		CkFreePool(FullProcessFilePath);
	}
	else
	{
		*OutProcessName = FullProcessFilePath;
	}
	
	return STATUS_SUCCESS;
}

/// <summary>
/// Terminates a process by its object with the given exit status.
/// </summary>
/// <param name="InProcess">The process object.</param>
/// <param name="InExitStatus">The exist status.</param>
NTSTATUS PsTerminateProcess(CONST PEPROCESS InProcess, NTSTATUS InExitStatus)
{
	NTSTATUS Status = { };

	// 
	// Verify the passed parameters.
	// 

	if (InProcess == NULL)
		return STATUS_INVALID_PARAMETER_1;

	// 
	// Open a handle to the process.
	// 

	HANDLE Handle = NULL;
				
	if (!NT_SUCCESS(Status = ObOpenObjectByPointer(InProcess, OBJ_KERNEL_HANDLE, NULL, PROCESS_TERMINATE, NULL, KernelMode, &Handle)))
		return Status;

	// 
	// Terminate the process.
	// 

	Status = ZwTerminateProcess(Handle, InExitStatus);

	// 
	// Close the handle to the process.
	// 

	ZwClose(Handle);
	return Status;
}

/// <summary>
/// Gets a value indicating whether the specified process is terminating.
/// </summary>
/// <param name="InProcess">The process.</param>
BOOLEAN PsProcessIsTerminating(CONST PEPROCESS InProcess)
{
	// 
	// Verify the passed parameters.
	// 

	if (InProcess == NULL)
		return FALSE;
	
	// 
	// Check if the given process's object header has been set.
	// 

	LARGE_INTEGER ZeroTime = { 0, 0 };
	return KeWaitForSingleObject(InProcess, Executive, KernelMode, FALSE, &ZeroTime) == STATUS_WAIT_0;
}
