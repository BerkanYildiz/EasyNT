#pragma once

/// <summary>
/// Returns the number of characters in a string.
/// </summary>
/// <param name="InString">The string.</param>
SIZE_T RtlStringLength(CONST CHAR* InString);

/// <summary>
/// Returns the number of characters in a string.
/// </summary>
/// <param name="InString">The string.</param>
SIZE_T RtlStringLength(CONST WCHAR* InString);

/// <summary>
/// Returns a value indicating whether the string is null or empty.
/// </summary>
/// <param name="InString">The string.</param>
BOOLEAN RtlStringNullOrEmpty(CONST CHAR* InString);

/// <summary>
/// Returns a value indicating whether the string is null or empty.
/// </summary>
/// <param name="InString">The string.</param>
BOOLEAN RtlStringNullOrEmpty(CONST WCHAR* InString);

/// <summary>
/// Returns a value indicating whether the string is null or empty.
/// </summary>
/// <param name="InString">The string.</param>
BOOLEAN RtlAnsiStringNullOrEmpty(CONST ANSI_STRING* InString);

/// <summary>
/// Returns a value indicating whether the string is null or empty.
/// </summary>
/// <param name="InString">The string.</param>
BOOLEAN RtlUnicodeStringNullOrEmpty(CONST UNICODE_STRING* InString);

/// <summary>
/// Returns a value indicating whether the string is null or empty or filled with white spaces.
/// </summary>
/// <param name="InString">The string.</param>
BOOLEAN RtlStringNullOrEmptyOrWhiteSpace(CONST CHAR* InString);

/// <summary>
/// Returns a value indicating whether the string is null or empty or filled with white spaces.
/// </summary>
/// <param name="InString">The string.</param>
BOOLEAN RtlStringNullOrEmptyOrWhiteSpace(CONST WCHAR* InString);

/// <summary>
/// Returns a value indicating whether the string is null or empty or filled with white spaces.
/// </summary>
/// <param name="InString">The string.</param>
BOOLEAN RtlAnsiStringNullOrEmptyOrWhiteSpace(CONST ANSI_STRING* InString);

/// <summary>
/// Returns a value indicating whether the string is null or empty or filled with white spaces.
/// </summary>
/// <param name="InString">The string.</param>
BOOLEAN RtlUnicodeStringNullOrEmptyOrWhiteSpace(CONST UNICODE_STRING* InString);

/// <summary>
/// Converts the specified character to lowercase.
/// </summary>
/// <param name="InCharacter">The character.</param>
CHAR RtlDowncaseChar(CONST CHAR InCharacter);

/// <summary>
/// Converts the specified character to uppercase.
/// </summary>
/// <param name="InCharacter">The character.</param>
CHAR RtlUpcaseChar(CONST CHAR InCharacter);

/// <summary>
/// Converts the specified unicode character to lowercase.
/// </summary>
/// <param name="InCharacter">The character.</param>
WCHAR RtlDowncaseChar(CONST WCHAR InCharacter);

/// <summary>
/// Converts the specified unicode character to uppercase.
/// </summary>
/// <param name="InCharacter">The character.</param>
WCHAR RtlUpcaseChar(CONST WCHAR InCharacter);

/// <summary>
/// Converts the specified string to lowercase.
/// </summary>
/// <param name="InString">The string.</param>
VOID RtlDowncaseString(CHAR* InString);

/// <summary>
/// Converts the specified string to uppercase.
/// </summary>
/// <param name="InString">The string.</param>
VOID RtlUpcaseString(CHAR* InString);

/// <summary>
/// Converts the specified string to lowercase.
/// </summary>
/// <param name="InString">The string.</param>
VOID RtlDowncaseString(WCHAR* InString);

/// <summary>
/// Converts the specified string to uppercase.
/// </summary>
/// <param name="InString">The string.</param>
VOID RtlUpcaseString(WCHAR* InString);

/// <summary>
/// Returns a value indicating whether two strings are equal.
/// </summary>
/// <param name="InLeftString">The left string.</param>
/// <param name="InRightString">The right string.</param>
/// <param name="InCaseInsensitive">Whether to check the casing of every characters or not.</param>
BOOLEAN RtlEqualString(CONST CHAR* InLeftString, CONST CHAR* InRightString, BOOLEAN InCaseInsensitive = FALSE);

/// <summary>
/// Returns a value indicating whether two strings are equal.
/// </summary>
/// <param name="InLeftString">The left string.</param>
/// <param name="InRightString">The right string.</param>
/// <param name="InCaseInsensitive">Whether to check the casing of every characters or not.</param>
BOOLEAN RtlEqualString(CONST WCHAR* InLeftString, CONST WCHAR* InRightString, BOOLEAN InCaseInsensitive = FALSE);

