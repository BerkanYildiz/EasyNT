#include "../../Headers/EasyNT.h"

/// <summary>
/// Returns the number of characters in a string.
/// </summary>
/// <param name="InString">The string.</param>
SIZE_T RtlStringLength(CONST CHAR* InString)
{
	if (InString == nullptr)
		return 0;

	return __builtin_strlen(InString);
}

/// <summary>
/// Returns the number of characters in a string.
/// </summary>
/// <param name="InString">The string.</param>
SIZE_T RtlStringLength(CONST WCHAR* InString)
{
	if (InString == nullptr)
		return 0;
	
	return __builtin_wcslen(InString);
}

/// <summary>
/// Returns a value indicating whether the string is null or empty.
/// </summary>
/// <param name="InString">The string.</param>
BOOLEAN RtlStringNullOrEmpty(CONST CHAR* InString)
{
	if (InString == nullptr)
		return true;

	return InString[0] == '\0';
}

/// <summary>
/// Returns a value indicating whether the string is null or empty.
/// </summary>
/// <param name="InString">The string.</param>
BOOLEAN RtlStringNullOrEmpty(CONST WCHAR* InString)
{
	if (InString == nullptr)
		return true;

	return InString[0] == L'\0';
}

/// <summary>
/// Returns a value indicating whether the string is null or empty.
/// </summary>
/// <param name="InString">The string.</param>
BOOLEAN RtlAnsiStringNullOrEmpty(CONST ANSI_STRING* InString)
{
	if (InString == nullptr)
		return true;

	if (InString->Buffer == nullptr ||
		InString->Length == 0 ||
		InString->MaximumLength == 0)
		return true;

	return InString->Buffer[0] == '\0';
}

/// <summary>
/// Returns a value indicating whether the string is null or empty.
/// </summary>
/// <param name="InString">The string.</param>
BOOLEAN RtlUnicodeStringNullOrEmpty(CONST UNICODE_STRING* InString)
{
	if (InString == nullptr)
		return true;

	if (InString->Buffer == nullptr ||
		InString->Length == 0 ||
		InString->MaximumLength == 0)
		return true;

	return InString->Buffer[0] == L'\0';
}

/// <summary>
/// Returns a value indicating whether the string is null or empty or filled with white spaces.
/// </summary>
/// <param name="InString">The string.</param>
BOOLEAN RtlStringNullOrEmptyOrWhiteSpace(CONST CHAR* InString)
{
	if (RtlStringNullOrEmpty(InString))
		return true;

	for (SIZE_T I = 0;; I++)
	{
		if (InString[I] == '\0')
			return true;

		if (InString[I] == ' ')
			continue;
		
		return false;
	}
}

/// <summary>
/// Returns a value indicating whether the string is null or empty or filled with white spaces.
/// </summary>
/// <param name="InString">The string.</param>
BOOLEAN RtlStringNullOrEmptyOrWhiteSpace(CONST WCHAR* InString)
{
	if (RtlStringNullOrEmpty(InString))
		return true;

	for (SIZE_T I = 0;; I++)
	{
		if (InString[I] == L'\0')
			return true;

		if (InString[I] == L' ')
			continue;
		
		return false;
	}
}

/// <summary>
/// Returns a value indicating whether the string is null or empty or filled with white spaces.
/// </summary>
/// <param name="InString">The string.</param>
BOOLEAN RtlAnsiStringNullOrEmptyOrWhiteSpace(CONST ANSI_STRING* InString)
{
	if (RtlAnsiStringNullOrEmpty(InString))
		return true;

	for (SIZE_T I = 0; I < InString->Length; I++)
	{
		if (InString->Buffer[I] == ' ')
			continue;

		return false;
	}

	return true;
}

/// <summary>
/// Returns a value indicating whether the string is null or empty or filled with white spaces.
/// </summary>
/// <param name="InString">The string.</param>
BOOLEAN RtlUnicodeStringNullOrEmptyOrWhiteSpace(CONST UNICODE_STRING* InString)
{
	if (RtlUnicodeStringNullOrEmpty(InString))
		return true;

	for (SIZE_T I = 0; I < InString->Length; I++)
	{
		if (InString->Buffer[I] == L' ')
			continue;

		return false;
	}

	return true;
}

/// <summary>
/// Converts the specified character to lowercase.
/// </summary>
/// <param name="InCharacter">The character.</param>
CHAR RtlDowncaseChar(CONST CHAR InCharacter)
{
	if (InCharacter >= 'A' && InCharacter <= 'Z')
		return InCharacter + 'a' - 'A';

	return InCharacter;
}

