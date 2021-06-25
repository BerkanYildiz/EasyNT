#include "../../Headers/EasyNT.h"

/// <summary>
/// Gets information about every modules loaded into the given process.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="OutModuleEntries">The modules.</param>
/// <param name="OutNumberOfModules">The number of modules.</param>
NTSTATUS PsGetProcessModules(CONST PEPROCESS InProcess, OUT RTL_PROCESS_MODULE_INFORMATION** OutModuleEntries, OUT ULONG* OutNumberOfModules)
{
	NTSTATUS Status = { };

	// 
	// Verify the passed parameters.
	// 

	if (InProcess == nullptr)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (OutModuleEntries == nullptr)
	{
		return STATUS_INVALID_PARAMETER_2;
	}

	if (OutNumberOfModules == nullptr)
	{
		return STATUS_INVALID_PARAMETER_3;
	}

	// 
	// The system process uses a different list.
	// 

	if (InProcess == PsInitialSystemProcess)
	{
		// 
		// Retrieve the name of the process with the given PID.
		// 

	    PVOID Buffer = nullptr;
		ULONG BufferLength = 0;
		Status = ZwQuerySystemInformation(SystemModuleInformation, Buffer, BufferLength, &BufferLength);

		while (Status == STATUS_BUFFER_TOO_SMALL ||
			   Status == STATUS_INFO_LENGTH_MISMATCH)
		{
			// 
			// Round the buffer's length up.
			// 

			BufferLength = PAGE_ROUND_UP(BufferLength);

			// 
			// Allocate memory for the system modules.
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
			// Attempt to retrieve the system modules again.
			// 

			Status = ZwQuerySystemInformation(SystemModuleInformation, Buffer, BufferLength, &BufferLength);
		}

		// 
		// If we encountered any errors...
		// 

		if (NT_ERROR(Status))
		{
	        if (Buffer != nullptr)
	            CkFreePool(Buffer);
			
	        return Status;
		}

		// 
		// Allocate memory for the output.
		// 
		
		RTL_PROCESS_MODULES* ProcessModules = (RTL_PROCESS_MODULES*) Buffer;
		RTL_PROCESS_MODULE_INFORMATION* ModuleEntries = (RTL_PROCESS_MODULE_INFORMATION*) CkAllocatePool(NonPagedPoolNx, ProcessModules->NumberOfModules * sizeof(RTL_PROCESS_MODULE_INFORMATION));

		if (ModuleEntries == nullptr)
		{
			if (Buffer != nullptr)
				CkFreePool(Buffer);

			return STATUS_INSUFFICIENT_RESOURCES;
		}

		// 
		// Copy the modules information to the buffer we've just allocated.
		// 

		for (ULONG I = 0; I < ProcessModules->NumberOfModules; I++)
		{
			RtlCopyMemory(&ModuleEntries[I], &ProcessModules->Modules[I], sizeof(RTL_PROCESS_MODULE_INFORMATION));
		}
		
		// 
		// Return the results.
		// 

		*OutModuleEntries = ModuleEntries;
		*OutNumberOfModules = ProcessModules->NumberOfModules;

		CkFreePool(Buffer);
		return STATUS_SUCCESS;
	}

	// 
	// Retrieve the process environment block.
	// 

	PEB* Peb = (PEB*) PsGetProcessPeb(InProcess);

	if (Peb == nullptr)
		return STATUS_PROCESS_IS_PROTECTED;

	// 
	// Attach to the target process.
	// 

	KAPC_STATE ApcState = { };
	KeStackAttachProcess(InProcess, &ApcState);

	// 
	// Calculate the number of entries in the list.
	// 

	ULONG NumberOfEntries = 0;

	for (auto* ListEntry = Peb->Ldr->InLoadOrderModuleList.Flink; ListEntry != &Peb->Ldr->InLoadOrderModuleList && ListEntry != nullptr; ListEntry = ListEntry->Flink)
	{
		NumberOfEntries++;
	}

	// 
	// Allocate memory for the output.
	// 
	
	RTL_PROCESS_MODULE_INFORMATION* ModuleEntries = (RTL_PROCESS_MODULE_INFORMATION*) CkAllocatePool(NonPagedPoolNx, NumberOfEntries * sizeof(RTL_PROCESS_MODULE_INFORMATION));

	if (ModuleEntries == nullptr)
		return STATUS_INSUFFICIENT_RESOURCES;

	// 
	// Copy the entries to the buffer.
	// 

	ULONG CurrentEntryIdx = 0;

	for (auto* ModuleEntry = (PLDR_DATA_TABLE_ENTRY) Peb->Ldr->InLoadOrderModuleList.Flink; ModuleEntry != (PLDR_DATA_TABLE_ENTRY ) &Peb->Ldr->InLoadOrderModuleList && ModuleEntry != nullptr && CurrentEntryIdx < NumberOfEntries; ModuleEntry = (PLDR_DATA_TABLE_ENTRY) ModuleEntry->InLoadOrderLinks.Flink)
	{
		// 
		// Copy the most important fields.
		// 
		
		auto* SavedEntry = &ModuleEntries[CurrentEntryIdx++];
		SavedEntry->ImageBase = ModuleEntry->DllBase;
		SavedEntry->MappedBase = ModuleEntry->DllBase;
		SavedEntry->ImageSize = ModuleEntry->SizeOfImage;
		SavedEntry->LoadCount = ModuleEntry->LoadCount;
		SavedEntry->Section = ModuleEntry->SectionPointer;
		SavedEntry->Flags = ModuleEntry->Flags;
		SavedEntry->InitOrderIndex = CurrentEntryIdx != 0 ? CurrentEntryIdx - 1 : 0;
		SavedEntry->LoadOrderIndex = CurrentEntryIdx != 0 ? CurrentEntryIdx - 1 : 0;
		SavedEntry->OffsetToFileName = 0;

		// 
		// Convert the UNICODE file path to ANSI.
		// 
					
		ANSI_STRING AnsiPath = { };
		AnsiPath.Buffer = (CHAR*) &SavedEntry->FullPathName[0];
		AnsiPath.Length = 0;
		AnsiPath.MaximumLength = sizeof(SavedEntry->FullPathName);

		if (NT_SUCCESS(RtlUnicodeStringToAnsiString(&AnsiPath, &ModuleEntry->FullDllName, FALSE)))
		{
			// 
			// Calculate the offset to the filename.
			// 
			
			SavedEntry->OffsetToFileName = AnsiPath.Length - (ModuleEntry->BaseDllName.Length / sizeof(WCHAR));
		}
	}

	// 
	// Detach from the target process.
	// 

	KeUnstackDetachProcess(&ApcState);

	// 
	// Return the results.
	// 

	*OutModuleEntries = ModuleEntries;
	*OutNumberOfModules = min(NumberOfEntries, CurrentEntryIdx);
	return STATUS_SUCCESS;
}

