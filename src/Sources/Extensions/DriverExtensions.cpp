#include "../../Headers/EasyNT.h"

// 
// Driver Objects.
// 

/// <summary>
/// Gets a driver object by its IO filename.
/// </summary>
/// <param name="InDriverName">The name of the driver.</param>
/// <param name="OutDriverObject">The returned driver object.</param>
/// <remarks>The object's reference count is incremented.</remarks>
NTSTATUS CkGetDriverObject(ANSI_STRING InDriverName, OPTIONAL OUT PDRIVER_OBJECT* OutDriverObject)
{
	UNICODE_STRING DriverName;
	NTSTATUS Status = RtlAnsiStringToUnicodeString(&DriverName, &InDriverName, TRUE);

	if (NT_ERROR(Status))
		return Status;
	
	Status = CkGetDriverObject(DriverName, OutDriverObject);
	RtlFreeUnicodeString(&DriverName);
	return Status;
}

/// <summary>
/// Gets a driver object by its IO filename.
/// </summary>
/// <param name="InDriverName">The name of the driver.</param>
/// <param name="OutDriverObject">The returned driver object.</param>
/// <remarks>The object's reference count is incremented.</remarks>
NTSTATUS CkGetDriverObject(UNICODE_STRING InDriverName, OPTIONAL OUT PDRIVER_OBJECT* OutDriverObject)
{
	NTSTATUS Status;
	DRIVER_OBJECT* DriverObject;

	// 
	// Retrieve the driver object.
	// 

	if (NT_ERROR(Status = ObReferenceObjectByName(&InDriverName, OBJ_CASE_INSENSITIVE, NULL, 0, *IoDriverObjectType, KernelMode, NULL, (PVOID*) &DriverObject)))
		return Status;

	// 
	// Return the result.
	// 
	
	if (OutDriverObject != nullptr)
		*OutDriverObject = DriverObject;

	if (OutDriverObject == nullptr && DriverObject != nullptr)
		ObfDereferenceObject(DriverObject);

	return STATUS_SUCCESS;
}

/// <summary>
/// Gets a driver object by its IO filename.
/// </summary>
/// <param name="InDriverName">The name of the driver.</param>
/// <param name="OutDriverObject">The returned driver object.</param>
/// <remarks>The object's reference count is incremented.</remarks>
NTSTATUS CkGetDriverObject(CONST CHAR* InDriverName, OPTIONAL OUT PDRIVER_OBJECT* OutDriverObject)
{
	ANSI_STRING DriverName;
	RtlInitAnsiString(&DriverName, InDriverName);
	return CkGetDriverObject(DriverName, OutDriverObject);
}

/// <summary>
/// Gets a driver object by its IO filename.
/// </summary>
/// <param name="InDriverName">The name of the driver.</param>
/// <param name="OutDriverObject">The returned driver object.</param>
/// <remarks>The object's reference count is incremented.</remarks>
NTSTATUS CkGetDriverObject(CONST WCHAR* InDriverName, OPTIONAL OUT PDRIVER_OBJECT* OutDriverObject)
{
	UNICODE_STRING DriverName;
	RtlInitUnicodeString(&DriverName, InDriverName);
	return CkGetDriverObject(DriverName, OutDriverObject);
}

// 
// Enumerations.
// 

/// <summary>
/// Enumerates every devices owned by the given driver.
/// </summary>
/// <param name="InDriverObject">The driver object.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS CkEnumerateDevicesOfDriver(CONST DRIVER_OBJECT* InDriverObject, bool(*InCallback)(ULONG, DEVICE_OBJECT*))
{
	// 
	// Verify the passed parameters.
	// 

	if (InDriverObject == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InCallback == nullptr)
		return STATUS_INVALID_PARAMETER_2;

	// 
	// Retrieve the first device object.
	// 

	DEVICE_OBJECT* CurrentDevice = InDriverObject->DeviceObject;

	if (CurrentDevice == nullptr)
		return STATUS_NO_MORE_ENTRIES;

	// 
	// Loop through the device objects owned by the driver.
	// 
	
	for (ULONG Idx = 0; ; Idx++)
	{
		ObfReferenceObject(CurrentDevice);
		const auto SkipOtherEntries = InCallback(Idx, CurrentDevice);
		ObfDereferenceObject(CurrentDevice);
		
		if (SkipOtherEntries)
			break;

		// 
		// Move onto the next entry.
		// 

		CurrentDevice = CurrentDevice->NextDevice;

		if (CurrentDevice == nullptr)
			break;
	}

	return STATUS_SUCCESS;
}