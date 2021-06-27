#pragma once

/// <summary>
/// Searches for a certain pattern inside the given memory range.
/// </summary>
/// <param name="InBaseAddress">The base address.</param>
/// <param name="InSize">The size of the region in bytes.</param>
/// <param name="InSignature">The signature.</param>
/// <param name="OutResult">The signature scan result.</param>
BOOLEAN CkTryFindPattern(CONST PVOID InBaseAddress, SIZE_T InSize, CONST CHAR* InSignature, OPTIONAL OUT PVOID* OutResult);
