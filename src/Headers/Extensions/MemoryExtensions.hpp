#pragma once

/// <summary>
/// Maps physical memory and read its content.
/// </summary>
/// <param name="InPhysicalAddress">The physical address.</param>
/// <param name="OutBuffer">The buffer.</param>
/// <param name="InNumberOfBytes">The number of bytes to read.</param>
NTSTATUS MmReadPhysicalMemory(LARGE_INTEGER InPhysicalAddress, PVOID OutBuffer, SIZE_T InNumberOfBytes);

/// <summary>
/// Maps physical memory and write data to it.
/// </summary>
/// <param name="InPhysicalAddress">The physical address.</param>
/// <param name="InBuffer">The buffer.</param>
/// <param name="InNumberOfBytes">The number of bytes to write.</param>
NTSTATUS MmWritePhysicalMemory(LARGE_INTEGER InPhysicalAddress, PVOID InBuffer, SIZE_T InNumberOfBytes);
