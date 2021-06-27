#pragma once

typedef bool(* ENUMERATE_DEVICE_STACK)(ULONG InIndex, DEVICE_OBJECT* InDeviceObject);
typedef bool(* ENUMERATE_DEVICE_STACK_WITH_CONTEXT)(ULONG InIndex, DEVICE_OBJECT* InDeviceObject, VOID* InContext);

/// <summary>
/// Gets a device object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InDeviceName">The name of the device.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutDeviceObject">The device object.</param>
NTSTATUS IoGetDeviceObject(UNICODE_STRING InDeviceName, ACCESS_MASK InDesiredAccess, OUT PDEVICE_OBJECT* OutDeviceObject);

/// <summary>
/// Gets a device object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InDeviceName">The name of the device.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutDeviceObject">The device object.</param>
NTSTATUS IoGetDeviceObject(CONST WCHAR* InDeviceName, ACCESS_MASK InDesiredAccess, OUT PDEVICE_OBJECT* OutDeviceObject);

/// <summary>
/// Gets a device object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InDeviceName">The name of the device.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutDeviceObject">The device object.</param>
NTSTATUS IoGetDeviceObject(CONST CHAR* InDeviceName, ACCESS_MASK InDesiredAccess, OUT PDEVICE_OBJECT* OutDeviceObject);

/// <summary>
/// Gets a file object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InFileName">The name of the file.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutFileObject">The file object.</param>
NTSTATUS IoGetFileObject(UNICODE_STRING InFileName, ACCESS_MASK InDesiredAccess, OUT PFILE_OBJECT* OutFileObject);

/// <summary>
/// Gets a file object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InFileName">The name of the file.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutFileObject">The file object.</param>
NTSTATUS IoGetFileObject(CONST WCHAR* InFileName, ACCESS_MASK InDesiredAccess, OUT PFILE_OBJECT* OutFileObject);

/// <summary>
/// Gets a file object by its IO filename, with the desired permissions.
/// </summary>
/// <param name="InFileName">The name of the file.</param>
/// <param name="InDesiredAccess">The desired access.</param>
/// <param name="OutFileObject">The file object.</param>
NTSTATUS IoGetFileObject(CONST CHAR* InFileName, ACCESS_MASK InDesiredAccess, OUT PFILE_OBJECT* OutFileObject);

/// <summary>
/// Gets the physical device object supposedly present in the given device object's stack.
/// </summary>
/// <param name="InDeviceObject">The device object whose stack will be enumerated.</param>
DEVICE_OBJECT* IoGetPhysicalDeviceObject(DEVICE_OBJECT* InDeviceObject);

/// <summary>
/// Gets the lowest device object of the specified device type, supposedly present in the given device object's stack.
/// </summary>
/// <param name="InDeviceObject">The device object whose stack will be enumerated.</param>
/// <param name="InDeviceType">The type of the device we want.</param>
DEVICE_OBJECT* IoGetLowestDeviceObjectOfType(DEVICE_OBJECT* InDeviceObject, ULONG InDeviceType);

/// <summary>
/// Gets the lowest device object owned by the specified driver, supposedly present in the given device object's stack.
/// </summary>
/// <param name="InDeviceObject">The device object whose stack will be enumerated.</param>
/// <param name="InOwningDriver">The driver owning the device we want.</param>
DEVICE_OBJECT* IoGetLowestDeviceObjectOwnedByDriver(DEVICE_OBJECT* InDeviceObject, CONST DRIVER_OBJECT* InOwningDriver);

/// <summary>
/// Enumerates the device objects present in the given device's stack.
/// </summary>
/// <param name="InDeviceObject">The device whose stack will be enumerated.</param>
/// <param name="InContext">The context.</param>
/// <param name="InCallback">The function executed on each device in the stack.</param>
NTSTATUS IoEnumerateDeviceStack(DEVICE_OBJECT* InDeviceObject, VOID* InContext, ENUMERATE_DEVICE_STACK_WITH_CONTEXT InCallback);

/// <summary>
/// Enumerates the device objects present in the given device's stack.
/// </summary>
/// <param name="InDeviceObject">The device whose stack will be enumerated.</param>
/// <param name="InCallback">The function executed on each device in the stack.</param>
NTSTATUS IoEnumerateDeviceStack(DEVICE_OBJECT* InDeviceObject, ENUMERATE_DEVICE_STACK InCallback);
