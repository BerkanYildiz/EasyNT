#pragma once

// 
// Some general type definitions.
// 

typedef UINT64	QWORD;		// QWORD	= unsigned 64 bit value
typedef UINT16	WORD;		// WORD		= unsigned 16 bit value
typedef UINT8	BYTE;		// BYTE		= unsigned  8 bit value

// 
// Non-documented or exported object types.
// 

EXTERN_C NTKERNELAPI POBJECT_TYPE* IoDriverObjectType;
EXTERN_C NTKERNELAPI POBJECT_TYPE* IoDeviceObjectType;
EXTERN_C NTKERNELAPI POBJECT_TYPE* IoProcessObjectType;
EXTERN_C NTKERNELAPI POBJECT_TYPE* IoThreadObjectType;

// 
// Non-documented or exported functions.
// 

EXTERN_C NTKERNELAPI NTSTATUS ObReferenceObjectByName(
    PUNICODE_STRING ObjectPath,
    ULONG Attributes,
    PACCESS_STATE PassedAccessState OPTIONAL,
    ACCESS_MASK DesiredAccess OPTIONAL,
    POBJECT_TYPE ObjectType,
    KPROCESSOR_MODE AccessMode,
    PVOID ParseContext OPTIONAL,
    PVOID *ObjectPtr
);

EXTERN_C NTKERNELAPI NTSTATUS MmCopyVirtualMemory(
	CONST PEPROCESS SourceProcess,
	CONST PVOID SourceAddress,
	CONST PEPROCESS TargetProcess,
	PVOID TargetAddress,
	SIZE_T BufferSize,
	KPROCESSOR_MODE PreviousMode,
	PSIZE_T ReturnSize
);

EXTERN_C NTKERNELAPI PVOID RtlFindExportedRoutineByName(
	CONST PVOID DllBase,
	CONST CHAR* RoutineName
);

// 
// Non-documented or exported defines for executables formats.
// 

#define IMAGE_NT_OPTIONAL_HDR32_MAGIC		 0x10b
#define IMAGE_NT_OPTIONAL_HDR64_MAGIC		 0x20b
#define IMAGE_ROM_OPTIONAL_HDR_MAGIC		 0x107

#define IMAGE_DOS_SIGNATURE					 0x5A4D      // MZ
#define IMAGE_OS2_SIGNATURE					 0x454E      // NE
#define IMAGE_OS2_SIGNATURE_LE				 0x454C      // LE
#define IMAGE_NT_SIGNATURE					 0x00004550  // PE00
#define IMAGE_SIZEOF_SHORT_NAME              8
#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES    16

#define IMAGE_DIRECTORY_ENTRY_EXPORT          0   // Export Directory
#define IMAGE_DIRECTORY_ENTRY_IMPORT          1   // Import Directory
#define IMAGE_DIRECTORY_ENTRY_RESOURCE        2   // Resource Directory
#define IMAGE_DIRECTORY_ENTRY_EXCEPTION       3   // Exception Directory
#define IMAGE_DIRECTORY_ENTRY_SECURITY        4   // Security Directory
#define IMAGE_DIRECTORY_ENTRY_BASERELOC       5   // Base Relocation Table
#define IMAGE_DIRECTORY_ENTRY_DEBUG           6   // Debug Directory
#define IMAGE_DIRECTORY_ENTRY_ARCHITECTURE    7   // Architecture Specific Data
#define IMAGE_DIRECTORY_ENTRY_GLOBALPTR       8   // RVA of GP
#define IMAGE_DIRECTORY_ENTRY_TLS             9   // TLS Directory
#define IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG    10   // Load Configuration Directory
#define IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT   11   // Bound Import Directory in headers
#define IMAGE_DIRECTORY_ENTRY_IAT            12   // Import Address Table
#define IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT   13   // Delay Load Import Descriptors
#define IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR 14   // COM Runtime descriptor

#define IMAGE_REL_BASED_ABSOLUTE              0
#define IMAGE_REL_BASED_HIGH                  1
#define IMAGE_REL_BASED_LOW                   2
#define IMAGE_REL_BASED_HIGHLOW               3
#define IMAGE_REL_BASED_HIGHADJ               4
#define IMAGE_REL_BASED_MACHINE_SPECIFIC_5    5
#define IMAGE_REL_BASED_RESERVED              6
#define IMAGE_REL_BASED_MACHINE_SPECIFIC_7    7
#define IMAGE_REL_BASED_MACHINE_SPECIFIC_8    8
#define IMAGE_REL_BASED_MACHINE_SPECIFIC_9    9
#define IMAGE_REL_BASED_DIR64                 10

