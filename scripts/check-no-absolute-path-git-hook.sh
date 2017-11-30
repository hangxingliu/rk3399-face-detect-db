#!/usr/bin/env bash

GREEN="\x1b[1;32m";  RED="\x1b[1;31m";  BLUE="\x1b[1;34m";  RESET="\x1b[0m";
SELF="./scripts/check-no-absolute-path-git-hook.sh";

function install_me_as_git_pre_commit_hook() {
	HOOK_FILE="./.git/hooks/pre-commit"
	echo -e "#!/usr/bin/env bash" > "${HOOK_FILE}" &&
	echo -e "bash ${SELF}" >> "${HOOK_FILE}" &&
	chmod +x "${HOOK_FILE}" &&
	echo -e "${GREEN}[+] install as git pre-commit success!${RESET}" ||
	(echo -e "\n  ${RED}error: install as git pre-commit failed${RESET}\n"; exit 1;);
}

function check() {
	find . -type f \
		-not -path "./.git/*" \
		-not -path "./build/*" \
		-not -path "${SELF}"\
		| xargs -I __file__ grep __file__ -e "/home/"\
			--with-filename --line-number\
			--initial-tab\
			--before-context=2\
			--after-context=2\
			--color=always;
}


# get directory this script located in
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
	DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )";
	SOURCE="$(readlink "$SOURCE")"; [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE";
done
DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )";
DIR="$(dirname "$DIR")";

pushd $DIR;
echo -e "[~] goto project root path";

if [[ "$1" == "install" ]]; then
	install_me_as_git_pre_commit_hook;
	exit 0;
fi

echo -e "${BLUE}[.] checking home folder absolute path in project files...${RESET}";
RESULT=`check`;
if [[ -z "$RESULT" ]]; then
	echo -e "\n${GREEN}[+] success: no any home folder absolute path in project files!${RESET}\n";
	exit 0;
else
	echo "$RESULT";
	echo -e "\n  ${RED}error: there has any home folder absolute path in project files!${RESET}\n";
	exit 1;
fi

