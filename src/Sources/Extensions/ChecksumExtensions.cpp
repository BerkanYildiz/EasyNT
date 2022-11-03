#include "../../Headers/EasyNT.h"

/// <summary>
/// Calculates a sum of every bytes in the given memory range.
/// </summary>
/// <param name="InVirtualAddress">The virtual address.</param>
/// <param name="InNumberOfBytes">The number of bytes.</param>
UINT8 CkCalculateFletcher(CONST PVOID InVirtualAddress, SIZE_T InNumberOfBytes)
{
	UINT8 Checksum = 0;

	for (SIZE_T I = 0; I < InNumberOfBytes; I++)
		Checksum += *(UINT8*) RtlAddOffsetToPointer(InVirtualAddress, I);

	return Checksum;
}

/// <summary>
/// Calculates a sum of every bytes in the given memory range and verify if the result is equal to zero.
/// </summary>
/// <param name="InVirtualAddress">The virtual address.</param>
/// <param name="InNumberOfBytes">The number of bytes.</param>
BOOLEAN CkValidateFletcher(CONST PVOID InVirtualAddress, SIZE_T InNumberOfBytes)
{
	return CkCalculateFletcher(InVirtualAddress, InNumberOfBytes) == (UINT8) 0x00;
}