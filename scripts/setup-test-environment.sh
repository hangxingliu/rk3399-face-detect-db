#!/usr/bin/env bash

# get directory this script located in
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
	DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )";
	SOURCE="$(readlink "$SOURCE")"; [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE";
done
DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

cd "$DIR/..";

FOLDER="test-env"
if [[ -e "$FOLDER" ]]; then
	if [[ ! -d "$FOLDER" ]]; then
		echo -e "\n  error: \"$FOLDER\" is not folder!";
		exit 1;
	else
		rm "$FOLDER" -rf || exit 1;
	fi
fi
mkdir -p "$FOLDER";
echo "success: created \"$FOLDER\""
exit 0;
