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

#include "LogLevel.hpp"
#include "LogProvider.hpp"
#include "LoggerConfig.hpp"
#include "Logger.hpp"

// 
// Include the default logging providers.
// 

#include "Providers/DbgPrintProvider.hpp"
#include "Providers/TempFileProvider.hpp"
