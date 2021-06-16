#pragma once

// 
// Include the basic windows kernel headers.
// 

#include <ntdef.h>
#include <ntifs.h>
#include <tchar.h>
#include <wdm.h>

// 
// Define the library globals.
// 

#define LOGGER_NT_VERSION_MAJOR 1
#define LOGGER_NT_VERSION_MINOR 0
#define LOGGER_NT_VERSION_BUILD 0

// 
// Include the library headers.
// 

#include "Extensions/PointerExtensions.hpp"
#include "Extensions/StringExtensions.hpp"
#include "Extensions/VersionExtensions.hpp"
#include "Extensions/TimeExtensions.hpp"
#include "Extensions/ThreadExtensions.hpp"
#include "Extensions/ArrayExtensions.hpp"
