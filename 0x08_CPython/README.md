# 0x08. CPython

This project explores the internals of CPython, the reference C implementation of Python.
It uses the Python/C API to read raw Python object structures directly from C and print
information such as size, allocation, type, and value for lists, bytes, floats, strings,
and integers of any size.

## Tasks

- **0-python_lists_bigO**: Big O notation of Python list operations.
- **1-python.c**: Print info about a Python list.
- **2-python.c**: Print info about Python lists and bytes objects.
- **3-python.c**: Print info about Python lists, bytes, and float objects.
- **4-python.c**: Print info about Python strings, handling both ASCII and Unicode.
- **5-python.c**: Print the value of a Python integer as a C unsigned long, detecting overflow.
- **100-python.c**: Print the value of a Python integer of any size using arbitrary-precision conversion.

## Compilation

Each task is compiled into a shared library using gcc with the flags:
`-Wall -Werror -Wextra -pedantic -std=c99 -shared -fPIC`

Example:
