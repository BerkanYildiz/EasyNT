#pragma once

/// <summary>
/// Enumerates every device objects registered to the given interface.
/// </summary>
/// <typeparam name="TContext">The type of the context.</typeparam>
/// <param name="InInterfaceGuid">The interface GUID.</param>
/// <param name="InContext">The context.</param>
/// <param name="InCallback">The callback.</param>
template <typename TContext = PVOID>
NTSTATUS CkEnumerateDevicesByInterface(GUID InInterfaceGuid, TContext InContext, bool(*InCallback)(DEVICE_OBJECT*, TContext))
{
	NTSTATUS Status;
	
	// 
	// Verify the passed parameters.
	// 
	
	if (InContext == nullptr)
		return STATUS_INVALID_PARAMETER_2;

	if (InCallback == nullptr)
		return STATUS_INVALID_PARAMETER_3;

	// 
	// Retrieve the names of the devices implementing the given interface.
	// 

	WCHAR* SymbolicList;
	Status = IoGetDeviceInterfaces(&InInterfaceGuid, nullptr, 0, &SymbolicList);

	if (NT_ERROR(Status))
		return Status;

	// 
	// Parse the symbolic links list.
	// 

	auto* CurrentSymbolicLink = (WCHAR*) RtlAddOffsetToPointer(SymbolicList, 0);
	
	while (TRUE)
	{
		UNICODE_STRING SymbolicName;
		RtlInitUnicodeString(&SymbolicName, CurrentSymbolicLink);

		if (SymbolicName.Length == 0)
			break;

		// 
		// Retrieve the device object with this symbolic name.
		// 

		DEVICE_OBJECT* DeviceObject;
		Status = CkGetDeviceObject(SymbolicName, SYNCHRONIZE, &DeviceObject);

		if (NT_SUCCESS(Status))
		{
			// 
			// Execute the callback on this device object.
			// 

			const auto SkipOtherEntries = InCallback(DeviceObject, InContext);

			ObfDereferenceObject(DeviceObject);

			if (SkipOtherEntries)
				break;
		}

		// 
		// Move onto the next entry.
		// 

		CurrentSymbolicLink = (WCHAR*) RtlAddOffsetToPointer(CurrentSymbolicLink, SymbolicName.MaximumLength);
	}

	// 
	// Release the memory allocated for the symbolic links list.
	// 

	ExFreePoolWithTag(SymbolicList, 0);
	return STATUS_SUCCESS;
}

/// <summary>
/// Enumerates every device objects registered to the given interface.
/// </summary>
/// <param name="InInterfaceGuid">The interface GUID.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS CkEnumerateDevicesByInterface(GUID InInterfaceGuid, bool(*InCallback)(DEVICE_OBJECT*));

/// <summary>
/// Enumerates every device objects registered to the given interface.
/// </summary>
/// <typeparam name="TContext">The type of the context.</typeparam>
/// <param name="InInterfaceGuid">The interface GUID.</param>
/// <param name="InContext">The context.</param>
/// <param name="InCallback">The callback.</param>
template <typename TContext = PVOID>
NTSTATUS CkEnumerateDevicesByInterface(CONST WCHAR* InInterfaceGuid, TContext InContext, bool(*InCallback)(DEVICE_OBJECT*, TContext))
{
	NTSTATUS Status;

	// 
	// Verify the passed parameters.
	// 

	if (InInterfaceGuid == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	// 
	// Convert the interface GUID from a unicode string to an actual GUID structure.
	// 
	
	UNICODE_STRING InterfaceGuidToConvert;
	RtlInitUnicodeString(&InterfaceGuidToConvert, InInterfaceGuid);
	
	GUID InterfaceGuid;
	Status = RtlGUIDFromString(&InterfaceGuidToConvert, &InterfaceGuid);

	if (NT_ERROR(Status))
		return Status;

	return CkEnumerateDevicesByInterface<TContext>(InterfaceGuid, InContext, InCallback);
}

/// <summary>
/// Enumerates every device objects registered to the given interface.
/// </summary>
/// <param name="InInterfaceGuid">The interface GUID.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS CkEnumerateDevicesByInterface(CONST WCHAR* InInterfaceGuid, bool(*InCallback)(DEVICE_OBJECT*));
