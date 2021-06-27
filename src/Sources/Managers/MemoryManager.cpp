#include "../../Headers/EasyNT.h"

/// <summary>
/// Allocates memory from a specific pool with a tag.
/// </summary>
/// <param name="InPoolType">The type of pool.</param>
/// <param name="InNumberOfBytes">The number of bytes.</param>
/// <param name="InTag">The tag.</param>
PVOID CkAllocatePoolWithTag(POOL_TYPE InPoolType, SIZE_T InNumberOfBytes, ULONG InTag)
{
	CONST PVOID Pool = ExAllocatePoolWithTag(InPoolType, InNumberOfBytes, InTag);

	if (Pool != nullptr)
		RtlSecureZeroMemory(Pool, InNumberOfBytes);

	return Pool;
}

/// <summary>
/// Allocates memory from a specific pool.
/// </summary>
/// <param name="InPoolType">The type of pool.</param>
/// <param name="InNumberOfBytes">The number of bytes.</param>
PVOID CkAllocatePool(POOL_TYPE InPoolType, SIZE_T InNumberOfBytes)
{
	return CkAllocatePoolWithTag(InPoolType, InNumberOfBytes, EASYNT_ALLOCATION_TAG);
}

/// <summary>
/// Releases memory located at the given address.
/// </summary>
/// <param name="InAddress">The address of the pool allocation.</param>
/// <param name="InTag">The tag.</param>
void CkFreePoolWithTag(PVOID InAddress, ULONG InTag)
{
	if (InAddress == nullptr)
		return;

	ExFreePoolWithTag(InAddress, InTag);
}

/// <summary>
/// Releases memory located at the given address.
/// </summary>
/// <param name="InAddress">The address of the pool allocation.</param>
void CkFreePool(PVOID InAddress)
{
	CkFreePoolWithTag(InAddress, EASYNT_ALLOCATION_TAG);
}
