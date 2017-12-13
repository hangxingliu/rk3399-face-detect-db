#!/usr/bin/env bash

GREEN="\x1b[1;32m"
RED="\x1b[1;31m"
BLUE="\x1b[1;34m"
RESET="\x1b[0m"

APT_INSTALL="";
FAILED="";

function checkComponent() {
	if [[ -z `which $1` ]]; then
		echo -e "${RED}[-] FATAL: \"$1\" is missing!${RESET}";
		[[ "$2" == "apt" ]] && APT_INSTALL="${APT_INSTALL} $1" || echo -e "${RED}    Details: $2${RESET}";
		FAILED="true";
	else
		echo -e "${GREEN}[~] passed: \"$1\" is installed!${RESET}";
	fi
}

echo -e "\n[.] checking required components for compiling ...";

checkComponent "jq" apt;
checkComponent "gawk" apt;
checkComponent "ctags" "https://github.com/universal-ctags/ctags.git";
checkComponent "cmake" apt;
checkComponent "g++" apt;

if [[ "$FAILED" == "true" ]]; then
	echo -e "\n${RED}error: some components are missing!${RESET}";
	[[ -n "$APT_INSTALL" ]] && echo -e "${RED}        sudo apt install ${APT_INSTALL};${RESET}";
	echo -e "";
	exit 1;
fi
echo -e "${BLUE}[+] all component are installed!${RESET}\n"