/// <summary>
/// Gets information about a module with the given filename.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InModuleFilename">The filename of the module.</param>
/// <param name="OutModuleInformation">The result.</param>
NTSTATUS PsGetProcessModuleInformation(CONST PEPROCESS InProcess, CONST CHAR* InModuleFilename, OUT OPTIONAL RTL_PROCESS_MODULE_INFORMATION* OutModuleInformation)
{
	NTSTATUS Status = { };

	// 
	// Verify the passed parameters.
	// 

	if (InProcess == nullptr)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (InModuleFilename == nullptr)
	{
		return STATUS_INVALID_PARAMETER_2;
	}

	// 
	// Retrieve the modules loaded in the target process.
	// 

	RTL_PROCESS_MODULE_INFORMATION* Modules = NULL;
	ULONG ModulesCount = 0;

	if (!NT_SUCCESS(Status = PsGetProcessModules(InProcess, &Modules, &ModulesCount)))
	{
		return Status;
	}

	// 
	// Search for a module at the given address.
	// 
	
	BOOLEAN HasFoundModule = FALSE;
	RTL_PROCESS_MODULE_INFORMATION* Module = nullptr;

	for (ULONG I = 0; I < ModulesCount; I++)
	{
		// 
		// Compare the current entry's name against the wanted module's name.
		// 
		
		if (RtlEqualString((CHAR*) &Modules[I].FullPathName[Modules[I].OffsetToFileName], InModuleFilename, TRUE))
		{
			HasFoundModule = TRUE;
			Module = &Modules[I];
			break;
		}
	}

	// 
	// If we've found the module, return the result.
	// 

	if (HasFoundModule && OutModuleInformation != nullptr)
		RtlCopyMemory(OutModuleInformation, Module, sizeof(RTL_PROCESS_MODULE_INFORMATION));
	
	// 
	// Release the memory allocated when retrieving the modules.
	// 

	CkFreePool(Modules);
	return HasFoundModule ? STATUS_SUCCESS : STATUS_NOT_FOUND;
}

