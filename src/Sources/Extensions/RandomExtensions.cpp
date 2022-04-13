#include "../../Headers/EasyNT.h"

/// <summary>
/// Generates a random bytes array of the given length.
/// </summary>
void RtlRandomBytes(void* OutBuffer, SIZE_T InNumberOfBytes)
{
	auto* Buffer = (UINT8*) OutBuffer;

	for (SIZE_T I = 0; I < InNumberOfBytes; I++)
		Buffer[I] = RtlRandomByte();
}

/// <summary>
/// Generates a random byte value from 0x00 to a given number excluded.
/// </summary>
/// <param name="MaxValueExcluded">The maximum value (excluded) to be returned.</param>
UINT8 RtlRandomByte(UINT8 MaxValueExcluded)
{
	LARGE_INTEGER PerfCounter = KeQueryPerformanceCounter(nullptr);
	return RtlRandomEx(&PerfCounter.LowPart) % MaxValueExcluded;
}

/// <summary>
/// Generates a random byte value from 0x00 to 0xFF excluded.
/// </summary>
UINT8 RtlRandomByte()
{
	return RtlRandomByte(0xFF);
}

/// <summary>
/// Generates a random short value from 0 to a given number excluded.
/// </summary>
/// <param name="MaxValueExcluded">The maximum value (excluded) to be returned.</param>
UINT16 RtlRandomShort(UINT16 MaxValueExcluded)
{
	LARGE_INTEGER PerfCounter = KeQueryPerformanceCounter(nullptr);
	return RtlRandomEx(&PerfCounter.LowPart) % MaxValueExcluded;
}

/// <summary>
/// Generates a random short value from 0 to 65535 excluded.
/// </summary>
UINT16 RtlRandomShort()
{
	return RtlRandomShort(0xFFFF);
}

/// <summary>
/// Generates a random integer value from 0 to a given number excluded.
/// </summary>
/// <param name="MaxValueExcluded">The maximum value (excluded) to be returned.</param>
UINT32 RtlRandomInteger(UINT32 MaxValueExcluded)
{
	LARGE_INTEGER PerfCounter = KeQueryPerformanceCounter(nullptr);
	return RtlRandomEx(&PerfCounter.LowPart) % MaxValueExcluded;
}

/// <summary>
/// Generates a random integer value from 0 to 4,294,967,295 excluded.
/// </summary>
UINT32 RtlRandomInteger()
{
	return RtlRandomInteger(0xFFFFFFFF);
}

/// <summary>
/// Generates a random boolean value, which can be either false or true.
/// </summary>
BOOLEAN RtlRandomBoolean()
{
	return RtlRandomInteger() % 2 == 0;
}

/// <summary>
/// Generates a random character from the given string.
/// </summary>
/// <param name="InValidCharacters">The charset to select characters from.</param>
/// <param name="InNumberOfValidCharacters">The number of elements in the charset to select characters from.</param>
CHAR RtlRandomChar(CONST CHAR* InValidCharacters, SIZE_T InNumberOfValidCharacters)
{
	return InValidCharacters[RtlRandomInteger(InNumberOfValidCharacters)];
}

/// <summary>
/// Generates a random unicode character from the given string.
/// </summary>
/// <param name="InValidCharacters">The charset to select characters from.</param>
/// <param name="InNumberOfValidCharacters">The number of elements in the charset to select characters from.</param>
WCHAR RtlRandomChar(CONST WCHAR* InValidCharacters, SIZE_T InNumberOfValidCharacters)
{
	return InValidCharacters[RtlRandomInteger(InNumberOfValidCharacters)];
}

/// <summary>
/// Generates a random string using the letters from the given string.
/// </summary>
/// <param name="InBuffer">The output buffer.</param>
/// <param name="InNumberOfElements">The number of elements in the output buffer.</param>
/// <param name="InValidCharacters">The charset to select characters from.</param>
/// <param name="InNumberOfValidCharacters">The number of elements in the charset to select characters from.</param>
void RtlRandomString(CHAR* InBuffer, SIZE_T InNumberOfElements, CONST CHAR* InValidCharacters, SIZE_T InNumberOfValidCharacters)
{
	for (SIZE_T I = 0; I < InNumberOfElements; I++)
		InBuffer[I] = RtlRandomChar(InValidCharacters, InNumberOfValidCharacters);
}

/// <summary>
/// Generates a random string using the letters from the given string.
/// </summary>
/// <param name="InBuffer">The output buffer.</param>
/// <param name="InNumberOfElements">The number of elements in the output buffer.</param>
/// <param name="InValidCharacters">The charset to select characters from.</param>
/// <param name="InNumberOfValidCharacters">The number of elements in the charset to select characters from.</param>
void RtlRandomString(WCHAR* InBuffer, SIZE_T InNumberOfElements, CONST WCHAR* InValidCharacters, SIZE_T InNumberOfValidCharacters)
{
	for (SIZE_T I = 0; I < InNumberOfElements; I++)
		InBuffer[I] = RtlRandomChar(InValidCharacters, InNumberOfValidCharacters);
}