#define IMAGE_ORDINAL_FLAG32				  0x80000000
#define IMAGE_ORDINAL_FLAG64				  0x8000000000000000

#define IMAGE_SCN_LNK_NRELOC_OVFL			  0x01000000  // Section contains extended relocations.
#define IMAGE_SCN_MEM_DISCARDABLE             0x02000000  // Section can be discarded.
#define IMAGE_SCN_MEM_NOT_CACHED              0x04000000  // Section cannot be cached.
#define IMAGE_SCN_MEM_NOT_PAGED               0x08000000  // Section cannot be paged out.
#define IMAGE_SCN_MEM_SHARED                  0x10000000  // Section is shareable.
#define IMAGE_SCN_MEM_EXECUTE                 0x20000000  // Section is executable.
#define IMAGE_SCN_MEM_READ                    0x40000000  // Section is readable.
#define IMAGE_SCN_MEM_WRITE                   0x80000000  // Section is writable.

#define IMAGE_SCN_CNT_CODE					  0x00000020
#define IMAGE_SCN_CNT_INITIALIZED_DATA		  0x00000040
#define IMAGE_SCN_CNT_UNINITIALIZED_DATA	  0x00000080

// 
// Non-documented or exported structures for executables formats.
// 

typedef struct _IMAGE_DOS_HEADER {
	USHORT   e_magic;
	USHORT   e_cblp;
	USHORT   e_cp;
	USHORT   e_crlc;
	USHORT   e_cparhdr;
	USHORT   e_minalloc;
	USHORT   e_maxalloc;
	USHORT   e_ss;
	USHORT   e_sp;
	USHORT   e_csum;
	USHORT   e_ip;
	USHORT   e_cs;
	USHORT   e_lfarlc;
	USHORT   e_ovno;
	USHORT   e_res[4];
	USHORT   e_oemid;
	USHORT   e_oeminfo;
	USHORT   e_res2[10];
	LONG   e_lfanew;
} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

