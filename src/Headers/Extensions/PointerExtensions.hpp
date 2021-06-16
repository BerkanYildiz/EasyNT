#pragma once

// 
// Macros to do simple add/sub arithmetic with pointers.
// 

#define RtlAddOffsetToPointer(Pointer, Offset) ((void*) ((ULONG_PTR) (Pointer) + (SIZE_T) (Offset)))
#define RtlSubOffsetFromPointer(Pointer, Offset) ((void*) ((ULONG_PTR) (Pointer) - (SIZE_T) (Offset)))
