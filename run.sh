#!/bin/env bash
set -e
c=0; d=0; r=0; o=0
while (( $# > 0 )) && [ "${1:0:1}" == "-" ]
do
	[ "$1" == "--" ] && shift && break
	[ "${1:0:1}" != "-" ] && break
	i=1
	while [ "${1:${i}:1}" ]; do
		char=${1:${i}:1}
		if [ "$char" == "c" ]; then c=1
		elif [ "$char" == "d" ]; then d=1
		elif [ "$char" == "r" ]; then r=1
		elif [ "$char" == "o" ]; then o=1
		else
			printf "This program compiles c/cpp files using make. It also runs the \
executable file and calculates the elapsed time.
The default build uses debugging configurations.\n
./compile_and_run.sh [options] [--] [arg1 arg2 ...]\n
Options:
\t-c\tclean (deletes) already built files that have the configurations given (debug or release)
\t-r\tbuild using release configurations
\t-d\tbuild using debug configurations, and runs the debugger
\t-h\tshow this help message
\t-o\tcompile only\n
\t--\tsend the remaining parameters as arguments to main function (this should be the last argument)\n"
			[ "$char" == "h" ]
			exit
		fi
		i=$((i + 1))
	done
	shift
done
if (( c == 1 )); then
	make RELEASE=$r clean
fi
if (( d == 1 )); then
	make -j4 debug
	exit
fi
make -j4 RELEASE=$r all
if (( o == 1 )); then
	exit
fi
filename=$(make RELEASE=$r getTarget)
echo ------------------------------------------------------------
command time -f "\n--------------------------------------------------\n\
Elapsed Time: %e sec\nCPU Percentage: %P" "$filename" "$@"
cpp_file="${1::-4}cpp"
clang-format -i "$cpp_file"
output_program="${cpp_file::-4}"
make "$output_program"
echo ------------------------------------------------------------
cat "$cpp_file"
echo ------------------------------------------------------------
echo "Running your program:"
"./$output_program"
