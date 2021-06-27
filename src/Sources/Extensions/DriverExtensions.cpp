#include "../../Headers/EasyNT.h"

/// <summary>
/// Gets a driver object by its IO filename.
/// </summary>
/// <param name="InDriverName">The name of the driver.</param>
/// <param name="OutDriverObject">The returned driver object.</param>
NTSTATUS IoGetDriverObject(UNICODE_STRING InDriverName, PDRIVER_OBJECT* OutDriverObject)
{
	NTSTATUS Status;
	DRIVER_OBJECT* DriverObject;

	if (NT_ERROR(Status = ObReferenceObjectByName(&InDriverName, OBJ_CASE_INSENSITIVE, NULL, 0, *IoDriverObjectType, KernelMode, NULL, (PVOID*) &DriverObject)))
		return Status;

	*OutDriverObject = DriverObject;
	return STATUS_SUCCESS;
}

/// <summary>
/// Gets a driver object by its IO filename.
/// </summary>
/// <param name="InDriverName">The name of the driver.</param>
/// <param name="OutDriverObject">The returned driver object.</param>
NTSTATUS IoGetDriverObject(CONST WCHAR* InDriverName, PDRIVER_OBJECT* OutDriverObject)
{
	if (InDriverName == nullptr)
		return STATUS_INVALID_PARAMETER_1;
	
	UNICODE_STRING DriverName;
	RtlInitUnicodeString(&DriverName, InDriverName);
	return IoGetDriverObject(DriverName, OutDriverObject);
}

/// <summary>
/// Gets a driver object by its IO filename.
/// </summary>
/// <param name="InDriverName">The name of the driver.</param>
/// <param name="OutDriverObject">The returned driver object.</param>
NTSTATUS IoGetDriverObject(CONST CHAR* InDriverName, PDRIVER_OBJECT* OutDriverObject)
{
	if (InDriverName == nullptr)
		return STATUS_INVALID_PARAMETER_1;
	
	ANSI_STRING DriverNameToConvert;
	RtlInitAnsiString(&DriverNameToConvert, InDriverName);

	UNICODE_STRING DriverName;
	NTSTATUS Status = RtlAnsiStringToUnicodeString(&DriverName, &DriverNameToConvert, TRUE);

	if (NT_ERROR(Status))
		return Status;
	
	Status = IoGetDriverObject(DriverName, OutDriverObject);
	RtlFreeUnicodeString(&DriverName);
	return Status;
}