/// <summary>
/// Converts the specified character to uppercase.
/// </summary>
/// <param name="InCharacter">The character.</param>
CHAR RtlUpcaseChar(CONST CHAR InCharacter)
{
	if (InCharacter >= 'a' && InCharacter <= 'z')
		return InCharacter - 'a' + 'A';

	return InCharacter;
}

/// <summary>
/// Converts the specified unicode character to lowercase.
/// </summary>
/// <param name="InCharacter">The character.</param>
WCHAR RtlDowncaseChar(CONST WCHAR InCharacter)
{
#if (NTDDI_VERSION < NTDDI_WINXP)
	return RtlDowncaseUnicodeChar(InCharacter);
#else
	if (InCharacter >= L'A' && InCharacter <= L'Z')
		return InCharacter + L'a' - L'A';

	return InCharacter;
#endif
}

/// <summary>
/// Converts the specified unicode character to uppercase.
/// </summary>
/// <param name="InCharacter">The character.</param>
WCHAR RtlUpcaseChar(CONST WCHAR InCharacter)
{
#if (NTDDI_VERSION < NTDDI_WIN2K)
	return RtlUpcaseUnicodeChar(InCharacter);
#else
	
	if (InCharacter >= L'a' && InCharacter <= L'z')
		return InCharacter - L'a' + L'A';

	return InCharacter;
#endif
}

/// <summary>
/// Converts the specified string to lowercase.
/// </summary>
/// <param name="InString">The string.</param>
VOID RtlDowncaseString(CHAR* InString)
{
	for (SIZE_T I = 0;; I++)
	{
		if (InString[I] == '\0')
			return;

		InString[I] = RtlDowncaseChar(InString[I]);
	}
}

/// <summary>
/// Converts the specified string to uppercase.
/// </summary>
/// <param name="InString">The string.</param>
VOID RtlUpcaseString(CHAR* InString)
{
	for (SIZE_T I = 0;; I++)
	{
		if (InString[I] == '\0')
			return;

		InString[I] = RtlUpcaseChar(InString[I]);
	}
}

/// <summary>
/// Converts the specified string to lowercase.
/// </summary>
/// <param name="InString">The string.</param>
VOID RtlDowncaseString(WCHAR* InString)
{
	for (SIZE_T I = 0;; I++)
	{
		if (InString[I] == L'\0')
			return;

		InString[I] = RtlDowncaseUnicodeChar(InString[I]);
	}
}

/// <summary>
/// Converts the specified string to uppercase.
/// </summary>
/// <param name="InString">The string.</param>
VOID RtlUpcaseString(WCHAR* InString)
{
	for (SIZE_T I = 0;; I++)
	{
		if (InString[I] == L'\0')
			return;

		InString[I] = RtlUpcaseUnicodeChar(InString[I]);
	}
}

/// <summary>
/// Returns a value indicating whether two strings are equal.
/// </summary>
/// <param name="InLeftString">The left string.</param>
/// <param name="InRightString">The right string.</param>
/// <param name="InCaseInsensitive">Whether to check the casing of every characters or not.</param>
BOOLEAN RtlEqualString(CONST CHAR* InLeftString, CONST CHAR* InRightString, BOOLEAN InCaseInsensitive)
{
	STRING LeftString;
	RtlInitAnsiString(&LeftString, InLeftString);

	STRING RightString;
	RtlInitAnsiString(&RightString, InRightString);
	
	return RtlEqualString(&LeftString, &RightString, InCaseInsensitive);
}

/// <summary>
/// Returns a value indicating whether two strings are equal.
/// </summary>
/// <param name="InLeftString">The left string.</param>
/// <param name="InRightString">The right string.</param>
/// <param name="InCaseInsensitive">Whether to check the casing of every characters or not.</param>
BOOLEAN RtlEqualString(CONST WCHAR* InLeftString, CONST WCHAR* InRightString, BOOLEAN InCaseInsensitive)
{
	UNICODE_STRING LeftString;
	RtlInitUnicodeString(&LeftString, InLeftString);

	UNICODE_STRING RightString;
	RtlInitUnicodeString(&RightString, InRightString);

	return RtlEqualUnicodeString(&LeftString, &RightString, InCaseInsensitive);
}

