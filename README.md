# RK3399 Face Detect DB

A Face detect database utils/shared library for Rockchip-RK3399

## For Developer

0. Before coding, please install git pre-commit hook script:
	- Execute `./scripts/check-no-absolute-path-git-hook.sh install`
	- This hook could avoiding your home folder absolute path be committed into git.
	- It could keep this project could be continuous developed between different machine and OS.

## About Documents 

### Generate API documents by `Doxygen`

- Install doxygen: `sudo apt install doxygen doxygen-gui` 
	- Or install from other package manager/sources
- Generate API documents: `doxygen Doxyfile`
- Configure doxygen options visibly: `doxywizard Doxyfile`
