# ZINC
ZINC Is a Normal Compiler

## Development Instructions
* Before you start, run `make init` to generate the directories needed. `src` directory should only contain source files while `include` directory should only contain header files. You can make multiple directories with any structure you want.
* `g++` is used in the `Makefile`, along with `gdb` for debugging.
* To compile and run the program, run `./run.sh` for debugging mode, and `./run.sh -r` for release mode. See `./run.sh -h` for more options.
