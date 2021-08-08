#include "../../Headers/EasyNT.h"

/// <summary>
/// Gets a device object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InDeviceName">The name of the device.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutDeviceObject">The device object.</param>
NTSTATUS IoGetDeviceObject(UNICODE_STRING InDeviceName, ACCESS_MASK InDesiredAccess, OUT PDEVICE_OBJECT* OutDeviceObject)
{
	FILE_OBJECT* FileObject = nullptr;
	DEVICE_OBJECT* DeviceObject = nullptr;
	
	// 
	// Retrieve the device and file objects.
	// 
	
	CONST NTSTATUS Status = IoGetDeviceObjectPointer(&InDeviceName, InDesiredAccess, &FileObject, &DeviceObject);

	if (NT_ERROR(Status))
		return Status;

	// 
	// We only want the device object, and the kernel API only increments the reference count of the file object,
	// so we have to increment the reference count of the device object first and then decrement the reference count of the file object.
	// 

	ObfReferenceObject(DeviceObject);
	ObfDereferenceObject(FileObject);

	// 
	// Return the result.
	// 

	*OutDeviceObject = DeviceObject;
	return Status;
}

/// <summary>
/// Gets a device object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InDeviceName">The name of the device.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutDeviceObject">The device object.</param>
NTSTATUS IoGetDeviceObject(CONST WCHAR* InDeviceName, ACCESS_MASK InDesiredAccess, OUT PDEVICE_OBJECT* OutDeviceObject)
{
	UNICODE_STRING DeviceName;
	RtlInitUnicodeString(&DeviceName, InDeviceName);
	return IoGetDeviceObject(DeviceName, InDesiredAccess, OutDeviceObject);
}

/// <summary>
/// Gets a device object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InDeviceName">The name of the device.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutDeviceObject">The device object.</param>
NTSTATUS IoGetDeviceObject(CONST CHAR* InDeviceName, ACCESS_MASK InDesiredAccess, OUT PDEVICE_OBJECT* OutDeviceObject)
{
	ANSI_STRING DeviceNameToConvert;
	RtlInitAnsiString(&DeviceNameToConvert, InDeviceName);
	
	UNICODE_STRING DeviceName;
	NTSTATUS Status = RtlAnsiStringToUnicodeString(&DeviceName, &DeviceNameToConvert, TRUE);

	if (NT_ERROR(Status))
		return Status;
	
	Status = IoGetDeviceObject(DeviceName, InDesiredAccess, OutDeviceObject);
	RtlFreeUnicodeString(&DeviceName);
	return Status;
}

/// <summary>
/// Gets a file object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InFileName">The name of the file.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutFileObject">The file object.</param>
NTSTATUS IoGetFileObject(UNICODE_STRING InFileName, ACCESS_MASK InDesiredAccess, OUT PFILE_OBJECT* OutFileObject)
{
	FILE_OBJECT* FileObject = nullptr;
	DEVICE_OBJECT* DeviceObject = nullptr;
	
	// 
	// Retrieve the device and file objects.
	// 
	
	CONST NTSTATUS Status = IoGetDeviceObjectPointer(&InFileName, InDesiredAccess, &FileObject, &DeviceObject);

	if (NT_ERROR(Status))
		return Status;

	// 
	// Return the result.
	// 

	*OutFileObject = FileObject;
	return Status;
}

/// <summary>
/// Gets a file object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InFileName">The name of the file.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutFileObject">The file object.</param>
NTSTATUS IoGetFileObject(CONST WCHAR* InFileName, ACCESS_MASK InDesiredAccess, OUT PFILE_OBJECT* OutFileObject)
{
	UNICODE_STRING FileName;
	RtlInitUnicodeString(&FileName, InFileName);
	return IoGetFileObject(FileName, InDesiredAccess, OutFileObject);
}