/// <summary>
/// Returns a value indicating whether two strings are equal.
/// </summary>
/// <param name="InLeftString">The left string.</param>
/// <param name="InRightString">The right string.</param>
/// <param name="InCaseInsensitive">Whether to check the casing of every characters or not.</param>
BOOLEAN RtlEqualString(CONST CHAR* InLeftString, CONST WCHAR* InRightString, BOOLEAN InCaseInsensitive = FALSE);

/// <summary>
/// Returns a value indicating whether two strings are equal.
/// </summary>
/// <param name="InLeftString">The left string.</param>
/// <param name="InRightString">The right string.</param>
/// <param name="InCaseInsensitive">Whether to check the casing of every characters or not.</param>
BOOLEAN RtlEqualString(CONST WCHAR* InLeftString, CONST CHAR* InRightString, BOOLEAN InCaseInsensitive = FALSE);

/// <summary>
/// Searches a partial string from a bigger string and return a pointer to it.
/// </summary>
/// <param name="InString">The string.</param>
/// <param name="InSearchedString">The string to find.</param>
/// <param name="InCaseInsensitive">Whether to check the casing of every characters or not.</param>
CHAR* RtlFindString(CONST CHAR* InString, CONST CHAR* InSearchedString, BOOLEAN InCaseInsensitive = FALSE);

/// <summary>
/// Searches a partial string from a bigger string and return a pointer to it.
/// </summary>
/// <param name="InString">The string.</param>
/// <param name="InSearchedString">The string to find.</param>
/// <param name="InCaseInsensitive">Whether to check the casing of every characters or not.</param>
WCHAR* RtlFindString(CONST WCHAR* InString, CONST WCHAR* InSearchedString, BOOLEAN InCaseInsensitive = FALSE);

/// <summary>
/// Returns a value indicating whether a string contains a substring.
/// </summary>
/// <param name="InString">The string.</param>
/// <param name="InSearchedString">The string to find.</param>
/// <param name="InCaseInsensitive">Whether to check the casing of every characters or not.</param>
BOOLEAN RtlStringContains(CONST CHAR* InString, CONST CHAR* InSearchedString, BOOLEAN InCaseInsensitive = FALSE);

/// <summary>
/// Returns a value indicating whether a string contains a substring.
/// </summary>
/// <param name="InString">The string.</param>
/// <param name="InSearchedString">The string to find.</param>
/// <param name="InCaseInsensitive">Whether to check the casing of every characters or not.</param>
BOOLEAN RtlStringContains(CONST WCHAR* InString, CONST WCHAR* InSearchedString, BOOLEAN InCaseInsensitive = FALSE);

/// <summary>
/// Returns a value indicating whether the string starts with the substring.
/// </summary>
/// <param name="InString">The string.</param>
/// <param name="InSubstring">The substring to find.</param>
/// <param name="InCaseInsensitive">Whether to check the casing of every characters or not.</param>
BOOLEAN RtlStringStartsWith(CONST CHAR* InString, CONST CHAR* InSubstring, BOOLEAN InCaseInsensitive = FALSE);

/// <summary>
/// Returns a value indicating whether the string starts with the substring.
/// </summary>
/// <param name="InString">The string.</param>
/// <param name="InSubstring">The substring to find.</param>
/// <param name="InCaseInsensitive">Whether to check the casing of every characters or not.</param>
BOOLEAN RtlStringStartsWith(CONST WCHAR* InString, CONST WCHAR* InSubstring, BOOLEAN InCaseInsensitive = FALSE);

/// <summary>
/// Returns a value indicating whether the string starts with the substring.
/// </summary>
/// <param name="InString">The string.</param>
/// <param name="InSubstring">The substring to find.</param>
/// <param name="InCaseInsensitive">Whether to check the casing of every characters or not.</param>
BOOLEAN RtlAnsiStringStartsWith(CONST ANSI_STRING* InString, CONST CHAR* InSubstring, BOOLEAN InCaseInsensitive = FALSE);

/// <summary>
/// Returns a value indicating whether the string starts with the substring.
/// </summary>
/// <param name="InString">The string.</param>
/// <param name="InSubstring">The substring to find.</param>
/// <param name="InCaseInsensitive">Whether to check the casing of every characters or not.</param>
BOOLEAN RtlUnicodeStringStartsWith(CONST UNICODE_STRING* InString, CONST WCHAR* InSubstring, BOOLEAN InCaseInsensitive = FALSE);

/// <summary>
/// Returns a value indicating whether the string is valid.
/// </summary>
/// <param name="InString">The string.</param>
BOOLEAN RtlVerifyAnsiString(CONST ANSI_STRING* InString);

/// <summary>
/// Returns a value indicating whether the string is valid.
/// </summary>
/// <param name="InString">The string.</param>
BOOLEAN RtlVerifyUnicodeString(CONST UNICODE_STRING* InString);
