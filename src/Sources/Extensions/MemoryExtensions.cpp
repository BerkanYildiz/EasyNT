#include "../../Headers/EasyNT.h"

/// <summary>
/// Allocate virtual memory in a given process.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InNumberOfBytes">The number of bytes to allocate.</param>
/// <param name="InAllocationType">The type of the allocation.</param>
/// <param name="InProtection">The page protection.</param>
/// <param name="InOutAllocationAddress">In: The address to allocate memory at / Out: The resulting allocation address.</param>
NTSTATUS CkAllocateVirtualMemory(CONST PEPROCESS InProcess, SIZE_T InNumberOfBytes, ULONG InAllocationType, ULONG InProtection, IN OUT PVOID* InOutAllocationAddress)
{
	NTSTATUS Status = { };

	// 
	// Verify the passed parameters.
	// 

	if (InProcess == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InNumberOfBytes == 0)
		return STATUS_INVALID_PARAMETER_2;
	
	if (InAllocationType == 0)
		return STATUS_INVALID_PARAMETER_3;
	
	if (InProtection == 0)
		return STATUS_INVALID_PARAMETER_4;
	
	if (InOutAllocationAddress == nullptr)
		return STATUS_INVALID_PARAMETER_5;

	// 
	// Open a handle to the process.
	// 

	auto* Handle = ZwCurrentProcess();

	if (InProcess != PsGetCurrentProcess())
		if (NT_ERROR(Status = ObOpenObjectByPointer(InProcess, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, GENERIC_ALL, *PsProcessType, KernelMode, &Handle)))
			return Status;

	// 
	// Allocate virtual memory.
	// 

	SIZE_T NumberOfBytes = InNumberOfBytes;
	PVOID BaseAddress = *InOutAllocationAddress;
	Status = ZwAllocateVirtualMemory(Handle, &BaseAddress, 0, &NumberOfBytes, InAllocationType, InProtection);

	// 
	// If we opened a handle for the process, close it.
	// 

	if (Handle != ZwCurrentProcess())
		ZwClose(Handle);

	// 
	// If the allocation was successful...
	// 

	if (NT_SUCCESS(Status))
	{
		// 
		// Zero the page(s).
		// 

		CkZeroVirtualMemory(InProcess, BaseAddress, NumberOfBytes);

		// 
		// Return the allocation address.
		// 

		*InOutAllocationAddress = BaseAddress;
	}

	return Status;
}

/// <summary>
/// Releases virtual memory previously allocated in the given process.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InBaseAddress">The virtual address to free.</param>
/// <param name="InNumberOfBytes">The number of bytes to free.</param>
/// <param name="InFreeType">The type of the free.</param>
NTSTATUS CkFreeVirtualMemory(CONST PEPROCESS InProcess, CONST PVOID InBaseAddress, SIZE_T InNumberOfBytes, ULONG InFreeType)
{
	NTSTATUS Status = { };

	// 
	// Verify the passed parameters.
	// 

	if (InProcess == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InBaseAddress == nullptr)
		return STATUS_INVALID_PARAMETER_2;

	if (InNumberOfBytes == 0)
		return STATUS_INVALID_PARAMETER_3;

	if (InFreeType == 0)
		return STATUS_INVALID_PARAMETER_4;

	// 
	// Open a handle to the process.
	// 

	auto* Handle = ZwCurrentProcess();

	if (InProcess != PsGetCurrentProcess())
		if (NT_ERROR(Status = ObOpenObjectByPointer(InProcess, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, GENERIC_ALL, *PsProcessType, KernelMode, &Handle)))
			return Status;

	// 
	// Zero the virtual memory.
	// 

	if (InNumberOfBytes != 0)
		CkZeroVirtualMemory(InProcess, InBaseAddress, InNumberOfBytes);
	
	// 
	// Release the virtual memory.
	// 

	auto* VirtualAddress = InBaseAddress;
	auto  VirtualSize = InFreeType == MEM_RELEASE ? 0 : InNumberOfBytes;
	Status = ZwFreeVirtualMemory(Handle, &VirtualAddress, &VirtualSize, InFreeType);

	// 
	// If we opened a handle for the process, close it.
	// 

	if (Handle != ZwCurrentProcess())
		ZwClose(Handle);

	return Status;
}

