#include "../../Headers/EasyNT.h"

/// <summary>
/// Searches for a certain pattern inside the given memory range.
/// </summary>
/// <param name="InBaseAddress">The base address.</param>
/// <param name="InSize">The size of the region in bytes.</param>
/// <param name="InSignature">The signature.</param>
/// <param name="OutResult">The result.</param>
NTSTATUS CkTryFindPattern(CONST PVOID InBaseAddress, SIZE_T InSize, CONST CHAR* InSignature, OPTIONAL OUT PVOID* OutResult)
{
	// 
	// Verify the passed parameters.
	// 

	if (InBaseAddress == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InSize == 0)
		return STATUS_INVALID_PARAMETER_2;

	if (InSignature == nullptr)
		return STATUS_INVALID_PARAMETER_3;

	// 
	// Setup the scan structures.
	// 

	struct SIGNATURE_ENTRY
	{
		UINT8 Value;
		BOOLEAN IsWildcard;
	};

	SIGNATURE_ENTRY SignatureBytes[128];
	RtlZeroMemory(&SignatureBytes[0], sizeof(SignatureBytes));

	// 
	// Parse the signature according to the IDA format.
	// 

	SIZE_T SignatureLength = 0;
	SIZE_T SignatureStep = 0;
	
	while (InSignature[SignatureStep])
	{
		switch (InSignature[SignatureStep])
		{
			case ' ':
			case '-':
			{
				SignatureStep += sizeof(char) * 1;
				break;
			}

			case '?':
			{
				auto* Entry = &SignatureBytes[SignatureLength++];
				Entry->Value = 0x00;
				Entry->IsWildcard = TRUE;

				if (InSignature[SignatureStep + 1] == '?')
					SignatureStep += sizeof(char) * 2;
				else
					SignatureStep += sizeof(char) * 1;

				break;
			}
			
			default:
			{
				auto* Entry = &SignatureBytes[SignatureLength++];
				Entry->Value = RtlHexadecimalStringToByte(&InSignature[SignatureStep]);
				Entry->IsWildcard = FALSE;

				SignatureStep += sizeof(char) * 2;
				break;
			}
		}
	}

	// 
	// The signature cannot be bigger than the region of memory we are about to scan.
	// 
	
	if (InSize < SignatureLength)
		return STATUS_ARRAY_BOUNDS_EXCEEDED;

	// 
	// Loop through the memory region.
	// 

	for (SIZE_T X = 0; X < InSize - SignatureLength; X++)
	{
		// 
		// Search for the pattern.
		// 

		for (SIZE_T Y = 0; Y < SignatureLength; Y++)
		{
			auto* const Entry = &SignatureBytes[Y];

			// 
			// Check if the bytes matches.
			//
			
			if (Entry->IsWildcard == FALSE)
			{
				auto const Value = *(UINT8*) RtlAddOffsetToPointer(InBaseAddress, X + Y);

				if (Value != Entry->Value)
					break;
			}

			// 
			// If this was the last byte for the scan...
			// 
			
			if (Y == SignatureLength - 1)
			{
				if (OutResult != nullptr)
					*OutResult = RtlAddOffsetToPointer(InBaseAddress, X);

				return STATUS_SUCCESS;
			}

			break;
		}
	}

	return STATUS_NOT_FOUND;
}

/// <summary>
/// Searches for a successive pattern of a specific padding byte in the given memory range.
/// </summary>
/// <param name="InBaseAddress">The base address.</param>
/// <param name="InSize">The size of the region in bytes.</param>
/// <param name="InPaddingByte">The padding byte.</param>
/// <param name="InPaddingLength">The padding length.</param>
/// <param name="OutResult">The result.</param>
NTSTATUS CkTryFindPadding(CONST PVOID InBaseAddress, SIZE_T InSize, UINT8 InPaddingByte, SIZE_T InPaddingLength, OPTIONAL OUT PVOID* OutResult)
{
	// 
	// Verify the passed parameters.
	// 

	if (InBaseAddress == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InSize == 0)
		return STATUS_INVALID_PARAMETER_2;

	if (InPaddingLength == 0)
		return STATUS_INVALID_PARAMETER_4;

	// 
	// The padding cannot be bigger than the region of memory we are about to scan.
	// 

	if (InSize < InPaddingLength)
		return STATUS_ARRAY_BOUNDS_EXCEEDED;

	// 
	// Loop through the memory region.
	// 
	
	for (SIZE_T X = 0; X < InSize - InPaddingLength; X++)
	{
		// 
		// Search for the padding.
		// 

		for (SIZE_T Y = 0; Y < InPaddingLength; Y++)
		{
			// 
			// Check if the bytes matches.
			// 

			const auto CurrentByte = *(UINT8*) RtlAddOffsetToPointer(InBaseAddress, X + Y);

			if (CurrentByte != InPaddingByte)
				break;

			// 
			// If this was the last byte for the scan...
			// 
			
			if (Y == InPaddingLength - 1)
			{
				if (OutResult != nullptr)
					*OutResult = RtlAddOffsetToPointer(InBaseAddress, X);

				return STATUS_SUCCESS;
			}
		}
	}

	return STATUS_NOT_FOUND;
}

