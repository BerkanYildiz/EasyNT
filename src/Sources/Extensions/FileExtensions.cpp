#include "../../Headers/EasyNT.h"

/// <summary>
/// Opens an existing file and reads its entire content.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileBuffer">The file's content.</param>
/// <param name="OutFileSize">The size (in bytes) of the file's content.</param>
///	<remarks>The buffer needs to be released.</remarks>
NTSTATUS CkGetFileBuffer(ANSI_STRING InFilePath, OUT PVOID* OutFileBuffer, OUT SIZE_T* OutFileSize)
{
	UNICODE_STRING FilePath;
	NTSTATUS Status = RtlAnsiStringToUnicodeString(&FilePath, &InFilePath, TRUE);

	if (NT_ERROR(Status))
		return Status;

	Status = CkGetFileBuffer(FilePath, OutFileBuffer, OutFileSize);
	RtlFreeUnicodeString(&FilePath);
	return Status;
}

/// <summary>
/// Opens an existing file and reads its entire content.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileBuffer">The file's content.</param>
/// <param name="OutFileSize">The size (in bytes) of the file's content.</param>
///	<remarks>The buffer needs to be released.</remarks>
NTSTATUS CkGetFileBuffer(UNICODE_STRING InFilePath, OUT PVOID* OutFileBuffer, OUT SIZE_T* OutFileSize)
{
	NTSTATUS Status;

	// 
	// Verify the passed parameters.
	// 

	if (InFilePath.Buffer == nullptr)
		return STATUS_INVALID_PARAMETER_1;
	
	if (OutFileBuffer == nullptr)
		return STATUS_INVALID_PARAMETER_2;
	
	if (OutFileSize == nullptr)
		return STATUS_INVALID_PARAMETER_3;

	// 
	// Initialize the object attributes.
	// 

	OBJECT_ATTRIBUTES ObjectAttributes;
	InitializeObjectAttributes(&ObjectAttributes, &InFilePath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

	// 
	// Open a handle to the file.
	// 
	
	HANDLE FileHandle = nullptr;
	IO_STATUS_BLOCK IoStatusBlock = { };
	
	if (NT_ERROR(Status = ZwCreateFile(&FileHandle, FILE_GENERIC_READ, &ObjectAttributes, &IoStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ, FILE_OPEN, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0)))
		return Status;

	// 
	// Query basic information about the file.
	// 
	
	FILE_STANDARD_INFORMATION FileInformation = { };

	if (NT_ERROR(Status = ZwQueryInformationFile(FileHandle, &IoStatusBlock, &FileInformation, sizeof(FILE_STANDARD_INFORMATION), FileStandardInformation)))
	{
		ZwClose(FileHandle);
		return Status;
	}

	// 
	// Try to allocate memory to store the file's content.
	// 

	auto* FileBuffer = CkAllocatePool(NonPagedPoolNx, (SIZE_T) FileInformation.EndOfFile.QuadPart);

	if (FileBuffer == nullptr)
	{
		ZwClose(FileHandle);
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	// 
	// Read the file's content.
	// 

	LARGE_INTEGER ByteOffset;
	ByteOffset.QuadPart = 0;
	
	if (NT_ERROR(Status = ZwReadFile(FileHandle, NULL, NULL, NULL, &IoStatusBlock, FileBuffer, (ULONG) FileInformation.EndOfFile.QuadPart, &ByteOffset, NULL)))
	{
		CkFreePool(FileBuffer);
		ZwClose(FileHandle);
		return Status;
	}

	ZwClose(FileHandle);

	// 
	// Return the result.
	// 

	*OutFileBuffer = FileBuffer;
	*OutFileSize = (SIZE_T) FileInformation.EndOfFile.QuadPart;
	return STATUS_SUCCESS;
}

/// <summary>
/// Opens an existing file and reads its entire content.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileBuffer">The file's content.</param>
/// <param name="OutFileSize">The size (in bytes) of the file's content.</param>
///	<remarks>The buffer needs to be released.</remarks>
NTSTATUS CkGetFileBuffer(CONST CHAR* InFilePath, OUT PVOID* OutFileBuffer, OUT SIZE_T* OutFileSize)
{
	ANSI_STRING FilePath;
	RtlInitAnsiString(&FilePath, InFilePath);
	return CkGetFileBuffer(FilePath, OutFileBuffer, OutFileSize);
}

/// <summary>
/// Opens an existing file and reads its entire content.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileBuffer">The file's content.</param>
/// <param name="OutFileSize">The size (in bytes) of the file's content.</param>
///	<remarks>The buffer needs to be released.</remarks>
NTSTATUS CkGetFileBuffer(CONST WCHAR* InFilePath, OUT PVOID* OutFileBuffer, OUT SIZE_T* OutFileSize)
{
	UNICODE_STRING FilePath;
	RtlInitUnicodeString(&FilePath, InFilePath);
	return CkGetFileBuffer(FilePath, OutFileBuffer, OutFileSize);
}

/// <summary>
/// Opens an existing file and returns the total length of its content.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileSize">The size (in bytes) of the file's content.</param>
NTSTATUS CkGetFileSize(ANSI_STRING InFilePath, OUT SIZE_T* OutFileSize)
{
	UNICODE_STRING FilePath;
	NTSTATUS Status = RtlAnsiStringToUnicodeString(&FilePath, &InFilePath, TRUE);

	if (NT_ERROR(Status))
		return Status;

	Status = CkGetFileSize(FilePath, OutFileSize);
	RtlFreeUnicodeString(&FilePath);
	return Status;
}

/// <summary>
/// Opens an existing file and returns the total length of its content.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileSize">The size (in bytes) of the file's content.</param>
NTSTATUS CkGetFileSize(UNICODE_STRING InFilePath, OUT SIZE_T* OutFileSize)
{
	NTSTATUS Status;

	// 
	// Verify the passed parameters.
	// 

	if (InFilePath.Buffer == nullptr)
		return STATUS_INVALID_PARAMETER_1;
	
	if (OutFileSize == nullptr)
		return STATUS_INVALID_PARAMETER_2;

	// 
	// Initialize the object attributes.
	// 

	OBJECT_ATTRIBUTES ObjectAttributes;
	InitializeObjectAttributes(&ObjectAttributes, &InFilePath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

	// 
	// Open a handle to the file.
	// 
	
	HANDLE FileHandle = nullptr;
	IO_STATUS_BLOCK IoStatusBlock = { };
	
	if (NT_ERROR(Status = ZwCreateFile(&FileHandle, FILE_GENERIC_READ, &ObjectAttributes, &IoStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_OPEN, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0)))
		return Status;

	// 
	// Query basic information about the file.
	// 
	
	FILE_STANDARD_INFORMATION FileInformation = { };

	if (NT_ERROR(Status = ZwQueryInformationFile(FileHandle, &IoStatusBlock, &FileInformation, sizeof(FILE_STANDARD_INFORMATION), FileStandardInformation)))
	{
		ZwClose(FileHandle);
		return Status;
	}

	ZwClose(FileHandle);

	// 
	// Return the result.
	// 

	*OutFileSize = (SIZE_T) FileInformation.EndOfFile.QuadPart;
	return STATUS_SUCCESS;
}

/// <summary>
/// Opens an existing file and returns the total length of its content.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileSize">The size (in bytes) of the file's content.</param>
NTSTATUS CkGetFileSize(CONST CHAR* InFilePath, OUT SIZE_T* OutFileSize)
{
	ANSI_STRING FilePath;
	RtlInitAnsiString(&FilePath, InFilePath);
	return CkGetFileSize(FilePath, OutFileSize);
}

/// <summary>
/// Opens an existing file and returns the total length of its content.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileSize">The size (in bytes) of the file's content.</param>
NTSTATUS CkGetFileSize(CONST WCHAR* InFilePath, OUT SIZE_T* OutFileSize)
{
	UNICODE_STRING FilePath;
	RtlInitUnicodeString(&FilePath, InFilePath);
	return CkGetFileSize(FilePath, OutFileSize);
}
