#pragma once

// 
//  - PZE : PML5E (SLAT)
//  - PXE : PML4E
//  - PPE : PDPTE
//  - PDE : PDE
//  - PTE : PTE
// 

#define LARGE_PAGE_SIZE				(512 * 0x1000)
#define LARGE_PAGE_ROUND_DOWN(x)	((((ULONG_PTR) (x)) & (~(LARGE_PAGE_SIZE-1))))
#define LARGE_PAGE_ROUND_UP(x)		((((ULONG_PTR) (x)) + LARGE_PAGE_SIZE-1) & (~(LARGE_PAGE_SIZE-1)))

#define PFN_TO_PAGE(PFN)			(((LONG64) (PFN)) << 12llu)
#define PAGE_TO_PFN(PAGE)			(((LONG64) (PAGE)) >> 12llu)

#define VIRTUAL_ADDRESS_BITS		48
#define VIRTUAL_ADDRESS_MASK		((((ULONG_PTR) 1) << VIRTUAL_ADDRESS_BITS) - 1)
#define VA_SHIFT					0b0001'0000

#define PZE_PER_PAGE 512llu
#define PXE_PER_PAGE 512llu
#define PPE_PER_PAGE 512llu
#define PDE_PER_PAGE 512llu
#define PTE_PER_PAGE 512llu

#define PZI_MASK (PZE_PER_PAGE - 1)
#define PXI_MASK (PXE_PER_PAGE - 1)
#define PPI_MASK (PPE_PER_PAGE - 1)
#define PDI_MASK (PDE_PER_PAGE - 1)
#define PTI_MASK (PTE_PER_PAGE - 1)

#define PZI_SHIFT 48llu
#define PXI_SHIFT 39llu
#define PPI_SHIFT 30llu
#define PDI_SHIFT 21llu
#define PTI_SHIFT 12llu
#define PTE_SHIFT 3llu

#define MiGetVirtualAddressOfPml5e(Pml5e) (MiGetVirtualAddressOfPze(Pml5e))
#define MiGetVirtualAddressOfPml4e(Pml4e) (MiGetVirtualAddressOfPxe(Pml4e))
#define MiGetVirtualAddressOfPdpte(Ppdte) (MiGetVirtualAddressOfPpe(Ppdte))

#define MiGetVirtualAddressOfPze(Pze) ((PVOID) (((INT64) (Pze) << (PZI_SHIFT + VA_SHIFT - PTE_SHIFT)) >> VA_SHIFT))
#define MiGetVirtualAddressOfPxe(Pxe) ((PVOID) (((INT64) (Pxe) << (PXI_SHIFT + VA_SHIFT - PTE_SHIFT)) >> VA_SHIFT))
#define MiGetVirtualAddressOfPpe(Ppe) ((PVOID) (((INT64) (Ppe) << (PPI_SHIFT + VA_SHIFT - PTE_SHIFT)) >> VA_SHIFT))
#define MiGetVirtualAddressOfPde(Pde) ((PVOID) (((INT64) (Pde) << (PDI_SHIFT + VA_SHIFT - PTE_SHIFT)) >> VA_SHIFT))
#define MiGetVirtualAddressOfPte(Pte) ((PVOID) (((INT64) (Pte) << (PTI_SHIFT + VA_SHIFT - PTE_SHIFT)) >> VA_SHIFT))

#pragma pack(push, 1)

typedef union _VIRTUAL_ADDRESS
{
	void* Pointer;
	volatile void* VolatilePointer;
	UINT64 Long;
	volatile UINT64 VolatileLong;

	struct
	{
		UINT64 Offset : 12;
		UINT64 Pte : 9;
		UINT64 Pde : 9;
		UINT64 Ppe : 9;
		UINT64 Pxe : 9;
		UINT64 Reserved : 16;
	};
	
} VIRTUAL_ADDRESS;

typedef union _CR3
{
	UINT64 value;
	struct
	{
		UINT64 ignored_1 : 3;
		UINT64 write_through : 1;
		UINT64 cache_disable : 1;
		UINT64 ignored_2 : 7;
		UINT64 pml4_p : 40;
		UINT64 reserved : 12;
	};
} CR3, *PCR3;

#pragma pack(pop)

struct ADDRESS_TRANSLATION_INFO
{
	MMPXE* Pxe;
	MMPPE* Ppe;
	MMPDE* Pde;
	MMPTE* Pte;
};

/// <summary>
/// Retrieves the page table entries translating the given virtual address.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InVirtualAddress">The virtual address.</param>
/// <param name="OutTranslationInfo">The returned virtual address translation information.</param>
NTSTATUS CkVirtualAddressTranslation(CONST PEPROCESS InProcess, CONST PVOID InVirtualAddress, OUT ADDRESS_TRANSLATION_INFO* OutTranslationInfo);

/// <summary>
/// Enumerates every PXE in the given process page tables.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS CkEnumeratePxeOfProcess(CONST PEPROCESS InProcess, void(*InCallback)(ULONG, MMPXE*));

/// <summary>
/// Enumerates every PXE in the given process page tables.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InContext">The context.</param>
/// <param name="InCallback">The callback.</param>
template <typename TContext = PVOID>
NTSTATUS CkEnumeratePxeOfProcess(CONST PEPROCESS InProcess, TContext InContext, void(*InCallback)(ULONG, MMPXE*, TContext))
{
	// 
	// Verify the passed parameters.
	// 

	if (InProcess == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InCallback == nullptr)
		return STATUS_INVALID_PARAMETER_3;

	// 
	// Attach to the process.
	// 

	KAPC_STATE ApcState = { };
	KeStackAttachProcess(InProcess, &ApcState);

	// 
	// Retrieve the process page tables directory base.
	// 

	ULONGLONG ProcessDirectoryBase = *(ULONGLONG*) RtlAddOffsetToPointer(InProcess, 0x28);
	CR3 ProcessCr3 = { .value = ProcessDirectoryBase };

	// 
	// Enumerate the PXE entries.
	// 

	for (SIZE_T PxeIdx = 0; PxeIdx < PXE_PER_PAGE; PxeIdx++)
	{
		// 
		// Retrieve the PXE for this page.
		// 

		auto* PXE = (MMPXE*) MmGetVirtualForPhysical({ .QuadPart = (LONGLONG) (PFN_TO_PAGE(ProcessCr3.pml4_p) + (PxeIdx * sizeof(MMPXE))) });

		if (!MmIsAddressValid(PXE))
			continue;

		// 
		// Ensure the entry is valid.
		// 

		if (!PXE->u.Hard.Valid)
			continue;

		// 
		// Execute the provided callback.
		// 

		InCallback(PxeIdx, PXE, InContext);
	}

	// 
	// Detach from the process.
	// 

	KeUnstackDetachProcess(&ApcState);
	return STATUS_SUCCESS;
}

/// <summary>
/// Enumerates every PPE in the given PXE.
/// </summary>
/// <param name="InPxe">The PXE to enumerate the entries from.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS CkEnumeratePpeOfPxe(CONST MMPXE* InPxe, void(*InCallback)(ULONG, MMPPE*));

/// <summary>
/// Enumerates every PPE in the given PXE.
/// </summary>
/// <param name="InPxe">The PXE to enumerate the entries from.</param>
/// <param name="InContext">The context.</param>
/// <param name="InCallback">The callback.</param>
template <typename TContext = PVOID>
NTSTATUS CkEnumeratePpeOfPxe(CONST MMPXE* InPxe, TContext InContext, void(*InCallback)(ULONG, MMPPE*, TContext))
{
	// 
	// Verify the passed parameters.
	// 

	if (InPxe == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InCallback == nullptr)
		return STATUS_INVALID_PARAMETER_3;

	// 
	// Verify if the PXE entry is valid.
	// 

	if (!InPxe->u.Hard.Valid)
		return STATUS_INVALID_ADDRESS;

	if (InPxe->u.Hard.LargePage)
		return STATUS_INVALID_WEIGHT;

	// 
	// Enumerate the PPE entries of this PXE.
	// 

	for (SIZE_T PpeIdx = 0; PpeIdx < PPE_PER_PAGE; PpeIdx++)
	{
		// 
		// Retrieve the PPE for this page.
		// 

		auto* PPE = (MMPPE*) MmGetVirtualForPhysical({ .QuadPart = (LONGLONG) (PFN_TO_PAGE(InPxe->u.Hard.PageFrameNumber) + (PpeIdx * sizeof(MMPPE))) });

		if (!MmIsAddressValid(PPE))
			continue;

		// 
		// Ensure the entry is valid.
		// 

		if (!PPE->u.Hard.Valid)
			continue;

		// 
		// Execute the provided callback.
		// 

		InCallback(PpeIdx, PPE, InContext);
	}

	return STATUS_SUCCESS;
}

/// <summary>
/// Enumerates every PDE in the given PPE.
/// </summary>
/// <param name="InPpe">The PPE to enumerate the entries from.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS CkEnumeratePdeOfPpe(CONST MMPPE* InPpe, void(*InCallback)(ULONG, MMPDE*));

/// <summary>
/// Enumerates every PDE in the given PPE.
/// </summary>
/// <param name="InPpe">The PPE to enumerate the entries from.</param>
/// <param name="InContext">The context.</param>
/// <param name="InCallback">The callback.</param>
template <typename TContext = PVOID>
NTSTATUS CkEnumeratePdeOfPpe(CONST MMPPE* InPpe, TContext InContext, void(*InCallback)(ULONG, MMPDE*, TContext))
{
	// 
	// Verify the passed parameters.
	// 

	if (InPpe == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InCallback == nullptr)
		return STATUS_INVALID_PARAMETER_3;

	// 
	// Verify if the PPE entry is valid.
	// 

	if (!InPpe->u.Hard.Valid)
		return STATUS_INVALID_ADDRESS;

	if (InPpe->u.Hard.LargePage)
		return STATUS_INVALID_WEIGHT;

	// 
	// Enumerate the PDE entries of this PPE.
	// 

	for (SIZE_T PdeIdx = 0; PdeIdx < PDE_PER_PAGE; PdeIdx++)
	{
		// 
		// Retrieve the PDE for this page.
		// 

		auto* PDE = (MMPDE*) MmGetVirtualForPhysical({ .QuadPart = (LONGLONG) (PFN_TO_PAGE(InPpe->u.Hard.PageFrameNumber) + (PdeIdx * sizeof(MMPDE))) });

		if (!MmIsAddressValid(PDE))
			continue;

		// 
		// Ensure the entry is valid.
		// 

		if (!PDE->u.Hard.Valid)
			continue;

		// 
		// Execute the provided callback.
		// 

		InCallback(PdeIdx, PDE, InContext);
	}

	return STATUS_SUCCESS;
}

/// <summary>
/// Enumerates every PTE in the given PDE.
/// </summary>
/// <param name="InPde">The PDE to enumerate the entries from.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS CkEnumeratePteOfPde(CONST MMPDE* InPde, void(*InCallback)(ULONG, MMPTE*));

/// <summary>
/// Enumerates every PTE in the given PDE.
/// </summary>
/// <param name="InPde">The PDE to enumerate the entries from.</param>
/// <param name="InContext">The context.</param>
/// <param name="InCallback">The callback.</param>
template <typename TContext = PVOID>
NTSTATUS CkEnumeratePteOfPde(CONST MMPDE* InPde, TContext InContext, void(*InCallback)(ULONG, MMPTE*, TContext))
{
	// 
	// Verify the passed parameters.
	// 

	if (InPde == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InCallback == nullptr)
		return STATUS_INVALID_PARAMETER_3;

	// 
	// Verify if the PDE entry is valid.
	// 

	if (!InPde->u.Hard.Valid)
		return STATUS_INVALID_ADDRESS;

	if (InPde->u.Hard.LargePage)
		return STATUS_INVALID_WEIGHT;

	// 
	// Enumerate the PTE entries of this PDE.
	// 

	for (SIZE_T PteIdx = 0; PteIdx < PTE_PER_PAGE; PteIdx++)
	{
		// 
		// Retrieve the PTE for this page.
		// 

		auto* PTE = (MMPDE*) MmGetVirtualForPhysical({ .QuadPart = (LONGLONG) (PFN_TO_PAGE(InPde->u.Hard.PageFrameNumber) + (PteIdx * sizeof(MMPTE))) });

		if (!MmIsAddressValid(PTE))
			continue;

		// 
		// Ensure the entry is valid.
		// 

		if (!PTE->u.Hard.Valid)
			continue;

		// 
		// Execute the provided callback.
		// 

		InCallback(PteIdx, PTE, InContext);
	}

	return STATUS_SUCCESS;
}
