#include "../../Headers/EasyNT.h"

/// <summary>
/// Gets a driver object by its IO filename.
/// </summary>
/// <param name="InDriverName">The name of the driver.</param>
/// <param name="OutDriverObject">The returned driver object.</param>
NTSTATUS IoGetDriverObject(UNICODE_STRING InDriverName, PDRIVER_OBJECT* OutDriverObject)
{
	NTSTATUS Status;
	DRIVER_OBJECT* DriverObject;

	if (NT_ERROR(Status = ObReferenceObjectByName(&InDriverName, OBJ_CASE_INSENSITIVE, NULL, 0, *IoDriverObjectType, KernelMode, NULL, (PVOID*) &DriverObject)))
		return Status;

	*OutDriverObject = DriverObject;
	return STATUS_SUCCESS;
}

/// <summary>
/// Gets a driver object by its IO filename.
/// </summary>
/// <param name="InDriverName">The name of the driver.</param>
/// <param name="OutDriverObject">The returned driver object.</param>
NTSTATUS IoGetDriverObject(CONST WCHAR* InDriverName, PDRIVER_OBJECT* OutDriverObject)
{
	if (InDriverName == nullptr)
		return STATUS_INVALID_PARAMETER_1;
	
	UNICODE_STRING DriverName;
	RtlInitUnicodeString(&DriverName, InDriverName);
	return IoGetDriverObject(DriverName, OutDriverObject);
}

/// <summary>
/// Gets a driver object by its IO filename.
/// </summary>
/// <param name="InDriverName">The name of the driver.</param>
/// <param name="OutDriverObject">The returned driver object.</param>
NTSTATUS IoGetDriverObject(CONST CHAR* InDriverName, PDRIVER_OBJECT* OutDriverObject)
{
	if (InDriverName == nullptr)
		return STATUS_INVALID_PARAMETER_1;
	
	ANSI_STRING DriverNameToConvert;
	RtlInitAnsiString(&DriverNameToConvert, InDriverName);

	UNICODE_STRING DriverName;
	NTSTATUS Status = RtlAnsiStringToUnicodeString(&DriverName, &DriverNameToConvert, TRUE);

	if (NT_ERROR(Status))
		return Status;
	
	Status = IoGetDriverObject(DriverName, OutDriverObject);
	RtlFreeUnicodeString(&DriverName);
	return Status;
}

/// <summary>
/// Enumerates every devices owned by the given driver.
/// </summary>
/// <param name="InDriverObject">The driver object.</param>
/// <param name="InContext">The context.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS IoEnumerateDevices(CONST DRIVER_OBJECT* InDriverObject, PVOID InContext, ENUMERATE_DRIVER_DEVICES_WITH_CONTEXT InCallback)
{
	// 
	// Verify the passed parameters.
	// 

	if (InDriverObject == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InContext == nullptr)
		return STATUS_INVALID_PARAMETER_2;

	if (InCallback == nullptr)
		return STATUS_INVALID_PARAMETER_3;

	if (InDriverObject->DeviceObject == nullptr)
		return STATUS_NO_MORE_ENTRIES;

	// 
	// Loop through the device objects owned by the driver.
	// 

	auto CurrentDeviceIndex = 0;
	auto* CurrentDevice = InDriverObject->DeviceObject;

	while (TRUE)
	{
		ObfReferenceObject(CurrentDevice);
		const auto SkipOtherEntries = InCallback(CurrentDeviceIndex++, CurrentDevice, InContext);
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

/// <summary>
/// Enumerates every devices owned by the given driver.
/// </summary>
/// <param name="InDriverObject">The driver object.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS IoEnumerateDevices(CONST DRIVER_OBJECT* InDriverObject, ENUMERATE_DRIVER_DEVICES InCallback)
{
	return IoEnumerateDevices(InDriverObject, InCallback, [] (ULONG InIndex, DEVICE_OBJECT* InDeviceObject, PVOID InContext) -> bool
	{
		auto* Callback = (ENUMERATE_DRIVER_DEVICES) InContext;
		return Callback(InIndex, InDeviceObject);
	});
}
