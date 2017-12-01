#!/usr/bin/env bash

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

GENERATOR="./scripts/gen-hpp-util.sh";

pushd $DIR;
echo -e "[~] goto project root path";

NAME="API_API_HPP";
HPP="./src/api/api.hpp";
EXTERNC="--extern-c"
echo -e "[.] generating header: ${NAME} into ${HPP}..."
$GENERATOR --name="${NAME}" --output="${HPP}" ${EXTERNC}\
	 --input="./src/api/api.cc"\
	&& echo "[~] generated!" || fatal "generate failed!";


NAME="CONFIG_METHODS_HPP";
HPP="./src/config/config_methods.hpp";
EXTERNC="--extern-c"
echo -e "[.] generating header: ${NAME} into ${HPP}..."
$GENERATOR --name="${NAME}" --output="${HPP}" ${EXTERNC}\
	 --input="./src/config/config_methods.cc"\
	&& echo "[~] generated!" || fatal "generate failed!";


echo "[+] all header files are generated!";
