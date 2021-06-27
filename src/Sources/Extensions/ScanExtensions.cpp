#include "../../Headers/EasyNT.h"

/// <summary>
/// Searches for a certain pattern inside the given memory range.
/// </summary>
/// <param name="InBaseAddress">The base address.</param>
/// <param name="InSize">The size of the region in bytes.</param>
/// <param name="InSignature">The signature.</param>
/// <param name="OutResult">The signature scan result.</param>
BOOLEAN CkTryFindPattern(CONST PVOID InBaseAddress, SIZE_T InSize, CONST CHAR* InSignature, OPTIONAL OUT PVOID* OutResult)
{
	// 
	// Verify the passed parameters.
	// 

	if (InBaseAddress == nullptr)
		return false;

	if (InSize == 0)
		return false;

	if (InSignature == nullptr)
		return false;

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
	
	while (TRUE)
	{
		switch (InSignature[SignatureStep])
		{
			case NULL:
			{
				goto LoopBreak;
			}

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
				Entry->IsWildcard = true;

				if (InSignature[SignatureStep + 1] == '?')
					SignatureStep += sizeof(char) * 2;
				else
					SignatureStep += sizeof(char) * 1;

				break;
			}
			
			default:
			{
				auto* Entry = &SignatureBytes[SignatureLength++];
				Entry->Value = Entry->Value = RtlHexadecimalStringToByte(&InSignature[SignatureStep]);
				Entry->IsWildcard = false;

				SignatureStep += sizeof(char) * 2;
				break;
			}
		}
	}

	// 
	// The signature can't be bigger than the region of memory we are about to scan.
	// 

LoopBreak:
	
	if (InSize < SignatureLength)
		return false;

	// 
	// Begin to scan the given memory range.
	// 

	for (SIZE_T I = 0; I < InSize - SignatureLength; I++)
	{
		BOOLEAN HasBeenFound = true;

		for (SIZE_T X = 0; X < SignatureLength; X++)
		{
			auto* const Entry = &SignatureBytes[X];

			if (Entry->IsWildcard)
				continue;

			auto const Value = *(UINT8*) RtlAddOffsetToPointer(InBaseAddress, I + X);

			if (Value == Entry->Value)
				continue;

			HasBeenFound = false;
			break;
		}

		// 
		// We've found the signature, let's break both loops.
		// 

		if (HasBeenFound)
		{
			if (OutResult != nullptr)
				*OutResult = RtlAddOffsetToPointer(InBaseAddress, I);

			return TRUE;
		}
	}

	return FALSE;
}
