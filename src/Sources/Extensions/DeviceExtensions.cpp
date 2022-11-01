#include "../../Headers/EasyNT.h"

// 
// Device Objects.
// 

/// <summary>
/// Gets a device object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InDeviceName">The name of the device.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutDeviceObject">The device object.</param>
/// <remarks>The object's reference count is incremented.</remarks>
NTSTATUS CkGetDeviceObject(ANSI_STRING InDeviceName, ACCESS_MASK InDesiredAccess, OPTIONAL OUT PDEVICE_OBJECT* OutDeviceObject)
{
	UNICODE_STRING DeviceName;
	NTSTATUS Status = RtlAnsiStringToUnicodeString(&DeviceName, &InDeviceName, TRUE);

	if (NT_ERROR(Status))
		return Status;

	Status = CkGetDeviceObject(DeviceName, InDesiredAccess, OutDeviceObject);
	RtlFreeUnicodeString(&DeviceName);
	return Status;
}

/// <summary>
/// Gets a device object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InDeviceName">The name of the device.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutDeviceObject">The device object.</param>
/// <remarks>The object's reference count is incremented.</remarks>
NTSTATUS CkGetDeviceObject(UNICODE_STRING InDeviceName, ACCESS_MASK InDesiredAccess, OPTIONAL OUT PDEVICE_OBJECT* OutDeviceObject)
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

	if (OutDeviceObject != nullptr)
		ObfReferenceObject(DeviceObject);
	ObfDereferenceObject(FileObject);

	// 
	// Return the result.
	// 

	if (OutDeviceObject != nullptr)
		*OutDeviceObject = DeviceObject;

	return Status;
}

/// <summary>
/// Gets a device object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InDeviceName">The name of the device.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutDeviceObject">The device object.</param>
/// <remarks>The object's reference count is incremented.</remarks>
NTSTATUS CkGetDeviceObject(CONST CHAR* InDeviceName, ACCESS_MASK InDesiredAccess, OPTIONAL OUT PDEVICE_OBJECT* OutDeviceObject)
{
	ANSI_STRING DeviceName;
	RtlInitAnsiString(&DeviceName, InDeviceName);
	return CkGetDeviceObject(DeviceName, InDesiredAccess, OutDeviceObject);
}

/// <summary>
/// Gets a device object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InDeviceName">The name of the device.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutDeviceObject">The device object.</param>
/// <remarks>The object's reference count is incremented.</remarks>
NTSTATUS CkGetDeviceObject(CONST WCHAR* InDeviceName, ACCESS_MASK InDesiredAccess, OPTIONAL OUT PDEVICE_OBJECT* OutDeviceObject)
{
	UNICODE_STRING DeviceName;
	RtlInitUnicodeString(&DeviceName, InDeviceName);
	return CkGetDeviceObject(DeviceName, InDesiredAccess, OutDeviceObject);
}

// 
// File Objects.
// 

/// <summary>
/// Gets a file object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InFileName">The name of the file.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutFileObject">The file object.</param>
/// <remarks>The object's reference count is incremented.</remarks>
NTSTATUS CkGetFileObject(ANSI_STRING InFileName, ACCESS_MASK InDesiredAccess, OPTIONAL OUT PFILE_OBJECT* OutFileObject)
{
	UNICODE_STRING FileName;
	NTSTATUS Status = RtlAnsiStringToUnicodeString(&FileName, &InFileName, TRUE);

	if (NT_ERROR(Status))
		return Status;
	
	Status = CkGetFileObject(FileName, InDesiredAccess, OutFileObject);
	RtlFreeUnicodeString(&FileName);
	return Status;
}

/// <summary>
/// Gets a file object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InFileName">The name of the file.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutFileObject">The file object.</param>
/// <remarks>The object's reference count is incremented.</remarks>
NTSTATUS CkGetFileObject(UNICODE_STRING InFileName, ACCESS_MASK InDesiredAccess, OPTIONAL OUT PFILE_OBJECT* OutFileObject)
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

	if (OutFileObject != nullptr)
		*OutFileObject = FileObject;

	if (OutFileObject == nullptr && FileObject != nullptr)
		ObfDereferenceObject(FileObject);

	return Status;
}

/// <summary>
/// Gets a file object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InFileName">The name of the file.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutFileObject">The file object.</param>
/// <remarks>The object's reference count is incremented.</remarks>
NTSTATUS CkGetFileObject(CONST CHAR* InFileName, ACCESS_MASK InDesiredAccess, OPTIONAL OUT PFILE_OBJECT* OutFileObject)
{
	ANSI_STRING FileName;
	RtlInitAnsiString(&FileName, InFileName);
	return CkGetFileObject(FileName, InDesiredAccess, OutFileObject);
}

/// <summary>
/// Gets a file object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InFileName">The name of the file.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutFileObject">The file object.</param>
/// <remarks>The object's reference count is incremented.</remarks>
NTSTATUS CkGetFileObject(CONST WCHAR* InFileName, ACCESS_MASK InDesiredAccess, OPTIONAL OUT PFILE_OBJECT* OutFileObject)
{
	UNICODE_STRING FileName;
	RtlInitUnicodeString(&FileName, InFileName);
	return CkGetFileObject(FileName, InDesiredAccess, OutFileObject);
}

// 
// Enumerations.
// 

