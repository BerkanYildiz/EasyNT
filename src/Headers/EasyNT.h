#pragma once

// 
// Include the basic windows kernel headers.
// 

#include <ntdef.h>
#include <ntifs.h>
#include <wdm.h>
#include <ntstrsafe.h>
#include <intrin.h>

// 
// Include the process hacker headers.
// 

#define PHNT_VERSION 107
#define PHNT_MODE 0

#include "../../libraries/phnt/phnt.h"

// 
// Include our custom NT API header.
// 

#include "EasyNTAPI.h"

// 
// Include the memory manager.
// 

#include "Managers/MemoryManager.hpp"

// 
// Include the library headers.
// 

#include "Extensions/PointerExtensions.hpp"
#include "Extensions/MemoryExtensions.hpp"
#include "Extensions/PhysicalMemoryExtensions.hpp"
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
