#pragma once

typedef bool(*ENUMERATE_INTERFACE_DEVICES)(DEVICE_OBJECT* InDeviceObject);
typedef bool(*ENUMERATE_INTERFACE_DEVICES_WITH_CONTEXT)(DEVICE_OBJECT* InDeviceObject, VOID* InContext);

/// <summary>
/// Enumerates every device objects linked to the given interface GUID.
/// </summary>
/// <param name="InInterfaceGuid">The interface GUID.</param>
/// <param name="InContext">The context.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS IoEnumerateDevicesByInterface(GUID InInterfaceGuid, PVOID InContext, ENUMERATE_INTERFACE_DEVICES_WITH_CONTEXT InCallback);

/// <summary>
/// Enumerates every device objects linked to the given interface GUID.
/// </summary>
/// <param name="InInterfaceGuid">The interface GUID.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS IoEnumerateDevicesByInterface(GUID InInterfaceGuid, ENUMERATE_INTERFACE_DEVICES InCallback);