/// <summary>
/// Enumerates the device objects present in the given device's stack.
/// </summary>
/// <param name="InDeviceObject">The device whose stack will be enumerated.</param>
/// <param name="InCallback">The function executed on each device in the stack.</param>
NTSTATUS CkEnumerateDeviceStack(DEVICE_OBJECT* InDeviceObject, bool(*InCallback)(ULONG, DEVICE_OBJECT*))
{
	// 
	// Verify the passed parameters.
	// 

	if (InDeviceObject == nullptr)
		return STATUS_INVALID_PARAMETER_1;

	if (InCallback == nullptr)
		return STATUS_INVALID_PARAMETER_2;

	// 
	// Retrieve the highest level device in the stack.
	// 

	auto* HighestDevice = IoGetAttachedDeviceReference(InDeviceObject);

	// 
	// Loop through the device stack.
	// 

	ULONG Idx = 0;

	for (auto* CurrentDevice = HighestDevice; CurrentDevice != nullptr; CurrentDevice = IoGetLowerDeviceObject(CurrentDevice))
	{
		// 
		// Execute the callback.
		// 

		CONST BOOLEAN SkipOtherDevices = InCallback(Idx++, CurrentDevice);

		// 
		// Dereference the current device.
		// 

		ObDereferenceObject(CurrentDevice);

		if (SkipOtherDevices)
			break;
	}
	
	return STATUS_SUCCESS;
}

// 
// Relational.
// 

/// <summary>
/// Gets the physical device object present in the given device object's stack.
/// </summary>
/// <param name="InDeviceObject">The device object whose stack will be enumerated.</param>
/// <remarks>The object's reference count is incremented.</remarks>
DEVICE_OBJECT* CkGetPhysicalDeviceObject(DEVICE_OBJECT* InDeviceObject)
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

	CkEnumerateDeviceStack<ENUMERATION_CONTEXT*>(InDeviceObject, &EnumerationContext, [] (ULONG InIndex, DEVICE_OBJECT* InDeviceObject, ENUMERATION_CONTEXT* InContext) -> bool
	{
		// 
		// Check if the enumerated device is owned by a BUS driver.
		// 

		if (InDeviceObject->Flags & DO_BUS_ENUMERATED_DEVICE)
		{
			if (InContext->ReturnedDevice != nullptr)
			{
				ObfDereferenceObject(InContext->ReturnedDevice);
				InContext->ReturnedDevice = nullptr;
			}
			
			ObfReferenceObject(InDeviceObject);
			InContext->ReturnedDevice = InDeviceObject;
		}

		return false;
	});

	return EnumerationContext.ReturnedDevice;
}

/// <summary>
/// Gets the lowest device object of the specified device type present in the given device object's stack.
/// </summary>
/// <param name="InDeviceObject">The device object whose stack will be enumerated.</param>
/// <param name="InDeviceType">The type of the device we want.</param>
/// <remarks>The object's reference count is incremented.</remarks>
DEVICE_OBJECT* CkGetLowestDeviceObjectOfType(DEVICE_OBJECT* InDeviceObject, ULONG InDeviceType)
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

	CkEnumerateDeviceStack<ENUMERATION_CONTEXT*>(InDeviceObject, &EnumerationContext, [] (ULONG InIndex, DEVICE_OBJECT* InDeviceObject, ENUMERATION_CONTEXT* InContext) -> bool
	{
		// 
		// Check if the device is a PDO.
		// 

		if (InDeviceObject->Flags & DO_BUS_ENUMERATED_DEVICE)
			return false;

		// 
		// Check if the enumerated device object's type is of the one we want.
		// 

		if (InDeviceObject->DeviceType == InContext->DeviceType)
		{
			if (InContext->ReturnedDevice != nullptr)
			{
				ObfDereferenceObject(InContext->ReturnedDevice);
				InContext->ReturnedDevice = nullptr;
			}

			ObfReferenceObject(InDeviceObject);
			InContext->ReturnedDevice = InDeviceObject;
		}

		return false;
	});

	return EnumerationContext.ReturnedDevice;
}

/// <summary>
/// Gets the lowest device object owned by the specified driver present in the given device object's stack.
/// </summary>
/// <param name="InDeviceObject">The device object whose stack will be enumerated.</param>
/// <param name="InOwningDriver">The driver owning the device we want.</param>
/// <remarks>The object's reference count is incremented.</remarks>
DEVICE_OBJECT* CkGetLowestDeviceObjectOwnedByDriver(DEVICE_OBJECT* InDeviceObject, CONST DRIVER_OBJECT* InOwningDriver)
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

	CkEnumerateDeviceStack<ENUMERATION_CONTEXT*>(InDeviceObject, &EnumerationContext, [] (ULONG InIndex, DEVICE_OBJECT* InDeviceObject, ENUMERATION_CONTEXT* InContext) -> bool
	{
		// 
		// Check if the device is a PDO.
		// 

		if (InDeviceObject->Flags & DO_BUS_ENUMERATED_DEVICE)
			return false;
		
		// 
		// Check if the enumerated device's driver is of the one we target.
		// 

		if (InDeviceObject->DriverObject == InContext->DriverObject)
		{
			if (InContext->ReturnedDevice != nullptr)
			{
				ObfDereferenceObject(InContext->ReturnedDevice);
				InContext->ReturnedDevice = nullptr;
			}

			ObfReferenceObject(InDeviceObject);
			InContext->ReturnedDevice = InDeviceObject;
		}

		return false;
	});

	return EnumerationContext.ReturnedDevice;
}