typedef struct _IMAGE_SECTION_HEADER {
	BYTE  Name[IMAGE_SIZEOF_SHORT_NAME];
	union {
		DWORD PhysicalAddress;
		DWORD VirtualSize;
	} Misc;
	DWORD VirtualAddress;
	DWORD SizeOfRawData;
	DWORD PointerToRawData;
	DWORD PointerToRelocations;
	DWORD PointerToLinenumbers;
	WORD  NumberOfRelocations;
	WORD  NumberOfLinenumbers;
	DWORD Characteristics;
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;

typedef struct _IMAGE_FILE_HEADER {
	WORD    Machine;
	WORD    NumberOfSections;
	DWORD   TimeDateStamp;
	DWORD   PointerToSymbolTable;
	DWORD   NumberOfSymbols;
	WORD    SizeOfOptionalHeader;
	WORD    Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

typedef struct _IMAGE_DATA_DIRECTORY {
	DWORD   VirtualAddress;
	DWORD   Size;
} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

typedef struct _IMAGE_OPTIONAL_HEADER {
	//
	// Standard fields.
	//

	WORD    Magic;
	BYTE    MajorLinkerVersion;
	BYTE    MinorLinkerVersion;
	DWORD   SizeOfCode;
	DWORD   SizeOfInitializedData;
	DWORD   SizeOfUninitializedData;
	DWORD   AddressOfEntryPoint;
	DWORD   BaseOfCode;
	DWORD   BaseOfData;

	//
	// NT additional fields.
	//

	DWORD   ImageBase;
	DWORD   SectionAlignment;
	DWORD   FileAlignment;
	WORD    MajorOperatingSystemVersion;
	WORD    MinorOperatingSystemVersion;
	WORD    MajorImageVersion;
	WORD    MinorImageVersion;
	WORD    MajorSubsystemVersion;
	WORD    MinorSubsystemVersion;
	DWORD   Win32VersionValue;
	DWORD   SizeOfImage;
	DWORD   SizeOfHeaders;
	DWORD   CheckSum;
	WORD    Subsystem;
	WORD    DllCharacteristics;
	DWORD   SizeOfStackReserve;
	DWORD   SizeOfStackCommit;
	DWORD   SizeOfHeapReserve;
	DWORD   SizeOfHeapCommit;
	DWORD   LoaderFlags;
	DWORD   NumberOfRvaAndSizes;
	IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER32, *PIMAGE_OPTIONAL_HEADER32;

typedef struct _IMAGE_OPTIONAL_HEADER64 {
	WORD        Magic;
	BYTE        MajorLinkerVersion;
	BYTE        MinorLinkerVersion;
	DWORD       SizeOfCode;
	DWORD       SizeOfInitializedData;
	DWORD       SizeOfUninitializedData;
	DWORD       AddressOfEntryPoint;
	DWORD       BaseOfCode;
	ULONGLONG   ImageBase;
	DWORD       SectionAlignment;
	DWORD       FileAlignment;
	WORD        MajorOperatingSystemVersion;
	WORD        MinorOperatingSystemVersion;
	WORD        MajorImageVersion;
	WORD        MinorImageVersion;
	WORD        MajorSubsystemVersion;
	WORD        MinorSubsystemVersion;
	DWORD       Win32VersionValue;
	DWORD       SizeOfImage;
	DWORD       SizeOfHeaders;
	DWORD       CheckSum;
	WORD        Subsystem;
	WORD        DllCharacteristics;
	ULONGLONG   SizeOfStackReserve;
	ULONGLONG   SizeOfStackCommit;
	ULONGLONG   SizeOfHeapReserve;
	ULONGLONG   SizeOfHeapCommit;
	DWORD       LoaderFlags;
	DWORD       NumberOfRvaAndSizes;
	IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER64, *PIMAGE_OPTIONAL_HEADER64;

typedef struct _IMAGE_NT_HEADERS64 {
	DWORD Signature;
	IMAGE_FILE_HEADER FileHeader;
	IMAGE_OPTIONAL_HEADER64 OptionalHeader;
} IMAGE_NT_HEADERS64, *PIMAGE_NT_HEADERS64;

typedef struct _IMAGE_NT_HEADERS {
	DWORD Signature;
	IMAGE_FILE_HEADER FileHeader;
	IMAGE_OPTIONAL_HEADER32 OptionalHeader;
} IMAGE_NT_HEADERS32, *PIMAGE_NT_HEADERS32;

typedef struct _IMAGE_EXPORT_DIRECTORY
{
	DWORD Characteristics;
	DWORD TimeDateStamp;
	WORD  MajorVersion;
	WORD  MinorVersion;
	DWORD Name;
	DWORD Base;
	DWORD NumberOfFunctions;
	DWORD NumberOfNames;
	DWORD AddressOfFunctions;
	DWORD AddressOfNames;
	DWORD AddressOfNameOrdinals;
} IMAGE_EXPORT_DIRECTORY, *PIMAGE_EXPORT_DIRECTORY;

typedef struct _IMAGE_IMPORT_DESCRIPTOR
{
	union
	{
		DWORD Characteristics;
		DWORD OriginalFirstThunk;
	};
	DWORD TimeDateStamp;
	DWORD ForwarderChain;
	DWORD Name;
	DWORD FirstThunk;

} IMAGE_IMPORT_DESCRIPTOR, *PIMAGE_IMPORT_DESCRIPTOR;


typedef struct _IMAGE_IMPORT_BY_NAME
{
	WORD Hint;
	BYTE Name[1];

} IMAGE_IMPORT_BY_NAME, *PIMAGE_IMPORT_BY_NAME;

typedef struct _IMAGE_THUNK_DATA32
{
	union
	{
		DWORD ForwarderString;
		DWORD Function;
		DWORD Ordinal;
		DWORD AddressOfData;
	} u1;

} IMAGE_THUNK_DATA32, *PIMAGE_THUNK_DATA32;

typedef struct _IMAGE_THUNK_DATA64
{
	union
	{
		ULONGLONG ForwarderString;
		ULONGLONG Function;
		ULONGLONG Ordinal;
		ULONGLONG AddressOfData;
	} u1;

} IMAGE_THUNK_DATA64, *PIMAGE_THUNK_DATA64;

typedef struct _IMAGE_RESOURCE_DIRECTORY_ENTRY
{
	DWORD Name;
	DWORD OffsetToData;
} IMAGE_RESOURCE_DIRECTORY_ENTRY, *PIMAGE_RESOURCE_DIRECTORY_ENTRY;


typedef struct _IMAGE_RESOURCE_DATA_ENTRY
{
	DWORD OffsetToData;
	DWORD Size;
	DWORD CodePage;
	DWORD Reserved;
} IMAGE_RESOURCE_DATA_ENTRY, *PIMAGE_RESOURCE_DATA_ENTRY;

typedef struct _IMAGE_RESOURCE_DIRECTORY
{
	DWORD Characteristics;
	DWORD TimeDateStamp;
	WORD  MajorVersion;
	WORD  MinorVersion;
	WORD  NumberOfNamedEntries;
	WORD  NumberOfIdEntries;
} IMAGE_RESOURCE_DIRECTORY, *PIMAGE_RESOURCE_DIRECTORY;

typedef struct _IMAGE_DEBUG_DIRECTORY {
	DWORD Characteristics;
	DWORD TimeDateStamp;
	WORD  MajorVersion;
	WORD  MinorVersion;
	DWORD Type;
	DWORD SizeOfData;
	DWORD AddressOfRawData;
	DWORD PointerToRawData;
} IMAGE_DEBUG_DIRECTORY, *PIMAGE_DEBUG_DIRECTORY;

typedef struct _IMAGE_LOAD_CONFIG_CODE_INTEGRITY {
    WORD    Flags;          // Flags to indicate if CI information is available, etc.
    WORD    Catalog;        // 0xFFFF means not available
    DWORD   CatalogOffset;
    DWORD   Reserved;       // Additional bitmask to be defined later
} IMAGE_LOAD_CONFIG_CODE_INTEGRITY, *PIMAGE_LOAD_CONFIG_CODE_INTEGRITY;

typedef struct _IMAGE_LOAD_CONFIG_DIRECTORY64 {
	DWORD                            Size;
	DWORD                            TimeDateStamp;
	WORD                             MajorVersion;
	WORD                             MinorVersion;
	DWORD                            GlobalFlagsClear;
	DWORD                            GlobalFlagsSet;
	DWORD                            CriticalSectionDefaultTimeout;
	ULONGLONG                        DeCommitFreeBlockThreshold;
	ULONGLONG                        DeCommitTotalFreeThreshold;
	ULONGLONG                        LockPrefixTable;
	ULONGLONG                        MaximumAllocationSize;
	ULONGLONG                        VirtualMemoryThreshold;
	ULONGLONG                        ProcessAffinityMask;
	DWORD                            ProcessHeapFlags;
	WORD                             CSDVersion;
	WORD                             DependentLoadFlags;
	ULONGLONG                        EditList;
	ULONGLONG                        SecurityCookie;
	ULONGLONG                        SEHandlerTable;
	ULONGLONG                        SEHandlerCount;
	ULONGLONG                        GuardCFCheckFunctionPointer;
	ULONGLONG                        GuardCFDispatchFunctionPointer;
	ULONGLONG                        GuardCFFunctionTable;
	ULONGLONG                        GuardCFFunctionCount;
	DWORD                            GuardFlags;
	IMAGE_LOAD_CONFIG_CODE_INTEGRITY CodeIntegrity;
	ULONGLONG                        GuardAddressTakenIatEntryTable;
	ULONGLONG                        GuardAddressTakenIatEntryCount;
	ULONGLONG                        GuardLongJumpTargetTable;
	ULONGLONG                        GuardLongJumpTargetCount;
	ULONGLONG                        DynamicValueRelocTable;
	ULONGLONG                        CHPEMetadataPointer;
	ULONGLONG                        GuardRFFailureRoutine;
	ULONGLONG                        GuardRFFailureRoutineFunctionPointer;
	DWORD                            DynamicValueRelocTableOffset;
	WORD                             DynamicValueRelocTableSection;
	WORD                             Reserved2;
	ULONGLONG                        GuardRFVerifyStackPointerFunctionPointer;
	DWORD                            HotPatchTableOffset;
	DWORD                            Reserved3;
	ULONGLONG                        EnclaveConfigurationPointer;
	ULONGLONG                        VolatileMetadataPointer;
} IMAGE_LOAD_CONFIG_DIRECTORY64, *PIMAGE_LOAD_CONFIG_DIRECTORY64;