/// <summary>
/// Returns a value indicating whether two strings are equal.
/// </summary>
/// <param name="InLeftString">The left string.</param>
/// <param name="InRightString">The right string.</param>
/// <param name="InCaseInsensitive">Whether to check the casing of every characters or not.</param>
BOOLEAN RtlEqualString(CONST CHAR* InLeftString, CONST WCHAR* InRightString, BOOLEAN InCaseInsensitive)
{
	STRING LeftStringToConvert;
	RtlInitAnsiString(&LeftStringToConvert, InLeftString);

	UNICODE_STRING LeftString;
	RtlAnsiStringToUnicodeString(&LeftString, &LeftStringToConvert, TRUE);

	UNICODE_STRING RightString;
	RtlInitUnicodeString(&RightString, InRightString);
	
	auto const Result = RtlEqualUnicodeString(&LeftString, &RightString, InCaseInsensitive);
	RtlFreeUnicodeString(&LeftString);
	return Result;
}

/// <summary>
/// Returns a value indicating whether two strings are equal.
/// </summary>
/// <param name="InLeftString">The left string.</param>
/// <param name="InRightString">The right string.</param>
/// <param name="InCaseInsensitive">Whether to check the casing of every characters or not.</param>
BOOLEAN RtlEqualString(CONST WCHAR* InLeftString, CONST CHAR* InRightString, BOOLEAN InCaseInsensitive)
{
	UNICODE_STRING LeftString;
	RtlInitUnicodeString(&LeftString, InLeftString);
	
	STRING RightStringToConvert;
	RtlInitAnsiString(&RightStringToConvert, InRightString);

	UNICODE_STRING RightString;
	RtlAnsiStringToUnicodeString(&RightString, &RightStringToConvert, TRUE);

	auto const Result = RtlEqualUnicodeString(&LeftString, &RightString, InCaseInsensitive);
	RtlFreeUnicodeString(&RightString);
	return Result;
}

/// <summary>
/// Searches a partial string from a bigger string and return a pointer to it.
/// </summary>
/// <param name="InString">The string.</param>
/// <param name="InSearchedString">The string to find.</param>
/// <param name="InCaseInsensitive">Whether to check the casing of every characters or not.</param>
CHAR* RtlFindString(CONST CHAR* InString, CONST CHAR* InSearchedString, BOOLEAN InCaseInsensitive)
{
	if (InCaseInsensitive)
	{
		SIZE_T StringLength = RtlStringLength(InString);
		CHAR* LowercaseString = (CHAR*) ExAllocatePool(NonPagedPoolNx, StringLength + sizeof(CHAR));
		RtlCopyMemory(LowercaseString, InString, StringLength);
		
		SIZE_T SubstringLength = RtlStringLength(InSearchedString);
		CHAR* LowercaseSubstring = (CHAR*) ExAllocatePool(NonPagedPoolNx, SubstringLength + sizeof(CHAR));
		RtlCopyMemory(LowercaseSubstring, InSearchedString, SubstringLength);

		CHAR* Result = strstr(LowercaseString, LowercaseSubstring);

		if (Result != nullptr)
		{
			SIZE_T OffsetToSubstring = (SIZE_T) RtlSubOffsetFromPointer(Result, LowercaseString);
			Result = (CHAR*) RtlAddOffsetToPointer(InString, OffsetToSubstring);
		}

		ExFreePool(LowercaseSubstring);
		ExFreePool(LowercaseString);
		return Result;
	}

	return (CHAR*) strstr(InString, InSearchedString);
}

/// <summary>
/// Searches a partial string from a bigger string and return a pointer to it.
/// </summary>
/// <param name="InString">The string.</param>
/// <param name="InSearchedString">The string to find.</param>
/// <param name="InCaseInsensitive">The string to find.</param>
WCHAR* RtlFindString(CONST WCHAR* InString, CONST WCHAR* InSearchedString, BOOLEAN InCaseInsensitive)
{
	if (InCaseInsensitive)
	{
		SIZE_T StringLength = RtlStringLength(InString);
		WCHAR* LowercaseString = (WCHAR*) ExAllocatePool(NonPagedPoolNx, StringLength + sizeof(WCHAR));
		RtlCopyMemory(LowercaseString, InString, StringLength);
		
		SIZE_T SubstringLength = RtlStringLength(InSearchedString);
		WCHAR* LowercaseSubstring = (WCHAR*) ExAllocatePool(NonPagedPoolNx, SubstringLength + sizeof(WCHAR));
		RtlCopyMemory(LowercaseSubstring, InSearchedString, SubstringLength);

		WCHAR* Result = wcsstr(LowercaseString, LowercaseSubstring);

		if (Result != nullptr)
		{
			SIZE_T OffsetToSubstring = (SIZE_T) RtlSubOffsetFromPointer(Result, LowercaseString);
			Result = (WCHAR*) RtlAddOffsetToPointer(InString, OffsetToSubstring);
		}

		ExFreePool(LowercaseSubstring);
		ExFreePool(LowercaseString);
		return Result;
	}

	return (WCHAR*) wcsstr(InString, InSearchedString);
}

