#include "../../Headers/EasyNT.h"

/// <summary>
/// Calculates a pointer to the code/data segment pointed by the instruction at the given address.
/// </summary>
/// <param name="InBaseAddress">Address to the start of the instruction.</param>
/// <param name="InRelativeAddressOffset">The offset of the relative address.</param>
/// <param name="InTotalInstructionLength">Total length of the instruction.</param
PVOID ResolveRelativeAddress(CONST PVOID InBaseAddress, ULONG InRelativeAddressOffset, ULONG InTotalInstructionLength)
{
	auto const OffsetToDataSegment = *(INT32*) RtlAddOffsetToPointer(InBaseAddress, InRelativeAddressOffset);
	return RtlAddOffsetToPointer(RtlAddOffsetToPointer(InBaseAddress, InTotalInstructionLength), OffsetToDataSegment);
}
