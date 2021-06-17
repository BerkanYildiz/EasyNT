#pragma once

/// <summary>
/// Blocks the current thread for a certain amount of time.
/// </summary>
/// <param name="InMicroseconds">The duration in microseconds.</param>
NTSTATUS RtlThreadSleepInMicroseconds(LONGLONG InMicroseconds);

/// <summary>
/// Blocks the current thread for a certain amount of time.
/// </summary>
/// <param name="InMilliseconds">The duration in milliseconds.</param>
NTSTATUS RtlThreadSleepInMilliseconds(LONGLONG InMilliseconds);

/// <summary>
/// Blocks the current thread for a certain amount of time.
/// </summary>
/// <param name="InSeconds">The duration in seconds.</param>
NTSTATUS RtlThreadSleepInSeconds(LONGLONG InSeconds);
