#include "../../Headers/EasyNT.h"

/// <summary>
/// Allocate virtual memory in a given process.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InNumberOfBytes">The number of bytes to allocate.</param>
/// <param name="InAllocationType">The type of the allocation.</param>
/// <param name="InProtection">The page protection.</param>
/// <param name="InOutAllocationAddress">In: The address to allocate memory at / Out: The resulting allocation address.</param>
NTSTATUS CkAllocateVirtualMemory(CONST PEPROCESS InProcess, SIZE_T InNumberOfBytes, ULONG InAllocationType, ULONG InProtection, IN OUT PVOID* InOutAllocationAddress)
{
	NTSTATUS Status = { };

	// 
	// Verify the passed parameters.
	// 

	if (InProcess == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InNumberOfBytes == 0)
		return STATUS_INVALID_PARAMETER_2;
	
	if (InAllocationType == 0)
		return STATUS_INVALID_PARAMETER_3;
	
	if (InProtection == 0)
		return STATUS_INVALID_PARAMETER_4;
	
	if (InOutAllocationAddress == nullptr)
		return STATUS_INVALID_PARAMETER_5;

	// 
	// Open a handle to the process.
	// 

	auto* Handle = ZwCurrentProcess();

	if (InProcess != PsGetCurrentProcess())
		if (!NT_SUCCESS(Status = ObOpenObjectByPointer(InProcess, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, GENERIC_ALL, *PsProcessType, KernelMode, &Handle)))
			return Status;

	// 
	// Allocate virtual memory.
	// 

	SIZE_T NumberOfBytes = InNumberOfBytes;
	PVOID BaseAddress = *InOutAllocationAddress;
	Status = ZwAllocateVirtualMemory(Handle, &BaseAddress, 0, &NumberOfBytes, InAllocationType, InProtection);

	// 
	// If we opened a handle for the process, close it.
	// 

	if (Handle != ZwCurrentProcess())
		ZwClose(Handle);

	// 
	// If the allocation was successful...
	// 

	if (NT_SUCCESS(Status))
	{
		// 
		// Zero the page(s).
		// 

		CkZeroVirtualMemory(InProcess, BaseAddress, InNumberOfBytes);

		// 
		// Return the allocation address.
		// 

		*InOutAllocationAddress = BaseAddress;
	}

	return Status;
}

/// <summary>
/// Releases virtual memory previously allocated in the given process.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InBaseAddress">The virtual address to free.</param>
/// <param name="InNumberOfBytes">The number of bytes to free.</param>
/// <param name="InFreeType">The type of the free.</param>
NTSTATUS CkFreeVirtualMemory(CONST PEPROCESS InProcess, CONST PVOID InBaseAddress, SIZE_T InNumberOfBytes, ULONG InFreeType)
{
	NTSTATUS Status = { };

	// 
	// Verify the passed parameters.
	// 

	if (InProcess == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InBaseAddress == nullptr)
		return STATUS_INVALID_PARAMETER_2;

	if (InNumberOfBytes == 0)
		return STATUS_INVALID_PARAMETER_3;

	if (InFreeType == 0)
		return STATUS_INVALID_PARAMETER_4;

	// 
	// Open a handle to the process.
	// 

	auto* Handle = ZwCurrentProcess();

	if (InProcess != PsGetCurrentProcess())
		if (!NT_SUCCESS(Status = ObOpenObjectByPointer(InProcess, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, GENERIC_ALL, *PsProcessType, KernelMode, &Handle)))
			return Status;

	// 
	// Zero the virtual memory.
	// 

	CkZeroVirtualMemory(InProcess, InBaseAddress, InNumberOfBytes);
	
	// 
	// Release the virtual memory.
	// 

	auto* VirtualAddress = InBaseAddress;
	auto  VirtualSize = InFreeType == MEM_RELEASE ? 0 : InNumberOfBytes;

	Status = ZwFreeVirtualMemory(Handle, &VirtualAddress, &VirtualSize, InFreeType);

	// 
	// If we opened a handle for the process, close it.
	// 

	if (Handle != ZwCurrentProcess())
		ZwClose(Handle);

	return Status;
}

