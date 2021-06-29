#pragma once

// 
// Include the basic windows kernel headers.
// 

#include <ntdef.h>
#include <ntifs.h>
#include <tchar.h>
#include <wdm.h>
#include <ntstrsafe.h>

// 
// Include the process hacker headers.
// 

#define PHNT_VERSION 102
#define PHNT_MODE 0

#include "../../libraries/phnt/phnt.h"

// 
// Include our custom NT API header.
// 

#include "EasyNTAPI.h"

// 
// Define the library globals.
// 

#define LOGGER_NT_VERSION_MAJOR 1
#define LOGGER_NT_VERSION_MINOR 0
#define LOGGER_NT_VERSION_BUILD 0

// 
// Include the memory manager.
// 

#include "Managers/MemoryManager.hpp"

// 
// Include the library headers.
// 

#include "Extensions/PointerExtensions.hpp"
#include "Extensions/MemoryExtensions.hpp"
#include "Extensions/StringExtensions.hpp"
#include "Extensions/VersionExtensions.hpp"
#include "Extensions/TimeExtensions.hpp"
#include "Extensions/ThreadExtensions.hpp"
#include "Extensions/ArrayExtensions.hpp"
#include "Extensions/RandomExtension.hpp"
#include "Extensions/ProcessExtensions.hpp"
#include "Extensions/ModuleExtensions.hpp"
#include "Extensions/DeviceExtensions.hpp"
#include "Extensions/DriverExtensions.hpp"
#include "Extensions/FileExtensions.hpp"
#include "Extensions/InterfaceExtensions.hpp"
#include "Extensions/ScanExtensions.hpp"
#include "Extensions/ConversionExtensions.hpp"