/// <summary>
/// Gets information about a module with the given filename.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InModuleFilename">The filename of the module.</param>
/// <param name="OutModuleInformation">The result.</param>
NTSTATUS PsGetProcessModuleInformation(CONST PEPROCESS InProcess, CONST WCHAR* InModuleFilename, OUT OPTIONAL RTL_PROCESS_MODULE_INFORMATION* OutModuleInformation)
{
	NTSTATUS Status = { };

	// 
	// Verify the passed parameters.
	// 

	if (InProcess == nullptr)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (InModuleFilename == nullptr)
	{
		return STATUS_INVALID_PARAMETER_2;
	}

	// 
	// Convert the UNICODE string to ANSI.
	// 
	
	UNICODE_STRING ModuleFilenameToConvert;
	RtlInitUnicodeString(&ModuleFilenameToConvert, InModuleFilename);

	ANSI_STRING ModuleFilename;

	if (NT_SUCCESS(Status = RtlUnicodeStringToAnsiString(&ModuleFilename, &ModuleFilenameToConvert, TRUE)))
	{
		Status = PsGetProcessModuleInformation(InProcess, ModuleFilename.Buffer, OutModuleInformation);
		RtlFreeAnsiString(&ModuleFilename);
	}

	return Status;
}

/// <summary>
/// Gets information about a module that matches the given address range.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InModuleAddress">A virtual address pointing inside a module.</param>
/// <param name="OutModuleInformation">The result.</param>
NTSTATUS PsGetProcessModuleInformationByAddress(CONST PEPROCESS InProcess, CONST PVOID InModuleAddress, OUT OPTIONAL RTL_PROCESS_MODULE_INFORMATION* OutModuleInformation)
{
	NTSTATUS Status = { };

	// 
	// Verify the passed parameters.
	// 

	if (InProcess == nullptr)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (InModuleAddress == nullptr)
	{
		return STATUS_INVALID_PARAMETER_2;
	}

	// 
	// Retrieve the modules loaded in the target process.
	// 

	RTL_PROCESS_MODULE_INFORMATION* Modules = NULL;
	ULONG ModulesCount = 0;

	if (!NT_SUCCESS(Status = PsGetProcessModules(InProcess, &Modules, &ModulesCount)))
	{
		return Status;
	}

	// 
	// Search for a module at the given address.
	// 
	
	BOOLEAN HasFoundModule = FALSE;
	RTL_PROCESS_MODULE_INFORMATION* Module = nullptr;

	for (ULONG I = 0; I < ModulesCount; I++)
	{
		// 
		// Check if the given virtual address points inside this module's address range.
		// 

		if (InModuleAddress >= Module->ImageBase &&
			InModuleAddress < RtlAddOffsetToPointer(Module->ImageBase, Module->ImageSize))
		{
			HasFoundModule = TRUE;
			Module = &Modules[I];
			break;
		}
	}

	// 
	// If we've found the module, return the result.
	// 

	if (HasFoundModule && OutModuleInformation != nullptr)
		RtlCopyMemory(OutModuleInformation, Module, sizeof(RTL_PROCESS_MODULE_INFORMATION));
	
	// 
	// Release the memory allocated when retrieving the modules.
	// 

	CkFreePool(Modules);
	return HasFoundModule ? STATUS_SUCCESS : STATUS_NOT_FOUND;
}