/// <summary>
/// Generates a random string using the characters from the given charset.
/// </summary>
/// <param name="InBuffer">The output buffer.</param>
/// <param name="InNumberOfElements">The number of elements in the output buffer.</param>
/// <param name="InCharsetFormat">The charset to select characters from.</param>
void RtlRandomString(CHAR* InBuffer, SIZE_T InNumberOfElements, ECharsetFormat InCharsetFormat)
{
	if ((InCharsetFormat & ECharsetFormat::AlphaNumerical) != ECharsetFormat::None)
	{
		if ((InCharsetFormat & ECharsetFormat::OnlyUppercase) != ECharsetFormat::None)
			return RtlRandomString(InBuffer, InNumberOfElements, "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", 36);
		
		if ((InCharsetFormat & ECharsetFormat::OnlyLowercase) != ECharsetFormat::None)
			return RtlRandomString(InBuffer, InNumberOfElements, "abcdefghijklmnopqrstuvwxyz0123456789", 36);
		
		return RtlRandomString(InBuffer, InNumberOfElements, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", 62);
	}
	else if ((InCharsetFormat & ECharsetFormat::Alphabet) != ECharsetFormat::None)
	{
		if ((InCharsetFormat & ECharsetFormat::OnlyUppercase) != ECharsetFormat::None)
			return RtlRandomString(InBuffer, InNumberOfElements, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26);
		
		if ((InCharsetFormat & ECharsetFormat::OnlyLowercase) != ECharsetFormat::None)
			return RtlRandomString(InBuffer, InNumberOfElements, "abcdefghijklmnopqrstuvwxyz", 26);
		
		return RtlRandomString(InBuffer, InNumberOfElements, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 52);
	}
	else if ((InCharsetFormat & ECharsetFormat::Numerical) != ECharsetFormat::None)
	{
		return RtlRandomString(InBuffer, InNumberOfElements, "0123456789", 10);
	}
	else if ((InCharsetFormat & ECharsetFormat::Hexadecimal) != ECharsetFormat::None)
	{
		if ((InCharsetFormat & ECharsetFormat::OnlyLowercase) != ECharsetFormat::None)
			return RtlRandomString(InBuffer, InNumberOfElements, "abcdef0123456789", 16);
		
		return RtlRandomString(InBuffer, InNumberOfElements, "ABCDEF0123456789", 16);
	}
}

/// <summary>
/// Generates a random unicode string using the characters from the given charset.
/// </summary>
void RtlRandomString(WCHAR* InBuffer, SIZE_T InNumberOfElements, ECharsetFormat InCharsetFormat)
{
	if ((InCharsetFormat & ECharsetFormat::AlphaNumerical) != ECharsetFormat::None)
	{
		if ((InCharsetFormat & ECharsetFormat::OnlyUppercase) != ECharsetFormat::None)
			return RtlRandomString(InBuffer, InNumberOfElements, L"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", 36);
		
		if ((InCharsetFormat & ECharsetFormat::OnlyLowercase) != ECharsetFormat::None)
			return RtlRandomString(InBuffer, InNumberOfElements, L"abcdefghijklmnopqrstuvwxyz0123456789", 36);
		
		return RtlRandomString(InBuffer, InNumberOfElements, L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", 62);
	}
	else if ((InCharsetFormat & ECharsetFormat::Alphabet) != ECharsetFormat::None)
	{
		if ((InCharsetFormat & ECharsetFormat::OnlyUppercase) != ECharsetFormat::None)
			return RtlRandomString(InBuffer, InNumberOfElements, L"ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26);
		
		if ((InCharsetFormat & ECharsetFormat::OnlyLowercase) != ECharsetFormat::None)
			return RtlRandomString(InBuffer, InNumberOfElements, L"abcdefghijklmnopqrstuvwxyz", 26);
		
		return RtlRandomString(InBuffer, InNumberOfElements, L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 52);
	}
	else if ((InCharsetFormat & ECharsetFormat::Numerical) != ECharsetFormat::None)
	{
		return RtlRandomString(InBuffer, InNumberOfElements, L"0123456789", 10);
	}
	else if ((InCharsetFormat & ECharsetFormat::Hexadecimal) != ECharsetFormat::None)
	{
		if ((InCharsetFormat & ECharsetFormat::OnlyLowercase) != ECharsetFormat::None)
			return RtlRandomString(InBuffer, InNumberOfElements, L"abcdef0123456789", 16);
		
		return RtlRandomString(InBuffer, InNumberOfElements, L"ABCDEF0123456789", 16);
	}
}
