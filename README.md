# Toy Compression

This directory contains some toy header-only implementations of compression
code, developed in a test-driven manner. Right now, there are some
implementations of simple integer coders, for unary code, truncated binary code,
elias gamma and delta codes, Golomb and Rice codes, a ZigZag encoder to allow
encoding negative numbers and zero in those codes that otherwise can't, and a
variable-length integer encoder that's similar to the varints used by git.
There's no real documentation, but you can probably figure out how to use
the code by reading the test/test.cpp file and the headers it includes.

The toy_test/toy_test.hpp file contains a simple unit test framework that is
used in this project - I started out using Catch 2, but the long compile times
(almost 5 minutes when I only had the bit reader and bit writer implemented!)
deterred me, and I don't need even a tenth of its features.

The source-code.pdf file has the source code for all of the .cpp and .hpp files
in the project, except for those in the gsl-lite/ directory (which is a
submodule pointing to https://github.com/martinmoene/gsl-lite). Don't forget
to run `git submodule init` and `git submodule update` to update the submodules,
or use the `--recurse-submodules` option to `git clone`.

I'm running clang-tidy on my code, with no warnings or errors. I'm using a
LOT of the checks available in clang-tidy, including several groups of
clang static analyzer checks.

To compile and test the coders under Linux or Cygwin do the following:

```bash
mkdir build
cd build
cmake .. && make test
```

To do this under mingw-w64/msys2, you need to change the commands to:

```bash
mkdir build
cd build
cmake .. -G'Unix Makefiles' && make test
```

This requires that you have gcc 7 or 8 (for working C++17 support and
working gcov), gcov, cmake, lcov, awk, bash, and a web browser installed,
and it will run the test suite and generate a code coverage report. I'm
filtering the branch coverage to remove the stray branches inserted by the
compiler (I only care about branches I created myself), and also to remove
the test framework, gsl-lite, and the test cases themselves from the output.
Right now, I have 100% line, function, and branch coverage. I intend to keep
it that way.

The references below are in roughly the order that I'm planning on using them
as sources of inspiration for the code in this repository. I also intend to
only implement algorithms that are patent-free.

## References

1. Moffat, Alistair and Andrew Turpin. _Compression and Coding Algorithms._
New York: Kluwer Academic Publishers, 2002.

2. Salomon, David and Giocanni Motta. _Handbook of Data Compression, Fifth
Edition._ London: Springer-Verlag London Limited, 2010. Previous editions
published as _Data Compression: The Complete Reference_.

3. Wayner, Peter. _Compression Algorithms for Real Programmers._
San Diego, CA: Academic Press/Morgan Kaufmann, 2000.

4. Nelson, Mark and Jean-Loup Gailly. _The Data Compression Book,
Second Edition._ New York: M&T Books, 1996.

5. Pu, Ida M. _Fundamental Data Compression._ Burlington, MA:
Butterworth-Heinemann/Elsevier, 2006.

## License ![CC0](https://licensebuttons.net/p/zero/1.0/88x31.png)

This work is licensed under the terms of the CC0-1.0 Universal
license, or may be considered to be in the public domain,
at your option.
