#pragma once

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
NTSTATUS CkGetDeviceObject(ANSI_STRING InDeviceName, ACCESS_MASK InDesiredAccess, OPTIONAL OUT PDEVICE_OBJECT* OutDeviceObject);

/// <summary>
/// Gets a device object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InDeviceName">The name of the device.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutDeviceObject">The device object.</param>
/// <remarks>The object's reference count is incremented.</remarks>
NTSTATUS CkGetDeviceObject(UNICODE_STRING InDeviceName, ACCESS_MASK InDesiredAccess, OPTIONAL OUT PDEVICE_OBJECT* OutDeviceObject);

/// <summary>
/// Gets a device object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InDeviceName">The name of the device.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutDeviceObject">The device object.</param>
/// <remarks>The object's reference count is incremented.</remarks>
NTSTATUS CkGetDeviceObject(CONST CHAR* InDeviceName, ACCESS_MASK InDesiredAccess, OPTIONAL OUT PDEVICE_OBJECT* OutDeviceObject);

/// <summary>
/// Gets a device object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InDeviceName">The name of the device.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutDeviceObject">The device object.</param>
/// <remarks>The object's reference count is incremented.</remarks>
NTSTATUS CkGetDeviceObject(CONST WCHAR* InDeviceName, ACCESS_MASK InDesiredAccess, OPTIONAL OUT PDEVICE_OBJECT* OutDeviceObject);

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
NTSTATUS CkGetFileObject(ANSI_STRING InFileName, ACCESS_MASK InDesiredAccess, OPTIONAL OUT PFILE_OBJECT* OutFileObject);

/// <summary>
/// Gets a file object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InFileName">The name of the file.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutFileObject">The file object.</param>
/// <remarks>The object's reference count is incremented.</remarks>
NTSTATUS CkGetFileObject(UNICODE_STRING InFileName, ACCESS_MASK InDesiredAccess, OPTIONAL OUT PFILE_OBJECT* OutFileObject);

/// <summary>
/// Gets a file object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InFileName">The name of the file.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutFileObject">The file object.</param>
/// <remarks>The object's reference count is incremented.</remarks>
NTSTATUS CkGetFileObject(CONST CHAR* InFileName, ACCESS_MASK InDesiredAccess, OPTIONAL OUT PFILE_OBJECT* OutFileObject);

/// <summary>
/// Gets a file object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InFileName">The name of the file.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutFileObject">The file object.</param>
/// <remarks>The object's reference count is incremented.</remarks>
NTSTATUS CkGetFileObject(CONST WCHAR* InFileName, ACCESS_MASK InDesiredAccess, OPTIONAL OUT PFILE_OBJECT* OutFileObject);

// 
// Enumerations.
// 

/// <summary>
/// Enumerates the device objects present in the given device's stack.
/// </summary>
/// <typeparam name="TContext">The type of the context.</typeparam>
/// <param name="InDeviceObject">The device whose stack will be enumerated.</param>
/// <param name="InContext">The context.</param>
/// <param name="InCallback">The function executed on each device in the stack.</param>
template <typename TContext = PVOID>
NTSTATUS CkEnumerateDeviceStack(DEVICE_OBJECT* InDeviceObject, TContext InContext, bool(*InCallback)(ULONG, DEVICE_OBJECT*, TContext))
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
NTSTATUS CkEnumerateDeviceStack(DEVICE_OBJECT* InDeviceObject, bool(*InCallback)(ULONG, DEVICE_OBJECT*));

// 
// Relational.
// 

/// <summary>
/// Gets the physical device object present in the given device object's stack.
/// </summary>
/// <param name="InDeviceObject">The device object whose stack will be enumerated.</param>
/// <remarks>The object's reference count is incremented.</remarks>
DEVICE_OBJECT* CkGetPhysicalDeviceObject(DEVICE_OBJECT* InDeviceObject);

/// <summary>
/// Gets the lowest device object of the specified device type present in the given device object's stack.
/// </summary>
/// <param name="InDeviceObject">The device object whose stack will be enumerated.</param>
/// <param name="InDeviceType">The type of the device we want.</param>
/// <remarks>The object's reference count is incremented.</remarks>
DEVICE_OBJECT* CkGetLowestDeviceObjectOfType(DEVICE_OBJECT* InDeviceObject, ULONG InDeviceType);

/// <summary>
/// Gets the lowest device object owned by the specified driver present in the given device object's stack.
/// </summary>
/// <param name="InDeviceObject">The device object whose stack will be enumerated.</param>
/// <param name="InOwningDriver">The driver owning the device we want.</param>
/// <remarks>The object's reference count is incremented.</remarks>
DEVICE_OBJECT* CkGetLowestDeviceObjectOwnedByDriver(DEVICE_OBJECT* InDeviceObject, CONST DRIVER_OBJECT* InOwningDriver);
