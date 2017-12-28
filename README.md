# Dependencies

## Compiling

* Make
* gcc

## Development

* indent
* GDB
* Valgrind
* clang (scan-build)
* clang-tidy

# Make targets

## make

* Compiles the main executables under `main/*.elf`
* Runs static code analyzers (linters) on the source

## make format

Formats your source code to comply with the GNU style.

## make check

* Runs unit tests
* Applies GDB to produce backtrace in case of error to help debug
* Applies Valgrind to find memory leaks

## make clean

Deletes temporary files produced by the other targets.

# Directory hierarchy

## libmain

Main implementation of the solutions to the given problems.

## main

Single line wrappers to produce executables.
The sole responsibility here is to invoke the respective implementation
under `libmain`.

## test

Test cases for `libmain`.

## lib

Reusable utility functions that should be accessible from anywhere
in the project.

## libtest

Reusable utility functions that should be accessible from tests.

## var

Temporary files.

# Problem statement

## 1. cat

* Arguments: input file, output file
* Processing: Read the input byte by byte and write it to the output.
* Example in: aaa
* Example out: aaa

## 2. unique

* Arguments: input file, output file
* Preconditions: Bytes of the input file form a sorted sequence.
* Processing: Only collect the unique bytes.
* Example in: abbbdee
* Example out: abde

## 3. zipper

* Arguments: input1, input2, output
* Processing: Output the first byte of input1, then the first byte of
  input2, then the second byte of input1, second byte of intput2, etc.
* Example in1: cba
* Example in2: XYZ
* Example out: cXbYaZ

## 4. merge

* Arguments: input1, input2, output
* Preconditions: Bytes of each input forms a sorted sequence.
* Processing: produce the sorted sequence of bytes that are contained
  in any one of the inputs.
* Example in1: adef
* Example in2: abdg
* Example out: abdefg
