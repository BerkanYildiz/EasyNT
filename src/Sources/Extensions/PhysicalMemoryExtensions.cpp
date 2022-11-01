#include "../../Headers/EasyNT.h"

/// <summary>
/// Maps physical memory and execute a callback on its scope.
/// </summary>
/// <param name="InPhysicalAddress">The physical address.</param>
/// <param name="InNumberOfBytes">The number of bytes to read.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS CkScopePhysicalMemory(PHYSICAL_ADDRESS InPhysicalAddress, SIZE_T InNumberOfBytes, void(*InCallback)(PVOID, SIZE_T))
{
	return CkScopePhysicalMemory<PVOID>(InPhysicalAddress, InNumberOfBytes, InCallback, [] (PVOID InBuffer, SIZE_T InNumberOfBytes, PVOID InContext)
	{
		((void(*)(PVOID, SIZE_T)) InContext) (InBuffer, InNumberOfBytes);
	});
}

/// <summary>
/// Maps physical memory and execute a callback on its scope.
/// </summary>
/// <param name="InPhysicalAddress">The physical address.</param>
/// <param name="OutBuffer">The buffer.</param>
/// <param name="InNumberOfBytes">The number of bytes to read.</param>
NTSTATUS CkReadPhysicalMemory(PHYSICAL_ADDRESS InPhysicalAddress, PVOID OutBuffer, SIZE_T InNumberOfBytes)
{
	// 
	// Verify the passed parameters.
	// 

	if (InPhysicalAddress.QuadPart == 0)
		return STATUS_INVALID_PARAMETER_1;

	if (OutBuffer == nullptr)
		return STATUS_INVALID_PARAMETER_2;

	if (InNumberOfBytes == 0)
		return STATUS_INVALID_PARAMETER_3;

	// 
	// Map the physical memory to the system address space.
	// 
	
	CONST PVOID VirtualAddress = MmMapIoSpace(InPhysicalAddress, PAGE_ROUND_UP(InNumberOfBytes), MmNonCached);

	if (VirtualAddress == nullptr)
		return STATUS_INTERNAL_ERROR;

	// 
	// Copy the memory and revert the mapping.
	// 

	RtlCopyMemory(OutBuffer, VirtualAddress, InNumberOfBytes);
	MmUnmapIoSpace(VirtualAddress, PAGE_ROUND_UP(InNumberOfBytes));
	return STATUS_SUCCESS;
}

/// <summary>
/// Maps physical memory and write data into it.
/// </summary>
/// <param name="InPhysicalAddress">The physical address.</param>
/// <param name="InBuffer">The buffer.</param>
/// <param name="InNumberOfBytes">The number of bytes to write.</param>
NTSTATUS CkWritePhysicalMemory(PHYSICAL_ADDRESS InPhysicalAddress, PVOID InBuffer, SIZE_T InNumberOfBytes)
{
	// 
	// Verify the passed parameters.
	// 

	if (InPhysicalAddress.QuadPart == 0)
		return STATUS_INVALID_PARAMETER_1;

	if (InBuffer == nullptr)
		return STATUS_INVALID_PARAMETER_2;

	if (InNumberOfBytes == 0)
		return STATUS_INVALID_PARAMETER_3;

	// 
	// Map the physical memory to the system address space.
	// 
	
	CONST PVOID VirtualAddress = MmMapIoSpace(InPhysicalAddress, PAGE_ROUND_UP(InNumberOfBytes), MmNonCached);

	if (VirtualAddress == nullptr)
		return STATUS_INTERNAL_ERROR;

	// 
	// Copy the memory and revert the mapping.
	// 
	
	RtlCopyMemory(VirtualAddress, InBuffer, InNumberOfBytes);
	MmUnmapIoSpace(VirtualAddress, PAGE_ROUND_UP(InNumberOfBytes));
	return STATUS_SUCCESS;
}

/// <summary>
/// Maps physical memory and zero its content.
/// </summary>
/// <param name="InPhysicalAddress">The physical address.</param>
/// <param name="InNumberOfBytes">The number of bytes to write.</param>
NTSTATUS CkZeroPhysicalMemory(PHYSICAL_ADDRESS InPhysicalAddress, SIZE_T InNumberOfBytes)
{
	// 
	// Verify the passed parameters.
	// 

	if (InPhysicalAddress.QuadPart == 0)
		return STATUS_INVALID_PARAMETER_1;

	if (InNumberOfBytes == 0)
		return STATUS_INVALID_PARAMETER_2;

	// 
	// Map the physical memory to the system address space.
	// 
	
	CONST PVOID VirtualAddress = MmMapIoSpace(InPhysicalAddress, PAGE_ROUND_UP(InNumberOfBytes), MmNonCached);

	if (VirtualAddress == nullptr)
		return STATUS_INTERNAL_ERROR;

	// 
	// Zero the memory and revert the mapping.
	// 
	
	RtlSecureZeroMemory(VirtualAddress, InNumberOfBytes);
	MmUnmapIoSpace(VirtualAddress, PAGE_ROUND_UP(InNumberOfBytes));
	return STATUS_SUCCESS;
}