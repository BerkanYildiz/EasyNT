#include "../../Headers/EasyNT.h"

/// <summary>
/// Gets the current major version number for this operating system.
/// </summary>
ULONG RtlGetVersionMajorNumber()
{
	RTL_OSVERSIONINFOW VersionInfo = { };
	VersionInfo.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW);
	RtlGetVersion(&VersionInfo);
	return VersionInfo.dwMajorVersion;
}

/// <summary>
/// Gets the current minor version number for this operating system.
/// </summary>
ULONG RtlGetVersionMinorNumber()
{
	RTL_OSVERSIONINFOW VersionInfo = { };
	VersionInfo.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW);
	RtlGetVersion(&VersionInfo);
	return VersionInfo.dwMinorVersion;
}

/// <summary>
/// Gets the current build version number for this operating system.
/// </summary>
ULONG RtlGetVersionBuildNumber()
{
	RTL_OSVERSIONINFOW VersionInfo = { };
	VersionInfo.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW);
	RtlGetVersion(&VersionInfo);
	return VersionInfo.dwBuildNumber;
}