/// <summary>
/// Zero the virtual memory previously allocated in the given process.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InBaseAddress">The virtual address to zero.</param>
/// <param name="InNumberOfBytes">The number of bytes to zero.</param>
NTSTATUS CkZeroVirtualMemory(CONST PEPROCESS InProcess, CONST PVOID InBaseAddress, SIZE_T InNumberOfBytes)
{
	NTSTATUS Status = { };

	// 
	// Verify the passed parameters.
	// 

	if (InProcess == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InBaseAddress == nullptr)
		return STATUS_INVALID_PARAMETER_2;

	if (InNumberOfBytes == 0)
		return STATUS_INVALID_PARAMETER_3;

	// 
	// Attach to the process.
	// 

	KAPC_STATE ApcState = { };
	KeStackAttachProcess(InProcess, &ApcState);

	// 
	// Zero the virtual memory.
	// 

	RtlSecureZeroMemory(InBaseAddress, InNumberOfBytes);
	
	// 
	// Detach from the process.
	// 

	KeUnstackDetachProcess(&ApcState);
	return STATUS_SUCCESS;
}

/// <summary>
/// Safely copy virtual memory from the source address to the destination address.
/// </summary>
/// <param name="InSourceProcess">The source process.</param>
/// <param name="InSourceAddress">The source virtual address.</param>
/// <param name="InDestinationProcess">The destination process.</param>
/// <param name="InDestinationAddress">The destination virtual address.</param>
/// <param name="InNumberOfBytes">The number of bytes to copy.</param>
/// <param name="OutNumberOfBytesCopied">The number of bytes copied.</param>
NTSTATUS CkCopyVirtualMemory(CONST PEPROCESS InSourceProcess, CONST PVOID InSourceAddress, CONST PEPROCESS InDestinationProcess, PVOID InDestinationAddress, SIZE_T InNumberOfBytes, OPTIONAL OUT SIZE_T* OutNumberOfBytesCopied)
{
	NTSTATUS Status = { };

	// 
	// Verify the passed parameters.
	// 

	if (InSourceProcess == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InSourceAddress == nullptr)
		return STATUS_INVALID_PARAMETER_2;

	if (InDestinationProcess == nullptr)
		return STATUS_INVALID_PARAMETER_3;

	if (InDestinationAddress == nullptr)
		return STATUS_INVALID_PARAMETER_4;

	// 
	// Quick address validity check if we already are in the context of either process.
	// 

	if (InSourceProcess == PsGetCurrentProcess() && !MmIsAddressValid(InSourceAddress))
		return STATUS_INVALID_ADDRESS;

	if (InDestinationProcess == PsGetCurrentProcess() && !MmIsAddressValid(InDestinationAddress))
		return STATUS_INVALID_ADDRESS;

	// 
	// Copy virtual memory from the source process to the destination process.
	// 

	SIZE_T NumberOfBytesCopied = 0;
	Status = MmCopyVirtualMemory(InSourceProcess, InSourceAddress, InDestinationProcess, InDestinationAddress, InNumberOfBytes, KernelMode, &NumberOfBytesCopied);

	// 
	// Return the result.
	// 

	if (OutNumberOfBytesCopied)
		*OutNumberOfBytesCopied = NumberOfBytesCopied;

	return Status;
}

/// <summary>
/// Safely copy virtual memory from the source address to the destination address.
/// </summary>
/// <param name="InSourceAddress">The source virtual address.</param>
/// <param name="InDestinationAddress">The destination virtual address.</param>
/// <param name="InNumberOfBytes">The number of bytes to copy.</param>
/// <param name="OutNumberOfBytesCopied">The number of bytes copied.</param>
NTSTATUS CkCopyVirtualMemory(CONST PVOID InSourceAddress, PVOID InDestinationAddress, SIZE_T InNumberOfBytes, OPTIONAL OUT SIZE_T* OutNumberOfBytesCopied)
{
	return CkCopyVirtualMemory(PsGetCurrentProcess(), InSourceAddress, PsGetCurrentProcess(), InDestinationAddress, InNumberOfBytes, OutNumberOfBytesCopied);
}