/// <summary>
/// Zero the virtual memory previously allocated in the given process.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InBaseAddress">The virtual address to zero.</param>
/// <param name="InNumberOfBytes">The number of bytes to zero.</param>
NTSTATUS CkZeroVirtualMemory(CONST PEPROCESS InProcess, CONST PVOID InBaseAddress, SIZE_T InNumberOfBytes)
{
	NTSTATUS Status = { };

	// 
	// Verify the passed parameters.
	// 

	if (InProcess == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InBaseAddress == nullptr)
		return STATUS_INVALID_PARAMETER_2;

	if (InNumberOfBytes == 0)
		return STATUS_INVALID_PARAMETER_3;

	// 
	// Attach to the process.
	// 

	KAPC_STATE ApcState = { };
	KeStackAttachProcess(InProcess, &ApcState);

	// 
	// Zero the virtual memory.
	// 

	RtlSecureZeroMemory(InBaseAddress, InNumberOfBytes);
	
	// 
	// Detach from the process.
	// 

	KeUnstackDetachProcess(&ApcState);
	return STATUS_SUCCESS;
}

#define _WIN11_WORKAROUND
#ifdef _WIN11_WORKAROUND
//
// The maximum amount to try to Probe and Lock is 14 pages, this
// way it always fits in a 16 page allocation.
//

#define MAX_LOCK_SIZE ((ULONG)(14 * PAGE_SIZE))

//
// The maximum to move in a single block is 64k bytes.
//

#define MAX_MOVE_SIZE (LONG)0x10000

//
// Define the pool move threshold value.
//

#define POOL_MOVE_THRESHOLD 511

#define COPY_STACK_SIZE 64

NTSTATUS
MiDoPoolCopy(
	IN PEPROCESS FromProcess,
	IN CONST VOID* FromAddress,
	IN PEPROCESS ToProcess,
	OUT PVOID ToAddress,
	IN SIZE_T BufferSize,
	IN KPROCESSOR_MODE PreviousMode,
	OUT PSIZE_T NumberOfBytesRead
)

/*++
Routine Description:
	This function copies the specified address range from the specified
	process into the specified address range of the current process.
Arguments:
	 ProcessHandle - Supplies an open handle to a process object.
	 BaseAddress - Supplies the base address in the specified process
				   to be read.
	 Buffer - Supplies the address of a buffer which receives the
			  contents from the specified process address space.
	 BufferSize - Supplies the requested number of bytes to read from
				  the specified process.
	 PreviousMode - Supplies the previous processor mode.
	 NumberOfBytesRead - Receives the actual number of bytes
						 transferred into the specified buffer.
Return Value:
	NTSTATUS.
--*/

