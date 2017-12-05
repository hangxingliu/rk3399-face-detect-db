#!/usr/bin/env bash

URL="https://raw.githubusercontent.com/opencv/opencv/master/data/haarcascades/haarcascade_frontalface_alt2.xml"
TO_DIR="./resources"
TO_FILE="${URL##*/}";


# get directory this script located in
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
	DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )";
	SOURCE="$(readlink "$SOURCE")"; [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE";
done
DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
DIR="$(dirname "$DIR")"; # Project root path

pushd "$DIR";

echo -e "[.] downloading ..."
curl "$URL" -o "${TO_DIR}/${TO_FILE}" && echo "[+] success!" || (echo "[-] failed!"; exit 1;);
