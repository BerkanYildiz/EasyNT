#pragma once

// 
// Driver Objects.
// 

/// <summary>
/// Gets a driver object by its IO filename.
/// </summary>
/// <param name="InDriverName">The name of the driver.</param>
/// <param name="OutDriverObject">The returned driver object.</param>
/// <remarks>The object's reference count is incremented.</remarks>
NTSTATUS CkGetDriverObject(ANSI_STRING InDriverName, OPTIONAL OUT PDRIVER_OBJECT* OutDriverObject);

/// <summary>
/// Gets a driver object by its IO filename.
/// </summary>
/// <param name="InDriverName">The name of the driver.</param>
/// <param name="OutDriverObject">The returned driver object.</param>
/// <remarks>The object's reference count is incremented.</remarks>
NTSTATUS CkGetDriverObject(UNICODE_STRING InDriverName, OPTIONAL OUT PDRIVER_OBJECT* OutDriverObject);

/// <summary>
/// Gets a driver object by its IO filename.
/// </summary>
/// <param name="InDriverName">The name of the driver.</param>
/// <param name="OutDriverObject">The returned driver object.</param>
/// <remarks>The object's reference count is incremented.</remarks>
NTSTATUS CkGetDriverObject(CONST CHAR* InDriverName, OPTIONAL OUT PDRIVER_OBJECT* OutDriverObject);

/// <summary>
/// Gets a driver object by its IO filename.
/// </summary>
/// <param name="InDriverName">The name of the driver.</param>
/// <param name="OutDriverObject">The returned driver object.</param>
/// <remarks>The object's reference count is incremented.</remarks>
NTSTATUS CkGetDriverObject(CONST WCHAR* InDriverName, OPTIONAL OUT PDRIVER_OBJECT* OutDriverObject);

// 
// Enumerations.
// 

/// <summary>
/// Enumerates every devices owned by the given driver.
/// </summary>
/// <param name="InDriverObject">The driver object.</param>
/// <param name="InContext">The context.</param>
/// <param name="InCallback">The callback.</param>
template <typename TContext>
NTSTATUS CkEnumerateDevicesOfDriver(CONST DRIVER_OBJECT* InDriverObject, TContext InContext, bool(*InCallback)(ULONG, DEVICE_OBJECT*, TContext))
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
		const auto SkipOtherEntries = InCallback(Idx, CurrentDevice, InContext);
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
NTSTATUS CkEnumerateDevicesOfDriver(CONST DRIVER_OBJECT* InDriverObject, bool(*InCallback)(ULONG, DEVICE_OBJECT*));