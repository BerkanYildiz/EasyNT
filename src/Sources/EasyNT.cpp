#include "../Headers/EasyNT.h"

void Tests_Examples()
{
	CONST CHAR* ExampleUtf8String = "This is a dummy UTF8 example.";
	CONST WCHAR* ExampleUnicodeString = L"This is dummy UNICODE example.";

	RtlStringNullOrEmptyOrWhiteSpace(ExampleUtf8String);
	RtlStringNullOrEmptyOrWhiteSpace(ExampleUnicodeString);
}