#!/usr/bin/env bash

# get directory this script located in
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
	DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )";
	SOURCE="$(readlink "$SOURCE")"; [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE";
done
DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

LIB_SRC="$DIR/../src"
LIB_TARGET_DEBUG="$DIR/../build/debug"
LIB_TARGET_RELEASE="$DIR/../build/release"

TEST_SRC="$DIR/../test"
TEST_TARGET_DEBUG="$DIR/../build/test-debug"
TEST_TARGET_RELEASE="$DIR/../build/test-release"
TEST_EXECUTE_FILE_EXT="test.bin";

GREEN="\x1b[1;32m"
RED="\x1b[1;31m"
BLUE="\x1b[1;34m"
RESET="\x1b[0m"
function usage() {
	echo '';
	echo '  Usage: ./scripts/build.sh <Action> [BuildType]';
	echo '';
	echo '  Action:';
	echo '    cmake [BuildType] :  clean build folder and redo cmake command';
	echo '    build [BuildType] :  build target files from make file generated by cmake';
	echo '    test [BuildType] :   build unit test codes and execute each unit test';
	echo '    clean-all :          just remove build folder';
	echo '';
	echo '  BuildType:';
	echo '    debug, release';
	echo '';
	echo '  Examples:';
	echo '';
	echo '    ./scripts/build.sh cmake DEBUG';
	echo '    ./scripts/build.sh build TEST';
	echo '';
	exit 0;
}
function gotoDirectory() {
	if [[ -e "$1" ]]; then
		if [[ -d "$1" ]]; then
			pushd "$1";
		else
			fatal "\"$1\" is not a directory!";
		fi
	else
		mkdir -p "$1";
		if [[ "$?" == "0" ]]; then
			pushd "$1";
		else
			fatal "create directory \"$1\" failed!";
		fi
	fi
}
function fatal() { echo -e "\n  ${RED}error: $1$RESET\n"; exit 1; }
function finish() { echo -e "\n  ${GREEN}success: $1$RESET\n";}
function removeIfExisted() {
	for f in "$@"; do
		if [[ -d "$f" ]]; then rm -rf "$f" || fatal "remove directory \"$f\" failed!"; fi
	done
}
function executeCMake() {
	# $1 BUILD_TYPE $2 SOURCE_FOLDER $3 IS_TEST_PROJ
	IS_TEST="";  if [[ -n "$3" ]]; then IS_TEST="(test)"; fi
	cmake -DCMAKE_BUILD_TYPE="$1" "$2";
	CMAKE_RESULT=$?;
	[[ "$CMAKE_RESULT" == "0" ]] && finish "cmake for $1$IS_TEST" ||
		fatal "cmake for $1$IS_TEST failed!";
}
function executeMakeBuild() {
	# $1 BUILD_TYPE $2 IS_TEST_PROJ
	IS_TEST="";  if [[ -n "$2" ]]; then IS_TEST="(test)"; fi
	make;
	MAKE_RESULT=$?;
	[[ "$MAKE_RESULT" == "0" ]] && finish "building by make for $1$IS_TEST" ||
		fatal "building by make for $1$IS_TEST failed!";
}
function executeUnitTest() {
	TEST_FILE="$1";
	TEST_ALL=$(($TEST_ALL+1));
	TEST_NAME="${TEST_FILE%.${TEST_EXECUTE_FILE_EXT}}";
	echo -e "${BLUE}[.] testing ${TEST_NAME} ...${RESET}";

	"./$TEST_FILE" # execute test binary file
	TEST_RESULT="$?";
	if [[ "$TEST_RESULT" == "0" ]]; then
		TEST_PASSED=$(($TEST_PASSED+1));
		echo -e "${GREEN}[~] passed!${RESET}";
	else
		echo -e "\n${RED}[-] test ${TEST_NAME} failed (exit with ${TEST_RESULT})${RESET}\n";
	fi
}


# >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#   M a i n    >>>>>>>>>>>>>>>>>
# >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

ACTION="$1"
BUILD_TYPE="$2"

# ===================================
#       Checking  Arguments
# ===================================
IS_HELP=`echo "$*" | grep -e "-h"`
[[ -n "$IS_HELP" ]] && usage;
[[ "$ACTION" == "help" ]] && usage;

[[ -z "$ACTION" ]] && fatal "Action is missing! (usage: --help)";
[[ "$ACTION" != "cmake" ]] && [[ "$ACTION" != "build" ]] &&
[[ "$ACTION" != "test" ]] && [[ "$ACTION" != "clean-all" ]] &&
	fatal "Invalid Action! (usage: --help)";

if [[ "$ACTION" != "clean-all" ]]; then
	[[ -z "$BUILD_TYPE" ]] && fatal "BuildType is missing! (usage: --help)";
	[[ "$BUILD_TYPE" != "debug" ]] && [[ "$BUILD_TYPE" != "release" ]] &&
		fatal "Invalid BuildType! (usage: --help)";
fi
# ===================================

if [[ "$ACTION" == "clean-all" ]]; then
	removeIfExisted "$LIB_TARGET_DEBUG";
	removeIfExisted "$LIB_TARGET_RELEASE";
	removeIfExisted "$TEST_TARGET_DEBUG";
	removeIfExisted "$TEST_TARGET_RELEASE";

	finish "cleaned all build target directories!";
	exit 0;
fi


# =======================
#     Get target path
# =======================
TARGET_PATH="$LIB_TARGET_DEBUG"
if [[ "$BUILD_TYPE" == "release" ]]; then
	if [[ "$ACTION" == "test" ]]; then
		TARGET_PATH="$TEST_TARGET_RELEASE";
	else
		TARGET_PATH="$LIB_TARGET_RELEASE";
	fi
elif [[ "$ACTION" == "test" ]]; then
	TARGET_PATH="$TEST_TARGET_DEBUG";
fi
gotoDirectory "$TARGET_PATH";

if [[ "$ACTION" == "cmake" ]]; then
	executeCMake $BUILD_TYPE $LIB_SRC;
	exit 0;
fi

if [[ "$ACTION" == "build" ]]; then
	if [[ ! -f "CMakeCache.txt" ]]; then
		echo -e "[.] executing cmake ...";
		executeCMake $BUILD_TYPE $LIB_SRC;
	fi
	executeMakeBuild $BUILD_TYPE;
	exit 0;
fi

if [[ "$ACTION" == "test" ]]; then
	executeCMake $BUILD_TYPE $TEST_SRC "test";
	executeMakeBuild $BUILD_TYPE "test";

	TEST_ALL=0; TEST_PASSED=0;
	ls | grep "$TEST_EXECUTE_FILE_EXT$" | awk '{print $0;} END {print "EOF";}' |
		while read TEST_FILE; do
			if [[ "$TEST_FILE" == "EOF" ]]; then
				echo -e "\n Test result: (Passed: ${TEST_PASSED} / All: ${TEST_ALL})\n";
				if [[ "$TEST_ALL" != "$TEST_PASSED" ]]; then
					fatal "Some test failed!";
				fi
				finish "All unit tests are passed";
			else
				executeUnitTest $TEST_FILE;
			fi
		done

	exit $?;
fi

# <<<<<<<<<<<<<<<<<<<<<<<<<<
#   Main  <<<<<<<<<<<<<<<<<<
# <<<<<<<<<<<<<<<<<<<<<<<<<<

