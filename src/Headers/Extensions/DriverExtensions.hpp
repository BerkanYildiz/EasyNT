#pragma once

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
