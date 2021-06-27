#pragma once

typedef bool(*ENUMERATE_DRIVER_DEVICES)(ULONG InIndex, DEVICE_OBJECT* InDeviceObject);
typedef bool(*ENUMERATE_DRIVER_DEVICES_WITH_CONTEXT)(ULONG InIndex, DEVICE_OBJECT* InDeviceObject, VOID* InContext);

/// <summary>
/// Gets a driver object by its IO filename.
/// </summary>
/// <param name="InDriverName">The name of the driver.</param>
/// <param name="OutDriverObject">The returned driver object.</param>
NTSTATUS IoGetDriverObject(UNICODE_STRING InDriverName, PDRIVER_OBJECT* OutDriverObject);

/// <summary>
/// Gets a driver object by its IO filename.
/// </summary>
/// <param name="InDriverName">The name of the driver.</param>
/// <param name="OutDriverObject">The returned driver object.</param>
NTSTATUS IoGetDriverObject(CONST WCHAR* InDriverName, PDRIVER_OBJECT* OutDriverObject);

/// <summary>
/// Gets a driver object by its IO filename.
/// </summary>
/// <param name="InDriverName">The name of the driver.</param>
/// <param name="OutDriverObject">The returned driver object.</param>
NTSTATUS IoGetDriverObject(CONST CHAR* InDriverName, PDRIVER_OBJECT* OutDriverObject);

/// <summary>
/// Enumerates every devices owned by the given driver.
/// </summary>
/// <param name="InDriverObject">The driver object.</param>
/// <param name="InContext">The context.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS IoEnumerateDevices(CONST DRIVER_OBJECT* InDriverObject, PVOID InContext, ENUMERATE_DRIVER_DEVICES_WITH_CONTEXT InCallback);

/// <summary>
/// Enumerates every devices owned by the given driver.
/// </summary>
/// <param name="InDriverObject">The driver object.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS IoEnumerateDevices(CONST DRIVER_OBJECT* InDriverObject, ENUMERATE_DRIVER_DEVICES InCallback);