/// <summary>
/// Searches for a successive pattern of a specific padding byte in the given memory range, starting from the end.
/// </summary>
/// <param name="InBaseAddress">The base address.</param>
/// <param name="InSize">The size of the region in bytes.</param>
/// <param name="InPaddingByte">The padding byte.</param>
/// <param name="InPaddingLength">The padding length.</param>
/// <param name="OutResult">The result.</param>
NTSTATUS CkTryFindPaddingFromEnd(CONST PVOID InBaseAddress, SIZE_T InSize, UINT8 InPaddingByte, SIZE_T InPaddingLength, OPTIONAL OUT PVOID* OutResult)
{
	// 
	// Verify the passed parameters.
	// 

	if (InBaseAddress == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InSize == 0)
		return STATUS_INVALID_PARAMETER_2;

	if (InPaddingLength == 0)
		return STATUS_INVALID_PARAMETER_4;

	// 
	// The padding cannot be bigger than the region of memory we are about to scan.
	// 

	if (InSize < InPaddingLength)
		return STATUS_ARRAY_BOUNDS_EXCEEDED;

	// 
	// Loop through the memory region.
	// 
	
	for (SIZE_T X = InSize - InPaddingLength; X > 0 ; X--)
	{
		// 
		// Search for the padding.
		// 

		for (SIZE_T Y = 0; Y < InPaddingLength; Y++)
		{
			// 
			// Check if the bytes matches.
			// 

			const auto CurrentByte = *(UINT8*) RtlAddOffsetToPointer(InBaseAddress, X + Y);

			if (CurrentByte != InPaddingByte)
				break;

			// 
			// If this was the last byte for the scan...
			// 
			
			if (Y == InPaddingLength - 1)
			{
				if (OutResult != nullptr)
					*OutResult = RtlAddOffsetToPointer(InBaseAddress, X);

				return STATUS_SUCCESS;
			}
		}
	}

	return STATUS_NOT_FOUND;
}

/// <summary>
/// Searches for a certain pattern inside the given module's executable sections.
/// </summary>
/// <param name="InBaseAddress">The base address.</param>
/// <param name="InSignature">The signature.</param>
/// <param name="OutResult">The signature scan result.</param>
NTSTATUS CkTryFindPatternInModuleExecutableSections(CONST PVOID InBaseAddress, CONST CHAR* InSignature, OPTIONAL OUT PVOID* OutResult)
{
	// 
	// Verify the passed parameters.
	// 

	if (InBaseAddress == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InSignature == nullptr)
		return STATUS_INVALID_PARAMETER_2;

	// 
	// Setup the scan context.
	// 

	struct SCAN_CONTEXT
	{
		PVOID BaseAddress;
		CONST CHAR* Signature;
		PVOID Result;
	};

	SCAN_CONTEXT ScanContext;
	ScanContext.BaseAddress = InBaseAddress;
	ScanContext.Signature = InSignature;
	ScanContext.Result = NULL;

	// 
	// Enumerate the sections of the module.
	// 

	RtlEnumerateModuleSections<SCAN_CONTEXT*>(InBaseAddress, &ScanContext, [] (ULONG InIndex, IMAGE_SECTION_HEADER* InSectionHeader, SCAN_CONTEXT* InContext) -> bool
	{
		// 
		// Parse the section's characteristics.
		// 

		auto Executable	 = (InSectionHeader->Characteristics & IMAGE_SCN_MEM_EXECUTE) != 0;
		auto Readable	 = (InSectionHeader->Characteristics & IMAGE_SCN_MEM_READ) != 0;
		auto Writable	 = (InSectionHeader->Characteristics & IMAGE_SCN_MEM_WRITE) != 0;
		auto Discardable = (InSectionHeader->Characteristics & IMAGE_SCN_MEM_DISCARDABLE) != 0;
		auto ContainCode = (InSectionHeader->Characteristics & IMAGE_SCN_CNT_CODE) != 0;

		// 
		// Discardable sections are not mapped.
		// 

		if (Discardable)
			return FALSE;

		// 
		// Check if the section is executable or contains code.
		// 

		if (!Executable && !ContainCode)
			return FALSE;

		// 
		// Verify the validity of the address.
		// 

		auto* SectionData = RtlAddOffsetToPointer(InContext->BaseAddress, InSectionHeader->VirtualAddress);

		if (!MmIsAddressValid(SectionData))
			return FALSE;

		// 
		// Scan this section.
		// 

		return NT_SUCCESS(CkTryFindPattern(SectionData, InSectionHeader->Misc.VirtualSize, InContext->Signature, &InContext->Result));
	});

	// 
	// Return the resulting address.
	// 

	if (OutResult != nullptr)
		*OutResult = ScanContext.Result;

	return ScanContext.Result != nullptr ? STATUS_SUCCESS : STATUS_NOT_FOUND;
}
