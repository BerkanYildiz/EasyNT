#pragma once

/// <summary>
/// Calculates a sum of every bytes in the given memory range.
/// </summary>
/// <param name="InVirtualAddress">The virtual address.</param>
/// <param name="InNumberOfBytes">The number of bytes.</param>
UINT8 CkCalculateFletcher(CONST PVOID InVirtualAddress, SIZE_T InNumberOfBytes);

/// <summary>
/// Calculates a sum of every bytes in the given memory range and verify if the result is equal to zero.
/// </summary>
/// <param name="InVirtualAddress">The virtual address.</param>
/// <param name="InNumberOfBytes">The number of bytes.</param>
BOOLEAN CkValidateFletcher(CONST PVOID InVirtualAddress, SIZE_T InNumberOfBytes);