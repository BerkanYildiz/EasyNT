#pragma once

#define OS_PLATFORM_WINDOWS_11 11
#define OS_PLATFORM_WINDOWS_10 10
#define OS_PLATFORM_WINDOWS_7 7

/// <summary>
/// Gets the current major version number for this operating system.
/// </summary>
ULONG RtlGetVersionMajorNumber();

/// <summary>
/// Gets the current minor version number for this operating system.
/// </summary>
ULONG RtlGetVersionMinorNumber();

/// <summary>
/// Gets the current build version number for this operating system.
/// </summary>
ULONG RtlGetVersionBuildNumber();

/// <summary>
/// Gets a value indicating whether the current platform is the one passed as argument.
/// </summary>
BOOLEAN RtlCurrentPlatformIs(BYTE InPlatform);
