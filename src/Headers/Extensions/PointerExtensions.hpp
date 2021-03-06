#pragma once

// 
// Macros to do simple add/sub arithmetic with pointers.
// 

#define RtlAddOffsetToPointer(Pointer, Offset) ((void*) ((UINT64) (Pointer) + (SIZE_T) (Offset)))
#define RtlSubOffsetFromPointer(Pointer, Offset) ((void*) ((UINT64) (Pointer) - (SIZE_T) (Offset)))

/// <summary>
/// Calculates a pointer to the data segment pointed by the instruction at the given address.
/// </summary>
/// <param name="InBaseAddress">Address to the start of the instruction.</param>
/// <param name="InRelativeAddressOffset">The offset of the relative address.</param>
/// <param name="InTotalInstructionLength">Total length of the instruction.</param
PVOID ResolveRelativeAddress(CONST PVOID InBaseAddress, ULONG InRelativeAddressOffset, ULONG InTotalInstructionLength);

/// <summary>
/// Calculates a pointer to the data segment pointed by the instruction at the given address.
/// </summary>
/// <param name="InBaseAddress">Address to the start of the instruction.</param>
/// <param name="InRelativeAddressOffset">The offset of the relative address.</param>
/// <param name="InTotalInstructionLength">Total length of the instruction.</param
PVOID GetDataPointedByInstruction(CONST PVOID InBaseAddress, ULONG InRelativeAddressOffset, ULONG InTotalInstructionLength);
