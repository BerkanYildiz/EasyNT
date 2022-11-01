#pragma once

#define PAGE_ROUND_DOWN(x)	((((ULONG_PTR) (x)) & (~(PAGE_SIZE-1))))
#define PAGE_ROUND_UP(x)	((((ULONG_PTR) (x)) + PAGE_SIZE-1) & (~(PAGE_SIZE-1)))

#define EASYNT_DEFAULT_ALLOCATION_TAG 'CkNt'

#ifdef _RELEASE
	#define EASYNT_ALLOCATION_TAG 0
#endif

#ifndef EASYNT_ALLOCATION_TAG
	#define EASYNT_ALLOCATION_TAG EASYNT_DEFAULT_ALLOCATION_TAG
#endif

/// <summary>
/// Allocates memory from a specific pool with a tag.
/// </summary>
/// <param name="InPoolType">The type of pool.</param>
/// <param name="InNumberOfBytes">The number of bytes.</param>
/// <param name="InTag">The tag.</param>
PVOID CkAllocatePoolWithTag(POOL_TYPE InPoolType, SIZE_T InNumberOfBytes, ULONG InTag);

/// <summary>
/// Allocates memory from a specific pool.
/// </summary>
/// <param name="InPoolType">The type of pool.</param>
/// <param name="InNumberOfBytes">The number of bytes.</param>
PVOID CkAllocatePool(POOL_TYPE InPoolType, SIZE_T InNumberOfBytes);

/// <summary>
/// Allocates memory from a specific pool with a tag.
/// </summary>
/// <param name="InPoolType">The type of pool.</param>
/// <param name="InTag">The tag.</param>
template <typename T>
T* CkAllocatePoolWithTag(POOL_TYPE InPoolType, ULONG InTag)
{
	return (T*) CkAllocatePool(sizeof(T), InPoolType, InTag);
}

/// <summary>
/// Allocates memory from a specific pool.
/// </summary>
/// <param name="InPoolType">The type of pool.</param>
template <typename T>
T* CkAllocatePool(POOL_TYPE InPoolType)
{
	return (T*) CkAllocatePoolWithTag<T>(InPoolType, EASYNT_ALLOCATION_TAG);
}

/// <summary>
/// Releases memory located at the given address.
/// </summary>
/// <param name="InAddress">The address of the pool allocation.</param>
/// <param name="InTag">The tag.</param>
void CkFreePoolWithTag(PVOID InAddress, ULONG InTag);

/// <summary>
/// Releases memory located at the given address.
/// </summary>
/// <param name="InAddress">The address of the pool allocation.</param>
void CkFreePool(PVOID InAddress);

/// <summary>
/// Releases memory located at the given address.
/// </summary>
/// <param name="InAddress">The address of the pool allocation.</param>
/// <param name="InTag">The tag.</param>
template <typename T>
void CkFreePoolWithTag(PVOID InAddress, ULONG InTag)
{
	// RtlSecureZeroMemory(InAddress, sizeof(T));
	CkFreePoolWithTag(InAddress, InTag);
}

/// <summary>
/// Releases memory located at the given address.
/// </summary>
/// <param name="InAddress">The address of the pool allocation.</param>
template <typename T>
void CkFreePool(PVOID InAddress)
{
	CkFreePoolWithTag<T>(InAddress, EASYNT_ALLOCATION_TAG);
}
