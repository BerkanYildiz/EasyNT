#pragma once

/// <summary>
/// Reads and return the content of a file stored on disk.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileBuffer">The file's content.</param>
/// <param name="OutFileSize">The size (in bytes) of the file's content.</param>
NTSTATUS CkGetFileBuffer(UNICODE_STRING InFilePath, OUT PVOID* OutFileBuffer, OUT SIZE_T* OutFileSize);

/// <summary>
/// Reads and return the content of a file stored on disk.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileBuffer">The file's content.</param>
/// <param name="OutFileSize">The size (in bytes) of the file's content.</param>
NTSTATUS CkGetFileBuffer(CONST WCHAR* InFilePath, OUT PVOID* OutFileBuffer, OUT SIZE_T* OutFileSize);

/// <summary>
/// Reads and return the content of a file stored on disk.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileBuffer">The file's content.</param>
/// <param name="OutFileSize">The size (in bytes) of the file's content.</param>
NTSTATUS CkGetFileBuffer(CONST CHAR* InFilePath, OUT PVOID* OutFileBuffer, OUT SIZE_T* OutFileSize);

/// <summary>
/// Retrieves the size (in bytes) of a file stored on disk.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileSize">The size of the file in bytes.</param>
NTSTATUS CkGetFileSize(UNICODE_STRING InFilePath, OUT SIZE_T* OutFileSize);

/// <summary>
/// Retrieves the size (in bytes) of a file stored on disk.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileSize">The size of the file in bytes.</param>
NTSTATUS CkGetFileSize(CONST WCHAR* InFilePath, OUT SIZE_T* OutFileSize);

/// <summary>
/// Retrieves the size (in bytes) of a file stored on disk.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileSize">The size of the file in bytes.</param>
NTSTATUS CkGetFileSize(CONST CHAR* InFilePath, OUT SIZE_T* OutFileSize);
