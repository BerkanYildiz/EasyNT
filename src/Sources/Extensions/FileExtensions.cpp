#include "../../Headers/EasyNT.h"

/// <summary>
/// Reads and return the content of a file stored on disk.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileBuffer">The file's content.</param>
/// <param name="OutFileSize">The size (in bytes) of the file's content.</param>
NTSTATUS CkGetFileBuffer(UNICODE_STRING InFilePath, OUT PVOID* OutFileBuffer, OUT SIZE_T* OutFileSize)
{
	NTSTATUS Status;

	// 
	// Verify the passed parameters.
	// 

	if (InFilePath.Buffer == NULL)
		return STATUS_INVALID_PARAMETER_1;
	
	if (OutFileBuffer == NULL)
		return STATUS_INVALID_PARAMETER_2;
	
	if (OutFileSize == NULL)
		return STATUS_INVALID_PARAMETER_3;

	// 
	// Initialize the object attributes.
	// 

	OBJECT_ATTRIBUTES ObjectAttributes;
	InitializeObjectAttributes(&ObjectAttributes, &InFilePath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

	// 
	// Open a handle to the file.
	// 
	
	HANDLE FileHandle = NULL;
	IO_STATUS_BLOCK IoStatusBlock = { };
	
	if (!NT_SUCCESS(Status = ZwCreateFile(&FileHandle, FILE_GENERIC_READ, &ObjectAttributes, &IoStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ, FILE_OPEN, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0)))
		return Status;

	// 
	// Query basic information about the file.
	// 
	
	FILE_STANDARD_INFORMATION FileInformation = { };

	if (!NT_SUCCESS(Status = ZwQueryInformationFile(FileHandle, &IoStatusBlock, &FileInformation, sizeof(FILE_STANDARD_INFORMATION), FileStandardInformation)))
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
	
	if (!NT_SUCCESS(Status = ZwReadFile(FileHandle, NULL, NULL, NULL, &IoStatusBlock, FileBuffer, (ULONG) FileInformation.EndOfFile.QuadPart, &ByteOffset, NULL)))
	{
		CkFreePool(FileBuffer);
		ZwClose(FileHandle);
		return Status;
	}

	// 
	// Return the result.
	// 

	if (OutFileBuffer != nullptr)
		*OutFileBuffer = FileBuffer;
	else
		CkFreePool(FileBuffer);

	if (OutFileSize != nullptr)
		*OutFileSize = (SIZE_T) FileInformation.EndOfFile.QuadPart;

	ZwClose(FileHandle);
	return STATUS_SUCCESS;
}

/// <summary>
/// Reads and return the content of a file stored on disk.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileBuffer">The file's content.</param>
/// <param name="OutFileSize">The size (in bytes) of the file's content.</param>
NTSTATUS CkGetFileBuffer(CONST WCHAR* InFilePath, OUT PVOID* OutFileBuffer, OUT SIZE_T* OutFileSize)
{
	UNICODE_STRING FilePath;
	RtlInitUnicodeString(&FilePath, InFilePath);
	return CkGetFileBuffer(FilePath, OutFileBuffer, OutFileSize);
}

/// <summary>
/// Reads and return the content of a file stored on disk.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileBuffer">The file's content.</param>
/// <param name="OutFileSize">The size (in bytes) of the file's content.</param>
NTSTATUS CkGetFileBuffer(CONST CHAR* InFilePath, OUT PVOID* OutFileBuffer, OUT SIZE_T* OutFileSize)
{
	if (InFilePath == nullptr)
		return STATUS_INVALID_PARAMETER_1;
	
	ANSI_STRING FilePathToConvert;
	RtlInitAnsiString(&FilePathToConvert, InFilePath);

	UNICODE_STRING FilePath;
	NTSTATUS Status = RtlAnsiStringToUnicodeString(&FilePath, &FilePathToConvert, TRUE);

	if (NT_ERROR(Status))
		return Status;
	
	Status = CkGetFileBuffer(FilePath, OutFileBuffer, OutFileSize);
	RtlFreeUnicodeString(&FilePath);
	return Status;
}

/// <summary>
/// Retrieves the size (in bytes) of a file stored on disk.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileSize">The size of the file in bytes.</param>
NTSTATUS CkGetFileSize(UNICODE_STRING InFilePath, OUT SIZE_T* OutFileSize)
{
	NTSTATUS Status;

	// 
	// Verify the passed parameters.
	// 

	if (InFilePath.Buffer == NULL)
		return STATUS_INVALID_PARAMETER_1;
	
	if (OutFileSize == NULL)
		return STATUS_INVALID_PARAMETER_2;

	// 
	// Initialize the object attributes.
	// 

	OBJECT_ATTRIBUTES ObjectAttributes;
	InitializeObjectAttributes(&ObjectAttributes, &InFilePath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

	// 
	// Open a handle to the file.
	// 
	
	HANDLE FileHandle = NULL;
	IO_STATUS_BLOCK IoStatusBlock = { };
	
	if (!NT_SUCCESS(Status = ZwCreateFile(&FileHandle, FILE_GENERIC_READ, &ObjectAttributes, &IoStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_OPEN, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0)))
		return Status;

	// 
	// Query basic information about the file.
	// 
	
	FILE_STANDARD_INFORMATION FileInformation = { };

	if (!NT_SUCCESS(Status = ZwQueryInformationFile(FileHandle, &IoStatusBlock, &FileInformation, sizeof(FILE_STANDARD_INFORMATION), FileStandardInformation)))
	{
		ZwClose(FileHandle);
		return Status;
	}

	// 
	// Return the result.
	// 

	if (OutFileSize != nullptr)
		*OutFileSize = (SIZE_T) FileInformation.EndOfFile.QuadPart;
	
	ZwClose(FileHandle);
	return STATUS_SUCCESS;
}

/// <summary>
/// Retrieves the size (in bytes) of a file stored on disk.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileSize">The size of the file in bytes.</param>
NTSTATUS CkGetFileSize(CONST WCHAR* InFilePath, OUT SIZE_T* OutFileSize)
{
	UNICODE_STRING FilePath;
	RtlInitUnicodeString(&FilePath, InFilePath);
	return CkGetFileSize(FilePath, OutFileSize);
}

/// <summary>
/// Retrieves the size (in bytes) of a file stored on disk.
/// </summary>
/// <param name="InFilePath">The path of the file.</param>
/// <param name="OutFileSize">The size of the file in bytes.</param>
NTSTATUS CkGetFileSize(CONST CHAR* InFilePath, OUT SIZE_T* OutFileSize)
{
	if (InFilePath == nullptr)
		return STATUS_INVALID_PARAMETER_1;
	
	ANSI_STRING FilePathToConvert;
	RtlInitAnsiString(&FilePathToConvert, InFilePath);

	UNICODE_STRING FilePath;
	NTSTATUS Status = RtlAnsiStringToUnicodeString(&FilePath, &FilePathToConvert, TRUE);

	if (NT_ERROR(Status))
		return Status;
	
	Status = CkGetFileSize(FilePath, OutFileSize);
	RtlFreeUnicodeString(&FilePath);
	return Status;
}
