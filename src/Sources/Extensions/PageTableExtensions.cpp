#include "../../Headers/EasyNT.h"

/// <summary>
/// Retrieves the page table entries translating the given virtual address.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InVirtualAddress">The virtual address.</param>
/// <param name="OutTranslationInfo">The returned virtual address translation information.</param>
NTSTATUS CkVirtualAddressTranslation(CONST PEPROCESS InProcess, CONST PVOID InVirtualAddress, OUT ADDRESS_TRANSLATION_INFO* OutTranslationInfo)
{
	// 
	// Verify the passed parameters.
	// 

	if (InProcess == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InVirtualAddress == nullptr)
		return STATUS_INVALID_PARAMETER_2;

	if (OutTranslationInfo == nullptr)
		return STATUS_INVALID_PARAMETER_3;

	// 
	// Attach to the process.
	// 

	KAPC_STATE ApcState = { };
	KeStackAttachProcess(InProcess, &ApcState);

	// 
	// Retrieve the process page tables directory base.
	// 

	ADDRESS_TRANSLATION_INFO AddressTranslationInfo = { };
	ULONGLONG ProcessDirectoryBase = *(ULONGLONG*) RtlAddOffsetToPointer(InProcess, 0x28);
	CONST VIRTUAL_ADDRESS TranslationIndexes = { .Pointer = InVirtualAddress };
	CONST CR3 ProcessCr3 = { .value = ProcessDirectoryBase };

	// 
	// Calculate the physical address to the PXE.
	// 

	AddressTranslationInfo.Pxe = (MMPXE*) MmGetVirtualForPhysical({ .QuadPart = (LONGLONG) (PFN_TO_PAGE(ProcessCr3.pml4_p) + (TranslationIndexes.Pxe * sizeof(MMPXE))) });

	if (MmIsAddressValid(AddressTranslationInfo.Pxe) && AddressTranslationInfo.Pxe->u.Hard.Valid && !AddressTranslationInfo.Pxe->u.Hard.LargePage)
	{
		// 
		// Calculate the physical address to the PPE.
		// 

		AddressTranslationInfo.Ppe = (MMPPE*) MmGetVirtualForPhysical({ .QuadPart = (LONGLONG) (PFN_TO_PAGE(AddressTranslationInfo.Pxe->u.Hard.PageFrameNumber) + (TranslationIndexes.Ppe * sizeof(MMPPE))) });

		if (MmIsAddressValid(AddressTranslationInfo.Ppe) && AddressTranslationInfo.Ppe->u.Hard.Valid && !AddressTranslationInfo.Ppe->u.Hard.LargePage)
		{
			// 
			// Calculate the physical address to the PDE.
			// 

			AddressTranslationInfo.Pde = (MMPDE*) MmGetVirtualForPhysical({ .QuadPart = (LONGLONG) (PFN_TO_PAGE(AddressTranslationInfo.Ppe->u.Hard.PageFrameNumber) + (TranslationIndexes.Pde * sizeof(MMPDE))) });
			
			if (MmIsAddressValid(AddressTranslationInfo.Pde) && AddressTranslationInfo.Pde->u.Hard.Valid && !AddressTranslationInfo.Pde->u.Hard.LargePage)
			{
				// 
				// Calculate the physical address to the PTE.
				// 

				AddressTranslationInfo.Pte = (MMPTE*) MmGetVirtualForPhysical({ .QuadPart = (LONGLONG) (PFN_TO_PAGE(AddressTranslationInfo.Pde->u.Hard.PageFrameNumber) + (TranslationIndexes.Pte * sizeof(MMPTE))) });
			}
		}
	}

	// 
	// Detach from the process.
	// 

	KeUnstackDetachProcess(&ApcState);

	// 
	// Return the result.
	// 
	
	*OutTranslationInfo = AddressTranslationInfo;
	return STATUS_SUCCESS;
}

/// <summary>
/// Enumerates every PXE in the given process page tables.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS CkEnumeratePxeOfProcess(CONST PEPROCESS InProcess, void(*InCallback)(ULONG, MMPXE*))
{
	return CkEnumeratePxeOfProcess<PVOID>(InProcess, InCallback, [] (ULONG InIdx, MMPXE* InPxe, PVOID InContext)
	{
		((void(*)(ULONG, MMPXE*)) InContext) (InIdx, InPxe);
	});
}

/// <summary>
/// Enumerates every PPE in the given PXE.
/// </summary>
/// <param name="InPxe">The PXE to enumerate the entries from.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS CkEnumeratePpeOfPxe(CONST MMPXE* InPxe, void(*InCallback)(ULONG, MMPPE*))
{
	return CkEnumeratePpeOfPxe<PVOID>(InPxe, InCallback, [] (ULONG InIdx, MMPPE* InPpe, PVOID InContext)
	{
		((void(*)(ULONG, MMPPE*)) InContext) (InIdx, InPpe);
	});
}

/// <summary>
/// Enumerates every PDE in the given PPE.
/// </summary>
/// <param name="InPpe">The PPE to enumerate the entries from.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS CkEnumeratePdeOfPpe(CONST MMPPE* InPpe, void(*InCallback)(ULONG, MMPDE*))
{
	return CkEnumeratePdeOfPpe<PVOID>(InPpe, InCallback, [] (ULONG InIdx, MMPDE* InPde, PVOID InContext)
	{
		((void(*)(ULONG, MMPDE*)) InContext) (InIdx, InPde);
	});
}

/// <summary>
/// Enumerates every PTE in the given PDE.
/// </summary>
/// <param name="InPde">The PDE to enumerate the entries from.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS CkEnumeratePteOfPde(CONST MMPDE* InPde, void(*InCallback)(ULONG, MMPTE*))
{
	return CkEnumeratePteOfPde<PVOID>(InPde, InCallback, [] (ULONG InIdx, MMPTE* InPte, PVOID InContext)
	{
		((void(*)(ULONG, MMPTE*)) InContext) (InIdx, InPte);
	});
}
