# ZINC
ZINC Is a Naughty Compiler

## Development Instructions
* Before you start, run `make init` to generate the directories needed. `src` directory should only contain source files while `include` directory should only contain header files. You can make multiple directories with any structure you want.
* `clang` is used in the `Makefile`, along with `lldb` for debugging.
* To compile and run the program, run `./fbr` for debugging mode, and `./fbr -r` for release mode. See `./fbr -h` for more options.
