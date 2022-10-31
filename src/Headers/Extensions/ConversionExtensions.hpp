#pragma once

/// <summary>
/// Converts a hexadecimal string to a byte.
/// </summary>
/// <param name="InHexadecimalString">The hexadecimal string.</param>
UINT8 CkHexadecimalStringToByte(CONST CHAR* InHexadecimalString);

/// <summary>
/// Converts a hexadecimal unicode string to a byte.
/// </summary>
/// <param name="InHexadecimalString">The hexadecimal unicode string.</param>
UINT8 CkHexadecimalStringToByte(CONST WCHAR* InHexadecimalString);