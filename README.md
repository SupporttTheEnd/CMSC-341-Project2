# SatNet Implementation

## Overview
This repository contains the implementation of the SatNet class in C++. SatNet is a class that represents a network of satellites and provides functionalities to insert, remove, find satellites, and perform various operations on them.

## Files
- `satnet.h` and `satnet.cpp`: These files contain the implementation of the SatNet class, including constructors, destructor, methods for inserting, removing, finding satellites, and managing the tree structure.
- `mytest.cpp`: This file provides test cases to demonstrate the functionality of the SatNet class.
- `Makefile`: Contains instructions for compiling the project.

## How to Use
1. Include `satnet.h` in your C++ project.
2. Create an instance of the `SatNet` class.
3. Insert satellites into the network using the `insert` method.
4. Perform various operations such as removing satellites, setting states, counting satellites, etc.
5. Compile the project using the provided Makefile instructions.

## Compilation
To compile the project, you can use the provided Makefile. Use the following commands:
- `make p`: Compiles `mytest.cpp` with `satnet.o` to create an executable named `proj2`.
- `make b`: Runs `gdb` for debugging purposes.
- `make v`: Runs `valgrind` to check for memory leaks.
- `make r`: Runs the executable `proj2`.

## Cleaning Up
To clean up object files and executables, you can use:
- `make clean`: Removes object files and temporary files.