{
	KAPC_STATE ApcState;
	SIZE_T AmountToMove;
	LOGICAL ExceptionAddressConfirmed;
	PEPROCESS CurrentProcess;
	CONST VOID* InVa;
	SIZE_T LeftToMove;
	SIZE_T MaximumMoved;
	PVOID OutVa;
	PVOID PoolArea;
	LONGLONG StackArray[COPY_STACK_SIZE];
	ULONG FreePool;
	SIZE_T PageOffset;

	PAGED_CODE();

	ASSERT(BufferSize != 0);

	//
	// Get the address of the current process object and initialize copy
	// parameters.
	//

	CurrentProcess = PsGetCurrentProcess();

	InVa = FromAddress;
	OutVa = ToAddress;

	//
	// Allocate non-paged memory to copy in and out of.
	//

	MaximumMoved = MAX_MOVE_SIZE;
	if (BufferSize <= MAX_MOVE_SIZE)
	{
		MaximumMoved = BufferSize;
	}

	FreePool = FALSE;
	if (BufferSize <= sizeof(StackArray))
	{
		PoolArea = (PVOID)&StackArray[0];
	}
	else
	{
		do
		{
			PoolArea = ExAllocatePoolWithTag(NonPagedPool, MaximumMoved, 'wRmM');
			if (PoolArea != NULL)
			{
				FreePool = TRUE;
				break;
			}

			MaximumMoved = MaximumMoved >> 1;
			if (MaximumMoved <= sizeof(StackArray))
			{
				PoolArea = (PVOID)&StackArray[0];
				break;
			}
		} while (TRUE);
	}

	//
	// Initializing BadVa & ExceptionAddressConfirmed is not needed for
	// correctness but without it the compiler cannot compile this code
	// W4 to check for use of uninitialized variables.
	//

	ExceptionAddressConfirmed = FALSE;

	//
	// Copy the data into pool, then copy back into the ToProcess.
	//

	LeftToMove = BufferSize;
	AmountToMove = MaximumMoved;

	while (LeftToMove > 0)
	{
		if (LeftToMove < AmountToMove)
		{
			//
			// Set to move the remaining bytes.
			//

			AmountToMove = LeftToMove;
		}

		KeStackAttachProcess(FromProcess, &ApcState);

		//
		// Probe to make sure that the specified buffer is accessible in
		// the target process.
		//

		if ((InVa == FromAddress) && (PreviousMode != KernelMode))
		{
			// ProbeForRead(FromAddress, BufferSize, sizeof(CHAR));
		}

		for (PageOffset = 0; PageOffset < PAGE_ROUND_UP(AmountToMove); PageOffset += PAGE_SIZE)
		{
			if (!MmIsAddressValid(RtlAddOffsetToPointer(OutVa, PageOffset)))
			{
				ExceptionAddressConfirmed = TRUE;
				goto BadAddress;
			}
		}

		RtlCopyMemory(PoolArea, InVa, AmountToMove);

		KeUnstackDetachProcess(&ApcState);

		KeStackAttachProcess(ToProcess, &ApcState);

		//
		// Now operating in the context of the ToProcess.
		//

		if ((InVa == FromAddress) && (PreviousMode != KernelMode))
		{
			// ProbeForWrite(ToAddress, BufferSize, sizeof(CHAR));
		}

		for (PageOffset = 0; PageOffset < PAGE_ROUND_UP(AmountToMove); PageOffset += PAGE_SIZE)
		{
			if (!MmIsAddressValid(RtlAddOffsetToPointer(OutVa, PageOffset)))
			{
				ExceptionAddressConfirmed = TRUE;
				goto BadAddress;
			}
		}

		RtlCopyMemory(OutVa, PoolArea, AmountToMove);

	BadAddress:
		if (ExceptionAddressConfirmed)
		{
			//
			// If an exception occurs during the move operation or probe,
			// return the exception code as the status value.
			//

			KeUnstackDetachProcess(&ApcState);

			if (FreePool)
			{
				ExFreePool(PoolArea);
			}

			//
			// If the failure occurred during the move operation, determine
			// which move failed, and calculate the number of bytes
			// actually moved.
			//

			*NumberOfBytesRead = BufferSize - LeftToMove;

			return STATUS_PARTIAL_COPY;
		}

		KeUnstackDetachProcess(&ApcState);

		LeftToMove -= AmountToMove;
		InVa = (PVOID)((ULONG_PTR)InVa + AmountToMove);
		OutVa = (PVOID)((ULONG_PTR)OutVa + AmountToMove);
	}

	if (FreePool)
	{
		ExFreePool(PoolArea);
	}

	//
	// Set number of bytes moved.
	//

	*NumberOfBytesRead = BufferSize;
	return STATUS_SUCCESS;
}

NTSTATUS
MmCopyVirtualMemoryImpl(
	IN PEPROCESS FromProcess,
	IN CONST VOID* FromAddress,
	IN PEPROCESS ToProcess,
	OUT PVOID ToAddress,
	IN SIZE_T BufferSize,
	IN KPROCESSOR_MODE PreviousMode,
	OUT PSIZE_T NumberOfBytesCopied
)
{
	NTSTATUS Status;
	PEPROCESS ProcessToLock;

	if (BufferSize == 0)
	{
		ASSERT(FALSE);         // No one should call with a zero size.
		return STATUS_SUCCESS;
	}

	ProcessToLock = FromProcess;
	if (FromProcess == PsGetCurrentProcess())
	{
		ProcessToLock = ToProcess;
	}

	//
	// Make sure the process still has an address space.
	//

	if (NT_ERROR(Status = PsAcquireProcessExitSynchronization(ProcessToLock)))
	{
		return Status;
	}

	//
	// If the buffer size is greater than the pool move threshold,
	// then attempt to write the memory via direct mapping.
	//

	/*if (BufferSize > POOL_MOVE_THRESHOLD)
	{
		Status = MiDoMappedCopy(FromProcess,
								FromAddress,
								ToProcess,
								ToAddress,
								BufferSize,
								PreviousMode,
								NumberOfBytesCopied);

		//
		// If the completion status is not a working quota problem,
		// then finish the service. Otherwise, attempt to write the
		// memory through nonpaged pool.
		//

		if (Status != STATUS_WORKING_SET_QUOTA)
		{
			goto CompleteService;
		}

		*NumberOfBytesCopied = 0;
	}*/

	//
	// There was not enough working set quota to write the memory via
	// direct mapping or the size of the write was below the pool move
	// threshold. Attempt to write the specified memory through nonpaged
	// pool.
	//

	Status = MiDoPoolCopy(FromProcess,
		FromAddress,
		ToProcess,
		ToAddress,
		BufferSize,
		PreviousMode,
		NumberOfBytesCopied);

	//
	// Dereference the target process.
	//

CompleteService:

	//
	// Indicate that the vm operation is complete.
	//

	PsReleaseProcessExitSynchronization(ProcessToLock);

	return Status;
}
#endif

/// <summary>
/// Safely copy virtual memory from the source address to the destination address.
/// </summary>
/// <param name="InSourceProcess">The source process.</param>
/// <param name="InSourceAddress">The source virtual address.</param>
/// <param name="InDestinationProcess">The destination process.</param>
/// <param name="InDestinationAddress">The destination virtual address.</param>
/// <param name="InNumberOfBytes">The number of bytes to copy.</param>
/// <param name="OutNumberOfBytesCopied">The number of bytes copied.</param>
NTSTATUS CkCopyVirtualMemory(CONST PEPROCESS InSourceProcess, CONST PVOID InSourceAddress, CONST PEPROCESS InDestinationProcess, PVOID InDestinationAddress, SIZE_T InNumberOfBytes, OPTIONAL OUT SIZE_T* OutNumberOfBytesCopied)
{
	NTSTATUS Status = { };

	// 
	// Verify the passed parameters.
	// 

	if (InSourceProcess == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InSourceAddress == nullptr)
		return STATUS_INVALID_PARAMETER_2;

	if (InDestinationProcess == nullptr)
		return STATUS_INVALID_PARAMETER_3;

	if (InDestinationAddress == nullptr)
		return STATUS_INVALID_PARAMETER_4;

	// 
	// Quick address validity check if we already are in the context of either process.
	// 

	if (InSourceProcess == PsGetCurrentProcess() && !MmIsAddressValid(InSourceAddress))
		return STATUS_INVALID_ADDRESS;

	if (InDestinationProcess == PsGetCurrentProcess() && !MmIsAddressValid(InDestinationAddress))
		return STATUS_INVALID_ADDRESS;

	// 
	// Copy virtual memory from the source process to the destination process.
	// 

	SIZE_T NumberOfBytesCopied = 0;
#ifndef _WIN11_WORKAROUND
	Status = MmCopyVirtualMemory(InSourceProcess, InSourceAddress, InDestinationProcess, InDestinationAddress, InNumberOfBytes, KernelMode, &NumberOfBytesCopied);
#else
	Status = MmCopyVirtualMemoryImpl(InSourceProcess, InSourceAddress, InDestinationProcess, InDestinationAddress, InNumberOfBytes, KernelMode, &NumberOfBytesCopied);
#endif

	// 
	// Return the result.
	// 

	if (OutNumberOfBytesCopied)
		*OutNumberOfBytesCopied = NumberOfBytesCopied;

	return Status;
}

