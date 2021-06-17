#include "../../Headers/EasyNT.h"

/// <summary>
/// Converts a time offset to be relative to the current time.
/// </summary>
LONGLONG RtlRelativeTime(LONGLONG InTime)
{
	return -1 * InTime;
}

/// <summary>
/// Converts a time offset to be absolute.
/// </summary>
LONGLONG RtlAbsoluteTime(LONGLONG InTime)
{
	return InTime;
}

/// <summary>
/// Converts a time offset to be relative to the current time.
/// </summary>
LARGE_INTEGER RtlRelativeTime(LARGE_INTEGER InTime)
{
	InTime.QuadPart = -1 * InTime.QuadPart;
	return InTime;
}

/// <summary>
/// Converts a time offset to be absolute.
/// </summary>
LARGE_INTEGER RtlAbsoluteTime(LARGE_INTEGER InTime)
{
	return InTime;
}

/// <summary>
/// Gets the system time in microseconds.
/// </summary>
ULONGLONG KeQuerySystemTimePreciseInMicroseconds()
{
	LARGE_INTEGER SystemTime = { };
	KeQuerySystemTimePrecise(&SystemTime);
	return Rtl100NanosecondsIntervalToMicroseconds(SystemTime.QuadPart);
}

/// <summary>
/// Gets the system time in milliseconds.
/// </summary>
ULONGLONG KeQuerySystemTimePreciseInMilliseconds()
{
	LARGE_INTEGER SystemTime = { };
	KeQuerySystemTimePrecise(&SystemTime);
	return Rtl100NanosecondsIntervalToMilliseconds(SystemTime.QuadPart);
}

/// <summary>
/// Gets the system time in seconds.
/// </summary>
ULONGLONG KeQuerySystemTimePreciseInSeconds()
{
	LARGE_INTEGER SystemTime = { };
	KeQuerySystemTimePrecise(&SystemTime);
	return Rtl100NanosecondsIntervalToSeconds(SystemTime.QuadPart);
}

/// <summary>
/// Gets the system time in minutes.
/// </summary>
ULONGLONG KeQuerySystemTimePreciseInMinutes()
{
	LARGE_INTEGER SystemTime = { };
	KeQuerySystemTimePrecise(&SystemTime);
	return Rtl100NanosecondsIntervalToMinutes(SystemTime.QuadPart);
}

/// <summary>
/// Converts a value in minutes to seconds.
/// </summary>
ULONGLONG RtlMinutesToSeconds(ULONGLONG InMinutes)
{
	return InMinutes * 60;
}

/// <summary>
/// Converts a value in seconds to milliseconds.
/// </summary>
ULONGLONG RtlSecondsToMilliseconds(ULONGLONG InSeconds)
{
	return InSeconds * 1000;
}

/// <summary>
/// Converts a value in milliseconds to microseconds.
/// </summary>
ULONGLONG RtlMillisecondsToMicroseconds(ULONGLONG InMilliseconds)
{
	return InMilliseconds * 1000;
}

/// <summary>
/// Converts a value in microseconds to 100-nanoseconds interval.
/// </summary>
ULONGLONG RtlMicrosecondsTo100NanosecondsInterval(ULONGLONG InMicroseconds)
{
	return InMicroseconds * 10;
}

/// <summary>
/// Converts a value in 100-nanoseconds interval to microseconds.
/// </summary>
ULONGLONG Rtl100NanosecondsIntervalToMicroseconds(ULONGLONG In100NanosecondsInterval)
{
	return In100NanosecondsInterval / 10;
}

/// <summary>
/// Converts a value in microseconds to milliseconds.
/// </summary>
ULONGLONG RtlMicrosecondsToMilliseconds(ULONGLONG InMicroseconds)
{
	return InMicroseconds / 1000;
}

/// <summary>
/// Converts a value in milliseconds to seconds.
/// </summary>
ULONGLONG RtlMillisecondsToSeconds(ULONGLONG InMilliseconds)
{
	return InMilliseconds / 1000;
}

/// <summary>
/// Converts a value in seconds to minutes.
/// </summary>
ULONGLONG RtlSecondsToMinutes(ULONGLONG InSeconds)
{
	return InSeconds / 60;
}

/// <summary>
/// Converts a value in 100-nanoseconds interval to milliseconds.
/// </summary>
ULONGLONG Rtl100NanosecondsIntervalToMilliseconds(ULONGLONG In100NanosecondsInterval)
{
	return In100NanosecondsInterval / 10 / 1000;
}

/// <summary>
/// Converts a value in 100-nanoseconds interval to seconds.
/// </summary>
ULONGLONG Rtl100NanosecondsIntervalToSeconds(ULONGLONG In100NanosecondsInterval)
{
	return In100NanosecondsInterval / 10 / 1000 / 1000;
}

/// <summary>
/// Converts a value in 100-nanoseconds interval to minutes.
/// </summary>
ULONGLONG Rtl100NanosecondsIntervalToMinutes(ULONGLONG In100NanosecondsInterval)
{
	return In100NanosecondsInterval / 10 / 1000 / 1000 / 60;
}

/// <summary>
/// Converts a value in milliseconds to 100-nanoseconds interval.
/// </summary>
ULONGLONG RtlMillisecondsTo100NanosecondsInterval(ULONGLONG InMilliseconds)
{
	return InMilliseconds * 10 * 1000;
}

/// <summary>
/// Converts a value in seconds to 100-nanoseconds interval.
/// </summary>
ULONGLONG RtlSecondsTo100NanosecondsInterval(ULONGLONG InSeconds)
{
	return InSeconds * 10 * 1000 * 1000;
}

/// <summary>
/// Converts a value in minutes to 100-nanoseconds interval.
/// </summary>
ULONGLONG RtlMinutesTo100NanosecondsInterval(ULONGLONG InMinutes)
{
	return InMinutes * 10 * 1000 * 1000 * 60;
}
