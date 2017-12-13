#!/usr/bin/env bash

function usage() {
	echo -e '';
	echo -e '  Usage: ./build.sh ...options';
	echo -e '      options: ./scripts/make.sh --help';
	echo -e '';
	exit 0;
}

[[ -n `echo "$*" | gawk '/-h/||/help/'` ]] && usage;

[[ -n `uname -a | grep -e aarch64` ]] && IS_ARM="--arm" || IS_ARM="";


SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
	DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )";
	SOURCE="$(readlink "$SOURCE")"; [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE";
done
DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

cd "$DIR";
./scripts/make.sh build $IS_ARM -j`bash ./scripts/get-cpu-core-count.sh 8` $@;
exit $?;
