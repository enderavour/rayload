# rayload - Example of fully dynamical loading of raylib.dll without any static dependencies on Windows x64

The following logic is implemented:
- The program extracts pointer to PEB structure inside the process
- Lists all loaded modules and gets the adress of kernel32.dll
- Parses functions' name, RVA's and ordinals 
- Finds LoadLibrary, GetProcAddress and FreeLibrary and resolves their adresses
- Loads raylib.dll and performs window initialization
- Does not require any static linkage (e.g. kernel32.lib, user32.lib)

## Requirements
- MSVC
- NASM

## Notes
- Makefile is implemented specifically for nmake system
- The program works only on Win64 due to hardcoded offsets and extraction of PEB