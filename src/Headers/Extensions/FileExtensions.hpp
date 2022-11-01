#pragma once

// 
// Reading.
// 

/// <summary>
/// Opens an existing file and reads its entire content.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileBuffer">The file's content.</param>
/// <param name="OutFileSize">The size (in bytes) of the file's content.</param>
///	<remarks>The buffer needs to be released.</remarks>
NTSTATUS CkGetFileBuffer(ANSI_STRING InFilePath, OUT PVOID* OutFileBuffer, OUT SIZE_T* OutFileSize);

/// <summary>
/// Opens an existing file and reads its entire content.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileBuffer">The file's content.</param>
/// <param name="OutFileSize">The size (in bytes) of the file's content.</param>
///	<remarks>The buffer needs to be released.</remarks>
NTSTATUS CkGetFileBuffer(UNICODE_STRING InFilePath, OUT PVOID* OutFileBuffer, OUT SIZE_T* OutFileSize);

/// <summary>
/// Opens an existing file and reads its entire content.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileBuffer">The file's content.</param>
/// <param name="OutFileSize">The size (in bytes) of the file's content.</param>
///	<remarks>The buffer needs to be released.</remarks>
NTSTATUS CkGetFileBuffer(CONST CHAR* InFilePath, OUT PVOID* OutFileBuffer, OUT SIZE_T* OutFileSize);

/// <summary>
/// Opens an existing file and reads its entire content.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileBuffer">The file's content.</param>
/// <param name="OutFileSize">The size (in bytes) of the file's content.</param>
///	<remarks>The buffer needs to be released.</remarks>
NTSTATUS CkGetFileBuffer(CONST WCHAR* InFilePath, OUT PVOID* OutFileBuffer, OUT SIZE_T* OutFileSize);

// 
// Information.
// 

/// <summary>
/// Opens an existing file and returns the total length of its content.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileSize">The size (in bytes) of the file's content.</param>
NTSTATUS CkGetFileSize(ANSI_STRING InFilePath, OUT SIZE_T* OutFileSize);

/// <summary>
/// Opens an existing file and returns the total length of its content.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileSize">The size (in bytes) of the file's content.</param>
NTSTATUS CkGetFileSize(UNICODE_STRING InFilePath, OUT SIZE_T* OutFileSize);

/// <summary>
/// Opens an existing file and returns the total length of its content.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileSize">The size (in bytes) of the file's content.</param>
NTSTATUS CkGetFileSize(CONST CHAR* InFilePath, OUT SIZE_T* OutFileSize);

/// <summary>
/// Opens an existing file and returns the total length of its content.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileSize">The size (in bytes) of the file's content.</param>
NTSTATUS CkGetFileSize(CONST WCHAR* InFilePath, OUT SIZE_T* OutFileSize);
