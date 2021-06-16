#pragma once

/// <summary>
/// Gets the system time in microseconds.
/// </summary>
ULONGLONG KeQuerySystemTimePreciseInMicroseconds();

/// <summary>
/// Gets the system time in milliseconds.
/// </summary>
ULONGLONG KeQuerySystemTimePreciseInMilliseconds();

/// <summary>
/// Gets the system time in seconds.
/// </summary>
ULONGLONG KeQuerySystemTimePreciseInSeconds();
