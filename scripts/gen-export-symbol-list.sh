#!/usr/bin/env bash

EXPORT_FROM="../src/api/api.hpp";
LIST_TO="../src/export_symbols.map";

# get directory this script located in
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
	DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )";
	SOURCE="$(readlink "$SOURCE")"; [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE";
done
DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )";

pushd "$DIR";

function parse() {
	# --kinds-C++=+p --language-force=C++ --extras=+q
	# these options for fully function list bewteen #ifndef and #endif
	ctags -o - --kinds-C++=+p --language-force=C++ --extras=+q --output-format=xref "$EXPORT_FROM" |
		awk '$2=="prototype" {print "\t\t" $1 ";"}' |
		awk 'BEGIN { print "{\n\tglobal:"; }
			{ print $0; }
			END { print "\n\tlocal: *;\n};"; }';
}


parse > "$LIST_TO";
if [[ "$?" != "0" ]]; then
	echo -e "\n  error: generate failed!\n";
	exit 1;
else
	echo -e "\n  success!\n"
fi

