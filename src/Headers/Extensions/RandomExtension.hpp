#pragma once

enum class ECharsetFormat : int
{
	None			= 0x00000000,
	
	// 
	// Formats.
	// 
	
	Alphabet		= 0x00000001,
	Numerical		= 0x00000002,
	Hexadecimal		= 0x00000004,

	// 
	// Modifiers.
	// 
	
	AlphaNumerical	= Alphabet | Numerical,
	OnlyUppercase	= 0x00000008,
	OnlyLowercase	= 0x00000010,
};

DEFINE_ENUM_FLAG_OPERATORS(ECharsetFormat);

/// <summary>
/// Generates a random byte value from 0x00 to a given number excluded.
/// </summary>
/// <param name="MaxValueExcluded">The maximum value (excluded) to be returned.</param>
UINT8 RtlRandomByte(UINT8 MaxValueExcluded);

/// <summary>
/// Generates a random byte value from 0x00 to 0xFF excluded.
/// </summary>
UINT8 RtlRandomByte();

/// <summary>
/// Generates a random short value from 0 to a given number excluded.
/// </summary>
/// <param name="MaxValueExcluded">The maximum value (excluded) to be returned.</param>
UINT16 RtlRandomShort(UINT16 MaxValueExcluded);

/// <summary>
/// Generates a random short value from 0 to 65535 excluded.
/// </summary>
UINT16 RtlRandomShort();

/// <summary>
/// Generates a random integer value from 0 to a given number excluded.
/// </summary>
/// <param name="MaxValueExcluded">The maximum value (excluded) to be returned.</param>
UINT32 RtlRandomInteger(UINT32 MaxValueExcluded);

/// <summary>
/// Generates a random integer value from 0 to 4,294,967,295 excluded.
/// </summary>
UINT32 RtlRandomInteger();

/// <summary>
/// Generates a random boolean value, which can be either false or true.
/// </summary>
BOOLEAN RtlRandomBoolean();

/// <summary>
/// Generates a random character from the given string.
/// </summary>
/// <param name="InValidCharacters">The charset to select characters from.</param>
/// <param name="InNumberOfValidCharacters">The number of elements in the charset to select characters from.</param>
CHAR RtlRandomChar(CONST CHAR* InValidCharacters, SIZE_T InNumberOfValidCharacters);

/// <summary>
/// Generates a random unicode character from the given string.
/// </summary>
/// <param name="InValidCharacters">The charset to select characters from.</param>
/// <param name="InNumberOfValidCharacters">The number of elements in the charset to select characters from.</param>
WCHAR RtlRandomChar(CONST WCHAR* InValidCharacters, SIZE_T InNumberOfValidCharacters);

/// <summary>
/// Generates a random string using the letters from the given string.
/// </summary>
/// <param name="InBuffer">The output buffer.</param>
/// <param name="InNumberOfElements">The number of elements in the output buffer.</param>
/// <param name="InValidCharacters">The charset to select characters from.</param>
/// <param name="InNumberOfValidCharacters">The number of elements in the charset to select characters from.</param>
void RtlRandomString(CHAR* InBuffer, SIZE_T InNumberOfElements, CONST CHAR* InValidCharacters, SIZE_T InNumberOfValidCharacters);

/// <summary>
/// Generates a random string using the letters from the given string.
/// </summary>
/// <param name="InBuffer">The output buffer.</param>
/// <param name="InNumberOfElements">The number of elements in the output buffer.</param>
/// <param name="InValidCharacters">The charset to select characters from.</param>
/// <param name="InNumberOfValidCharacters">The number of elements in the charset to select characters from.</param>
void RtlRandomString(WCHAR* InBuffer, SIZE_T InNumberOfElements, CONST WCHAR* InValidCharacters, SIZE_T InNumberOfValidCharacters);

/// <summary>
/// Generates a random string using the characters from the given charset.
/// </summary>
/// <param name="InBuffer">The output buffer.</param>
/// <param name="InNumberOfElements">The number of elements in the output buffer.</param>
/// <param name="InCharsetFormat">The charset to select characters from.</param>
void RtlRandomString(CHAR* InBuffer, SIZE_T InNumberOfElements, ECharsetFormat InCharsetFormat);

/// <summary>
/// Generates a random unicode string using the characters from the given charset.
/// </summary>
void RtlRandomString(WCHAR* InBuffer, SIZE_T InNumberOfElements, ECharsetFormat InCharsetFormat);
