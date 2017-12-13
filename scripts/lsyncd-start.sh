#!/usr/bin/env bash

# get directory this script located in
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
	DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )";
	SOURCE="$(readlink "$SOURCE")"; [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE";
done
DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
pushd "$DIR";
pushd "..";

function fatal() { echo "error: $1"; exit 1; }

OLD_PID=`ps -o 'pid,cmd' -A x | gawk '!/gawk/ && /lsyncd\/configurations/ {print $1;}'`;
if [[ -n "$OLD_PID" ]]; then
	echo "found old lsyncd process, killing ...";
	kill "${OLD_PID}";
	[[ "$?" != "0" ]] && fatal "could not kill old process. (pid: ${OLD_PID})!";
	echo "killed old process. (pid: ${OLD_PID})";
fi

LOG_FILE="./lsyncd/lsyncd.log";
echo "cleaning log file: ${LOG_FILE} ...";
if [[ -f "$LOG_FILE" ]]; then
	rm "${LOG_FILE}" || fatal "could not delete log file";
fi

STATUS_FILE="./lsyncd/lsyncd.status";
echo "cleaning status file: ${STATUS_FILE} ...";
if [[ -f "$STATUS_FILE" ]]; then
	rm "${STATUS_FILE}" || fatal "could not delete status file";
fi

lsyncd ./lsyncd/configurations.lua;
if [[ "$?" != "0" ]]; then
	echo "error: could not launch lsyncd, details in lsyncd log file.";
	exit 1;
fi
echo "success: lsyncd launched!";