/// <summary>
/// Returns a value indicating whether a string contains a substring.
/// </summary>
/// <param name="InString">The string.</param>
/// <param name="InSearchedString">The string to find.</param>
/// <param name="InCaseInsensitive">Whether to check the casing of every characters or not.</param>
BOOLEAN RtlContainsString(CONST CHAR* InString, CONST CHAR* InSearchedString, BOOLEAN InCaseInsensitive)
{
	return RtlFindString(InString, InSearchedString, InCaseInsensitive) != nullptr;
}

/// <summary>
/// Returns a value indicating whether a string contains a substring.
/// </summary>
/// <param name="InString">The string.</param>
/// <param name="InSearchedString">The string to find.</param>
/// <param name="InCaseInsensitive">Whether to check the casing of every characters or not.</param>
BOOLEAN RtlContainsString(CONST WCHAR* InString, CONST WCHAR* InSearchedString, BOOLEAN InCaseInsensitive)
{
	return RtlFindString(InString, InSearchedString, InCaseInsensitive) != nullptr;
}

/// <summary>
/// Returns a value indicating whether the string starts with the substring.
/// </summary>
/// <param name="InString">The string.</param>
/// <param name="InSubstring">The substring to find.</param>
/// <param name="InCaseInsensitive">Whether to check the casing of every characters or not.</param>
BOOLEAN RtlStringStartsWith(CONST CHAR* InString, CONST CHAR* InSubstring, BOOLEAN InCaseInsensitive)
{
	return RtlFindString(InString, InSubstring, InCaseInsensitive) == InString;
}

/// <summary>
/// Returns a value indicating whether the string starts with the substring.
/// </summary>
/// <param name="InString">The string.</param>
/// <param name="InSubstring">The substring to find.</param>
/// <param name="InCaseInsensitive">Whether to check the casing of every characters or not.</param>
BOOLEAN RtlStringStartsWith(CONST WCHAR* InString, CONST WCHAR* InSubstring, BOOLEAN InCaseInsensitive)
{
	return RtlFindString(InString, InSubstring, InCaseInsensitive) == InString;
}

/// <summary>
/// Returns a value indicating whether the string starts with the substring.
/// </summary>
/// <param name="InString">The string.</param>
/// <param name="InSubstring">The substring to find.</param>
/// <param name="InCaseInsensitive">Whether to check the casing of every characters or not.</param>
BOOLEAN RtlAnsiStringStartsWith(CONST ANSI_STRING* InString, CONST CHAR* InSubstring, BOOLEAN InCaseInsensitive)
{
	return RtlFindString(InString->Buffer, InSubstring, InCaseInsensitive) == InString->Buffer;
}

/// <summary>
/// Returns a value indicating whether the string starts with the substring.
/// </summary>
/// <param name="InString">The string.</param>
/// <param name="InSubstring">The substring to find.</param>
/// <param name="InCaseInsensitive">Whether to check the casing of every characters or not.</param>
BOOLEAN RtlUnicodeStringStartsWith(CONST UNICODE_STRING* InString, CONST WCHAR* InSubstring, BOOLEAN InCaseInsensitive)
{
	return RtlFindString(InString->Buffer, InSubstring, InCaseInsensitive) == InString->Buffer;
}

/// <summary>
/// Returns a value indicating whether the string is valid.
/// </summary>
/// <param name="InString">The string.</param>
BOOLEAN RtlVerifyAnsiString(CONST ANSI_STRING* InString)
{
	if (InString == nullptr ||
		InString->Buffer == nullptr ||
		InString->Length % 2 != 0 ||
		InString->MaximumLength % 2 != 0 ||
		InString->Length > InString->MaximumLength)
	{
		return false;
	}

	return true;
}

/// <summary>
/// Returns a value indicating whether the string is valid.
/// </summary>
/// <param name="InString">The string.</param>
BOOLEAN RtlVerifyUnicodeString(CONST UNICODE_STRING* InString)
{
	if (InString == nullptr ||
		InString->Buffer == nullptr ||
		InString->Length % 2 != 0 ||
		InString->MaximumLength % 2 != 0 ||
		InString->Length > InString->MaximumLength)
	{
		return false;
	}

	return true;
}
