#include "../Headers/EasyNT.h"

void Tests()
{
	CHAR* ExampleBuffer = nullptr;
	SIZE_T ExampleBufferLength = 128;

	BOOLEAN AreAllCharsZero = RtlArrayMatchAll<CHAR>(ExampleBuffer, ExampleBufferLength, [](CHAR InEntry) -> bool { return InEntry == 0x00; });
	BOOLEAN AreAllCharsA = RtlArrayMatchAll<CHAR>(ExampleBuffer, ExampleBufferLength, 'A');
}