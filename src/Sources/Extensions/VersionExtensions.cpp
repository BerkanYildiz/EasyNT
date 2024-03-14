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

/// <summary>
/// Gets a value indicating whether the current platform is the one passed as argument.
/// </summary>
BOOLEAN RtlCurrentPlatformIs(BYTE InPlatform)
{
	const auto CurrentBuildNumber = RtlGetVersionBuildNumber();

	switch (InPlatform)
	{
		case OS_PLATFORM_WINDOWS_11:
		{
			return CurrentBuildNumber >= 22000 &&
				   CurrentBuildNumber < 30000;
		}

		case OS_PLATFORM_WINDOWS_10:
		{
			return CurrentBuildNumber >= 10240 &&
				   CurrentBuildNumber <  22000;
		}

		case OS_PLATFORM_WINDOWS_7:
		{
			return CurrentBuildNumber == 7601;
		}

		default:
		{
			return false;
		}
	}
}
