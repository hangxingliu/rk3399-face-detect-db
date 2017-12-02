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
	INPUT_MD5=`json .[${index}].input[] | xargs -I _f md5sum _f`;
	INCLUDE=`json .[${index}].include[] | awk '{printf("--include=" $0 " ")}'`

	echo "[.] generating header ${CONFIG_NAME} into ${HPP} ...";

	if [[ -f "$HPP" ]]; then
		MD5SUM_ALL=`echo "$INPUT_MD5" | wc -l`;
		MD5SUM_MATCHED=`echo "$INPUT_MD5" | awk '{print $1}' |
			xargs -I _regexp grep "$HPP" -e _regexp | wc -l`;

		if [[ "$MD5SUM_ALL" == "$MD5SUM_MATCHED" ]]; then
			echo -e "[~] keep old header file because it is not modified.\n";

			index=$[$index+1];
			continue;
		fi
	fi

	if [[ "$EXTERNC" == "true" ]]; then EXTERNC="--extern-c";
	else EXTERNC=""; fi
	COMMAND="${GENERATOR} --name=${CONFIG_NAME} --output=${HPP} ${EXTERNC} ${INPUT} ${INCLUDE}";

	$COMMAND &&
		echo "${INPUT_MD5}" |
			awk 'BEGIN {print "\n// md5sum:";} {print "// " $0;}' >> "$HPP"  &&
		echo -e "[~] generated!\n" || fatal "generate failed!";

	index=$[$index+1];
done

if [[ "$?" == "0" ]]; then
	echo "[+] all header files are generated!";
else
	exit 1;
fi