/// <summary>
/// Gets a file object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InFileName">The name of the file.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutFileObject">The file object.</param>
NTSTATUS IoGetFileObject(CONST CHAR* InFileName, ACCESS_MASK InDesiredAccess, OUT PFILE_OBJECT* OutFileObject)
{
	ANSI_STRING FileNameToConvert;
	RtlInitAnsiString(&FileNameToConvert, InFileName);
	
	UNICODE_STRING FileName;
	NTSTATUS Status = RtlAnsiStringToUnicodeString(&FileName, &FileNameToConvert, TRUE);

	if (NT_ERROR(Status))
		return Status;
	
	Status = IoGetFileObject(FileName, InDesiredAccess, OutFileObject);
	RtlFreeUnicodeString(&FileName);
	return Status;
}

/// <summary>
/// Gets the physical device object supposedly present in the given device object's stack.
/// </summary>
/// <param name="InDeviceObject">The device object whose stack will be enumerated.</param>
DEVICE_OBJECT* IoGetPhysicalDeviceObject(DEVICE_OBJECT* InDeviceObject)
{
	// 
	// Verify the passed parameters.
	// 

	if (InDeviceObject == nullptr)
		return nullptr;

	// 
	// Setup the enumeration context.
	// 

	struct ENUMERATION_CONTEXT
	{
		DEVICE_OBJECT* ReturnedDevice;
	};

	ENUMERATION_CONTEXT EnumerationContext;
	EnumerationContext.ReturnedDevice = nullptr;

	// 
	// Enumerate each device objects in the stack.
	// 

	IoEnumerateDeviceStack(InDeviceObject, &EnumerationContext, [] (ULONG InIndex, DEVICE_OBJECT* InDeviceObject, VOID* InContext) -> bool
	{
		auto* Context = (ENUMERATION_CONTEXT*) InContext;

		// 
		// Check if the enumerated device is owned by a BUS driver.
		// 

		if (InDeviceObject->Flags & DO_BUS_ENUMERATED_DEVICE)
		{
			if (Context->ReturnedDevice != nullptr)
				ObfDereferenceObject(Context->ReturnedDevice);
			
			Context->ReturnedDevice = InDeviceObject;
			ObfReferenceObject(InDeviceObject);
		}

		return false;
	});

	return EnumerationContext.ReturnedDevice;
}

/// <summary>
/// Gets the lowest device object of the specified device type, supposedly present in the given device object's stack.
/// </summary>
/// <param name="InDeviceObject">The device object whose stack will be enumerated.</param>
/// <param name="InDeviceType">The type of the device we want.</param>
DEVICE_OBJECT* IoGetLowestDeviceObjectOfType(DEVICE_OBJECT* InDeviceObject, ULONG InDeviceType)
{
	// 
	// Verify the passed parameters.
	// 

	if (InDeviceObject == nullptr)
		return nullptr;

	// 
	// Setup the enumeration context.
	// 

	struct ENUMERATION_CONTEXT
	{
		ULONG DeviceType;
		DEVICE_OBJECT* ReturnedDevice;
	};

	ENUMERATION_CONTEXT EnumerationContext;
	EnumerationContext.DeviceType = InDeviceType;
	EnumerationContext.ReturnedDevice = nullptr;

	// 
	// Enumerate each device in the stack.
	// 

	IoEnumerateDeviceStack(InDeviceObject, &EnumerationContext, [] (ULONG InIndex, DEVICE_OBJECT* InDeviceObject, VOID* InContext) -> bool
	{
		auto* Context = (ENUMERATION_CONTEXT*) InContext;

		// 
		// Check if the device is a PDO.
		// 

		if (InDeviceObject->Flags & DO_BUS_ENUMERATED_DEVICE)
			return false;

		// 
		// Check if the enumerated device object's type is of the one we want.
		// 

		if (InDeviceObject->DeviceType == Context->DeviceType)
		{
			if (Context->ReturnedDevice != nullptr)
				ObfDereferenceObject(Context->ReturnedDevice);

			Context->ReturnedDevice = InDeviceObject;
			ObfReferenceObject(InDeviceObject);
		}

		return false;
	});

	return EnumerationContext.ReturnedDevice;
}

