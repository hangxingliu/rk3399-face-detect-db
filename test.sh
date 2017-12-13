#!/usr/bin/env bash

function usage() {
	echo -e '';
	echo -e '  Usage: ./test.sh [unit|gui|once|all] [...opts]';
	echo -e '      default option: unit';
	echo -e '      more opts: ./scripts/make.sh --help';
	echo -e '';
	exit 0;
}

[[ -n `echo "$*" | gawk '/-h/||/help/'` ]] && usage;
[[ -n `uname -a | grep -e aarch64` ]] && IS_ARM="--arm" || IS_ARM="";

OTHER_OPTS="";
TYPE="";
for ARG in "$@"; do
	if [[ "$ARG" == --* ]]; then OTHER_OPTS="${OTHER_OPTS} $ARG";
	else TYPE="$ARG"; fi
done

CMAKE_TEST_UNIT="OFF";
CMAKE_TEST_GUI="OFF";
CMAKE_TEST_ONCE="OFF";
if [[ "$TYPE" == "unit" ]] || [[ -z "$TYPE" ]]; then CMAKE_TEST_UNIT="ON";
elif [[ "$TYPE" == "gui" ]]; then CMAKE_TEST_GUI="ON";
elif [[ "$TYPE" == "once" ]]; then CMAKE_TEST_ONCE="ON";
elif [[ "$TYPE" == "all" ]]; then CMAKE_TEST_UNIT="ON"; CMAKE_TEST_GUI="ON"; CMAKE_TEST_ONCE="ON";
else echo "  error: invalid test option: '$TYPE' "; exit 1;
fi

SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
	DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )";
	SOURCE="$(readlink "$SOURCE")"; [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE";
done
DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

cd "$DIR";
./scripts/make.sh test $IS_ARM -j`bash ./scripts/get-cpu-core-count.sh 8` $OTHER_OPTS \
	-DCMAKE_TEST_UNIT="$CMAKE_TEST_UNIT" -DCMAKE_TEST_GUI="$CMAKE_TEST_GUI" -DCMAKE_TEST_ONCE="$CMAKE_TEST_ONCE";
exit $?;
