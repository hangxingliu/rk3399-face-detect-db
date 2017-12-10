#!/usr/bin/env bash

CONFIG_FILE="../test.config.json";
TARGET_CMAKELIST="../test/CMakeLists.txt";
UNIT_TEST="../test/unit";
GUI_TEST="../test/gui";
ONCE_TEST="../test/once";

# get directory this script located in
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
	DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )";
	SOURCE="$(readlink "$SOURCE")"; [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE";
done
DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )";

# =====================
function fatal() { echo -e "\n  error: $1\n"; exit 1;}
function findCSources() { find "$1" -type f \( -name "*.cc" -o -name "*.cpp" -o -name "*.c" \); }

[[ -z `which jq` ]] && fatal "could not find denpedency: \"jq\" (sudo apt install jq) <https://stedolan.github.io/jq/>";

cd $DIR;

[[ ! -f "$CONFIG_FILE" ]] && fatal "${CONFIG_FILE} is missing!";

CONFIG=`cat "${CONFIG_FILE}"`;
CMAKE="";
function queryConfig() {
	if [[ "$2" == "raw" ]]; then echo "$CONFIG" | jq -r "$1";
	else echo "$CONFIG" | jq "$1"; fi
}
function appendCMake() { CMAKE="${CMAKE}$*\n"; }

appendCMake '# 888       888        d8888 8888888b.  888b    888 8888888 888b    888  .d8888b.   888 888 888'
appendCMake '# 888   o   888       d88888 888   Y88b 8888b   888   888   8888b   888 d88P  Y88b  888 888 888'
appendCMake '# 888  d8b  888      d88P888 888    888 88888b  888   888   88888b  888 888    888  888 888 888'
appendCMake '# 888 d888b 888     d88P 888 888   d88P 888Y88b 888   888   888Y88b 888 888         888 888 888'
appendCMake '# 888d88888b888    d88P  888 8888888P"  888 Y88b888   888   888 Y88b888 888  88888  888 888 888'
appendCMake '# 88888P Y88888   d88P   888 888 T88b   888  Y88888   888   888  Y88888 888    888  Y8P Y8P Y8P'
appendCMake '# 8888P   Y8888  d8888888888 888  T88b  888   Y8888   888   888   Y8888 Y88b  d88P   "   "   " '
appendCMake '# 888P     Y888 d88P     888 888   T88b 888    Y888 8888888 888    Y888  "Y8888P88  888 888 888'
appendCMake "# ";
appendCMake "# ";
appendCMake "# This CMakeLists file is generated by script.";
appendCMake "# DON'T modify manually!";
appendCMake "";

appendCMake 'cmake_minimum_required(' 'VERSION'\
 `queryConfig '.CMakeVersion' raw` ')\n';
appendCMake 'project(' `queryConfig '.Name' raw` ')\n';

appendCMake 'set( CMAKE_CXX_FLAGS' `queryConfig '.CXXFlags'` ')';
appendCMake 'set( CMAKE_CXX_FLAGS_DEBUG' \
	`queryConfig '.XCCFlags_Debug'` ')';
appendCMake 'set( CMAKE_CXX_FLAGS_DEBUG' \
	`queryConfig '.XCCFlags_Release'` ')\n';

DIRS=`queryConfig '.LinkDirectories[]' | awk '{print "    " $0;}'`;
CMAKE="${CMAKE}link_directories(\n${DIRS} )\n";

PACKAGES=`queryConfig '.Packages[]' raw |
	awk '{print "find_package( " $0 " REQUIRED )"}'`;
CMAKE="${CMAKE}${PACKAGES}\n";

COMPILE_WITH_FILES=`queryConfig '.With[]' | awk '{print "    " $0;}'`;
CMAKE="${CMAKE}file(GLOB_RECURSE CompileWithFiles . \n${COMPILE_WITH_FILES} )\n\n"

# For ARM
appendCMake 'option(CMAKE_FOR_ARM "Set this variable to ON if you compile this project on arm board" OFF)';
appendCMake "if (CMAKE_FOR_ARM)";
appendCMake '    message("CMake for aarch64(arm) platform ...")';
appendCMake '    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFOR_ARM")';
appendCMake "endif()\n";


function generateCMakeFromFile() {
	awk -vext="$1" '{
		file=$0;
		fileForCMake=file; gsub("../test/", "./", fileForCMake);

		name=""; soName=""; with=" ${CompileWithFiles}";
		# links=[];
		linkLen=0;
		while(getline line < file > 0) {
			if(match(line, /^\/\/\/\s+---/)) break;
			if(!match(line, /^\/\/\/\s+(\w+):\s+(.+)\s*/, r)) continue;
			switch(r[1]) {
				case "TestEntry": name=r[2] ext; break;
				case "SharedEntry": soName=r[2]; break;
				case "With":
					addWith=r[2];

					# extract glob
					if(index(addWith, "*") > 0) {
						comment="# " addWith "\n    ";

						command="GLOB=\"" addWith "\";echo $GLOB;";
						command | getline addWith;
						close(command);
						gsub(/\s+/, "\n    ", addWith);
						addWith=comment addWith;
					}

					with=with "\n    " addWith;
					break;
				case "Link": links[linkLen++]=r[2]; break;
			}
		}
		if(name) {
			print "add_executable( " name " " fileForCMake with " )";
			for(i=0;i<linkLen;i++)
				print "target_link_libraries( " name " " links[i] " )";
		}
		if(soName)
			print "add_library( " soName " SHARED " fileForCMake with " )";

		if(name || soName) print "";
	}';
}

UNIT_TEST_CFG=`findCSources "$UNIT_TEST" | generateCMakeFromFile "_unit.test"`;
GUI_TEST_CFG=`findCSources "$GUI_TEST" | generateCMakeFromFile "_test.gui"`;
ONCE_TEST_CFG=`findCSources "$ONCE_TEST" | generateCMakeFromFile "_once.test"`;


appendCMake "\n# ======================";
appendCMake "# Unit tests:";
appendCMake 'option(CMAKE_TEST_UNIT "Enable compile unit tests" OFF)';
appendCMake "if (CMAKE_TEST_UNIT)";
appendCMake 'message ( STATUS "Compile: enabled CMAKE_TEST_UNIT")';
CMAKE="${CMAKE}${UNIT_TEST_CFG}\n";
appendCMake "endif()\n\n";


appendCMake "\n# ======================";
appendCMake "# GUI tests:";
appendCMake 'option(CMAKE_TEST_GUI "Enable compile gui tests" OFF)';
appendCMake "if (CMAKE_TEST_GUI)";
appendCMake 'message ( STATUS "Compile: enabled CMAKE_TEST_GUI")';
CMAKE="${CMAKE}${GUI_TEST_CFG}\n";
appendCMake "endif()\n\n";

appendCMake "\n# ======================";
appendCMake "# Once tests:";
appendCMake 'option(CMAKE_TEST_ONCE "Enable compile once tests" OFF)';
appendCMake "if (CMAKE_TEST_ONCE)";
appendCMake 'message ( STATUS "Compile: enabled CMAKE_TEST_ONCE")';
CMAKE="${CMAKE}${ONCE_TEST_CFG}\n";
appendCMake "endif()\n\n";

[[ "$1" == "--echo" ]] && echo -e "$CMAKE";
echo -e "$CMAKE" > "$TARGET_CMAKELIST";

echo "[+] generate CMakeLists.txt for test success!";
