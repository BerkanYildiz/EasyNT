#pragma once

// EXTERN_C NTKERNELAPI ERESOURCE PsLoadedModuleResource;
// EXTERN_C NTKERNELAPI LIST_ENTRY PsLoadedModuleList;

typedef bool(* ENUMERATE_MODULE_SECTIONS)(ULONG InIndex, IMAGE_SECTION_HEADER* InSectionHeader);
typedef bool(* ENUMERATE_MODULE_SECTIONS_WITH_CONTEXT)(ULONG InIndex, IMAGE_SECTION_HEADER* InSectionHeader, VOID* InContext);

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

/// <summary>
/// Verifies and returns the DOS header of the module at the given address.
/// </summary>
/// <param name="InBaseAddress">The base address.</param>
PIMAGE_DOS_HEADER RtlModuleDosHeader(CONST PVOID InBaseAddress);

/// <summary>
/// Verifies and returns the NT headers of the module at the given address.
/// </summary>
/// <param name="InBaseAddress">The base address.</param>
PIMAGE_NT_HEADERS RtlModuleNtHeaders(CONST PVOID InBaseAddress);

/// <summary>
/// Verifies and returns a pointer to the first section header of the module at the given address.
/// </summary>
/// <param name="InBaseAddress">The base address.</param>
/// <param name="OutNumberOfSections">The number of sections.</param>
PIMAGE_SECTION_HEADER RtlModuleSectionHeaders(CONST PVOID InBaseAddress, OPTIONAL OUT ULONG* OutNumberOfSections = nullptr);

/// <summary>
/// Enumerates the sections headers of the module present at the given address.
/// </summary>
/// <param name="InBaseAddress">The base address.</param>
/// <param name="InContext">The context.</param>
/// <param name="InCallback">The callback.</param>
template <typename TContext = PVOID>
NTSTATUS RtlEnumerateModuleSections(CONST PVOID InBaseAddress, TContext InContext, bool(*InCallback)(ULONG InIndex, IMAGE_SECTION_HEADER* InSectionHeader, TContext InContext))
{
	// 
	// Verify the passed parameters.
	// 

	if (InBaseAddress == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InContext == nullptr)
		return STATUS_INVALID_PARAMETER_2;

	if (InCallback == nullptr)
		return STATUS_INVALID_PARAMETER_3;

	// 
	// Retrieve the NT headers.
	// 

	auto* const NtHeaders = RtlModuleNtHeaders(InBaseAddress);

	if (NtHeaders == nullptr)
		return STATUS_INVALID_IMAGE_FORMAT;
	
	// 
	// Retrieve the section headers.
	// 

	auto* const SectionHeaders = RtlModuleSectionHeaders(InBaseAddress);

	if (SectionHeaders == nullptr)
		return STATUS_INVALID_IMAGE_FORMAT;

	// 
	// Enumerates every section headers.
	// 

	for (WORD I = 0; I < NtHeaders->FileHeader.NumberOfSections; I++)
	{
		if (InCallback(I, &SectionHeaders[I], InContext))
			break;
	}
	
	return STATUS_SUCCESS;
}

/// <summary>
/// Enumerates the sections headers of the module present at the given address.
/// </summary>
/// <param name="InBaseAddress">The base address.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS RtlEnumerateModuleSections(CONST PVOID InBaseAddress, ENUMERATE_MODULE_SECTIONS InCallback);

/// <summary>
/// Gets the address of a function exported by the specified module.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InBaseAddress">The base address.</param>
/// <param name="InFunctionName">The name of the function.</param>
/// <param name="OutFunctionAddress">The address of the function.</param>
NTSTATUS RtlModuleFindExport(CONST PEPROCESS InProcess, CONST PVOID InBaseAddress, CONST CHAR* InFunctionName, OPTIONAL OUT PVOID* OutFunctionAddress = nullptr);
