#!/usr/bin/env bash

GENERATOR="./scripts/gen-hpp-util.sh";
CONFIG_FILE="./AutoHeaders.json";

# get directory this script located in
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
	DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )";
	SOURCE="$(readlink "$SOURCE")"; [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE";
done
DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )";
DIR="$(dirname "$DIR")"; # Project root path

# =====================
function fatal() { echo -e "\n  error: $1\n"; exit 1;}

[[ -z `which jq` ]] && fatal "could not find denpedency: \"jq\" (sudo apt install jq) <https://stedolan.github.io/jq/>";

pushd $DIR;
echo -e "[~] goto project root path";

CONFIG_CONTENT=`cat $CONFIG_FILE`;
# paramter => $1: json selector
function json() { echo "$CONFIG_CONTENT" | jq "$1" -r; }

CONFIG_LEN=`json 'length'`;
echo "[~] found $CONFIG_LEN configurations in ${CONFIG_FILE}";

index=0;
while [[ $index -lt $CONFIG_LEN ]]; do
    CONFIG_NAME=`json .[${index}].name`;
	HPP=`json .[${index}].hpp`;
	EXTERNC=`json .[${index}].\"extern-c\"`;
	INPUT=`json .[${index}].input[] | awk '{printf("--input=" $0 " ")}'`
	INCLUDE=`json .[${index}].include[] | awk '{printf("--include=" $0 " ")}'`

	echo "[.] generating header ${CONFIG_NAME} into ${HPP} ...";

	if [[ "EXTERNC" == "true" ]]; then EXTERNC="--extern-c";
	else EXTERNC=""; fi
	COMMAND="${GENERATOR} --name=${CONFIG_NAME} --output=${HPP} ${EXTERNC} ${INPUT} ${INCLUDE}";

	$COMMAND && echo -e "[~] generated!\n" || fatal "generate failed!";

	index=$[$index+1];
done

if [[ "$?" == "0" ]]; then
	echo "[+] all header files are generated!";
else
	exit 1;
fi
