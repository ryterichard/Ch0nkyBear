#include <windows.h>
#include <stdio.h> 
#include <iostream>




/* Definitions from reflectiveDLL injection.h (which we dont need)
    - _REFLECTIVE_DLLInjection_H
    - ULONG_PTR
    - BOOL
    - DLLEXPORT _declspec(dllexport)
*/





// 1. Define Caller using _declspec, use LoadRemoteLibraryR


/* 2. Set Pointers to NUll for necessary functions
    - LoadLibraryA
    - GetProcAddress
    - VirtualAlloc
    - NtFlushInstructionCache
*/

// 3. Initialize Variables for image relocation


//-----------------------------------------------------------------
//Start Processing

/* 0. Calculate Image's Current Base adress
    - use caller()
    - loop memory backwards to find image base address
*/

/* 1. Process Kernel's exports for the functions loader needs
    - process loaded modules
*/

/* 2. Load image into permanent Location in Memory
    - need virtual address of NT header of DLL
    - VirtualAlloc()
    - Copy the headers
*/

/* 3. Load Sections
    - uses PIMAGE_SECTION_HEADER
*/

/* 4. Process import table and handle base relocations
    - use dump_imports.cpp for reference (Lecture 18 code)
*/

/* 5. Call the image entry point
    - use ......... .AddressOfEntryPoint
*/

/* 6. Call DLLMain and pass lpParameter(?)
    (only if using LoadRemoteLibraryR)
    - return the new entrypoint's address
*/