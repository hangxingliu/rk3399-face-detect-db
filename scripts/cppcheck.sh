#!/usr/bin/env bash

# get directory this script located in
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
	DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )";
	SOURCE="$(readlink "$SOURCE")"; [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE";
done
DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

cd $DIR/..;

GREEN="\x1b[1;32m";
RED="\x1b[1;31m";
YELLOW="\x1b[1;33m";
BLUE="\x1b[1;34m";
RESET="\x1b[0m";
function fatal() { echo -e "\n  ${RED}error: $1$RESET\n"; exit 1; }
function colorable() {
	gawk \
		-vERROR="$RED"\
		-vWARNING="$YELLOW"\
		-vNORMAL="\x1b[33m"\
		-vIGNORE="\x1b[90m"\
		-vRESET="$RESET"\
		'$2 == "(error)"{ print ERROR $0 RESET; next;}
		$2 == "(warning)"{ print WARNING $0 RESET; next;}
		/never used/ || /Unused/ { print IGNORE $0 RESET; next; }
		{ print NORMAL $0 RESET;}';
}

CPPCHECK="cppcheck";
[[ -z `which $CPPCHECK` ]] && fatal "missing \"$CPPCHECK\" ";

[[ -n `echo "$*" | gawk '/test/'` ]] && PROJ="." || PROJ="src";
$CPPCHECK "$PROJ" --enable=all --quiet --force 2>&1 | colorable;
