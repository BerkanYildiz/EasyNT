#include "../../Headers/EasyNT.h"

/// <summary>
/// Blocks the current thread for a certain amount of time.
/// </summary>
/// <param name="InMilliseconds">The duration in milliseconds.</param>
NTSTATUS RtlThreadSleepInMilliseconds(LONGLONG InMilliseconds)
{
	LARGE_INTEGER WaitTime;
	WaitTime.QuadPart = -1 * (10000 * InMilliseconds);
	return KeDelayExecutionThread(ExGetPreviousMode(), ExGetPreviousMode() == UserMode, &WaitTime);
}

/// <summary>
/// Blocks the current thread for a certain amount of time.
/// </summary>
/// <param name="InSeconds">The duration in seconds.</param>
NTSTATUS RtlThreadSleepInSeconds(LONGLONG InSeconds)
{
	return RtlThreadSleepInMilliseconds(InSeconds * 1000);
}

/// <summary>
/// Blocks the current thread for a certain amount of time.
/// </summary>
/// <param name="InMinutes">The duration in minutes.</param>
NTSTATUS RtlThreadSleepInMinutes(LONGLONG InMinutes)
{
	return RtlThreadSleepInSeconds(InMinutes * 60);
}
