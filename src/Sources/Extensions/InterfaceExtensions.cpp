#include "../../Headers/EasyNT.h"

/// <summary>
/// Enumerates every device objects linked to the given interface GUID.
/// </summary>
/// <param name="InInterfaceGuid">The interface GUID.</param>
/// <param name="InContext">The context.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS IoEnumerateDevicesByInterface(GUID InInterfaceGuid, PVOID InContext, ENUMERATE_INTERFACE_DEVICES_WITH_CONTEXT InCallback)
{
	NTSTATUS Status;
	
	// 
	// Verify the passed parameters.
	// 

	if (InInterfaceGuid.Data1 == NULL ||
		InInterfaceGuid.Data2 == NULL ||
		InInterfaceGuid.Data3 == NULL)
	{
		return STATUS_INVALID_PARAMETER_1;
	}

	if (InContext == NULL)
	{
		return STATUS_INVALID_PARAMETER_2;
	}

	if (InCallback == NULL)
	{
		return STATUS_INVALID_PARAMETER_3;
	}

	// 
	// Retrieve the filenames of the devices implementing the given interface.
	// 

	WCHAR* SymbolicList;
	Status = IoGetDeviceInterfaces(&InInterfaceGuid, NULL, 0, &SymbolicList);

	if (NT_ERROR(Status))
		return Status;

	// 
	// Parse the symbolic links list.
	// 

	WCHAR* CurrentSymbolicLink = (WCHAR*) RtlAddOffsetToPointer(SymbolicList, 0);
	
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
		Status = IoGetDeviceObject(SymbolicList, GENERIC_ALL, &DeviceObject);

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

		CurrentSymbolicLink = (WCHAR*) RtlAddOffsetToPointer(CurrentSymbolicLink, SymbolicName.Length);
	}

	// 
	// Release the memory allocated for the symbolic links list.
	// 

	ExFreePoolWithTag(SymbolicList, 0);
	return STATUS_SUCCESS;
}

/// <summary>
/// Enumerates every device objects linked to the given interface GUID.
/// </summary>
/// <param name="InInterfaceGuid">The interface GUID.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS IoEnumerateDevicesByInterface(GUID InInterfaceGuid, ENUMERATE_INTERFACE_DEVICES InCallback)
{
	return IoEnumerateDevicesByInterface(InInterfaceGuid, InCallback, [] (DEVICE_OBJECT* InDeviceObject, PVOID InContext) -> bool
	{
		auto* Callback = (ENUMERATE_INTERFACE_DEVICES) InContext;
		return Callback(InDeviceObject);
	});
}

/// <summary>
/// Enumerates every device objects linked to the given interface GUID.
/// </summary>
/// <param name="InInterfaceGuid">The interface GUID.</param>
/// <param name="InContext">The context.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS IoEnumerateDevicesByInterface(CONST WCHAR* InInterfaceGuid, PVOID InContext, ENUMERATE_INTERFACE_DEVICES_WITH_CONTEXT InCallback)
{
	NTSTATUS Status;

	// 
	// Verify the passed parameters.
	// 

	if (InInterfaceGuid == NULL)
		return STATUS_INVALID_PARAMETER_1;

	// 
	// Convert the interface GUID from a unicode string to an actual parsed GUID structure.
	// 
	
	UNICODE_STRING InterfaceGuidToConvert;
	RtlInitUnicodeString(&InterfaceGuidToConvert, InInterfaceGuid);
	
	GUID InterfaceGuid;
	Status = RtlGUIDFromString(&InterfaceGuidToConvert, &InterfaceGuid);

	if (NT_ERROR(Status))
		return Status;

	return IoEnumerateDevicesByInterface(InterfaceGuid, InContext, InCallback);
}

/// <summary>
/// Enumerates every device objects linked to the given interface GUID.
/// </summary>
/// <param name="InInterfaceGuid">The interface GUID.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS IoEnumerateDevicesByInterface(CONST WCHAR* InInterfaceGuid, ENUMERATE_INTERFACE_DEVICES InCallback)
{
	NTSTATUS Status;

	// 
	// Verify the passed parameters.
	// 

	if (InInterfaceGuid == NULL)
		return STATUS_INVALID_PARAMETER_1;

	// 
	// Convert the interface GUID from a unicode string to an actual parsed GUID structure.
	// 
	
	UNICODE_STRING InterfaceGuidToConvert;
	RtlInitUnicodeString(&InterfaceGuidToConvert, InInterfaceGuid);
	
	GUID InterfaceGuid;
	Status = RtlGUIDFromString(&InterfaceGuidToConvert, &InterfaceGuid);

	if (NT_ERROR(Status))
		return Status;

	return IoEnumerateDevicesByInterface(InterfaceGuid, InCallback);
}
