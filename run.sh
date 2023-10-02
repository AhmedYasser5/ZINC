#!/usr/bin/env bash
for program in clang-format make time
do
	if ! which "$program" >/dev/null
	then
		printf "The program %s is required" "$program"
		exit 1
	fi
done
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
			printf "This script compiles cpp files using make. It also runs the \
executable file and calculates the elapsed time.
The default build uses debugging configurations.\n
Use this script to compile ZINC transcompiler (if not compiled), extract cpp \
code from ZINC file, compile it, and finally run the executable.\n
./run.sh [options] [--] [arg1 arg2 ...]\n
Options:
\t-c\tclean (delete) already built files with the configurations given (debug or release)
\t-r\tbuild using release configurations
\t-d\tbuild using debug configurations, and run the debugger
\t-o\tcompile only
\t-h\tshow this help message\n
\t--\tsend the remaining parameters as arguments to main function (this should be the last argument)
\t\tNote: this can be omitted if the first argument does not start with \'-\'\n"
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
printf -- "------------------------------------------------------------\n"
command time -f "\n------------------------------------------------------------\n\
Elapsed Time: %e sec\nCPU Percentage: %P" "$filename" "$@"
cpp_file="${1::-4}cpp"
clang-format -i "$cpp_file"
output_program="${cpp_file::-4}.exe"
make "$output_program"
printf -- "------------------------------------------------------------\n"
cat "$cpp_file"
printf -- "------------------------------------------------------------\nRunning your program:\n"
"./$output_program"
