#pragma once

typedef bool(* ENUMERATE_VIRTUAL_MEMORY)(ULONG InIndex, MEMORY_BASIC_INFORMATION* InMemoryInformation);
typedef bool(* ENUMERATE_VIRTUAL_MEMORY_WITH_CONTEXT)(ULONG InIndex, MEMORY_BASIC_INFORMATION* InMemoryInformation, PVOID InContext);

/// <summary>
/// Allocate virtual memory in a given process.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InNumberOfBytes">The number of bytes to allocate.</param>
/// <param name="InAllocationType">The type of the allocation.</param>
/// <param name="InProtection">The page protection.</param>
/// <param name="InOutAllocationAddress">In: The address to allocate memory at / Out: The resulting allocation address.</param>
NTSTATUS CkAllocateVirtualMemory(CONST PEPROCESS InProcess, SIZE_T InNumberOfBytes, ULONG InAllocationType, ULONG InProtection, IN OUT PVOID* InOutAllocationAddress);

/// <summary>
/// Releases virtual memory previously allocated in the given process.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InBaseAddress">The virtual address to free.</param>
/// <param name="InNumberOfBytes">The number of bytes to free.</param>
/// <param name="InFreeType">The type of the free.</param>
NTSTATUS CkFreeVirtualMemory(CONST PEPROCESS InProcess, CONST PVOID InBaseAddress, SIZE_T InNumberOfBytes, ULONG InFreeType = MEM_RELEASE);

/// <summary>
/// Zero the virtual memory previously allocated in the given process.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InBaseAddress">The virtual address to zero.</param>
/// <param name="InNumberOfBytes">The number of bytes to zero.</param>
NTSTATUS CkZeroVirtualMemory(CONST PEPROCESS InProcess, CONST PVOID InBaseAddress, SIZE_T InNumberOfBytes);

/// <summary>
/// Safely copy virtual memory from the source address to the destination address.
/// </summary>
/// <param name="InSourceProcess">The source process.</param>
/// <param name="InSourceAddress">The source virtual address.</param>
/// <param name="InDestinationProcess">The destination process.</param>
/// <param name="InDestinationAddress">The destination virtual address.</param>
/// <param name="InNumberOfBytes">The number of bytes to copy.</param>
/// <param name="OutNumberOfBytesCopied">The number of bytes copied.</param>
NTSTATUS CkCopyVirtualMemory(CONST PEPROCESS InSourceProcess, CONST PVOID InSourceAddress, CONST PEPROCESS InDestinationProcess, PVOID InDestinationAddress, SIZE_T InNumberOfBytes, OPTIONAL OUT SIZE_T* OutNumberOfBytesCopied = nullptr);

/// <summary>
/// Safely copy virtual memory from the source address to the destination address.
/// </summary>
/// <param name="InSourceAddress">The source virtual address.</param>
/// <param name="InDestinationAddress">The destination virtual address.</param>
/// <param name="InNumberOfBytes">The number of bytes to copy.</param>
/// <param name="OutNumberOfBytesCopied">The number of bytes copied.</param>
NTSTATUS CkCopyVirtualMemory(CONST PVOID InSourceAddress, PVOID InDestinationAddress, SIZE_T InNumberOfBytes, OPTIONAL OUT SIZE_T* OutNumberOfBytesCopied = nullptr);

/// <summary>
/// Queries information about the memory region located at the given virtual address.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InVirtualAddress">The virtual address of the region to lookup.</param>
/// <param name="OutMemoryInformation">The returned memory information.</param>
NTSTATUS CkQueryVirtualMemory(CONST PEPROCESS InProcess, CONST PVOID InVirtualAddress, OUT MEMORY_BASIC_INFORMATION* OutMemoryInformation);

