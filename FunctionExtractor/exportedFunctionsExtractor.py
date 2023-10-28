import pefile
import os
import sys

def main():
    try:
        dll = sys.argv[1]
    except IndexError:
        print("No path to a DLL provided\n")

    filename = os.path.basename(dll)
    filename_without_extension = os.path.splitext(filename)[0]

    # Open DLL file
    pe = pefile.PE(dll)

    if hasattr(pe, 'DIRECTORY_ENTRY_EXPORT'):
        exported_functions = pe.DIRECTORY_ENTRY_EXPORT.symbols
    else:
        print("PE object doesn't have a DIRECTORY_ENTRY_EXPORT attribute")

    for function in exported_functions:
        name = function.name.decode("utf-8")
        ordinal = function.ordinal
        #exporting with ordinal does not work, idk why :/
        #print(f'#pragma comment(linker, "/EXPORT:{name}={filename_without_extension}.{name}@{ordinal}")')
        print(f'#pragma comment(linker, "/EXPORT:{name}={filename_without_extension}.{name}")')
if __name__ == "__main__":
    main()