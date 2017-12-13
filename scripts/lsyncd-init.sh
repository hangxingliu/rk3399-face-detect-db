#!/usr/bin/env bash

WORKSPACE="../lsyncd";

# get directory this script located in
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
	DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )";
	SOURCE="$(readlink "$SOURCE")"; [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE";
done
DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
pushd "$DIR";

GREEN="\x1b[1;32m";
RED="\x1b[1;31m";
BLUE="\x1b[1;34m";
RESET="\x1b[0m";
function fatal() { echo -e "\n  ${RED}error: $1$RESET\n"; exit 1; }

[[ -z `which lsyncd` ]] && fatal "please install \"lsyncd\" first.";

[[ -e "$WORKSPACE" ]] && fatal "${WORKSPACE} is existed! (please remove it before initialize)";
mkdir -p "$WORKSPACE" || fatal "Could not create directory: ${WORKSPACE}"
pushd "$WORKSPACE";


DEFAULT_HOSTNAME="firefly";
read -p "Enter hostname(default is \"$DEFAULT_HOSTNAME\") > " CFG_HOSTNAME;
[[ -z "$CFG_HOSTNAME" ]] && CFG_HOSTNAME="$DEFAULT_HOSTNAME";

DEFAULT_TARGET="./Projects/FaceDB";
read -p "Enter target dirname(default is \"$DEFAULT_TARGET\") > " CFG_TARGET;
[[ -z "$CFG_TARGET" ]] && CFG_TARGET="$DEFAULT_TARGET";

DEFAULT_KEY="$HOME/.ssh/id_rsa";
read -p "Enter ssh key file(default is \"$DEFAULT_KEY\") > " CFG_KEY;
[[ -z "$CFG_KEY" ]] && CFG_KEY="$DEFAULT_KEY";

DEFAULT_KNOWN_HOSTS="$HOME/.ssh/known_hosts";
read -p "Enter know_hosts file(default is \"$DEFAULT_KNOWN_HOSTS\") > " CFG_KNOWN_HOSTS;
[[ -z "$CFG_KNOWN_HOSTS" ]] && CFG_KNOWN_HOSTS="$DEFAULT_KNOWN_HOSTS";

echo -e "";
echo -e "Hostname:        ${CFG_HOSTNAME}";
echo -e "Target:          ${CFG_TARGET}";
echo -e "SSH Key:         ${CFG_KEY}";
echo -e "SSH Known hosts: ${CFG_KNOWN_HOSTS}";

read -p "Are you sure? (yes/no) > " CONFIRMED;
if [[ "$CONFIRMED" != "yes" ]]; then
	echo -e "${RED}cancelled by user!${REST}";
	exit 0;
fi

CFG_DIR=`pwd`;
CFG_EXCLUDE='".*", "build\-*", "demo", "docs", "resources", "test\-env", "lsyncd", "*.autosave"';
CONF="-- # generated date: $(date '+%y/%m/%d %H:%M')
settings {
	logfile = \"${CFG_DIR}/lsyncd.log\",
	statusFile = \"${CFG_DIR}/lsyncd.status\"
}
sync {
	default.rsyncssh,
	delay = 1,
	exclude =  {${CFG_EXCLUDE}},
	source = \".\",
	host = \"${CFG_HOSTNAME}\", targetdir = \"${CFG_TARGET}\",
	rsync = {
		archive = true, compress = false, whole_file = false,
		rsh = \"ssh -p 22 -i ${CFG_KEY} -o UserKnownHostsFile=${CFG_KNOWN_HOSTS}\"
	}
}
";

CFG_LUA_FILE="${PWD}/configurations.lua";
echo "$CONF" > ${CFG_LUA_FILE};
[[ "$?" == "0" ]] && echo -e "\n  ${GREEN}success: configuration file: ${CFG_LUA_FILE}${RESET}\n" ||
	fatal "write configuration to ${CFG_LUA_FILE} failed!";