/// <summary>
/// Enumerates the memory regions in the given process and execute a callback for each entries.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InContext">The context.</param>
/// <param name="InCallback">The callback.</param>
template <typename TContext = PVOID>
NTSTATUS CkEnumerateVirtualMemory(CONST PEPROCESS InProcess, TContext InContext, bool(*InCallback)(ULONG InIndex, MEMORY_BASIC_INFORMATION* InMemoryInformation, TContext InContext))
{
	// 
	// Verify the passed parameters.
	// 

	if (InProcess == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InContext == nullptr)
		return STATUS_INVALID_PARAMETER_2;

	if (InCallback == nullptr)
		return STATUS_INVALID_PARAMETER_3;

	// 
	// Scan the memory regions.
	// 

	ULONG Index = 0;
	PVOID CurrentAddress = MM_LOWEST_USER_ADDRESS;
	
	while (CurrentAddress < MM_HIGHEST_USER_ADDRESS)
	{
		// 
		// Query information about the memory region.
		// 
		
		MEMORY_BASIC_INFORMATION MemoryInformation;

		if (!NT_SUCCESS(CkQueryVirtualMemory(InProcess, CurrentAddress, &MemoryInformation)))
			break;

		// 
		// Execute the callback.
		// 

		auto const SkipOtherEntries = InCallback(Index++, &MemoryInformation, InContext);

		if (SkipOtherEntries)
			break;

		// 
		// Move onto the next region.
		// 

		CurrentAddress = RtlAddOffsetToPointer(CurrentAddress, MemoryInformation.RegionSize);
	}

	return Index != 0 ? STATUS_SUCCESS : STATUS_NO_MORE_ENTRIES;
}

/// <summary>
/// Enumerates the memory regions in the given process and execute a callback for each entries.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS CkEnumerateVirtualMemory(CONST PEPROCESS InProcess, ENUMERATE_VIRTUAL_MEMORY InCallback);

/// <summary>
/// Enumerates the memory regions in the given process and execute a callback for each entries inside the specified range.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InBaseAddress">The base address.</param>
/// <param name="InNumberOfBytes">The number of bytes.</param>
/// <param name="InContext">The context.</param>
/// <param name="InCallback">The callback.</param>
template <typename TContext = PVOID>
NTSTATUS CkEnumerateVirtualMemoryInRange(CONST PEPROCESS InProcess, CONST PVOID InBaseAddress, SIZE_T InNumberOfBytes, TContext InContext, bool(*InCallback)(ULONG InIndex, MEMORY_BASIC_INFORMATION* InMemoryInformation, TContext InContext))
{
	// 
	// Verify the passed parameters.
	// 

	if (InProcess == nullptr)
		return STATUS_INVALID_PARAMETER_1;
	
	if (InBaseAddress == nullptr)
		return STATUS_INVALID_PARAMETER_2;
	
	if (InNumberOfBytes == 0)
		return STATUS_INVALID_PARAMETER_3;

	if (InContext == nullptr)
		return STATUS_INVALID_PARAMETER_4;

	if (InCallback == nullptr)
		return STATUS_INVALID_PARAMETER_5;

	// 
	// Scan the memory regions.
	// 

	ULONG Index = 0;
	PVOID CurrentAddress = InBaseAddress;
	CONST PVOID UpperLimit = RtlAddOffsetToPointer(InBaseAddress, InNumberOfBytes);
	
	while (CurrentAddress < UpperLimit)
	{
		// 
		// Query information about the memory region.
		// 
		
		MEMORY_BASIC_INFORMATION MemoryInformation;

		if (!NT_SUCCESS(CkQueryVirtualMemory(InProcess, CurrentAddress, &MemoryInformation)))
			break;

		// 
		// Execute the callback.
		// 

		auto const SkipOtherEntries = InCallback(Index++, &MemoryInformation, InContext);

		if (SkipOtherEntries)
			break;

		// 
		// Move onto the next region.
		// 

		if (CurrentAddress > MemoryInformation.BaseAddress)
			CurrentAddress = RtlAddOffsetToPointer(CurrentAddress, MemoryInformation.RegionSize - (SIZE_T) RtlSubOffsetFromPointer(CurrentAddress, MemoryInformation.BaseAddress));
		else
			CurrentAddress = RtlAddOffsetToPointer(CurrentAddress, MemoryInformation.RegionSize);
	}

	return Index != 0 ? STATUS_SUCCESS : STATUS_NO_MORE_ENTRIES;
}

/// <summary>
/// Enumerates the memory regions in the given process and execute a callback for each entries inside the specified range.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InBaseAddress">The base address.</param>
/// <param name="InNumberOfBytes">The number of bytes.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS CkEnumerateVirtualMemoryInRange(CONST PEPROCESS InProcess, CONST PVOID InBaseAddress, SIZE_T InNumberOfBytes, ENUMERATE_VIRTUAL_MEMORY InCallback);
