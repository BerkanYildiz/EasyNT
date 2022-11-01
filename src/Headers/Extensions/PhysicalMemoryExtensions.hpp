#pragma once

/// <summary>
/// Maps physical memory and execute a callback on its scope.
/// </summary>
/// <typeparam name="TContext">The type of the context.</typeparam>
/// <param name="InPhysicalAddress">The physical address.</param>
/// <param name="InNumberOfBytes">The number of bytes to read.</param>
/// <param name="InContext">The context.</param>
/// <param name="InCallback">The callback.</param>
template <typename TContext>
NTSTATUS CkScopePhysicalMemory(PHYSICAL_ADDRESS InPhysicalAddress, SIZE_T InNumberOfBytes, TContext InContext, void(*InCallback)(PVOID, SIZE_T, TContext))
{
	// 
	// Verify the passed parameters.
	// 

	if (InPhysicalAddress.QuadPart == 0)
		return STATUS_INVALID_PARAMETER_1;

	if (InNumberOfBytes == 0)
		return STATUS_INVALID_PARAMETER_2;

	if (InCallback == nullptr)
		return STATUS_INVALID_PARAMETER_4;

	// 
	// Map the physical memory to the system address space.
	// 
	
	CONST PVOID VirtualAddress = MmMapIoSpace(InPhysicalAddress, PAGE_ROUND_UP(InNumberOfBytes), MmNonCached);

	if (VirtualAddress == nullptr)
		return STATUS_INTERNAL_ERROR;

	// 
	// Execute the callback.
	// 

	InCallback(VirtualAddress, InNumberOfBytes, InContext);

	// 
	// Revert the mapping.
	// 
	
	MmUnmapIoSpace(VirtualAddress, PAGE_ROUND_UP(InNumberOfBytes));
	return STATUS_SUCCESS;
}

/// <summary>
/// Maps physical memory and execute a callback on its scope.
/// </summary>
/// <param name="InPhysicalAddress">The physical address.</param>
/// <param name="InNumberOfBytes">The number of bytes to read.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS CkScopePhysicalMemory(PHYSICAL_ADDRESS InPhysicalAddress, SIZE_T InNumberOfBytes, void(*InCallback)(PVOID, SIZE_T));

/// <summary>
/// Maps physical memory and read its content.
/// </summary>
/// <param name="InPhysicalAddress">The physical address.</param>
/// <param name="OutBuffer">The buffer.</param>
/// <param name="InNumberOfBytes">The number of bytes to read.</param>
NTSTATUS CkReadPhysicalMemory(PHYSICAL_ADDRESS InPhysicalAddress, PVOID OutBuffer, SIZE_T InNumberOfBytes);

/// <summary>
/// Maps physical memory and write data into it.
/// </summary>
/// <param name="InPhysicalAddress">The physical address.</param>
/// <param name="InBuffer">The buffer.</param>
/// <param name="InNumberOfBytes">The number of bytes to write.</param>
NTSTATUS CkWritePhysicalMemory(PHYSICAL_ADDRESS InPhysicalAddress, PVOID InBuffer, SIZE_T InNumberOfBytes);

/// <summary>
/// Maps physical memory and zero its content.
/// </summary>
/// <param name="InPhysicalAddress">The physical address.</param>
/// <param name="InNumberOfBytes">The number of bytes to write.</param>
NTSTATUS CkZeroPhysicalMemory(PHYSICAL_ADDRESS InPhysicalAddress, SIZE_T InNumberOfBytes);