/// <summary>
/// Gets the lowest device object owned by the specified driver, supposedly present in the given device object's stack.
/// </summary>
/// <param name="InDeviceObject">The device object whose stack will be enumerated.</param>
/// <param name="InOwningDriver">The driver owning the device we want.</param>
DEVICE_OBJECT* IoGetLowestDeviceObjectOwnedByDriver(DEVICE_OBJECT* InDeviceObject, CONST DRIVER_OBJECT* InOwningDriver)
{
	// 
	// Verify the passed parameters.
	// 

	if (InDeviceObject == nullptr)
		return nullptr;
	
	if (InOwningDriver == nullptr)
		return nullptr;

	// 
	// Setup the enumeration context.
	// 

	struct ENUMERATION_CONTEXT
	{
		CONST DRIVER_OBJECT* DriverObject;
		DEVICE_OBJECT* ReturnedDevice;
	};

	ENUMERATION_CONTEXT EnumerationContext;
	EnumerationContext.DriverObject = InOwningDriver;
	EnumerationContext.ReturnedDevice = nullptr;

	// 
	// Enumerate each device in the stack.
	// 

	IoEnumerateDeviceStack(InDeviceObject, &EnumerationContext, [] (ULONG InIndex, DEVICE_OBJECT* InDeviceObject, VOID* InContext) -> bool
	{
		auto* Context = (ENUMERATION_CONTEXT*) InContext;

		// 
		// Check if the device is a PDO.
		// 

		if (InDeviceObject->Flags & DO_BUS_ENUMERATED_DEVICE)
			return false;
		
		// 
		// Check if the enumerated device's driver is of the one we target.
		// 

		if (InDeviceObject->DriverObject == Context->DriverObject)
		{
			if (Context->ReturnedDevice != nullptr)
				ObfDereferenceObject(Context->ReturnedDevice);

			Context->ReturnedDevice = InDeviceObject;
			ObfReferenceObject(InDeviceObject);
		}

		return false;
	});

	return EnumerationContext.ReturnedDevice;
}

/// <summary>
/// Enumerates the device objects present in the given device's stack.
/// </summary>
/// <param name="InDeviceObject">The device whose stack will be enumerated.</param>
/// <param name="InContext">The context.</param>
/// <param name="InCallback">The function executed on each device in the stack.</param>
NTSTATUS IoEnumerateDeviceStack(DEVICE_OBJECT* InDeviceObject, VOID* InContext, ENUMERATE_DEVICE_STACK_WITH_CONTEXT InCallback)
{
	// 
	// Verify the passed parameters.
	// 

	if (InDeviceObject == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InContext == nullptr)
		return STATUS_INVALID_PARAMETER_2;

	if (InCallback == nullptr)
		return STATUS_INVALID_PARAMETER_3;

	// 
	// Retrieve the highest level device in the stack.
	// 

	CONST DEVICE_OBJECT* HighestDevice = IoGetAttachedDeviceReference(InDeviceObject);

	// 
	// Loop through the device stack.
	// 

	ULONG Idx = 0;

	for (auto* CurrentDevice = (DEVICE_OBJECT*) HighestDevice; CurrentDevice != nullptr; CurrentDevice = IoGetLowerDeviceObject(CurrentDevice))
	{
		// 
		// Execute the callback.
		// 

		CONST BOOLEAN SkipOtherDevices = InCallback(Idx++, CurrentDevice, InContext);

		// 
		// Dereference the current device.
		// 

		ObDereferenceObject(CurrentDevice);

		if (SkipOtherDevices)
			break;
	}
	
	return STATUS_SUCCESS;
}

/// <summary>
/// Enumerates the device objects present in the given device's stack.
/// </summary>
/// <param name="InDeviceObject">The device whose stack will be enumerated.</param>
/// <param name="InCallback">The function executed on each device in the stack.</param>
NTSTATUS IoEnumerateDeviceStack(DEVICE_OBJECT* InDeviceObject, ENUMERATE_DEVICE_STACK InCallback)
{
	return IoEnumerateDeviceStack(InDeviceObject, InCallback, [] (ULONG InIndex, DEVICE_OBJECT* InDeviceObject, PVOID InContext) -> bool
	{
		auto* Callback = (ENUMERATE_DEVICE_STACK) InContext;
		return Callback(InIndex, InDeviceObject);
	});
}
