#include "../../Headers/EasyNT.h"

/// <summary>
/// Enumerates every device objects registered to the given interface.
/// </summary>
/// <param name="InInterfaceGuid">The interface GUID.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS CkEnumerateDevicesByInterface(GUID InInterfaceGuid, bool(*InCallback)(DEVICE_OBJECT*))
{
	return CkEnumerateDevicesByInterface<PVOID>(InInterfaceGuid, InCallback, [] (DEVICE_OBJECT* InDeviceObject, PVOID InContext) -> bool
	{
		return ((bool(*)(DEVICE_OBJECT*)) InContext) (InDeviceObject);
	});
}

/// <summary>
/// Enumerates every device objects registered to the given interface.
/// </summary>
/// <param name="InInterfaceGuid">The interface GUID.</param>
/// <param name="InCallback">The callback.</param>
NTSTATUS CkEnumerateDevicesByInterface(CONST WCHAR* InInterfaceGuid, bool(*InCallback)(DEVICE_OBJECT*))
{
	return CkEnumerateDevicesByInterface<PVOID>(InInterfaceGuid, InCallback, [] (DEVICE_OBJECT* InDeviceObject, PVOID InContext) -> bool
	{
		return ((bool(*)(DEVICE_OBJECT*)) InContext) (InDeviceObject);
	});
}
