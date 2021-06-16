#include "../../Headers/EasyNT.h"

/// <summary>
/// Gets the system time in microseconds.
/// </summary>
ULONGLONG KeQuerySystemTimePreciseInMicroseconds()
{
	LARGE_INTEGER SystemTime = { };
	KeQuerySystemTimePrecise(&SystemTime);
	return SystemTime.QuadPart * 1.0e-1;
}

/// <summary>
/// Gets the system time in milliseconds.
/// </summary>
ULONGLONG KeQuerySystemTimePreciseInMilliseconds()
{
	LARGE_INTEGER SystemTime = { };
	KeQuerySystemTimePrecise(&SystemTime);
	return SystemTime.QuadPart * 1.0e-4;
}

/// <summary>
/// Gets the system time in seconds.
/// </summary>
ULONGLONG KeQuerySystemTimePreciseInSeconds()
{
	LARGE_INTEGER SystemTime = { };
	KeQuerySystemTimePrecise(&SystemTime);
	return SystemTime.QuadPart * 1.0e-7;
}
