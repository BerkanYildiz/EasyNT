#include "../../Headers/EasyNT.h"

/// <summary>
/// Blocks the current thread for a certain amount of time.
/// </summary>
/// <param name="InMicroseconds">The duration in microseconds.</param>
NTSTATUS RtlThreadSleepInMicroseconds(LONGLONG InMicroseconds)
{
	LARGE_INTEGER WaitTime;
	WaitTime.QuadPart = RtlMicrosecondsTo100NanosecondsInterval(InMicroseconds);
	WaitTime.QuadPart = RtlRelativeTime(WaitTime.QuadPart);
	
	return KeDelayExecutionThread(ExGetPreviousMode(), ExGetPreviousMode() == UserMode, &WaitTime);
}

/// <summary>
/// Blocks the current thread for a certain amount of time.
/// </summary>
/// <param name="InMilliseconds">The duration in milliseconds.</param>
NTSTATUS RtlThreadSleepInMilliseconds(LONGLONG InMilliseconds)
{
	LARGE_INTEGER WaitTime;
	WaitTime.QuadPart = RtlMillisecondsTo100NanosecondsInterval(InMilliseconds);
	WaitTime.QuadPart = RtlRelativeTime(WaitTime.QuadPart);
	
	return KeDelayExecutionThread(ExGetPreviousMode(), ExGetPreviousMode() == UserMode, &WaitTime);
}

/// <summary>
/// Blocks the current thread for a certain amount of time.
/// </summary>
/// <param name="InSeconds">The duration in seconds.</param>
NTSTATUS RtlThreadSleepInSeconds(LONGLONG InSeconds)
{
	LARGE_INTEGER WaitTime;
	WaitTime.QuadPart = RtlSecondsTo100NanosecondsInterval(InSeconds);
	WaitTime.QuadPart = RtlRelativeTime(WaitTime.QuadPart);

	return KeDelayExecutionThread(ExGetPreviousMode(), ExGetPreviousMode() == UserMode, &WaitTime);
}
