#pragma once

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