/// <summary>
/// Queries information about the memory region located at the given virtual address.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InVirtualAddress">The virtual address of the region to lookup.</param>
/// <param name="OutMemoryInformation">The returned memory information.</param>
NTSTATUS CkQueryVirtualMemory(CONST PEPROCESS InProcess, CONST PVOID InVirtualAddress, OUT MEMORY_BASIC_INFORMATION* OutMemoryInformation)
{
	NTSTATUS Status = { };

	// 
	// Verify the passed parameters.
	// 

	if (InProcess == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InVirtualAddress == nullptr)
		return STATUS_INVALID_PARAMETER_2;

	if (OutMemoryInformation == nullptr)
		return STATUS_INVALID_PARAMETER_3;

	// 
	// Open a handle to the process.
	// 

	auto* Handle = ZwCurrentProcess();

	if (InProcess != PsGetCurrentProcess())
		if (!NT_SUCCESS(Status = ObOpenObjectByPointer(InProcess, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, GENERIC_ALL, *PsProcessType, KernelMode, &Handle)))
			return Status;

	// 
	// Retrieve the memory region information.
	// 

	MEMORY_BASIC_INFORMATION MemoryInformation = { };
	SIZE_T NumberOfBytesRead = 0;
	Status = ZwQueryVirtualMemory(Handle, InVirtualAddress, (MEMORY_INFORMATION_CLASS) MemoryBasicInformation, &MemoryInformation, sizeof(MEMORY_BASIC_INFORMATION), &NumberOfBytesRead);

	// 
	// If we opened a handle to the process, close it.
	// 

	if (Handle != ZwCurrentProcess())
		ZwClose(Handle);

	// 
	// Return the result.
	// 

	if (OutMemoryInformation != nullptr)
		RtlCopyMemory(OutMemoryInformation, &MemoryInformation, sizeof(MEMORY_BASIC_INFORMATION));
	
	return Status;
}

/// <summary>
/// Enumerates the memory regions in the given process and execute a callback for each entries.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InContext">The context.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS CkEnumerateVirtualMemory(CONST PEPROCESS InProcess, PVOID InContext, ENUMERATE_VIRTUAL_MEMORY_WITH_CONTEXT InCallback)
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
NTSTATUS CkEnumerateVirtualMemory(CONST PEPROCESS InProcess, ENUMERATE_VIRTUAL_MEMORY InCallback)
{
	return CkEnumerateVirtualMemory(InProcess, InCallback, [] (ULONG InIndex, MEMORY_BASIC_INFORMATION* InMemoryInformation, PVOID InContext) -> bool
	{
		auto* Callback = (ENUMERATE_VIRTUAL_MEMORY) InContext;
		return Callback(InIndex, InMemoryInformation);
	});
}

/// <summary>
/// Enumerates the memory regions in the given process and execute a callback for each entries inside the specified range.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InBaseAddress">The base address.</param>
/// <param name="InNumberOfBytes">The number of bytes.</param>
/// <param name="InContext">The context.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS CkEnumerateVirtualMemoryInRange(CONST PEPROCESS InProcess, CONST PVOID InBaseAddress, SIZE_T InNumberOfBytes, PVOID InContext, ENUMERATE_VIRTUAL_MEMORY_WITH_CONTEXT InCallback)
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
NTSTATUS CkEnumerateVirtualMemoryInRange(CONST PEPROCESS InProcess, CONST PVOID InBaseAddress, SIZE_T InNumberOfBytes, ENUMERATE_VIRTUAL_MEMORY InCallback)
{
	return CkEnumerateVirtualMemoryInRange(InProcess, InBaseAddress, InNumberOfBytes, InCallback, [] (ULONG InIndex, MEMORY_BASIC_INFORMATION* InMemoryInformation, PVOID InContext) -> bool
	{
		auto* Callback = (ENUMERATE_VIRTUAL_MEMORY) InContext;
		return Callback(InIndex, InMemoryInformation);
	});
}
