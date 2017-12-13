#!/usr/bin/env bash

GENERATOR="./scripts/gen-hpp-util.sh";
CONFIG_FILE="./auto-headers.json";
CONFIG_CACHE_FILE="./.auto-headers-lock.json"

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

[[ -n `echo "$*" | gawk '/-q/||/quiet/'` ]] && QUIET=true || QUIET=false;
[[ -n `echo "$*" | gawk '/-f/||/force/'` ]] && FORCE_REFRESH=true || FORCE_REFRESH=false;

[[ "$FORCE_REFRESH" == "true" ]] && echo "Force mode: on (ignore cache)";

cd $DIR;

CONFIG_CONTENT=`cat $CONFIG_FILE`;
if [[ -f "$CONFIG_CACHE_FILE" ]]; then
	CACHE_CONTENT=`cat $CONFIG_CACHE_FILE`;
	[[ "$FORCE_REFRESH" == "true" ]] && CACHE_CONTENT="[]";
else
	CACHE_CONTENT="[]";
fi

# paramter => $1: json selector
function json() { echo "$CONFIG_CONTENT" | jq "$1" -r; }
# paramter => $1: config id
function isMatchConfigCache() {
	_CFG=`echo "$CONFIG_CONTENT" | jq ".[$1]" | md5sum | gawk '{print $1;}'`;
	_CACHE=`echo "$CACHE_CONTENT" | jq ".[$1]" | md5sum | gawk '{print $1;}'`;
	[[ "$_CFG" == "$_CACHE" ]] && echo "true" || echo "false";
}

CONFIG_LEN=`json 'length'`;
[[ "$QUIET" == "true" ]] || echo "[~] found $CONFIG_LEN configurations in ${CONFIG_FILE}";

index=0;
while [[ $index -lt $CONFIG_LEN ]]; do
    CONFIG_NAME=`json .[${index}].name`;
	HPP=`json .[${index}].hpp`;
	EXTERNC=`json .[${index}].\"extern-c\"`;
	INPUT=`json .[${index}].input[] | gawk '{printf("--input=" $0 " ")}'`
	INPUT_MD5=`json .[${index}].input[] | xargs -I _f md5sum _f`;
	INCLUDE=`json .[${index}].include[] | gawk '{printf("--include=" $0 " ")}'`

	[[ "$QUIET" == "true" ]] || echo "[.] generating header ${CONFIG_NAME} into ${HPP} ...";

	# ignore if matched config cache and file content cache
	if [[ "$FORCE_REFRESH" == "false" ]]  && [[ -f "$HPP" ]]; then
		MD5SUM_ALL=`echo "$INPUT_MD5" | wc -l`;
		MD5SUM_MATCHED=`echo "$INPUT_MD5" | gawk '{print $1}' |
			xargs -I _regexp grep "$HPP" -e _regexp | wc -l`;

		if [[ "$MD5SUM_ALL" == "$MD5SUM_MATCHED" ]] &&
			[[ `isMatchConfigCache "$index"` == "true" ]]; then

			[[ "$QUIET" == "true" ]] || echo -e "[~] keep old header file because it is not modified.\n";

			index=$[$index+1];
			continue;
		fi
	fi

	if [[ "$EXTERNC" == "true" ]]; then EXTERNC="--extern-c";
	else EXTERNC=""; fi
	COMMAND="${GENERATOR} --name=${CONFIG_NAME} --output=${HPP} ${EXTERNC} ${INPUT} ${INCLUDE}";

	$COMMAND && echo "${INPUT_MD5}" | gawk 'BEGIN {print "\n// md5sum:";} {print "// " $0;}' >> "$HPP";

	if [[ "$?" == "0" ]]; then
		[[ "$QUIET" == "true" ]] || echo -e "[~] generated!\n";
	else
		fatal "generate failed!";
	fi

	index=$[$index+1];
done

if [[ "$?" == "0" ]]; then
	echo "$CONFIG_CONTENT" > "$CONFIG_CACHE_FILE";
	echo "[+] all header files are generated!";
else
	exit 1;
fi
