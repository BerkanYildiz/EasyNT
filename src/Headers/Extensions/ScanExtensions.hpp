#pragma once

/// <summary>
/// Searches for a certain pattern inside the given memory range.
/// </summary>
/// <param name="InBaseAddress">The base address.</param>
/// <param name="InSize">The size of the region in bytes.</param>
/// <param name="InSignature">The signature.</param>
/// <param name="OutResult">The result.</param>
NTSTATUS CkTryFindPattern(CONST PVOID InBaseAddress, SIZE_T InSize, CONST CHAR* InSignature, OPTIONAL OUT PVOID* OutResult = nullptr);

/// <summary>
/// Searches for a successive pattern of a specific padding byte in the given memory range.
/// </summary>
/// <param name="InBaseAddress">The base address.</param>
/// <param name="InSize">The size of the region in bytes.</param>
/// <param name="InPaddingByte">The padding byte.</param>
/// <param name="InPaddingLength">The padding length.</param>
/// <param name="OutResult">The result.</param>
NTSTATUS CkTryFindPadding(CONST PVOID InBaseAddress, SIZE_T InSize, UINT8 InPaddingByte, SIZE_T InPaddingLength, OPTIONAL OUT PVOID* OutResult = nullptr);

/// <summary>
/// Searches for a successive pattern of a specific padding byte in the given memory range, starting from the end.
/// </summary>
/// <param name="InBaseAddress">The base address.</param>
/// <param name="InSize">The size of the region in bytes.</param>
/// <param name="InPaddingByte">The padding byte.</param>
/// <param name="InPaddingLength">The padding length.</param>
/// <param name="OutResult">The result.</param>
NTSTATUS CkTryFindPaddingFromEnd(CONST PVOID InBaseAddress, SIZE_T InSize, UINT8 InPaddingByte, SIZE_T InPaddingLength, OPTIONAL OUT PVOID* OutResult = nullptr);

/// <summary>
/// Searches for a certain pattern inside the given module's executable sections.
/// </summary>
/// <param name="InBaseAddress">The base address.</param>
/// <param name="InSignature">The signature.</param>
/// <param name="OutResult">The signature scan result.</param>
NTSTATUS CkTryFindPatternInModuleExecutableSections(CONST PVOID InBaseAddress, CONST CHAR* InSignature, OPTIONAL OUT PVOID* OutResult = nullptr);
