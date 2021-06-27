#pragma once

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

EXTERN_C NTKERNELAPI NTSTATUS ObReferenceObjectByName(PUNICODE_STRING ObjectPath,
    ULONG Attributes,
    PACCESS_STATE PassedAccessState OPTIONAL,
    ACCESS_MASK DesiredAccess OPTIONAL,
    POBJECT_TYPE ObjectType,
    KPROCESSOR_MODE AccessMode,
    PVOID ParseContext OPTIONAL,
    PVOID *ObjectPtr
);