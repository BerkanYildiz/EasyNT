#pragma once

// 
// Time Offset.
// 

/// <summary>
/// Converts a time offset to be relative to the current time.
/// </summary>
LONGLONG RtlRelativeTime(LONGLONG InTime);

/// <summary>
/// Converts a time offset to be absolute.
/// </summary>
LONGLONG RtlAbsoluteTime(LONGLONG InTime);

/// <summary>
/// Converts a time offset to be relative to the current time.
/// </summary>
LARGE_INTEGER RtlRelativeTime(LARGE_INTEGER InTime);

/// <summary>
/// Converts a time offset to be absolute.
/// </summary>
LARGE_INTEGER RtlAbsoluteTime(LARGE_INTEGER InTime);

// 
// Time Resolution.
// 

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

/// <summary>
/// Gets the system time in minutes.
/// </summary>
ULONGLONG KeQuerySystemTimePreciseInMinutes();

// 
// Time Conversion.
// 

/// <summary>
/// Converts a value in minutes to seconds.
/// </summary>
ULONGLONG RtlMinutesToSeconds(ULONGLONG InMinutes);

/// <summary>
/// Converts a value in seconds to milliseconds.
/// </summary>
ULONGLONG RtlSecondsToMilliseconds(ULONGLONG InSeconds);

/// <summary>
/// Converts a value in milliseconds to microseconds.
/// </summary>
ULONGLONG RtlMillisecondsToMicroseconds(ULONGLONG InMilliseconds);

/// <summary>
/// Converts a value in microseconds to 100-nanoseconds interval.
/// </summary>
ULONGLONG RtlMicrosecondsTo100NanosecondsInterval(ULONGLONG InMicroseconds);

/// <summary>
/// Converts a value in 100-nanoseconds interval to microseconds.
/// </summary>
ULONGLONG Rtl100NanosecondsIntervalToMicroseconds(ULONGLONG In100NanosecondsInterval);

/// <summary>
/// Converts a value in microseconds to milliseconds.
/// </summary>
ULONGLONG RtlMicrosecondsToMilliseconds(ULONGLONG InMicroseconds);

/// <summary>
/// Converts a value in milliseconds to seconds.
/// </summary>
ULONGLONG RtlMillisecondsToSeconds(ULONGLONG InMilliseconds);

/// <summary>
/// Converts a value in seconds to minutes.
/// </summary>
ULONGLONG RtlSecondsToMinutes(ULONGLONG InSeconds);

/// <summary>
/// Converts a value in 100-nanoseconds interval to milliseconds.
/// </summary>
ULONGLONG Rtl100NanosecondsIntervalToMilliseconds(ULONGLONG In100NanosecondsInterval);

/// <summary>
/// Converts a value in 100-nanoseconds interval to seconds.
/// </summary>
ULONGLONG Rtl100NanosecondsIntervalToSeconds(ULONGLONG In100NanosecondsInterval);

/// <summary>
/// Converts a value in 100-nanoseconds interval to minutes.
/// </summary>
ULONGLONG Rtl100NanosecondsIntervalToMinutes(ULONGLONG In100NanosecondsInterval);

/// <summary>
/// Converts a value in milliseconds to 100-nanoseconds interval.
/// </summary>
ULONGLONG RtlMillisecondsTo100NanosecondsInterval(ULONGLONG InMilliseconds);

/// <summary>
/// Converts a value in seconds to 100-nanoseconds interval.
/// </summary>
ULONGLONG RtlSecondsTo100NanosecondsInterval(ULONGLONG InSeconds);

/// <summary>
/// Converts a value in minutes to 100-nanoseconds interval.
/// </summary>
ULONGLONG RtlMinutesTo100NanosecondsInterval(ULONGLONG InMinutes);