/// <summary>
/// Safely copy virtual memory from the source address to the destination address.
/// </summary>
/// <param name="InSourceAddress">The source virtual address.</param>
/// <param name="InDestinationAddress">The destination virtual address.</param>
/// <param name="InNumberOfBytes">The number of bytes to copy.</param>
/// <param name="OutNumberOfBytesCopied">The number of bytes copied.</param>
NTSTATUS CkCopyVirtualMemory(CONST PVOID InSourceAddress, PVOID InDestinationAddress, SIZE_T InNumberOfBytes, OPTIONAL OUT SIZE_T* OutNumberOfBytesCopied)
{
	return CkCopyVirtualMemory(PsGetCurrentProcess(), InSourceAddress, PsGetCurrentProcess(), InDestinationAddress, InNumberOfBytes, OutNumberOfBytesCopied);
}

// 
// Information.
// 

/// <summary>
/// Queries information about the memory region located at the given virtual address.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InVirtualAddress">The virtual address of the region to lookup.</param>
/// <param name="OutMemoryInformation">The returned memory information.</param>
NTSTATUS CkQueryVirtualMemory(CONST PEPROCESS InProcess, CONST PVOID InVirtualAddress, OPTIONAL OUT MEMORY_BASIC_INFORMATION* OutMemoryInformation)
{
	NTSTATUS Status = { };

	// 
	// Verify the passed parameters.
	// 

	if (InProcess == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InVirtualAddress == nullptr)
		return STATUS_INVALID_PARAMETER_2;

	if (OutMemoryInformation == nullptr)
		return STATUS_INVALID_PARAMETER_3;

	// 
	// Open a handle to the process.
	// 

	auto* Handle = ZwCurrentProcess();

	if (InProcess != PsGetCurrentProcess())
		if (NT_ERROR(Status = ObOpenObjectByPointer(InProcess, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, GENERIC_ALL, *PsProcessType, KernelMode, &Handle)))
			return Status;

	// 
	// Retrieve the memory region information.
	// 

	MEMORY_BASIC_INFORMATION MemoryInformation = { };
	SIZE_T NumberOfBytesRead = 0;
	Status = ZwQueryVirtualMemory(Handle, InVirtualAddress, (MEMORY_INFORMATION_CLASS) MemoryBasicInformation, &MemoryInformation, sizeof(MEMORY_BASIC_INFORMATION), &NumberOfBytesRead);

	// 
	// If we opened a handle to the process, close it.
	// 

	if (Handle != ZwCurrentProcess())
		ZwClose(Handle);

	// 
	// Return the result.
	// 

	if (OutMemoryInformation != nullptr)
		RtlCopyMemory(OutMemoryInformation, &MemoryInformation, sizeof(MEMORY_BASIC_INFORMATION));
	
	return Status;
}

/// <summary>
/// Enumerates the memory regions in the given process and execute a callback for each entries.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS CkEnumerateVirtualMemory(CONST PEPROCESS InProcess, ENUMERATE_VIRTUAL_MEMORY InCallback)
{
	return CkEnumerateVirtualMemory<ENUMERATE_VIRTUAL_MEMORY>(InProcess, InCallback, [] (ULONG InIndex, MEMORY_BASIC_INFORMATION* InMemoryInformation, ENUMERATE_VIRTUAL_MEMORY InContext) -> bool
	{
		return InContext(InIndex, InMemoryInformation);
	});
}

/// <summary>
/// Enumerates the memory regions in the given process and execute a callback for each entries inside the specified range.
/// </summary>
/// <param name="InProcess">The process.</param>
/// <param name="InBaseAddress">The base address.</param>
/// <param name="InNumberOfBytes">The number of bytes.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS CkEnumerateVirtualMemoryInRange(CONST PEPROCESS InProcess, CONST PVOID InBaseAddress, SIZE_T InNumberOfBytes, ENUMERATE_VIRTUAL_MEMORY InCallback)
{
	return CkEnumerateVirtualMemoryInRange<ENUMERATE_VIRTUAL_MEMORY>(InProcess, InBaseAddress, InNumberOfBytes, InCallback, [] (ULONG InIndex, MEMORY_BASIC_INFORMATION* InMemoryInformation, ENUMERATE_VIRTUAL_MEMORY InContext) -> bool
	{
		return InContext(InIndex, InMemoryInformation);
	});
}
