#pragma once

typedef bool(* ENUMERATE_VIRTUAL_MEMORY)(ULONG InIndex, MEMORY_BASIC_INFORMATION* InMemoryInformation);
typedef bool(* ENUMERATE_VIRTUAL_MEMORY_WITH_CONTEXT)(ULONG InIndex, MEMORY_BASIC_INFORMATION* InMemoryInformation, VOID* InContext);

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
NTSTATUS CkEnumerateVirtualMemory(CONST PEPROCESS InProcess, PVOID InContext, ENUMERATE_VIRTUAL_MEMORY_WITH_CONTEXT InCallback);

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
NTSTATUS CkEnumerateVirtualMemoryInRange(CONST PEPROCESS InProcess, CONST PVOID InBaseAddress, SIZE_T InNumberOfBytes, PVOID InContext, ENUMERATE_VIRTUAL_MEMORY_WITH_CONTEXT InCallback);

/// <summary>
/// Enumerates the memory regions in the given process and execute a callback for each entries inside the specified range.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InBaseAddress">The base address.</param>
/// <param name="InNumberOfBytes">The number of bytes.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS CkEnumerateVirtualMemoryInRange(CONST PEPROCESS InProcess, CONST PVOID InBaseAddress, SIZE_T InNumberOfBytes, ENUMERATE_VIRTUAL_MEMORY InCallback);
